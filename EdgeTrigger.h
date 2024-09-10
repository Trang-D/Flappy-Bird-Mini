// EdgeTrigger.h
// Trang Do
// Last Modified: 5/3/2022
// Lab 10: Flappy Bird Game
// Runs on TM4C123 or LM4F120
// Use edge triggered interrupts to read PF0 and PF4

#ifndef __EdgeTrigger_h
#define __EdgeTrigger_h
#include <stdint.h>

// ********************PortF_Switches_Init********************
// Initialize PortF SW1 (PF4) and SW2 (PF0) as Digital Inputs
// Inputs: None
// Outputs: None
void PortF_Switches_Init(void);


// ********************PortF_EdgeTrig_Init********************
// Initialize Port F Switches as Edge Trigger Interrupt 
// Inputs: None
// Outputs: None
void PortF_EdgeTrig_Init(void);


// ********************PortF_EdgeTrig_Stop********************
// Disable PortF Edge Trigger Interrupt
// Input: none
// Output: none
void PortF_EdgeTrig_Stop(void);


// ********************PortF_EdgeTrig_Start********************
// Enable PortF Edge Trigger Interrupt
// Input: none
// Output: none
void PortF_EdgeTrig_Start(void);


#endif /*__EdgeTrigger_h */

