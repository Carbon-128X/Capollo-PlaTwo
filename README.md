# Platwo

# Producers
Carbon-128X
Apollo0018

# repo link
https://github.com/Carbon-128X/Capollo-PlaTwo.git

Platwo is a multiplayer board game application developed in C++ using the Qt Framework.

This project was developed as the final project for the Advanced Programming course at Isfahan University of Technology.

---

# Games

Platwo currently includes three classic board games:

- Dots and Boxes
- Nine Men's Morris
- Fanorona

---

# Features

- User registration and login
- Local account management
- Game history
- TCP Host / Guest multiplayer
- Turn synchronization
- Optional turn timer
- Player color selection
- Rematch support
- Modern Qt Widgets graphical interface
- Animated backgrounds
- Image, video and font resources
- Windows installer

---

# Technologies

- C++17
- Qt 6
- Qt Widgets
- Qt Network
- Qt Multimedia
- qmake
- MinGW 64-bit

---

# Project Structure

Platwo
│
├── logic
│   ├── dotsAndBoxes.*
│   ├── nineMensMorris.*
│   ├── fanorona.*
│   └── gameMediator.*
│
├── network
│   ├── protocol.*
│   ├── gameSession.*
│   ├── networkGame.*
│
├── images
├── videos
├── fonts
│
├── resources.qrc
├── main.cpp
├── Platwo.pro
│
└── ...


---

# Requirements

The project was developed using:

- Windows 10 / Windows 11
- Qt 6.11.1
- Qt Creator
- MinGW 13.1.0 (64-bit)

Required Qt modules:

- Qt Widgets
- Qt Network
- Qt Multimedia
- Qt Multimedia Widgets

---

# Building

Open Platwo.pro using Qt Creator.

Select:

- Qt 6.11.1 MinGW 64-bit
- Release Mode

Then:

Build
→ Run qmake
→ Rebuild Project


Finally run the application.

---

# Multiplayer

Platwo supports multiplayer through a direct TCP connection.

## Local Network

1. One player selects Host.
2. The second player selects Guest.
3. The host shares the local IPv4 address and port.
4. The guest connects using that address.

## Internet

Players can also use Tailscale.

Steps:

1. Install Tailscale.
2. Join the same Tailscale network.
3. Start the Host.
4. Share the Tailscale IPv4 address.
5. Guest connects using that address.

---

# Application Data

The application stores user accounts and game history locally.

Generated files:

users.json
history.json


These files are created automatically when needed.

---

# Network Architecture

The multiplayer system consists of:

- protocol
- gameSession
- networkGame

The host validates every move and synchronizes the game state with the guest.

---

# Windows Installer

A Windows installer is included with the project.

Run:

Platwo_Setup.exe


to install the application.

Qt is not required on the destination computer.

---

# Notes

- Build the project using the Release configuration.
- Both players should use the same version of the application.
- Windows Firewall may request permission when hosting.
- Internet multiplayer requires a reachable IP address (for example through Tailscale).

---

# Course Information

- Course: Advanced Programming
- University: Isfahan University of Technology
- Project Type: Team Project

---

# License

This project was developed for educational purposes.

Third-party resources remain under their respective licenses.

