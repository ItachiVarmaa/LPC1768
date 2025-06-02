#include<LPC17xx.h>
void timer0_init(void);
int main(){
	LPC_SC->PCONP |=(1<<1);
	LPC_GPIO0->FIODIR|=(1<<5);
	LPC_GPIO0->FIODIR&=~(1<<4);
	while(1){
		if (!(LPC_GPIO0->FIOPIN&(1<<4))){
	  LPC_GPIO0->FIOSET = (1 << 5); 
		timer0_delay();
		LPC_GPIO0->FIOCLR |=(1<<5);
	}
	else {
		LPC_GPIO0->FIOCLR |=(1<<5);
	}}}
void timer0_delay(){
	LPC_TIM0->TCR=(1<<1);
	LPC_TIM0->PR=24999;
	LPC_TIM0->MR0=5000;
	LPC_TIM0->TCR=(1<<0);
	LPC_TIM0->MCR=(1<<0)|(1<<1);
	while(!(LPC_TIM0->IR&(1<<0)));
		LPC_TIM0->IR=(1<<0);
}