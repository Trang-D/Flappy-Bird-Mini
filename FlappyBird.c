// FlappyBird.c
// Runs on TM4C123
// Trang Do
// This is a starter project for the EE319K Lab 10
// Last Modified: 5/3/2022 

// ********************Flappy Bird********************
// Game: Flappy Bird
// Two Languages: English (SW2) and Spanish (SW1)
// Control the bird with the slide potentiometer
// 2 Buttons: SW1 - Move the bird forward
// 						SW2 - Pause the Game

// ******* Possible Hardware I/O connections*******************
// Slide pot pin 1 connected to ground (top of 2 left pins)
// Slide pot pin 2 connected to PD2/AIN5 (bottom of left pins)
// Slide pot pin 3 connected to +3.3V (right pin)
// 32*R resistor DAC bit 0 on PB0 (least significant bit)
// 16*R resistor DAC bit 1 on PB1
// 8*R resistor DAC bit 2 on PB2 
// 4*R resistor DAC bit 3 on PB3
// 2*R resistor DAC bit 4 on PB4
// 1*R resistor DAC bit 5 on PB5 (most significant bit)
// **********HiLetgo ST7735 TFT and SDC (SDC not tested)*******************
// ST7735
// LED-   (pin 16) TFT, connected to ground
// LED+   (pin 15) TFT, connected to +3.3 V
// SD_CS  (pin 14) SDC, chip select
// MOSI   (pin 13) SDC, MOSI
// MISO   (pin 12) SDC, MISO
// SCK    (pin 11) SDC, serial clock
// CS     (pin 10) TFT, PA3 (SSI0Fss)
// SCL    (pin 9)  TFT, SCK  PA2 (SSI0Clk)
// SDA    (pin 8)  TFT, MOSI PA5 (SSI0Tx)
// A0     (pin 7)  TFT, Data/Command PA6 (GPIO), high for data, low for command
// RESET  (pin 6)  TFT, to PA7 (GPIO)
// NC     (pins 3,4,5) not connected
// VCC    (pin 2)  connected to +3.3 V
// GND    (pin 1)  connected to ground



#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "ST7735.h"
#include "Print.h"
#include "Random.h"
#include "TExaS.h"
#include "ADC.h"
#include "Images.h"
#include "Sound.h"
#include "Timer0.h"
#include "Timer1.h"
#include "FlappyBirdImage.h"
#include "DAC.h"
#include "EdgeTrigger.h"
#include "Phrases.h"


#define ROWS 3
#define COLS 3

uint32_t Mail;
uint32_t Status;


uint32_t array[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}; 

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Delay100ms(uint32_t count); // time delay in 0.1 seconds
//typedef enum { False, True} bool;
//typedef enum {happy, sad, angry} state_t;
	
//struct Sprite{
	//int16_t x,y;
	//uint8_t w,h;
	//state_t mood;
//};
	
	//typedef struct Sprite Sprite_t;

//Sprite_t dude = {50,50, 32, 32, happy};
//void Draw(Sprite_t p);
//uint16_t *Cool[2] = {};	
//int8_t dx =1, dy =1;
//bool redraw = False;

//initialize buttons//pe3-0  <<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>

void PortE_Init(void){
	
	SYSCTL_RCGCGPIO_R |= 0x10;  // 0001 0000    
	
	volatile int delay = 0;
	
	delay = delay +1; 
	delay = delay +1; 
	
	GPIO_PORTE_DIR_R &= ~0x0F;   //0000 11111
  GPIO_PORTE_DEN_R |= 0x0F;   // 
}

void Timer3A_Init(uint32_t period, uint32_t priority){
  volatile uint32_t delay;
  SYSCTL_RCGCTIMER_R |= 0x08;   // 0) activate TIMER3
  delay = SYSCTL_RCGCTIMER_R;         // user function
  TIMER3_CTL_R = 0x00000000;    // 1) disable timer2A during setup
  TIMER3_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER3_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER3_TAILR_R = period-1;    // 4) reload value
  TIMER3_TAPR_R = 0;            // 5) bus clock resolution
  TIMER3_ICR_R = 0x00000001;    // 6) clear timer2A timeout flag
  TIMER3_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI8_R = (NVIC_PRI8_R&0x00FFFFFF)|(priority<<29); // priority  
// interrupts enabled in the main program after all devices initialized
// vector number 39, interrupt number 23
  NVIC_EN1_R = 1<<(35-32);      // 9) enable IRQ 35 in NVIC
  TIMER3_CTL_R = 0x00000001;    // 10) enable timer3A
}

void Timer3A_Stop(void){
  NVIC_DIS1_R = 1<<(35-32);   // 9) disable interrupt 35 in NVIC
  TIMER3_CTL_R = 0x00000000;  // 10) disable timer3
}


uint32_t Data; // 0 to 4095
uint32_t Flag; // 1 means new data

void Timer3A_Handler(void){
  TIMER3_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER2A timeout
  // write this
  GPIO_PORTF_DATA_R ^= 0x04; // toggle PD2
  Mail = ADC_In();      // Sample ADC
  Status = 1;    //flag to see if there is new data to read
}

uint32_t Convert(uint32_t x){

	uint32_t position =0;
//m = 0.0004x + 0.3054
  position = (1638 * x)/4096+305; 
  return position;
}

void PortF_Init(void){
  volatile int delay;
  SYSCTL_RCGCGPIO_R |= 0x20;
  delay = SYSCTL_RCGCGPIO_R;
  GPIO_PORTF_DIR_R |= 0x0E;
  GPIO_PORTF_DEN_R |= 0x0E;
}


int32_t coordinates(int32_t x, int32_t y){
	return 128*(159-y) + x; 
}
//int display_lines(void) {
	
	
	// draw one vertical line
	/*
	// draw one horizontal line
	ST7735_DrawFastHLine(0, 50, 150, ST7735_RED);
	ST7735_DrawFastHLine(0, 110, 150, ST7735_RED);
	ST7735_DrawFastVLine(40, 0, 160, ST7735_RED);
	ST7735_DrawFastVLine(90, 0, 160, ST7735_RED);

	// empty loop

	
	return 0;
}
*/
uint32_t Mail;
uint32_t Status;

int main (void) {
}	
	//void Move(void)s;
int main1(void){ 
	char l;
	// dummy main. Comment this out for your actual game to work
	DisableInterrupts();
	TExaS_Init(SCOPE);
	PortF_Init();
	PortE_Init(); 
	//Sound_Init();
	ADC_Init();
	Timer3A_Init(8000000,0);
	Output_Init();
	EnableInterrupts();
  ST7735_FillScreen(ST7735_BLACK); 
	
	
	//DisableInterrupts();
 // TExaS_Init(NONE);       // Bus clock is 80 MHz 
  
	
	
	//Draw(dude);
	//Timer1_Init(&Move,80000000/10);
//`q		EnableInterrupts();
	
	
	// GRID PIXEL Values
	
	//Grid #1 : 
	
	//START OF GAME (PSEUDOCODE)   <<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>
	//  1) Ask for English or Spanish   (slideplot 0 -> English   1.9 --> Spanish)
	//  2) Display Instruction [In that Language]
	//  3) Start Game
					//use buttons to find the exact location of where to put the x or o
					//four buttons for four directions
					// set boundaries : pixels (if statements with pixel)
							//buttons cannot move beyond that point	
					// need one button for each player (one button [a]- X; one button [b] - O)
	
	//Find location in the Grid
	//logic: Find the pixel location for each grid (need nine different values) [AREA? 
			// assume that the players switch and the buttons are switched 
			//if the selected location is that grid put the picture there 
	
	
	//SIDE NOTES  <<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>
		
		//4 buttons for four direction
		//2 buttons for two players
		//1 slidepot for English/Spanish
		
		//Figure out how to use slidepot values from LAB8 to control the English/Spanish
		// look at lecture to find how to put overlapping pictures/background
		// where should the position start (middle? how to set it to middle?)
		
		
	//ST7735_OutString("Hello"); 
	//ST7735_DrawBitmap(160,0,blackScreen,128,160);// set screen to black //change color
	
	//ST7735_DrawBitmap(0,160,tictactoe,128,160); /// x and 0 change the same 
	//ST7735_DrawBitmap(50,50,equis, 128,160); /// x and 0 change the same 
//	display_lines();
	
	
	
	
	
//	playsound(Ouch);
//	 playsound(Bump);
	// press a button to inital point
//	ST7735_DrawBitmap(50,100,black, 29,30); /// x and 0 change the same  //(center square)
	
		
		

	/*\\	//left button
		if(GPIO_PORTE_DATA_R ^ 0x02 == 1) {//if button is pressed
			
			ST7735_DrawBitmap(50-46,100,black, 29,30); /// x and 0 change the same 
			//(center square)
			
			//ST7735_OutString("Hello"); 
		}
				//place the X picture
	
		*/
	
	

	
	//English  
	
	//Spanish
	
	/*
	if(GPIO_PORTE_DATA_R || 0x00 == 1) { //if button PE0 is pressed move to the tictactoe screen
		ST7735_DrawBitmap(0,160,tictactoe,128,160);// set screen to black
	}
	*/

	
		uint32_t yes = 0; 

		
	
//	ST7735_DrawBitmap(0,160,tictactoe,128,160);// set screen to black
       // set screen to black
  while(1){
		
		
		// slidepot code 
		if(Status == 1){
			Status =0;
			ST7735_SetCursor(0,0);
			//ST7735_OutFix(Convert(Mail));
				if((Mail > 0.00) && (Mail < 1365)) {
					ST7735_OutString("Engli"); 
					//ST7735_DrawBitmap(4,45,black, 29,30);
				}
				
				else if((Mail >= 1365) && (Mail < 2730)) {
					ST7735_OutString("Love"); 
					//ST7735_DrawBitmap(4,100,black, 29,30); /// x and 0 change the same 

				}
				else {
					ST7735_OutString("Spanish"); 
						//ST7735_DrawBitmap(4,148,black, 29,30); /// x and 0 change the same 

				}
		//	ST7735_OutString( " cm");
    // check semaphore
    // output to LCD
  }

		
		
  
	
//		while(GPIO_PORTE_DATA_R == 0) {};
			
			

		
			//if direction button is pressed (button pressed and released)
					// if player button is pressed ( pressed and released) 
							// put the sprite
					// else move to the next 
		 
		// if player button is pressed
		if ((GPIO_PORTE_DATA_R & 0x02) == 0x02) {//if button is pressed     0000 0010
				//ST7735_DrawBitmap(4,100,black, 29,30); /// x and 0 change the same 
			//	ST7735_OutString ("\n One"); 
				yes = GPIO_PORTE_DATA_R; 			
		}
			
		//check to see if released
		else {
			if(yes == GPIO_PORTE_DATA_R) {				
				if((GPIO_PORTE_DATA_R & 0x01) == 0x01){
					yes = 0; 
					//ST7735_DrawBitmap(4,45,zebraX, 29,30);
				}
				else {
					// if there is no pixel print black screen else continue
					//ST7735_DrawBitmap(4,100,black, 29,30);
				}
			//	ST7735_OutString("button was release"); 
			}
			
		}
		
			
					
					//tranerse through an array to see if it is all black
					// find the coordinates of that area
					
					//ST7735_OutString(" \n button was pressed");
				////(center square)
				
				//ST7735_OutString("Hello"); 
				
		
		if ((GPIO_PORTE_DATA_R & 0x04) == 0x04) {//if button is pressed     0000 0010
				//ST7735_DrawBitmap(4,100,black, 29,30); /// x and 0 change the same 
				ST7735_OutString ("\n Two"); 
				////(center square)
				
				//ST7735_OutString("Hello"); 
			}	
		
		else if ((GPIO_PORTE_DATA_R & 0x08) == 0x08) {//if button is pressed     0000 0010
				//ST7735_DrawBitmap(4,100,black, 29,30); /// x and 0 change the same 
				ST7735_OutString ("\n Three"); 
				////(center square)
				
				//ST7735_OutString("Hello"); 
			}	


			
	
		
		//	Delay1ms(3); 
			
		//	ST7735_OutString("Hello World"); 
		//left Button
			
			
	//add a delay statement 
		
		
	// make sure after every player plays set the position back to the center
		
	\
		//if (redraw){
			//Draw(dude);
		//#ifndef __images_h
//#define __images_h
//#include <stdint.h>
		
			//redraw= False;
		//}
    }  
}

 //void Draw(Sprite_t p){
	 //ST7735_DrawBitmap(p.x,p.y,  pink_snake_tongue_pixel, p.w, p.h);
//}

//void Move(){
	//dude.x += dx;
	//dude.y += dy;
	//redraw = True;
	
//}



int checkWin() {
    int i, j;
    // Check rows
    for (i = 0; i < ROWS; i++) {
        if (array[i][0] == array[i][1] && array[i][0] == array[i][2] && array[i][0] != '-') {  // '-' does not equal to the empty space
            return 1;
        }
    }
    // Check columns
    for (j = 0; j < COLS; j++) {
        if (array[0][j] == array[1][j] && array[0][j] == array[2][j] && array[0][j] != '-') {
            return 1;
        }
    }
    // Check diagonals
    if (array[0][0] == array[1][1] && array[0][0] == array[2][2] && array[0][0] != '-') {
        return 1;
    }
    if (array[0][2] == array[1][1] && array[0][2] == array[2][0] && array[0][2] != '-') {
        return 1;
    }
    return 0;
}

int checkTie() {
    int i, j;
    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            if (array[i][j] == '-') {
                return 0;
            }
        }
    }
    return 1;
}


