// EdgeTrigger.c
// Trang Do
// Last Modified: 5/3/2022
// Lab 10: Flappy Bird Game
// Runs on TM4C123 or LM4F120
// Use edge triggered interrupts to read PF0 and PF4


#include "../inc/tm4c123gh6pm.h"
#include <stdint.h>
#include "Sound.h"


// ********************PortF_Switches_Init********************
// Initialize PortF SW1 (PF4) and SW2 (PF0) as Digital Inputs
// Inputs: None
// Outputs: None

void PortF_Switches_Init(void){volatile long delay;                            
  SYSCTL_RCGCGPIO_R |= 0x20;          	 // 1) activate Port F clock
  while((SYSCTL_PRGPIO_R&0x20)==0){}; 	 //    allow time for clock to start
  delay = 100;                        	 //    allow time to finish activating
  GPIO_PORTF_LOCK_R = 0x4C4F434B;     	 // 2) unlock GPIO Port F (for PF0)
  GPIO_PORTF_CR_R = 0x1F;           		 // 	 allow changes to PF4-0                              
  GPIO_PORTF_AMSEL_R &= ~0x11;  				 // 3) disable analog on PF4,0							 
  GPIO_PORTF_PCTL_R &= ~0x000F000F;			 // 4) configure PF4,0 as GPIO
  GPIO_PORTF_DIR_R &= ~0x11;    				 // 5) make PF4,0 inputs
  GPIO_PORTF_AFSEL_R &= ~0x11;  				 // 6) disable alt funct on PF4,0
	GPIO_PORTF_PUR_R |= 0x11;     				 //    enable pull-up on PF0 and PF4 (negative logic switches)
  GPIO_PORTF_DEN_R |= 0x11;     				 // 7) enable digital I/O on PF4,0
	};


// ********************PortF_EdgeTrigInt_Init********************
// Initialize Port F Switches as Edge Trigger Interrupt 
// Interrupt routine is void GPIOPortF_Handler()
// Inputs: None
// Outputs: None
void PortF_EdgeTrig_Init(void) {volatile long delay;
	SYSCTL_RCGCGPIO_R |= 0x20;          	 // 1) activate Port F clock
  while((SYSCTL_PRGPIO_R&0x20)==0){}; 	 //    allow time for clock to start
  delay = 100;                        	 //    allow time to finish activating
  GPIO_PORTF_LOCK_R = 0x4C4F434B;     	 // 2) unlock GPIO Port F (for PF0)
  GPIO_PORTF_CR_R = 0x1F;           		 // 	 allow changes to PF4-0                              
  GPIO_PORTF_AMSEL_R &= ~0x11;  				 // 3) disable analog on PF4,0							 
  GPIO_PORTF_PCTL_R &= ~0x000F000F;			 // 4) configure PF4,0 as GPIO
  GPIO_PORTF_DIR_R &= ~0x11;    				 // 5) make PF4,0 inputs
  GPIO_PORTF_AFSEL_R &= ~0x11;  				 // 6) disable alt funct on PF4,0
	GPIO_PORTF_PUR_R |= 0x11;     				 //    enable pull-up on PF0 and PF4 (negative logic switches)
  GPIO_PORTF_DEN_R |= 0x11;     				 // 7) enable digital I/O on PF4,0
  GPIO_PORTF_IS_R &= ~0x11;     					//    PF4,0 is edge-sensitive
  GPIO_PORTF_IBE_R &= ~0x11;    					//    PF4,0 is not both edges
  GPIO_PORTF_IEV_R &= ~0x11;     					//    PF4,0 falling edge event (Neg logic)
  GPIO_PORTF_ICR_R = 0x11;      					//    clear flag1-0
  GPIO_PORTF_IM_R |= 0x11;      					// 8) arm interrupt on PF4,0
																					// 9) GPIOF priority 1
  NVIC_PRI7_R = (NVIC_PRI7_R&0xFF0FFFFF)|0x00200000;
  NVIC_EN0_R |= 1<<30;   									// 10)enable interrupt 30 in NVIC
}


// ********************PortF_EdgeTrig_Stop********************
// Disable PortF Edge Trigger Interrupt
// Input: none
// Output: none
void PortF_EdgeTrig_Stop(void) {
	NVIC_EN0_R &= ~(1<<30);
}


// ********************PortF_EdgeTrig_Start********************
// Enable PortF Edge Trigger Interrupt
// Input: none
// Output: none
void PortF_EdgeTrig_Start(void) {
	NVIC_EN0_R |= 1<<30;
}

