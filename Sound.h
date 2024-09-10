// Sound.h
// Trang Do
// Last Modified: 5/3/2022
// Lab 10: Flappy Bird Game
// Runs on TM4C123 or LM4F120
// Play Sound Effects and/or Music for the game

#ifndef __sound_h
#define __sound_h
#include <stdint.h>
typedef enum {Pause, Munch} soundeffect; //have length of this the same as length in sound.c sounds[] array
void playsound(soundeffect);
	
#endif /* __sound_h */

//// Sound.h
//// Runs on TM4C123 or LM4F120
//// Prototypes for basic functions to play sounds from the
//// original Space Invaders.
//// Jonathan Valvano
//// 11/15/2021 
//#ifndef SOUND_H
//#define SOUND_H
//#include <stdint.h>

//void Sound_Init(void);

////******* Sound_Start ************
//// This function does not output to the DAC. 
//// Rather, it sets a pointer and counter, and then enables the timer interrupt.
//// It starts the sound, and the timer ISR does the output
//// feel free to change the parameters
//// Input: pt is a pointer to an array of DAC outputs
////        count is the length of the array
//// Output: none
//// special cases: as you wish to implement
//void Sound_Start(const uint8_t *pt, uint32_t count);

//void Sound_Shoot(void);
//void Sound_Killed(void);
//void Sound_Explosion(void);

//void Sound_Fastinvader1(void);
//void Sound_Fastinvader2(void);
//void Sound_Fastinvader3(void);
//void Sound_Fastinvader4(void);
//void Sound_Highpitch(void);


