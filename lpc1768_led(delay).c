#include "lpc17xx.h"

void timer0_delay(void) {
    LPC_TIM0->TCR = (1 << 1);         
    LPC_TIM0->PR = 24999;             
    LPC_TIM0->MR0 = 1000;             
    LPC_TIM0->TCR = (1 << 0);         
    LPC_TIM0->MCR = (1 << 0) | (1 << 1); 

    while (!(LPC_TIM0->IR & (1 << 0))); 
    LPC_TIM0->IR = (1 << 0);            
}

int main(void) {
    LPC_SC->PCONP |= (1 << 1);         

    LPC_GPIO0->FIODIR |= (1 << 22);    
    LPC_GPIO0->FIODIR &= ~(1 << 5);    

    while (1) {
        if (LPC_GPIO0->FIOPIN & (1 << 5)) { 
            LPC_GPIO0->FIOSET = (1 << 22);  
            timer0_delay();                
            LPC_GPIO0->FIOCLR = (1 << 22); 
        } else {
            LPC_GPIO0->FIOCLR = (1 << 22); 
        }
    }
}





