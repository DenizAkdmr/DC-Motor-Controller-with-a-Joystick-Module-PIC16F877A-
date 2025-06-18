# PIC16F877A Joystick-Controlled Motor with UART and Bluetooth Feedback

## Description

This project controls a DC motor using a joystick connected to a PIC16F877A microcontroller. Motor direction (Forward, Reverse, Stop) is determined by the joystick X-axis, and speed (0–100%) is set via PWM using the joystick Y-axis. Feedback is sent over both UART and Bluetooth in the format:

```
DIR: <Forward/Reverse/Stop>, SPD: <n>%
```

## 🎯 Features

- Bidirectional DC motor control (Forward/Reverse)  
- Proportional speed control via joystick X-axis  
- Smooth PWM transition for realistic motor behavior  
- Wireless transmission of motor speed & direction to mobile bluetooth app 
- Real-time display of direction, % speed, and joystick X value  

---

## 🧩 Hardware Components

| Component         | Model                |
|-------------------|----------------------|
| Microcontroller   | PIC16F877A           |
| Motor Driver      | L298N Dual H-Bridge  |
| Joystick Module   | Analog (X-axis used) |
| Bluetooth Module  | HC-05 (Slave mode)   |
| DC Motor          | Brushed DC Motor     |
| Power Supply      | 9V battery + 5V regulator |

---

## 🪛 Pin Connections

### 📟 PIC16F877A

| PIC Pin | Connected To             | Description                       |
|---------|--------------------------|-----------------------------------|
| RA0     | Joystick X-axis (VRx)    | ADC input (Direction & Speed)     |
| RA1     | Joystick Y-axis (VRy)    | ADC input (unused)                |
| RC0     | L298N IN1                | Motor direction A                 |
| RC1     | L298N IN2                | Motor direction B                 |
| RC2     | L298N ENA (PWM, CCP1)    | Speed control                     |
| RC6     | HC-05 RX (via 1 kΩ)      | UART TX from PIC                  |
| GND     | HC-05 GND, L298N GND     | Common ground                     |
| VDD     | +5 V regulated           | Power to PIC, L298N, HC-05 VCC    |

### 🔌 Other Connections

| Device        | Connection Details                                                      |
|---------------|-------------------------------------------------------------------------|
| HC-05 Module  | VCC → 5 V, GND → GND<br>TX → (unused), RX ← RC6 (via 1 kΩ resistor)      |
| L298N Driver  | ENA ← RC2, IN1 ← RC0, IN2 ← RC1<br>OUT1/OUT2 → DC motor terminals       |
| Joystick      | VCC → 5 V, GND → GND, VRx → RA0, VRy → RA1                               |

## Software Requirements

- **IDE:** MPLAB X IDE  
- **Compiler:** XC8  

## Setup & Usage

1. Open the project in MPLAB X and build.  
2. Program the PIC16F877A.  
3. Open a serial terminal at 9600 baud, 8N1 for UART feedback.  
4. Pair a mobile device with the HC-05 module and open a Bluetooth serial app at 9600 baud for mobile feedback.    
5. Move the joystick to control motor direction and speed; feedback appears on terminal, and mobile app.  

## License

This project is for educational purposes.

## Project Credits

*Developed by:*  
- Deniz Akdemir  
- Taha Emre Gündoğan  

*Institution:*  
Istanbul Medipol University - 
Microcontrollers Course Project  

*Course Instructor:*  
Mustafa Türkboyları  
