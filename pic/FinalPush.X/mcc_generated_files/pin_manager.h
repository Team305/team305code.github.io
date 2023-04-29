/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for .
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.8
        Device            :  PIC18LF67K40
        Driver Version    :  2.11
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

#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

/**
  Section: Included Files
*/

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set RD5 procedures
#define RD5_SetHigh()            do { LATDbits.LATD5 = 1; } while(0)
#define RD5_SetLow()             do { LATDbits.LATD5 = 0; } while(0)
#define RD5_Toggle()             do { LATDbits.LATD5 = ~LATDbits.LATD5; } while(0)
#define RD5_GetValue()              PORTDbits.RD5
#define RD5_SetDigitalInput()    do { TRISDbits.TRISD5 = 1; } while(0)
#define RD5_SetDigitalOutput()   do { TRISDbits.TRISD5 = 0; } while(0)
#define RD5_SetPullup()             do { WPUDbits.WPUD5 = 1; } while(0)
#define RD5_ResetPullup()           do { WPUDbits.WPUD5 = 0; } while(0)
#define RD5_SetAnalogMode()         do { ANSELDbits.ANSELD5 = 1; } while(0)
#define RD5_SetDigitalMode()        do { ANSELDbits.ANSELD5 = 0; } while(0)

// get/set RD6 procedures
#define RD6_SetHigh()            do { LATDbits.LATD6 = 1; } while(0)
#define RD6_SetLow()             do { LATDbits.LATD6 = 0; } while(0)
#define RD6_Toggle()             do { LATDbits.LATD6 = ~LATDbits.LATD6; } while(0)
#define RD6_GetValue()              PORTDbits.RD6
#define RD6_SetDigitalInput()    do { TRISDbits.TRISD6 = 1; } while(0)
#define RD6_SetDigitalOutput()   do { TRISDbits.TRISD6 = 0; } while(0)
#define RD6_SetPullup()             do { WPUDbits.WPUD6 = 1; } while(0)
#define RD6_ResetPullup()           do { WPUDbits.WPUD6 = 0; } while(0)
#define RD6_SetAnalogMode()         do { ANSELDbits.ANSELD6 = 1; } while(0)
#define RD6_SetDigitalMode()        do { ANSELDbits.ANSELD6 = 0; } while(0)

// get/set RE0 procedures
#define RE0_SetHigh()            do { LATEbits.LATE0 = 1; } while(0)
#define RE0_SetLow()             do { LATEbits.LATE0 = 0; } while(0)
#define RE0_Toggle()             do { LATEbits.LATE0 = ~LATEbits.LATE0; } while(0)
#define RE0_GetValue()              PORTEbits.RE0
#define RE0_SetDigitalInput()    do { TRISEbits.TRISE0 = 1; } while(0)
#define RE0_SetDigitalOutput()   do { TRISEbits.TRISE0 = 0; } while(0)
#define RE0_SetPullup()             do { WPUEbits.WPUE0 = 1; } while(0)
#define RE0_ResetPullup()           do { WPUEbits.WPUE0 = 0; } while(0)
#define RE0_SetAnalogMode()         do { ANSELEbits.ANSELE0 = 1; } while(0)
#define RE0_SetDigitalMode()        do { ANSELEbits.ANSELE0 = 0; } while(0)

// get/set RE1 procedures
#define RE1_SetHigh()            do { LATEbits.LATE1 = 1; } while(0)
#define RE1_SetLow()             do { LATEbits.LATE1 = 0; } while(0)
#define RE1_Toggle()             do { LATEbits.LATE1 = ~LATEbits.LATE1; } while(0)
#define RE1_GetValue()              PORTEbits.RE1
#define RE1_SetDigitalInput()    do { TRISEbits.TRISE1 = 1; } while(0)
#define RE1_SetDigitalOutput()   do { TRISEbits.TRISE1 = 0; } while(0)
#define RE1_SetPullup()             do { WPUEbits.WPUE1 = 1; } while(0)
#define RE1_ResetPullup()           do { WPUEbits.WPUE1 = 0; } while(0)
#define RE1_SetAnalogMode()         do { ANSELEbits.ANSELE1 = 1; } while(0)
#define RE1_SetDigitalMode()        do { ANSELEbits.ANSELE1 = 0; } while(0)

// get/set RG1 procedures
#define RG1_SetHigh()            do { LATGbits.LATG1 = 1; } while(0)
#define RG1_SetLow()             do { LATGbits.LATG1 = 0; } while(0)
#define RG1_Toggle()             do { LATGbits.LATG1 = ~LATGbits.LATG1; } while(0)
#define RG1_GetValue()              PORTGbits.RG1
#define RG1_SetDigitalInput()    do { TRISGbits.TRISG1 = 1; } while(0)
#define RG1_SetDigitalOutput()   do { TRISGbits.TRISG1 = 0; } while(0)
#define RG1_SetPullup()             do { WPUGbits.WPUG1 = 1; } while(0)
#define RG1_ResetPullup()           do { WPUGbits.WPUG1 = 0; } while(0)
#define RG1_SetAnalogMode()         do { ANSELGbits.ANSELG1 = 1; } while(0)
#define RG1_SetDigitalMode()        do { ANSELGbits.ANSELG1 = 0; } while(0)

// get/set RG2 procedures
#define RG2_SetHigh()            do { LATGbits.LATG2 = 1; } while(0)
#define RG2_SetLow()             do { LATGbits.LATG2 = 0; } while(0)
#define RG2_Toggle()             do { LATGbits.LATG2 = ~LATGbits.LATG2; } while(0)
#define RG2_GetValue()              PORTGbits.RG2
#define RG2_SetDigitalInput()    do { TRISGbits.TRISG2 = 1; } while(0)
#define RG2_SetDigitalOutput()   do { TRISGbits.TRISG2 = 0; } while(0)
#define RG2_SetPullup()             do { WPUGbits.WPUG2 = 1; } while(0)
#define RG2_ResetPullup()           do { WPUGbits.WPUG2 = 0; } while(0)
#define RG2_SetAnalogMode()         do { ANSELGbits.ANSELG2 = 1; } while(0)
#define RG2_SetDigitalMode()        do { ANSELGbits.ANSELG2 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/