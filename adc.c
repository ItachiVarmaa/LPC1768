#include "LPC17xx.h"
#include <stdio.h>

void UART1_Init(void);
void UART1_SendString(char *str);
void ADC_Init(void);

int main(void) {
    UART1_Init();  // Initialize UART1
    ADC_Init();    // Initialize ADC

    while (1) {
        LPC_ADC->ADCR |= (1 << 24);           // Start ADC conversion
        while (!(LPC_ADC->ADGDR & (1 << 31))); // Wait for conversion to complete

        uint32_t result = (LPC_ADC->ADGDR >> 4) & 0xFFF; // Extract 12-bit result
        char buffer[20];
        sprintf(buffer, "ADC Value: %u\n", result); // Format result as a string
        UART1_SendString(buffer);                  // Send result via UART1

        for (volatile unsigned int i = 0; i <= 60000; i++); // Simple delay
    }
}

void UART1_Init(void) {
    LPC_SC->PCONP |= 0x10;              // Power up UART1 (PCUART1)
    LPC_PINCON->PINSEL4 |= 0x0000000A;  // Configure P2.0 as TXD1 and P2.1 as RXD1
    LPC_UART1->LCR = 0x83;              // Enable DLAB and set 8-bit word length
    LPC_UART1->DLM = 0x00;              // Set baud rate divisor (high byte)
    LPC_UART1->DLL = 0xA2;              // Set baud rate divisor (low byte) for 9600 bps
    LPC_UART1->LCR = 0x03;              // Disable DLAB, 8-bit word length
    LPC_UART1->FCR = 0x07;              // Enable FIFO and clear TX/RX FIFOs
}

void UART1_SendString(char *str) {
    while (*str) {
        while (!(LPC_UART1->LSR & 0x20));  // Wait for THR to be empty
        LPC_UART1->THR = *str++;           // Transmit character
    }
}

void ADC_Init(void) {
    LPC_SC->PCONP |= (1 << 12);            // Power up ADC
    LPC_PINCON->PINSEL1 |= (1 << 16);      // Configure P0.24 as AD0.0
    LPC_SC->PCLKSEL0 &= ~(0x3 << 24);      // Clear ADC clock bits
    LPC_SC->PCLKSEL0 |= (0x0 << 24);       // Set ADC clock to CCLK/4
    LPC_ADC->ADCR = (1 << 1)               // Select AD0.0
                  | (0x0 << 8)             // ADC clock division factor
                  | (1 << 21)              // Enable ADC
                  | (0 << 16);             // Disable Burst mode
}
