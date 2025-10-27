#pragma once
namespace Keyboard {
    typedef void (*key_cb_t)(char);
    void init();
    void set_callback(key_cb_t cb);
}
