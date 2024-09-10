// Phrases.h
// Trang Do
// Last Modified: 5/3/2022
// Lab 10: Flappy Bird Game
// Runs on TM4C123 or LM4F120
// Define Phrases in Languages to be used in the Game

#ifndef __Phrases_h
#define __Phrases_h

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"

// define Language and Phrase variable types that would be the indices of the Phrases 2-D array
typedef enum {English, Spanish, Portuguese, French} Language_t;
typedef enum {LANGUAGE, THANK, PRESS, STARTOVER, SW2, SW1, FORWARD, CAREFUL, BACKWARD} phrase_t;

// Defining all phrases
const char Language_English[]=" English ";
const char Language_Spanish[]="Espa\xA4ol";
const char Thank_English[] = "Thank You";
const char Thank_Spanish[] = "\xADGracias!";
const char Press_English[] = "  Press Reset to  ";
const char Press_Spanish[] = "Presione Reset para";
const char StartOver_English[] = "   Start Over   ";
const char StartOVer_Spanish[] = "Comenzar de Nuevo";
const char SW2_English[] = "SW2: Pause";
const char SW2_Spanish[] = "SW2: Pausa";
const char SW1_English[] = "SW1: Move the Bird";
const char SW1_Spanish[] = "SW1: Mueva el P\x85jaro";
const char Forward_English[] = "Forward";
const char Forward_Spanish[] = "Hacia Adelante";
const char Careful_English[] = "Be Careful, You";
const char Careful_Spanish[] = "\xADTen Cuidado, No";
const char Backward_English[] = "Can't Go Backward!";
const char Backward_Spanish[] = "Puedes Retroceder!";

//********************Phrases 2-D array********************
// Row: Phrases
// Column: Language
const char *Phrases[9][2]={
	{Language_English, Language_Spanish},
	{Thank_English, Thank_Spanish},
	{Press_English, Press_Spanish},
	{StartOver_English, StartOVer_Spanish},
	{SW2_English, SW2_Spanish},
	{SW1_English, SW1_Spanish},
	{Forward_English, Forward_Spanish},
	{Careful_English, Careful_Spanish},
	{Backward_English, Backward_Spanish},
};

#endif /* __Phrases_h */

