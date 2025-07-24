# What is Damavand-OS?
**Damavand-OS** is a _terminal-based operating system simulator_ developed entirely in **Python**. The project provides an opportunity for enthusiasts to gain hands-on experience interacting with the **command line**, **managing files**, and even a **custom cryptographic protocol** in a simulated environment. The main goal of the project is to ***explore Python's capabilities in building command-line user interfaces and provide a platform for learning basic operating system concepts in an interactive format***.

## ✨ Features
**Damavand-OS** comes packed with a variety of commands and utilities:

User Authentication: Secure user registration and login system with a custom encryption protocol (for educational purposes).

System Commands:

`clear / cls` : Clears the terminal screen.

`reboot` : Kills the OS & run it again

`sd` : Kills the OS

`uname` : Displays system information (OS, processor, user).

`fetch` : Shows project version and developer info.

### File Management:

`vi` : A simple text editor to write, read, or edit files.

### Utilities:

`ping` : Pings a specified website or server.

`time` : Displays the current time.

`date` : Displays the current date and weekday.

`timer` : A countdown timer.

`qcd` : Generates colorful QR codes from links.

`echo` : Prints text to the console.

### Games:

`ngg` : A number guessing game.

### Customization:

`chp` : Change user password.

`chu` : Change current user (switch to a different account).

` cdp` : Customize the command prompt display.

### Help & Info:

`help` : Displays a list of all available commands.

`lgc` : Generates large ASCII art from a word.

`damavand` : Displays the main DAMAVAND-OS logo.

`about` : Information about the developer.

## 🚀 Installation
To run DAMAVAND-OS, you need Python 3 installed on your system.

Clone the repository:

``git clone https://github.com/Artin-khodayari/Damavand-OS.git``

``cd Damavand-OS``

## Install dependencies:
**Damavand-OS**  uses a few external Python libraries. You can install them using pip:

`pip install -r requirements.txt`

## 🎮 Usage
To start **DAMAVAND-OS**, simply run the main Python script:

`python damavand.py`

Upon first launch, you will be prompted to create a new user account. If you already have an account, you can log in.

To make an account DAMAVAND-OS asks a key, It's important to make your account & infos secure;
So let's enter a key to stay secure, then memories key & the password you'll enter.
USERNAME & PASSWORD(ecrypted) will save in a file named : USERNAME.DUP

### What does `.DUP` mean?

**> DAMAVAND's USERNAME & PASSWORD**

Once logged in, you can type help to see a list of available commands and start interacting with the simulated OS!

## ⚠️ Important Note on Custom Encryption

**This project includes a custom encryption protocol developed for _educational purposes_ and to _explore cryptographic principles_. It combines additive shifts and bitwise XOR operations.**

***It is crucial to understand that this **custom encryption is NOT cryptographically secure and should NOT be used for protecting real-world sensitive data or passwords**. Standard, battle-tested cryptographic libraries (like hashlib, bcrypt, pycryptodome in Python) are designed by experts and have undergone rigorous scrutiny to ensure their security. Always use these industry-standard libraries for any application requiring genuine data protection.***

# 🧑‍💻 About the Developer

This project is made by [Artin khodayari](https://github.com/Artin-khodayari).

You can contact me and report the problems and bugs to my [Gmail-Account](mailto:ArtinKhodayari2010@gmail.com)

Feel free to reach out for questions, feedback, or collaborations!

# 📄 License
Also read [License](https://github.com/Artin-khodayari/DAMAVAND-OS/blob/main/License)
