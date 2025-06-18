#include <xc.h>
#include <stdio.h>

#define _XTAL_FREQ 4000000

// CONFIGURATION
#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#define IN1 RC0
#define IN2 RC1

void UART_Init(){
    TRISC6 = 0; TRISC7 = 1;
    SPBRG = 25; BRGH = 1;
    SYNC = 0; SPEN = 1;
    TXEN = 1;
}

void UART_SendString(const char *str){
    while (*str){
        while (!TXIF);
        TXREG = *str++;
    }
}

void PWM_Init(){
    TRISC2 = 0;
    PR2 = 249;
    CCP1CON = 0x0C;
    T2CON = 0x04;
}

void Set_PWM_Duty(unsigned int duty){
    if (duty > 249) duty = 249;
    CCPR1L = duty;
}

void ADC_Init(){
    ADCON1 = 0x80; // RA0/RA1 analog
    ADCON0 = 0x41; // Kanal 0 açık
}

unsigned int ADC_Read(unsigned char channel){
    ADCON0 &= 0xC5;
    ADCON0 |= (channel << 3);
    __delay_ms(2);
    GO_nDONE = 1;
    while (GO_nDONE);
    return ((ADRESH << 8) + ADRESL);
}

void main(){
    UART_Init();
    PWM_Init();
    ADC_Init();

    TRISC0 = 0; // IN1
    TRISC1 = 0; // IN2

    char buffer[40];

    while(1){
        unsigned int x = ADC_Read(0); // Joystick X (yön)
        unsigned int y = ADC_Read(1); // Joystick Y (hız)

        // Yön kontrolü
        if (x > 600){
            IN1 = 1; IN2 = 0;
        } else if (x < 400){
            IN1 = 0; IN2 = 1;
        } else {
            IN1 = 0; IN2 = 0;
        }

        // Hız kontrolü
        unsigned int pwm_value = 0;
        if (y > 600){
            pwm_value = (y - 512) * 249 / 511;
        } else if (y < 400){
            pwm_value = (512 - y) * 249 / 511;
        }

        Set_PWM_Duty(pwm_value);
        unsigned int percent = pwm_value * 100 / 249;

        const char* direction = "Durdu";
        if (IN1 && !IN2) direction = "Ileri";
        else if (IN2 && !IN1) direction = "Geri";

        sprintf(buffer, "YON: %s, HIZ: %u%%\r\n", direction, (percent-27) * (100/54) * (100/15));
        UART_SendString(buffer);
        __delay_ms(500);
    }
}