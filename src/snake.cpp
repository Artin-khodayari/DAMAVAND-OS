#include "snake.hpp"
#include "vga.hpp"

// low-level port IO (no stdlib)
static inline void outb(u16 port, u8 val) {
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}
static inline u8 inb(u16 port) {
    u8 ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}
static inline u16 inw(u16 port) {
    u16 ret;
    asm volatile ("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}
static inline void outw(u16 port, u16 val) {
    asm volatile ("outw %0, %1" : : "a"(val), "Nd"(port));
}

// PIC master mask port
#define PIC1_CMD 0x20
#define PIC1_DATA 0x21
#define KBD_IRQ_BIT (1 << 1)

// keyboard status port and data port
#define KBD_STATUS_PORT 0x64
#define KBD_DATA_PORT   0x60

// Drawing GameBoard: characters
#define SnakeHead '@'
#define SnakeStomach '$'
#define wall '#'
#define food '*'

// scancodes we care about
enum Key {
    KB_UP    = 0x48,
    KB_DOWN  = 0x50,
    KB_LEFT  = 0x4B,
    KB_RIGHT = 0x4D,
    KB_ESC   = 0x01,
    KB_Q     = 0x10, // 'q' scancode (make sure it's correct for your map)
};

// Simple LCG for pseudo-random positions
static unsigned rng_state = 1;
static unsigned rand_u(unsigned max) {
    rng_state = rng_state * 1103515245 + 12345;
    return (rng_state >> 16) % max;
}

// Game parameters and drawing region
static const int FIELD_W = 40;
static const int FIELD_H = 15;
static const int FIELD_X = 18; // column offset on screen
static const int FIELD_Y = 4;  // row offset on screen

// Map grid coords -> screen
static inline void draw_cell(int gx, int gy, char ch, u8 color_fg = 0xE, u8 color_bg = 0) {
    VGA::setcolor(color_fg, color_bg);
    VGA::setCursor(FIELD_X + gx, FIELD_Y + gy);
    VGA::putc(ch);
}

static const int MAX_BODY = FIELD_W * FIELD_H;
struct Pos { int x, y; };
static Pos body[MAX_BODY];
static int head_idx = 0; // index of head in circular buffer
static int tail_idx = 0;
static int length_snake = 0;

static void push_head(int x, int y) {
    head_idx = (head_idx + 1) % MAX_BODY;
    body[head_idx].x = x;
    body[head_idx].y = y;
    if (length_snake < MAX_BODY) ++length_snake;
    // if buffer full, advance tail
    if (length_snake == MAX_BODY) tail_idx = (tail_idx + 1) % MAX_BODY;
}
static void pop_tail() {
    if (length_snake > 0) {
        VGA::setCursor(FIELD_X + body[tail_idx].x, FIELD_Y + body[tail_idx].y);
        VGA::putc(' ');
        tail_idx = (tail_idx + 1) % MAX_BODY;
        --length_snake;
    }
}
static Pos get_head() { return body[head_idx]; }

static bool collides_with_body(int x, int y) {
    int idx = tail_idx;
    for (int i = 0; i < length_snake; ++i) {
        if (body[idx].x == x && body[idx].y == y) return true;
        idx = (idx + 1) % MAX_BODY;
    }
    return false;
}

// --- Score --> displayed on top wall ---
static int score = 0;

static void draw_score() {
    // prepare "SCORE: <n>"
    char buf[16];
    int n = 0;
    int tmp = score;
    if (tmp == 0) { buf[n++] = '0'; }
    else {
        while (tmp > 0 && n < (int)sizeof(buf)-1) {
            buf[n++] = '0' + (tmp % 10);
            tmp /= 10;
        }
    }
    // score label
    const char *label = "SCORE: ";
    int label_len = 7; // "SCORE: "
    // total length = label_len + digits
    int total = label_len + n + 2;
    int startx = (FIELD_W - total) / 2;
    if (startx < 1) startx = 1; // don't overwrite left wall

    // draw label with wall color so it blends, but we'll use highlight for digits
    for (int i = 0; i < label_len; ++i) {
        draw_cell(startx + i, 0, label[i], 0x7); // wall-like color for label
    }
    // draw digits
    for (int i = 0; i < n; ++i) {
        char d = buf[n - 1 - i];
        draw_cell(startx + label_len + i, 0, d, 0xE); // yellow digits
    }
}

// draw frame border
static void draw_border() {
    // top/bottom
    for (int x = 0; x < FIELD_W; ++x) {
        draw_cell(x, 0, wall, 0x7);
        draw_cell(x, FIELD_H - 1, wall, 0x7);
    }
    for (int y = 0; y < FIELD_H; ++y) {
        draw_cell(0, y, wall, 0x7);
        draw_cell(FIELD_W - 1, y, wall, 0x7);
    }

    // overlay score onto the top wall
    draw_score();
}

// read one scancode if available; returns 0 if none or release code
static u8 poll_scancode_or_zero() {
    // check status output buffer full
    if (inb(KBD_STATUS_PORT) & 1) {
        u8 sc = inb(KBD_DATA_PORT);
        // ignore releases
        if (sc & 0x80) return 0;
        return sc;
    }
    return 0;
}

// mask/unmask keyboard IRQ in master PIC, return previous mask
static u8 pic_read_master_mask() {
    return inb(PIC1_DATA);
}
static void pic_write_master_mask(u8 m) {
    outb(PIC1_DATA, m);
}

static void drain_kbd_fifo() {
    while (inb(KBD_STATUS_PORT) & 1) {
        (void)inb(KBD_DATA_PORT);
    }
}

void Snake::run() {
    // Prepare field: clear region
    for (int ry = 0; ry < FIELD_H; ++ry) {
        for (int rx = 0; rx < FIELD_W; ++rx) {
            draw_cell(rx, ry, ' ');
        }
    }
    draw_border();

    // Initialize snake in center
    int sx = FIELD_W / 2;
    int sy = FIELD_H / 2;
    length_snake = 0;
    head_idx = tail_idx = 0;
    // push initial 3 cells
    push_head(sx - 1, sy);
    push_head(sx, sy);
    push_head(sx + 1, sy);

    // draw snake
    int idx = tail_idx;
    for (int i = 0; i < length_snake; ++i) {
        char ch = (i == length_snake - 1) ? SnakeHead : SnakeStomach;
        draw_cell(body[idx].x, body[idx].y, ch, 0xA);
        idx = (idx + 1) % MAX_BODY;
    }

    // reset and draw initial score
    score = 0;
    draw_score();

    // place first food
    int food_x = 2 + (rand_u(FIELD_W - 4));
    int food_y = 2 + (rand_u(FIELD_H - 4));
    while (collides_with_body(food_x, food_y)) {
        food_x = 2 + (rand_u(FIELD_W - 4));
        food_y = 2 + (rand_u(FIELD_H - 4));
    }
    draw_cell(food_x, food_y, food, 0xC);

    // Temporarily mask keyboard IRQ so existing keyboard ISR doesn't interfere
    u8 old_mask = pic_read_master_mask();
    pic_write_master_mask(old_mask | KBD_IRQ_BIT); // mask IRQ1

    // drain any pending scancodes
    drain_kbd_fifo();

    // direction: dx,dy
    int dx = 1, dy = 0; // start moving right
    bool quit = false;
    unsigned speed = 6; // moves per loop steps (smaller -> faster)
    unsigned ticks = 0;

    while (!quit) {
        // small busy-wait delay to control game speed
        for (volatile unsigned d = 0; d < 4000000u; ++d) { asm volatile ("nop"); }

        // poll keyboard non-blocking
        u8 sc = poll_scancode_or_zero();
        if (sc) {
            if (sc == KB_ESC || sc == KB_Q) {
                quit = true;
                break;
            } else if (sc == KB_UP && !(dx == 0 && dy == 1)) { dx = 0; dy = -1; }
            else if (sc == KB_DOWN && !(dx == 0 && dy == -1)) { dx = 0; dy = 1; }
            else if (sc == KB_LEFT && !(dx == 1 && dy == 0)) { dx = -1; dy = 0; }
            else if (sc == KB_RIGHT && !(dx == -1 && dy == 0)) { dx = 1; dy = 0; }
            // ignore other keys
        }

        // advance tick
        ++ticks;
        if (ticks % speed != 0) continue;

        // compute new head
        Pos head = get_head();
        int nx = head.x + dx;
        int ny = head.y + dy;

        // wall collision => game over
        if (nx <= 0 || nx >= FIELD_W - 1 || ny <= 0 || ny >= FIELD_H - 1) {
            VGA::setCursor(FIELD_X, FIELD_Y + FIELD_H + 1);
            VGA::puts("Game Over (hit wall). Press any key to continue...\n");
            while (!(inb(KBD_STATUS_PORT) & 1)) { asm volatile("pause"); }
            (void)inb(KBD_DATA_PORT);
            break;
        }

        // collision with body => game over
        if (collides_with_body(nx, ny)) {
            VGA::setCursor(FIELD_X, FIELD_Y + FIELD_H + 1);
            VGA::puts("Game Over (hit body). Press any key to continue...\n");
            while (!(inb(KBD_STATUS_PORT) & 1)) { asm volatile("pause"); }
            (void)inb(KBD_DATA_PORT);
            break;
        }

        // move head
        push_head(nx, ny);
        // draw new head
        draw_cell(nx, ny, SnakeHead, 0xA);
        // previous head becomes body char - find previous head index
        int prev_head_idx = (head_idx - 1 + MAX_BODY) % MAX_BODY;
        Pos prev = body[prev_head_idx];
        draw_cell(prev.x, prev.y, SnakeStomach, 0xA);

        // did we eat food?
        if (nx == food_x && ny == food_y) {
            // grow - do not pop tail
            // increase score
            score += 10;
            draw_score(); // refresh score display

            // place new food
            do {
                food_x = 2 + (rand_u(FIELD_W - 4));
                food_y = 2 + (rand_u(FIELD_H - 4));
            } while (collides_with_body(food_x, food_y));
            draw_cell(food_x, food_y, food, 0xC);
        } else {
            // erase tail
            pop_tail();
        }
    }

    // restore PIC mask
    pic_write_master_mask(old_mask);

    // drain any pending scancodes left from gameplay
    drain_kbd_fifo();

    // restore cursor to below field
    VGA::setCursor(0, FIELD_Y + FIELD_H + 2);
    VGA::puts("\nReturned to shell.\n");
}
