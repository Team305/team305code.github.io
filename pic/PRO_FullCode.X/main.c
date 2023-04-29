/* PROJECT: PRO_FullCode.X
 * EGR314 6PM Team 305 Microcontroller Code for Weather Station Project
 * Mark Alvarez, Tristan Dunton, Kevin Hays, Aaron Karsten
 * Professor: Travis Kelley
 * Main author: Kevin Hays
 * Supporting authors: Tristan Dunton, Mark Alvarez, Aaron Karsten
 * Last Rev. 04/23/2023
 */

/*                                                                         TODO:
 * Ask Kelley about Timestamps over MQTT-----------------------------------------Call to something? Server maybe?
 * Ask Kelley how to push to different topics with ESP---------------------------Change variable in async mqtt.py
 * Ask Kelley how to check if the data was sent----------------------------------Unknown
*/

#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/examples/i2c2_master_example.h"
#include "mcc_generated_files/i2c2_master.h"
#include "stdio.h"
#include <stdint.h>
#include <stdbool.h>
#include <xc.h>
#include <stdlib.h>

//Check if sensors and motor are readable
int CheckSystem(void){
    //Initialize variables
    uint8_t tempCheck, lightCheck, errorCode;
    char motorData[100];
    
    //Reads anything from the sensors to make sure they're connected
    tempCheck = I2C2_Read1ByteRegister(0x4C, 0x00);
    lightCheck = I2C2_Read1ByteRegister(0x39, 0x5A);
    
    //Reads anything from the motor to make sure it's connected
    CSN1_SetLow();
    __delay_ms(10);
    motorData[0] = 0b11001101;
    SPI1_ExchangeBlock(motorData, 1);
    __delay_ms(100);
    CSN1_SetHigh();
    
    //Returns errors if nothing was read!
    if (tempCheck == 0){
        printf("Error Code 8!");
        errorCode = errorCode + 8;
    }
    if (lightCheck == 0){
        printf("Error Code 9!");
        errorCode = errorCode + 9;
    }
    if (motorData[0] != 0b11001101){
        printf("Error Code 7!");
        errorCode = errorCode + 7;
    }
    switch(errorCode){
        case 7:
            printf("Error Code 7!");
            break;
        case 8:
            printf("Error Code 8!");
            break;   
        case 9:
            printf("Error Code 9!");
            break;   
        case 15:
            printf("Error Codes 7 & 8!");
            break; 
        case 16:
            printf("Error Codes 7 & 9!");
            break; 
        case 17:
            printf("Error Codes 8 & 9!");
            break; 
        case 24:
            printf("Error Codes 7, 8, & 9!");
            break; 
    }
    
    return errorCode;
}

void Enable_LS(void){
    I2C2_Write1ByteRegister(0x39,0x80, 0b00011011);   //enable device function       
}

//Runs the motor forward (Steps 1 & 3)
void MotorForward(void){
    char data[100];
    
    CSN1_SetHigh();
    SPI1_Open(SPI1_DEFAULT);
    //Forward
        CSN1_SetLow();
        __delay_ms(10);
        data[0]=0b11111111;
        SPI1_ExchangeBlock(data, 1);
        __delay_ms(1000);
        CSN1_SetHigh();
    //printf("Forward!\r");
    
    //Stop
        CSN1_SetLow();
        __delay_ms(10);
        data[0]=0b11101101;
        SPI1_ExchangeBlock(data, 1);
        __delay_ms(1000);
        CSN1_SetHigh();
    //printf("STOP!!\r");
}

//Runs the Motor Backwards (Step 2)
void MotorReverse(void){
    char data[100];

    //Reverse
    for(int i; i < 20; i++){
        CSN1_SetLow();
        __delay_ms(10);
        data[0]=0b1111110;
        SPI1_ExchangeBlock(data, 1);
        __delay_ms(100);
        CSN1_SetHigh();
    }
    //printf("Reverse!!!\r");
    //__delay_ms(1000);
    
    //Stop
    for(int i; i < 10; i++){
        CSN1_SetLow();
        __delay_ms(10);
        data[0]=0b11101101;
        SPI1_ExchangeBlock(data, 1);
        __delay_ms(100);
        CSN1_SetHigh();
    }
    //printf("STOP!!!!!!\r");
    //__delay_ms(1000);
}


//PID Error checker
int ErrorCheck(uint8_t tAvg){
    uint8_t tOrigin = I2C2_Read1ByteRegister(0x4C, 0x00);
    uint8_t pidError = 0, t0 = 0, t1 = 0;
    //Calculates the error. If it's greater than 2 degrees C, Run a test again.
    if(abs(tOrigin - tAvg) > 2){
        //Motor goes forward
        MotorForward();
                
        //Reads the sensors
        t0 = I2C2_Read1ByteRegister(0x4C, 0x00);
                
        //Motor goes backwards
        MotorReverse();
                
        //Reads the sensor
        t1 = I2C2_Read1ByteRegister(0x4C, 0x00);
                
        //Motor goes forwards to the origin
        MotorForward();
                
        //Averages the temperatures
        tAvg = ((t0 + t1) / 2);
        pidError = abs(tOrigin - tAvg);
        printf("Recorded Temperature: %d; Temperature Error: %d; ", tAvg, pidError);
    }
    else{
        printf("Recorded Temperature: %d; ", tAvg);
    }
    return pidError;
}

void EUSART2Transmit(void){
    EUSART2_Receive_ISR();
    
    volatile uint8_t unifData;
    
    if(EUSART2_is_rx_ready()){
        unifData = EUSART2_Read();
        if(EUSART3_is_tx_ready()){
            EUSART3_Write(unifData);
        }
        if(EUSART3_is_tx_done()){ 
        }
        //LED_0_Toggle();
    }
}

void EUSART3Transmit(void){
    EUSART3_Receive_ISR();
    
    volatile uint8_t unixData;
    
    if(EUSART3_is_rx_ready()){
        unixData = EUSART3_Read();
        if(EUSART2_is_tx_ready()){
            EUSART2_Write(unixData);
        }
        if(EUSART2_is_tx_done()){
        }
    }
}

//Main Loop
void main(void)
{
    // Initialize the device
    SYSTEM_Initialize();
    EUSART2_SetRxInterruptHandler(EUSART2Transmit);
    EUSART3_SetRxInterruptHandler(EUSART3Transmit);
    Enable_LS();
    
    // Enable Interrupts
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
 
    //Initialize variables
    uint8_t t0, t1, tAvg, lightData[2], lightBytes[2];
    uint16_t L0, L1, LAvg;
    int runTimer = 0;
    int runTimeTotal = 0;
    int errorCode = 0;
    int pidError = 0;
    int systemOK = 1;
    float timeBuffer;
    uint8_t ii = 0;
    while (1)
    {
        t0 = I2C2_Read1ByteRegister(0x4C, 0x00);
        lightBytes[0] = I2C2_Read1ByteRegister(0x39, 0x95);  // data low bytes
        lightBytes[1] = I2C2_Read1ByteRegister(0x39, 0x96); //data high bytes
        lightData[0] = I2C2_Read1ByteRegister(0x39, 0xAD);  // data low bytes, low light 
        lightData[1] = I2C2_Read1ByteRegister(0x39, 0xAE);  // data high bytes, high light level
        L0 = lightData[0] << 8 | lightData[1];
        ii++;
        printf("ii: %d; Temp: %d; Light %u; \r", ii, t0, L0);
        __delay_ms(1000);
        if(runTimer == 0){
            errorCode = CheckSystem();
            if(errorCode == 0){
                //Motor goes forward
                MotorForward();
                
                //Reads the sensors
                t0 = I2C2_Read1ByteRegister(0x4C, 0x00);
                lightBytes[0] = I2C2_Read1ByteRegister(0x39, 0x95);  // data low bytes
                lightBytes[1] = I2C2_Read1ByteRegister(0x39, 0x96); //data high bytes
                lightData[0] = I2C2_Read1ByteRegister(0x39, 0xAD);  // data low bytes, low light 
                lightData[1] = I2C2_Read1ByteRegister(0x39, 0xAE);  // data high bytes, high light level
                L0 = lightData[0] << 8 | lightData[1];
                
                //Motor goes backwards
                MotorReverse();
                
                //Reads the sensors
                t1 = I2C2_Read1ByteRegister(0x4C, 0x00);
                lightBytes[0] = I2C2_Read1ByteRegister(0x39, 0x95);  // data low bytes
                lightBytes[1] = I2C2_Read1ByteRegister(0x39, 0x96); //data high bytes
                lightData[0] = I2C2_Read1ByteRegister(0x39, 0xAD);  // data low bytes, low light 
                lightData[1] = I2C2_Read1ByteRegister(0x39, 0xAE);  // data high bytes, high light level
                L1 = lightData[0] << 8 | lightData[1];
                
                //Motor goes forwards to the origin
                MotorForward();
                
                //Averages the temperatures and light
                tAvg = ((t0 + t1) / 2);
                LAvg = ((L0 + L1) / 2);
                
                //Checks the error for the temperature. If it's too high, run the tests for temperature again to confirm
                pidError = ErrorCheck(tAvg);
                
                printf("Light Sensed: %d nm; \r", LAvg);
            }
            else{
                systemOK = 0;
            }
            runTimer++;
            runTimeTotal = runTimeTotal + runTimer;
            timeBuffer = runTimeTotal / 10.0;
            printf("Total Run Time: %f s.\r", timeBuffer);
        }
        else if(runTimer >= 18000){
            runTimer = 0;
        }
        else{
            runTimer++;
        }
        __delay_ms(100);
    }
}

/**
 End of File
*/
