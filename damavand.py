from colorama import init, Fore, Back, Style
from datetime import datetime, date
from getpass import getpass
from PIL import Image
import subprocess
import platform
import pyfiglet
import shutil
import random
import qrcode
import time
import sys
import os

OS = platform.uname()[0]
clr = "clear" if OS == "Linux" else "cls"

MAGENTA = Fore.MAGENTA
YELLOW = Fore.YELLOW
WHITE = Fore.WHITE
RESET = Fore.RESET
GREEN = Fore.GREEN
BLACK = Fore.BLACK
CYAN = Fore.CYAN
RED = Fore.RED
CYAN = CYAN

os.system(clr)
init()

def LOGO():
    logo = [
        "██████╗  █████╗ ███╗   ███╗ █████╗ ██╗   ██╗ █████╗ ███╗   ██╗██████╗",
        "██╔══██╗██╔══██╗████╗ ████║██╔══██╗██║   ██║██╔══██╗████╗  ██║██╔══██╗",
        "██║  ██║███████║██╔████╔██║███████║██║   ██║███████║██╔██╗ ██║██║  ██║",
        "██║  ██║██╔══██║██║╚██╔╝██║██╔══██║╚██╗ ██╔╝██╔══██║██║╚██╗██║██║  ██║",
        "██████╔╝██║  ██║██║ ╚═╝ ██║██║  ██║ ╚████╔╝ ██║  ██║██║ ╚████║██████╔╝",
        "╚═════╝ ╚═╝  ╚═╝╚═╝     ╚═╝╚═╝  ╚═╝  ╚═══╝  ╚═╝  ╚═╝╚═╝  ╚═══╝╚═════╝"
    ]
    columns = shutil.get_terminal_size().columns
    box_width = columns - 8
    print("")
    print(f'{CYAN}   ╔{"═" * box_width}╗')
    print(f'   ║{" " * box_width}║')
    print(f'   ║{" " * box_width}║')
    for line in logo:
        print(f'   ║{line.center(box_width)}║')
        time.sleep(0.08)
    print(f'   ║{" " * box_width}║')
    print(f'   ║{" " * box_width}║')
    print(f'   ╚{"═" * box_width}╝')
    print("")

def logo2():
    os.system(clr)
    print(f'''
                {f"""{CYAN}
                $$$$$$$\   $$$$$$\  $$\      $$\  $$$$$$\  $$\    $$\  $$$$$$\  $$\   $$\ $$$$$$\\             
                $$  __$$\ $$  __$$\ $$$\    $$$ |$$  __$$\ $$ |   $$ |$$  __$$\ $$$\  $$ |$$  __$$\\             
                $$ |  $$ |$$ /  $$ |$$$$\  $$$$ |$$ /  $$ |$$ |   $$ |$$ /  $$ |$$$$\ $$ |$$ |  $$ |
                $$ |  $$ |$$$$$$$$ |$$\$$\$$ $$ |$$$$$$$$ |\$$\  $$  |$$$$$$$$ |$$ $$\$$ |$$ |  $$ |             
                $$ |  $$ |$$  __$$ |$$ \$$$  $$ |$$  __$$ | \$$\$$  / $$  __$$ |$$ \$$$$ |$$ |  $$ |             
                $$ |  $$ |$$ |  $$ |$$ |\$  /$$ |$$ |  $$ |  \$$$  /  $$ |  $$ |$$ |\$$$ |$$ |  $$ |
                $$$$$$$  |$$ |  $$ |$$ | \_/ $$ |$$ |  $$ |   \$  /   $$ |  $$ |$$ | \$$ |$$$$$$$ / 
                \_______/ \__|  \__|\__|     \__|\__|  \__|\_ _/ \__|  \__|\__|  \__|\______/
                """}
                {f"{CYAN}(/)> Damavand project"}
                {f"{CYAN}(/)> Made with laugh, happiness, and so much love"}
                {f"{CYAN}(/)> version : 1.0"}
        ''')

def lgcd():
    LGCI = input(f'{CYAN}LGC WORD : ')
    ascii_art = pyfiglet.figlet_format(LGCI)
    os.system(clr)
    print('')
    print(f'{CYAN}----------------------------------------------------------------------')
    print('')
    print(Fore.LIGHTMAGENTA_EX + ascii_art)
    print(f'{CYAN}----------------------------------------------------------------------')
    print(f'{CYAN}')

def help():
    commands = [
        "COMMANDS LIST :",
        f'clear ---> Clear the screen',
        "reboot ---> Restart the os",
        "sd ---> Shutdown the os",
        "damavand ---> Showing logo",
        "lgc ---> Big letter",
        "ping ---> Ping website and server",
        "time ---> Current hour, minute, second",
        "date ---> Current date at today",
        "chp ---> Change password",
        "ngg ---> number guessing game",
        "timer ---> special timer",
        "chu ---> Change user",
        "cdp ---> Change command input text",
        "fetch ---> Damavand version and ...",
        "uname ---> Computer informations",
        "vi ---> A file editor",
        "qcd ---> A QrCode generator",
        "echo ---> print something you want",
        "about ---> About developer",
        "help ---> :)))))))"
    ]
    max_length = max(len(command) for command in commands) + 4
    os.system(clr)
    print(f'{CYAN}')
    print(f'╔{"═" * max_length}╗')
    for command in commands:
        print(f'║ {command.ljust(max_length - 2)} ║')
        time.sleep(0.07)
    print(f'╚{"═" * max_length}╝')
    print()

def encryption(message, key):
    key_str = str(key)
    key_base_val = sum(ord(c) * (i + (int(key_str)**2) if key_str.isdigit() else i + 1) for i, c in enumerate(key_str)) % 256

    encrypted_parts = []
    for i, char_code in enumerate(message):
        shifted_value = (ord(char_code) + key_base_val + i) % 256
        xor_key_part = (key_base_val ^ i) % 256
        final_encrypted_value = (shifted_value ^ xor_key_part) % 256

        encrypted_parts.append(str(final_encrypted_value))

    return '#'.join(encrypted_parts)

def decryption(encrypted_message, key):
    key_str = str(key)
    key_base_val = sum(ord(c) * (i + (int(key_str)**2) if key_str.isdigit() else i + 1) for i, c in enumerate(key_str)) % 256

    parts = encrypted_message.split('#')
    decrypted_chars = []

    for i, part in enumerate(parts):
        if part:
            try:
                encrypted_value = int(part)

                xor_key_part = (key_base_val ^ i) % 256
                un_xor_value = (encrypted_value ^ xor_key_part) % 256

                decrypted_value = (un_xor_value - key_base_val - i) % 256

                decrypted_chars.append(chr(decrypted_value))
            except ValueError:
                print(f"{RED}Warning: Invalid encrypted part encountered during decryption. Possible corruption or incorrect key.{RESET}")
                return ""
            except Exception as e:
                print(f"{RED}An error occurred during decryption: {e}{RESET}")
                return ""

    return ''.join(decrypted_chars)

USERNAME = None
USER_PASSWORD_HASH = None

def register_user():
    global USERNAME, USER_PASSWORD_HASH
    os.system(clr)
    print(f"""
To use Damavand-OS you should make account!
      First enter {RED}[Username]{RESET}
      If you have an account Enter {RED}'y'{RESET}
""")
    username_input = input(f"{CYAN}Enter a name {CYAN}: {YELLOW}")

    if username_input.lower() == "y":
        existing_username = input(f"{CYAN}Enter your existing USERNAME : {YELLOW}")
        user_data_file = f"{existing_username}.dup"
        if not os.path.exists(user_data_file):
            print(f"{RED}Error: User '{existing_username}' not found. Please register first.{RESET}")
            sys.exit()
        else:
            try:
                with open(user_data_file, "r") as f:
                    lines = f.readlines()
                    if len(lines) < 2:
                        print(f"{RED}Error: Invalid user data file format for '{existing_username}'.{RESET}")
                        sys.exit()
                    
                    stored_username_line = lines[0].strip()
                    stored_password_hash_line = lines[1].strip()

                    if not stored_username_line.startswith("Username :") or \
                       not stored_password_hash_line.startswith("PasswordHash :"):
                       print(f"{RED}Error: Malformed user data file for '{existing_username}'.{RESET}")
                       sys.exit()

                    stored_username = stored_username_line.split(":", 1)[1].strip()
                    stored_password_hash = stored_password_hash_line.split(":", 1)[1].strip()

                    if stored_username != existing_username:
                         print(f"{RED}Error: Username mismatch in file. Expected '{existing_username}', found '{stored_username}'.{RESET}")
                         sys.exit()

                USERNAME = stored_username
                current_session_key = getpass(f"{CYAN}Enter your KEY to log in: {RESET}")
                password_input = getpass(f'{CYAN}Enter your password for ({RED}{USERNAME}{f"{CYAN})"} : {RESET}')
                
                decrypted_password = decryption(stored_password_hash, current_session_key)

                if password_input == decrypted_password:
                    USER_PASSWORD_HASH = stored_password_hash
                    print(f"{MAGENTA}[+] Welcome back, {USERNAME}!{RESET}")
                    time.sleep(2)
                    os.system(clr)
                else:
                    print(f"{RED}[-] Incorrect password or key. You shall not pass!{RESET}")
                    sys.exit()
            except Exception as e:
                print(f"{RED}An error occurred during login: {e}{RESET}")
                sys.exit()
    else:
        USERNAME = username_input
        current_session_key = getpass(f"{CYAN}Enter a key to secure your password: {RESET}")
        password_input = getpass(f'{CYAN}Enter a password for ({RED}{USERNAME}{f"{CYAN})"} : {RESET}')
        
        USER_PASSWORD_HASH = encryption(password_input, current_session_key)
        
        user_data_file = f"{USERNAME}.dup"
        try:
            with open(user_data_file, "w") as DUP:
                DUP.write(f"Username : {USERNAME}\n")
                DUP.write(f"PasswordHash : {USER_PASSWORD_HASH}\n")
            print(f"{GREEN}[+] User '{USERNAME}' created successfully!{RESET}")
            time.sleep(2)
            os.system(clr)
        except Exception as e:
            print(f"{RED}An error occurred during user creation: {e}{RESET}")
            sys.exit()

def write_file(file):
    os.system(clr)
    print(f"{RED}Editing file -> {file.name}")
    file_line = input(f"{WHITE}")
    while file_line != ":>end":
        file.write(f"{file_line}\n")
        file_line = input(f"{WHITE}")
    print(f"{GREEN}[+] File written successfully.{RESET}")

def read_file(file):
    os.system(clr)
    print(f"{RED}Reading file -> {file.name}")
    print(f"{WHITE}{file.read()}")
    input(f"{GREEN}Press Enter to continue...")

def edit_file(file_name):
    os.system(clr)
    print(f"{RED}Editing file -> {file_name}")
    try:
        with open(file_name, 'r') as file:
            lines = file.readlines()
    except FileNotFoundError:
        print(f"{RED}[-] File not found.{RESET}")
        return
    except Exception as e:
        print(f"{RED}[-] An error occurred while reading the file: {e}{RESET}")
        return

    print(f"{YELLOW}Current file content:")
    for i, line in enumerate(lines, start=1):
        print(f"{i}: {line.strip()}")

    while True:
        line_number_input = input(f"{WHITE}Enter line number to edit (or type 'done' to finish): ")
        if line_number_input.lower() == 'done':
            break
        
        try:
            line_number = int(line_number_input)
            if 1 <= line_number <= len(lines):
                new_content = input(f"{WHITE}Enter new content: ")
                lines[line_number - 1] = new_content + "\n"
            else:
                print(f"{RED}[-] Invalid line number. Try again.{RESET}")
        except ValueError:
            print(f"{RED}[-] Invalid input. Please enter a number or 'done'.{RESET}")

    try:
        with open(file_name, 'w') as file:
            file.writelines(lines)
        print(f"{GREEN}[+] File edited successfully.{RESET}")
    except Exception as e:
        print(f"{RED}[-] An error occurred while writing to the file: {e}{RESET}")

def create_colorful_qr(link, output_path):
    qr = qrcode.QRCode(
        version=1,
        error_correction=qrcode.constants.ERROR_CORRECT_H,
        box_size=10,
        border=4,
    )
    qr.add_data(link)
    qr.make(fit=True)
    qr_img = qr.make_image(fill_color="black", back_color="white").convert('RGB')
    pixels = qr_img.load()
    for y in range(qr_img.size[1]):
        for x in range(qr_img.size[0]):
            if pixels[x, y] == (0, 0, 0):
                pixels[x, y] = (random.randint(0, 255), random.randint(0, 255), random.randint(0, 255))
    qr_img.save(output_path)


LOGO()
time.sleep(1.5)

if USERNAME is None:
    register_user()

if USERNAME is None: # Ensure user is logged in/registered
    print(f"{RED}Authentication failed. Exiting.{RESET}")
    sys.exit()

logo2()
CDP = 'C-:' # Command Path Display

try :
    while True :
        COMMAND = input(f'{RED}({YELLOW + USERNAME}{RED}) {CDP}{f'{CYAN} '}').lower()
        
        if COMMAND == 'clear' or COMMAND == 'clr':
            os.system(clr)
        elif COMMAND == 'sd' :
            current_action_key = getpass(f"{CYAN}Enter your KEY to shutdown: {RESET}")
            password_attempt = getpass(f"{CYAN}Enter password to shutdown: {RESET}")
            if encryption(password_attempt, current_action_key) == USER_PASSWORD_HASH:
                print(f"{GREEN}Shutting down Damavand...{RESET}")
                time.sleep(1)
                os.system(clr)
                sys.exit()
            else:
                print(f"{RED}Incorrect password or key.{RESET}")
        elif COMMAND == 'reboot' :
            current_action_key = getpass(f"{CYAN}Enter your KEY to reboot: {RESET}")
            password_attempt = getpass(f"{CYAN}Enter password to reboot: {RESET}")
            if encryption(password_attempt, current_action_key) == USER_PASSWORD_HASH:
                print(f"{YELLOW}Rebooting Damavand...{RESET}")
                time.sleep(1)
                os.system(clr)
                print(f"{GREEN}Damavand restarted.{RESET}")
            else:
                print(f"{RED}Incorrect password or key.{RESET}")
        elif COMMAND == 'damavand' :
            os.system(clr)
            LOGO()
        elif COMMAND == 'lgc' :
            os.system(clr)
            lgcd()
        elif COMMAND.startswith("ping ") :
            target = COMMAND[5:].strip()
            if not target:
                print(f"{RED}Please provide a target to ping (e.g., ping google.com){RESET}")
                continue
            print('')
            print(Fore.LIGHTRED_EX + '-------------------------------------------------------------')
            try:
                if platform.system() == "Windows":
                    ping_command = ["ping", "-n", "4", target]
                else: # Linux/macOS
                    ping_command = ["ping", "-c", "4", target]

                result = subprocess.run(ping_command, capture_output=True, text=True, check=False)
                print(result.stdout)
                if result.stderr:
                    print(f"{RED}Error during ping: {result.stderr}{RESET}")
                
            except FileNotFoundError:
                print(f"{RED}Error: 'ping' command not found. Ensure it's in your system's PATH.{RESET}")
            except Exception as e:
                print(f"{RED}An error occurred during ping: {e}{RESET}")
            print('')
            print(Fore.LIGHTRED_EX + '-------------------------------------------------------------')
            print(CYAN + '')
        elif COMMAND == 'help' :
            help()
        elif COMMAND == 'time' :
            current_time = time.strftime("%H:%M:%S")
            CT_b = len(current_time) + 4
            os.system(clr)
            print('')
            print(f'╔{"═" * CT_b}╗')
            print(f'║{" " * CT_b}║')
            print(f'║ {current_time.center(CT_b - 2)} ║')
            print(f'║{" " * CT_b}║')
            print(f'╚{"═" * CT_b}╝')
            print(Back.BLACK + f'{CYAN}')
        elif COMMAND == 'date' :
            today = date.today()
            weekday_number = today.weekday()
            weekdays = ["Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"]
            date_text = f'{today.year} / {today.month} / {today.day} / {weekdays[weekday_number]}'
            De_b = len(date_text) + 4
            os.system(clr)
            print(Fore.LIGHTRED_EX + f'╔{"═" * De_b}╗')
            print(Fore.LIGHTRED_EX + f'║{" " * De_b}║')
            print(Fore.LIGHTRED_EX + f'║ {date_text.center(De_b - 2)} ║')
            print(Fore.LIGHTRED_EX + f'║{" " * De_b}║')
            print(Fore.LIGHTRED_EX + f'╚{"═" * De_b}╝')
            print(Back.BLACK + f'{CYAN}')

        elif COMMAND == 'chp' :
            current_action_key = getpass(f"{CYAN}Enter your KEY to change password: {RESET}")
            password_attempt = getpass(f'{CYAN}Enter your current password: ')
            if encryption(password_attempt, current_action_key) == USER_PASSWORD_HASH:
                new_password = getpass(f'{CYAN}Enter your new password: ')
                retype_new_password = getpass(f'{CYAN}Retype your new password: ')
                if new_password == retype_new_password:
                    USER_PASSWORD_HASH = encryption(new_password, current_action_key)
                    user_data_file = f"{USERNAME}.dup"
                    try:
                        with open(user_data_file, "w") as DUP:
                            DUP.write(f"Username : {USERNAME}\n")
                            DUP.write(f"PasswordHash : {USER_PASSWORD_HASH}\n")
                        print(f"{GREEN}[+] Password changed successfully!{RESET}")
                    except Exception as e:
                        print(f"{RED}An error occurred while updating the password: {e}{RESET}")
                else:
                    print(f"{RED}New passwords do not match.{RESET}")
            else:
                print(f"{RED}Incorrect current password or key.{RESET}")
        elif COMMAND == 'ngg' :
            try:
                RN = random.randint(1, 20)
                guess_count = 0
                while True:
                    guess_input = input(f"{CYAN}[?] Enter your guess (or 'quit' to exit): {YELLOW}")
                    if guess_input.lower() == 'quit':
                        print(f"{RED}Exiting Number Guessing Game.{RESET}")
                        break
                    GRN = int(guess_input)
                    guess_count += 1
                    if GRN == RN:
                        os.system(clr)
                        winner_banner = pyfiglet.figlet_format(f'YOU WON!\n(Took {guess_count} guesses)')
                        lines = winner_banner.split('\n')
                        max_length = max(len(line) for line in lines) + 4
                        print(f'{CYAN}')
                        print(f'╔{"═" * max_length}╗')
                        for line in lines:
                            print(f'║ {line.ljust(max_length - 2)} ║')
                        print(f'╚{"═" * max_length}╝')
                        print(f'{CYAN}')
                        break
                    elif GRN < RN:
                        print(f"{YELLOW}Too low! Try again.{RESET}")
                    else:
                        print(f"{YELLOW}Too high! Try again.{RESET}")
            except ValueError:
                print(f"{RED}[-] Invalid input. Please enter a number or 'quit'.{RESET}")
            except Exception as e:
                print(f"{RED}[-] An unexpected error occurred: {e}{RESET}")
        elif COMMAND == 'timer' :
            try:
                timer_input = input(f'{CYAN}[X] Enter seconds for the timer: {YELLOW}')
                TIMER = int(timer_input)
                if TIMER < 0:
                    print(f"{RED}Timer cannot be negative.{RESET}")
                    continue

                for i in range(TIMER, -1, -1):
                    os.system(clr)
                    print('')
                    T_counting = pyfiglet.figlet_format(str(i))
                    lines_T_counting = T_counting.split('\n')
                    max_length_T = max(len(line) for line in lines_T_counting) + 4
                    print(f'╔{"═" * max_length_T}╗')
                    for line in lines_T_counting:
                        print(f'║ {line.ljust(max_length_T - 2)} ║')
                    print(f'╚{"═" * max_length_T}╝')
                    print(f'{CYAN}')
                    if i > 0:
                        time.sleep(1)
                print(f"{GREEN}[+] Timer finished!{RESET}")
            except ValueError:
                print(f"{RED}[-] Invalid input. Please enter a valid number of seconds.{RESET}")
            except Exception as e:
                print(f"{RED}[-] An unexpected error occurred: {e}{RESET}")

        elif COMMAND == 'about' :
            about_start_text = pyfiglet.figlet_format('About Developer')
            lines = about_start_text.split('\n')
            max_length = max(len(line) for line in lines) + 4
            print(Fore.LIGHTGREEN_EX + '')
            print(f'╔{"═" * max_length}╗')
            for line in lines:
                print(f'║ {line.ljust(max_length - 2)} ║')
            print(f'╚{"═" * max_length}╝')
            print(f'{CYAN}')
            time.sleep(1.5)
            os.system(clr)
            print('')
            about_text = """
            You can contact me by my gmail account!	
            My personal email : artinkhodayari2010@gmail.com	
            My personal github : github.com/Artin-khodayari	
            """
            lines = about_text.split('\n')
            max_length = max(len(line) for line in lines) + 4
            print(Fore.LIGHTGREEN_EX + '')
            print(f'╔{"═" * max_length}╗')
            for line in lines:
                print(f'║ {line.strip().ljust(max_length - 2)} ║')
            print(f'╚{"═" * max_length}╝')
            print(f'{CYAN}')
            time.sleep(1)
        elif COMMAND == 'chu' :
            current_action_key = getpass(f"{CYAN}Enter your KEY to change user: {RESET}")
            password_attempt = getpass(f'{CYAN}Enter your current password: ')
            if encryption(password_attempt, current_action_key) == USER_PASSWORD_HASH:
                new_username = input(f'{CYAN}Enter new username: {YELLOW}')
                if not new_username:
                    print(f"{RED}Username cannot be empty.{RESET}")
                    continue
                
                new_password = getpass(f'{CYAN}Enter a password for the new user: {RESET}')
                new_key = getpass(f"{CYAN}Enter a new key for the new user: {RESET}")

                new_password_hash = encryption(new_password, new_key)

                new_user_data_file = f"{new_username}.dup"
                try:
                    with open(new_user_data_file, "w") as DUP:
                        DUP.write(f"Username : {new_username}\n")
                        DUP.write(f"PasswordHash : {new_password_hash}\n")

                    USERNAME = new_username
                    USER_PASSWORD_HASH = new_password_hash

                    print(f"{GREEN}[+] User changed to '{USERNAME}' successfully!{RESET}")
                    time.sleep(1)
                    os.system(clr)
                except Exception as e:
                    print(f"{RED}An error occurred while changing user: {e}{RESET}")
            else:
                print(f"{RED}Incorrect password. Cannot change user.{RESET}")
        elif COMMAND == 'cdp' :
            new_cdp = input(f'{CYAN}Enter new command input display (e.g., C:\\Users\\> ): {YELLOW}')
            if new_cdp:
                CDP = new_cdp
                os.system(clr)
            else:
                print(f"{RED}Command input display cannot be empty.{RESET}")
        elif COMMAND == 'fetch' :
            logo2()
        elif COMMAND == 'uname' :
            os.system(clr)
            print('')
            cpu_info = platform.processor()
            os_name = platform.system()
            os_release = platform.release()
            os_version = platform.version()
            machine = platform.machine()
            current_user = getpass.getuser() if hasattr(getpass, 'getuser') else "N/A"

            system_info = [
                f"{RED}[+] System Information:{RESET}",
                f"{YELLOW}  System: {os_name}",
                f"  Release: {os_release}",
                f"  Version: {os_version}",
                f"  Machine: {machine}",
                f"  Processor: {cpu_info}",
                f"  Current User: {current_user}{RESET}"
            ]

            max_length = max(len(info) for info in system_info) + 4
            print(f'╔{"═" * max_length}╗')
            print(f'║{" " * max_length}║')
            for info in system_info:
                print(f'║ {info.ljust(max_length - 2)} ║')
            print(f'╚{"═" * max_length}╝')
            print(f'{CYAN}')

        elif COMMAND == "vi" :
            file_name = input(f"{CYAN}Enter file name: {YELLOW}")
            if not file_name:
                print(f"{RED}File name cannot be empty.{RESET}")
                continue

            print(f"{RED}Editing file -> {file_name}{RESET}")
            print(f"{GREEN}Choose an option:")
            print(f"{CYAN}[1] Write (Create/Overwrite)")
            print("[2] Read")
            print(f"[3] Edit (Modify existing lines){RESET}")
            
            try:
                choice = int(input(f"{CYAN}[?] Enter your choice (1-3): {YELLOW}"))
                
                if choice == 1:
                    try:
                        with open(file_name, 'w') as file:
                            write_file(file)
                    except Exception as e:
                        print(f"{RED}Error writing to file: {e}{RESET}")
                elif choice == 2:
                    try:
                        with open(file_name, 'r') as file:
                            read_file(file)
                    except FileNotFoundError:
                        print(f"{RED}File '{file_name}' not found.{RESET}")
                    except Exception as e:
                        print(f"Error reading file: {e}")
                elif choice == 3:
                    edit_file(file_name)
                else:
                    print(f"{RED}[-] Invalid choice. Please select 1, 2, or 3.{RESET}")
            except ValueError:
                print(f"{RED}[-] Invalid input. Please enter a number.{RESET}")
            except Exception as e:
                print(f"{RED}[-] An unexpected error occurred: {e}{RESET}")

        elif COMMAND == "qcd" :
            link = input(Fore.LIGHTGREEN_EX + "Enter the link to encode: ")
            if not link:
                print(f"{RED}Link cannot be empty.{RESET}")
                continue
            output_filename = input(f"{CYAN}Enter output filename (e.g., my_qr): {YELLOW}")
            if not output_filename:
                output_filename = "qrcode_output"
                print(f"{YELLOW}Using default filename: {output_filename}.png{RESET}")
            
            output_path = f"{output_filename}.png"
            try:
                create_colorful_qr(link, output_path)
                print(f"{GREEN}[+] QR Code generated successfully as '{output_path}'!{RESET}")
            except Exception as e:
                print(f"{RED}Error generating QR Code: {e}{RESET}")

        elif COMMAND.startswith("echo "):
            echo_text = COMMAND[5:]
            print(echo_text)

        else :
            if COMMAND:
                print(f"{RED}[-] Unknown command: '{COMMAND}'. Type 'help' for a list of commands.{RESET}")
            continue

except KeyboardInterrupt:
    print(f"\n{RED}[-] Program interrupted by user. Exiting Damavand.{RESET}")
    time.sleep(1)
    os.system(clr)
    sys.exit()
except Exception as e:
    print(f"\n{RED}[-] An unexpected critical error occurred: {e}{RESET}")
    print(f"{RED}Exiting Damavand.{RESET}")
    time.sleep(2)
    os.system(clr)
    sys.exit()
