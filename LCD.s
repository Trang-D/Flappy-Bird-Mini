; LCD.s
; Student names: Trang Do
; Last modification date: 3/22/2022

; Runs on TM4C123
; Use SSI0 to send an 8-bit code to the ST7735 160x128 pixel LCD.

; As part of Lab 7, students need to implement these writecommand and writedata
; This driver assumes two low-level LCD functions

; Backlight (pin 10) connected to +3.3 V
; MISO (pin 9) unconnected
; SCK (pin 8) connected to PA2 (SSI0Clk)
; MOSI (pin 7) connected to PA5 (SSI0Tx)
; TFT_CS (pin 6) connected to PA3 (SSI0Fss)
; CARD_CS (pin 5) unconnected
; Data/Command (pin 4) connected to PA6 (GPIO)
; RESET (pin 3) connected to PA7 (GPIO)
; VCC (pin 2) connected to +3.3 V
; Gnd (pin 1) connected to ground

;**********HiLetgo ST7735 TFT and SDC (SDC not tested)*******************
;ST7735
;LED-   (pin 16) TFT, connected to ground
;LED+   (pin 15) TFT, connected to +3.3 V
;SD_CS  (pin 14) SDC, chip select
;MOSI   (pin 13) SDC, MOSI
;MISO   (pin 12) SDC, MISO
;SCK    (pin 11) SDC, serial clock
;CS     (pin 10) TFT, PA3 (SSI0Fss)
;SCL    (pin 9)  TFT, SCK  PA2 (SSI0Clk)
;SDA    (pin 8)  TFT, MOSI PA5 (SSI0Tx)
;A0     (pin 7)  TFT, Data/Command PA6 (GPIO), high for data, low for command
;RESET  (pin 6)  TFT, to PA7 (GPIO)
;NC     (pins 3,4,5) not connected
;VCC    (pin 2)  connected to +3.3 V
;GND    (pin 1)  connected to ground

GPIO_PORTA_DATA_R       EQU   0x400043FC
SSI0_DR_R               EQU   0x40008008
SSI0_SR_R               EQU   0x4000800C

      EXPORT   writecommand
      EXPORT   writedata

      AREA    |.text|, CODE, READONLY, ALIGN=2
      THUMB
      ALIGN

; The Data/Command pin must be valid when the eighth bit is
; sent.  The SSI module has hardware input and output FIFOs
; that are 8 locations deep.  Based on the observation that
; the LCD interface tends to send a few commands and then a
; lot of data, the FIFOs are not used when writing
; commands, and they are used when writing data.  This
; ensures that the Data/Command pin status matches the byte
; that is actually being transmitted.
; The write command operation waits until all data has been
; sent, configures the Data/Command pin for commands, sends
; the command, and then waits for the transmission to
; finish.
; The write data operation waits until there is room in the
; transmit FIFO, configures the Data/Command pin for data,
; and then adds the data to the transmit FIFO.
; NOTE: These functions will crash or stall indefinitely if
; the SSI0 module is not initialized and enabled.

; This is a helper function that sends an 8-bit command to the LCD.
; Input: R0  8-bit command to transmit
; Output: none
; Assumes: SSI0 and port A have already been initialized and enabled
writecommand
;; --UUU-- Code to write a command to the LCD
;1) Read SSI0_SR_R and check bit 4, 
;2) If bit 4 is high, loop back to step 1 (wait for BUSY bit to be low)
;3) Clear D/C=PA6 to zero
;4) Write the command to SSI0_DR_R
;5) Read SSI0_SR_R and check bit 4, 
;6) If bit 4 is high, loop back to step 5 (wait for BUSY bit to be low)
	
	PUSH {R0, R4, R5, LR}
	; Step 1)
	LDR R4, =SSI0_SR_R
command_one	
	LDR R5, [R4]
	AND R5, #0x10
	
	; Step 2)
	CMP R5, #0x10
	BEQ command_one
	
	; Step 3)
	LDR R4, =GPIO_PORTA_DATA_R
	LDR R5, [R4]
	BIC R5, #0x40
	STR R5, [R4]
	
	; Step 4)
	LDR R4, =SSI0_DR_R
	STRB R0, [R4]
	
    ; Step 5)
command_five
	LDR R4, =SSI0_SR_R
	LDR R5, [R4]
	AND R5, #0x10
	
	; Step 6)
	CMP R5, #0x10
	BEQ command_five
    
	POP {R0, R4, R5, LR}
    BX  LR                          ;   return

; This is a helper function that sends an 8-bit data to the LCD.
; Input: R0  8-bit data to transmit
; Output: none
; Assumes: SSI0 and port A have already been initialized and enabled
writedata
;; --UUU-- Code to write data to the LCD
;1) Read SSI0_SR_R and check bit 1, 
;2) If bit 1 is low loop back to step 1 (wait for TNF bit to be high)
;3) Set D/C=PA6 to one
;4) Write the 8-bit data to SSI0_DR_R

	PUSH {R0, R4, R5, LR}
	; Step 1)
	LDR R4, =SSI0_SR_R
data_SR	
	LDR R5, [R4]
	AND R5, #0x02
	
	; Step 2)
	CMP R5, #0
	BEQ data_SR
	
	; Step 3)
	LDR R4, =GPIO_PORTA_DATA_R
	LDR R5, [R4]
	ORR R5, #0x40
	STR R5, [R4]
	
	; Step 4)
	LDR R4, =SSI0_DR_R
	STRB R0, [R4]
       
    POP {R0, R4, R5, LR}
    BX  LR                          ;   return


;***************************************************
; This is a library for the Adafruit 1.8" SPI display.
; This library works with the Adafruit 1.8" TFT Breakout w/SD card
; ----> http://www.adafruit.com/products/358
; as well as Adafruit raw 1.8" TFT display
; ----> http://www.adafruit.com/products/618
;
; Check out the links above for our tutorials and wiring diagrams
; These displays use SPI to communicate, 4 or 5 pins are required to
; interface (RST is optional)
; Adafruit invests time and resources providing this open source code,
; please support Adafruit and open-source hardware by purchasing
; products from Adafruit!
;
; Written by Limor Fried/Ladyada for Adafruit Industries.
; MIT license, all text above must be included in any redistribution
;****************************************************

    ALIGN                           ; make sure the end of this section is aligned
    END                             ; end of file
