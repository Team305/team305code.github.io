/* PROJECT: PRO_FullCode.X
 * EGR314 6PM Team 305 Microcontroller Code for Weather Station Project -- DEMONSTRATION CODE
 * Mark Alvarez, Tristan Dunton, Kevin Hays, Aaron Karsten
 * Professor: Travis Kelley
 * Main author: Tristan Dunton
 * Supporting authors: Kevin Hays
 * Last Rev. 04/28/2023
 */

#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/examples/i2c2_master_example.h"
#include "mcc_generated_files/i2c2_master.h"
#include "stdio.h"
/*
                         Main application
 */
void Enable_LS(void);

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
    // add your EUSART2 interrupt custom code
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

void main(void){
    // Initialize the device
    SYSTEM_Initialize();
    EUSART2_SetRxInterruptHandler(EUSART2Transmit);
    EUSART3_SetRxInterruptHandler(EUSART3Transmit);
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
    
    I2C2_Initialize();
    uint8_t data[2];
    uint8_t data1[2];
    uint16_t output;
    uint8_t ID;
          
    Enable_LS();
    
    while (1)
    {
        data1[0] = I2C2_Read1ByteRegister(0x39, 0x95);  // data low bytes
        data1[1] = I2C2_Read1ByteRegister(0x39, 0x96);
        
        data[0] = I2C2_Read1ByteRegister(0x39, 0xAD);  // data low bytes
        data[1] = I2C2_Read1ByteRegister(0x39, 0xAE);  // data high bytes
        ID = I2C2_Read1ByteRegister(0x39, 0x5A);  // ID register
        
             printf("Alive = %u \n \r", ID);
             
        output = data[0] << 8 | data[1];
        printf("Data = %u \n \r", output);
        __delay_ms(100);
        
      //if (ID = (1))
          //IO_RE0_Toggle();
          // __delay_(500);
      
            
        
    }
}
void Enable_LS(void){
    I2C2_Write1ByteRegister(0x39,0x80, 0b00011011);   //enable device
}