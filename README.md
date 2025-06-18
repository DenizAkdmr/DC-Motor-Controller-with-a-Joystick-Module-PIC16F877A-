# DC-Motor-Controller-with-a-Joystick-Module-PIC16F877A-
# PIC16F877A Joystick-Controlled Motor with UART Feedback

## Description

This project controls a DC motor using a joystick connected to a PIC16F877A microcontroller. Motor direction (Forward, Reverse, Stop) is determined by the joystick X-axis, and speed (0–100%) is set via PWM using the joystick Y-axis. Feedback is sent over UART in the format:

```
DIR: <Forward/Reverse/Stop>, SPD: <n>%
```

## Features

* **Direction control** based on joystick X-axis (AN0)
* **Speed control** (0–100%) via PWM on CCP1 (RC2)
* **UART feedback** at 9600 baud, 8N1 format

## Hardware Requirements

* **Microcontroller:** PIC16F877A @ 4 MHz oscillator
* **Joystick:** X → AN0, Y → AN1
* **Motor driver:** IN1 → RC0, IN2 → RC1, Enable (PWM) → RC2
* **UART TX:** RC6

## Software Requirements

* **IDE:** MPLAB X IDE
* **Compiler:** XC8

## Setup & Usage

1. Clone the repository:

   ```bash
   git clone https://github.com/KullaniciAdi/pic16f877a-joystick-motor.git
   cd pic16f877a-joystick-motor
   ```
2. Open the project in MPLAB X and build.
3. Program the PIC16F877A.
4. Open a serial terminal at 9600 baud, 8N1.
5. Move the joystick to control motor direction and speed; feedback appears in the terminal.

## License

MIT License

## Author

Deniz Akdemir

