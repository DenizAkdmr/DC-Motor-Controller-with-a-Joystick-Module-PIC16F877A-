# DC-Motor-Controller-with-a-Joystick-Module-PIC16F877A-

  Description:
    This code controls a DC motor using a joystick connected to a PIC16F877A microcontroller.
    - Joystick X-axis (AN0) determines motor direction: Forward, Reverse, or Stop.
    - Joystick Y-axis (AN1) sets motor speed via PWM (0–100% duty cycle).
    - Sends feedback over UART (9600 baud) in the format: "DIR: <Forward/Reverse/Stop>, SPD: <n>%"

  Hardware Setup:
    - Microcontroller: PIC16F877A @ 4 MHz oscillator.
    - Joystick X → AN0, Y → AN1.
    - Motor driver IN1 → RC0, IN2 → RC1, PWM → CCP1 (RC2).
    - UART TX → RC6.
    - No external files or libraries needed; all configuration and code are within this file.

  Configuration:
    - MPLAB X IDE with XC8 compiler.
    - FOSC = HS, Watchdog off, Power-up timer off, Brown-out on, Low-voltage programming off.

  Usage:
    1. Compile and program the PIC using MPLAB X.
    2. Open a serial terminal at 9600 baud, 8N1.
    3. Move the joystick to control motor direction and speed.
*/

#include <xc.h>
#include <stdio.h>

#define _XTAL_FREQ 4000000

// CONFIGURATION BITS
#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

// Motor control pins
#define IN1 RC0
#define IN2 RC1

void UART_Init(void){
    TRISC6 = 0;    // TX pin as output
    TRISC7 = 1;    // RX pin as input
    SPBRG = 25;    // Baud rate 9600 @ 4MHz, BRGH=1
    BRGH = 1;      // High speed
    SYNC = 0;      // Asynchronous mode
    SPEN = 1;      // Enable serial port
    TXEN = 1;      // Enable transmission
}

void UART_SendString(const char *str){
    while(*str){
        while(!TXIF);
        TXREG = *str++;
    }
}

void PWM_Init(void){
    TRISC2 = 0;      // CCP1 output
    PR2 = 249;       // PWM period
    CCP1CON = 0x0C;  // PWM mode
    T2CON = 0x04;    // Timer2 on, prescaler 1:1
}

void Set_PWM_Duty(unsigned int duty){
    if(duty > 249) duty = 249;
    CCPR1L = duty >> 2;
    CCP1CONbits.DC1B = duty & 0x03;
}

void ADC_Init(void){
    ADCON1 = 0x80; // Configure AN0 and AN1 as analog
    ADCON0 = 0x41; // Select channel 0 and enable ADC
}

unsigned int ADC_Read(unsigned char channel){
    ADCON0 = (ADCON0 & 0xC7) | (channel << 3);
    __delay_ms(2);      // Acquisition time
    GO_nDONE = 1;       // Start conversion
    while(GO_nDONE);
    return (ADRESH << 8) | ADRESL;
}

void main(void){
    char buffer[50];
    unsigned int x_val, y_val, pwm_val, speed_percent;
    const char *dir;

    UART_Init();
    PWM_Init();
    ADC_Init();
    TRISC0 = 0; // IN1 output
    TRISC1 = 0; // IN2 output

    while(1){
        x_val = ADC_Read(0);
        y_val = ADC_Read(1);

        // Motor direction
        if(x_val > 600){
            IN1 = 1; IN2 = 0;
            dir = "Forward";
        }
        else if(x_val < 400){
            IN1 = 0; IN2 = 1;
            dir = "Reverse";
        }
        else {
            IN1 = 0; IN2 = 0;
            dir = "Stop";
        }

        // Motor speed
        if(y_val > 512){
            pwm_val = (y_val - 512) * 249 / 512;
        } else {
            pwm_val = (512 - y_val) * 249 / 512;
        }
        Set_PWM_Duty(pwm_val);
        speed_percent = pwm_val * 100 / 249;

        // Send status via UART
        sprintf(buffer, "DIR: %s, SPD: %u%%\r\n", dir, speed_percent);
        UART_SendString(buffer);
        __delay_ms(500);
    }
}
