/**
  EUSART3 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    eusart3.c

  @Summary
    This is the generated driver implementation file for the EUSART3 driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This source file provides APIs for EUSART3.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.8
        Device            :  PIC18LF67K40
        Driver Version    :  2.1.1
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.36 and above
        MPLAB 	          :  MPLAB X 6.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

/**
  Section: Included Files
*/
#include "eusart3.h"

/**
  Section: Macro Declarations
*/

#define EUSART3_TX_BUFFER_SIZE 8
#define EUSART3_RX_BUFFER_SIZE 8

/**
  Section: Global Variables
*/
volatile uint8_t eusart3TxHead = 0;
volatile uint8_t eusart3TxTail = 0;
volatile uint8_t eusart3TxBuffer[EUSART3_TX_BUFFER_SIZE];
volatile uint8_t eusart3TxBufferRemaining;

volatile uint8_t eusart3RxHead = 0;
volatile uint8_t eusart3RxTail = 0;
volatile uint8_t eusart3RxBuffer[EUSART3_RX_BUFFER_SIZE];
volatile eusart3_status_t eusart3RxStatusBuffer[EUSART3_RX_BUFFER_SIZE];
volatile uint8_t eusart3RxCount;
volatile eusart3_status_t eusart3RxLastError;

/**
  Section: EUSART3 APIs
*/
void (*EUSART3_TxDefaultInterruptHandler)(void);
void (*EUSART3_RxDefaultInterruptHandler)(void);

void (*EUSART3_FramingErrorHandler)(void);
void (*EUSART3_OverrunErrorHandler)(void);
void (*EUSART3_ErrorHandler)(void);

void EUSART3_DefaultFramingErrorHandler(void);
void EUSART3_DefaultOverrunErrorHandler(void);
void EUSART3_DefaultErrorHandler(void);

void EUSART3_Initialize(void)
{
    // disable interrupts before changing states
    PIE4bits.RC3IE = 0;
    EUSART3_SetRxInterruptHandler(EUSART3_Receive_ISR);
    PIE4bits.TX3IE = 0;
    EUSART3_SetTxInterruptHandler(EUSART3_Transmit_ISR);
    // Set the EUSART3 module to the options selected in the user interface.

    // ABDOVF no_overflow; SCKP Non-Inverted; BRG16 16bit_generator; WUE disabled; ABDEN disabled; 
    BAUD3CON = 0x08;

    // SPEN enabled; RX9 8-bit; CREN enabled; ADDEN disabled; SREN disabled; 
    RC3STA = 0x90;

    // TX9 8-bit; TX9D 0; SENDB sync_break_complete; TXEN enabled; SYNC asynchronous; BRGH hi_speed; CSRC slave; 
    TX3STA = 0x24;

    // SP3BRGL 25; 
    SP3BRGL = 0x19;

    // SP3BRGH 0; 
    SP3BRGH = 0x00;


    EUSART3_SetFramingErrorHandler(EUSART3_DefaultFramingErrorHandler);
    EUSART3_SetOverrunErrorHandler(EUSART3_DefaultOverrunErrorHandler);
    EUSART3_SetErrorHandler(EUSART3_DefaultErrorHandler);

    eusart3RxLastError.status = 0;

    // initializing the driver state
    eusart3TxHead = 0;
    eusart3TxTail = 0;
    eusart3TxBufferRemaining = sizeof(eusart3TxBuffer);

    eusart3RxHead = 0;
    eusart3RxTail = 0;
    eusart3RxCount = 0;

    // enable receive interrupt
    PIE4bits.RC3IE = 1;
}

bool EUSART3_is_tx_ready(void)
{
    return (eusart3TxBufferRemaining ? true : false);
}

bool EUSART3_is_rx_ready(void)
{
    return (eusart3RxCount ? true : false);
}

bool EUSART3_is_tx_done(void)
{
    return TX3STAbits.TRMT;
}

eusart3_status_t EUSART3_get_last_status(void){
    return eusart3RxLastError;
}

uint8_t EUSART3_Read(void)
{
    uint8_t readValue  = 0;
    
    while(0 == eusart3RxCount)
    {
    }

    eusart3RxLastError = eusart3RxStatusBuffer[eusart3RxTail];

    readValue = eusart3RxBuffer[eusart3RxTail++];
    if(sizeof(eusart3RxBuffer) <= eusart3RxTail)
    {
        eusart3RxTail = 0;
    }
    PIE4bits.RC3IE = 0;
    eusart3RxCount--;
    PIE4bits.RC3IE = 1;

    return readValue;
}

void EUSART3_Write(uint8_t txData)
{
    while(0 == eusart3TxBufferRemaining)
    {
    }

    if(0 == PIE4bits.TX3IE)
    {
        TX3REG = txData;
    }
    else
    {
        PIE4bits.TX3IE = 0;
        eusart3TxBuffer[eusart3TxHead++] = txData;
        if(sizeof(eusart3TxBuffer) <= eusart3TxHead)
        {
            eusart3TxHead = 0;
        }
        eusart3TxBufferRemaining--;
    }
    PIE4bits.TX3IE = 1;
}

char getch(void)
{
    return EUSART3_Read();
}

void putch(char txData)
{
    EUSART3_Write(txData);
}

void EUSART3_Transmit_ISR(void)
{

    // add your EUSART3 interrupt custom code
    if(sizeof(eusart3TxBuffer) > eusart3TxBufferRemaining)
    {
        TX3REG = eusart3TxBuffer[eusart3TxTail++];
        if(sizeof(eusart3TxBuffer) <= eusart3TxTail)
        {
            eusart3TxTail = 0;
        }
        eusart3TxBufferRemaining++;
    }
    else
    {
        PIE4bits.TX3IE = 0;
    }
}

void EUSART3_Receive_ISR(void)
{
    
    eusart3RxStatusBuffer[eusart3RxHead].status = 0;

    if(RC3STAbits.FERR){
        eusart3RxStatusBuffer[eusart3RxHead].ferr = 1;
        EUSART3_FramingErrorHandler();
    }

    if(RC3STAbits.OERR){
        eusart3RxStatusBuffer[eusart3RxHead].oerr = 1;
        EUSART3_OverrunErrorHandler();
    }
    
    if(eusart3RxStatusBuffer[eusart3RxHead].status){
        EUSART3_ErrorHandler();
    } else {
        EUSART3_RxDataHandler();
    }
    
    // or set custom function using EUSART3_SetRxInterruptHandler()
}

void EUSART3_RxDataHandler(void){
    // use this default receive interrupt handler code
    eusart3RxBuffer[eusart3RxHead++] = RC3REG;
    if(sizeof(eusart3RxBuffer) <= eusart3RxHead)
    {
        eusart3RxHead = 0;
    }
    eusart3RxCount++;
}

void EUSART3_DefaultFramingErrorHandler(void){}

void EUSART3_DefaultOverrunErrorHandler(void){
    // EUSART3 error - restart

    RC3STAbits.CREN = 0;
    RC3STAbits.CREN = 1;

}

void EUSART3_DefaultErrorHandler(void){
    EUSART3_RxDataHandler();
}

void EUSART3_SetFramingErrorHandler(void (* interruptHandler)(void)){
    EUSART3_FramingErrorHandler = interruptHandler;
}

void EUSART3_SetOverrunErrorHandler(void (* interruptHandler)(void)){
    EUSART3_OverrunErrorHandler = interruptHandler;
}

void EUSART3_SetErrorHandler(void (* interruptHandler)(void)){
    EUSART3_ErrorHandler = interruptHandler;
}

void EUSART3_SetTxInterruptHandler(void (* interruptHandler)(void)){
    EUSART3_TxDefaultInterruptHandler = interruptHandler;
}

void EUSART3_SetRxInterruptHandler(void (* interruptHandler)(void)){
    EUSART3_RxDefaultInterruptHandler = interruptHandler;
}
/**
  End of File
*/
