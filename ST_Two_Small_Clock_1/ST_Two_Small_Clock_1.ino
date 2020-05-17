

//  *****************************************************************************************************************
//  *                                                                                                               *
//  *                                         SpikenzieLabs.com                                                     *
//  *                                                                                                               *
//  *                                       Solder:Time Desk Clock                                                  *
//  *                                                                                                               *
//  *****************************************************************************************************************
//
// BY: MARK DEMERS
// May 2013
// VERSION 1.0
//
// Brief:
// Sketch used in the Solder: Time Desk Clock Kit, more info and build instructions at http://www.spikenzielabs.com/stdc
//
//
// LEGAL:
// This code is provided as is. No guaranties or warranties are given in any form. It is your responsibilty to
// determine this codes suitability for your application.
//
// Changes:
// A. Modified LEDupdateTHREE() void used by ST:2 Watch to function with the new circuits in the Solder:Time Desk Clock
// B. Modified port dirctions on some pins to deal with new circuits.
// C. Changed sleep mode into a "change back to display time" mode

#include <Wire.h>
#include <EEPROM.h>

#include <TimerOne.h>

#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>

int gameIndex = 0;
int games = 1;
bool inGame = false;

int score = 0;

char gameNames[1][4] = {
  "BNC ",
};

#define BNCXposDefault 19
#define BNCTposDefault 0

int Xpos = 0;
int Ypos = 0;

int Xposprev = 0;
int Yposprev = 2;

#define BNCdirXdefault -1
#define BNCdirYdefault 1

int dirX = -1;
int dirY = 1;
unsigned long BNCballTimer = 0;
#define BNCballSpeed 300;// Lower is faster, milliseconds per diagonal square moved

int BNCplayerPos = 9;
bool BNCplayerUpdate = true;

int ROWBITINDEX = 0;

#define BUTTON1 2
#define MODEBUTTON 2
#define BUTTON2 3
#define SETBUTTON 3

bool ModeButton = false;
bool SetButton = false;
bool ModeButtonTriggered = false;
bool SetButtonTriggered = false;

uint8_t SWDigit4 = 0;
uint8_t SWDigit3 = 0;
uint8_t SWDigit2 = 0;
uint8_t SWDigit1 = 0;

volatile uint8_t Mcolumn = 0;
volatile uint8_t LEDMAT[20];

// Constants
// DS1337+ Address locations
#define		RTCDS1337	B01101000               // was B11010000  
#define		RTCCONT		B00001110		//; Control
#define		RTCSTATUS       B00001111		//; Status

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Bit Map Letter - data array
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const byte LETTERS[106][5] = {
  0, 0, 0, 0, 0,        // Space
  0, 0, 95, 0, 0,       // !
  0, 3, 0, 3, 0,        // "
  18, 63, 18, 63, 18,   // #
  36, 42, 127, 42, 18,  // $
  34, 21, 42, 84, 34,   // %
  54, 73, 81, 34, 80,   // &
  0, 0, 3, 0, 0,        // '
  0, 0, 62, 65, 0,      // (
  0, 0, 62, 65, 0,      // (
  4, 20, 15, 20, 4,     // *
  0, 8, 28, 8, 0,       // +
  0, 0, 2, 1, 0,        // ´
  8, 8, 8, 8, 8,        // -
  0, 0, 64, 0, 0,       // .
  32, 16, 8, 4, 2,      // /

  62, 65, 73, 65, 62,    // 0
  0, 66, 127, 64, 0,     // 1
  98, 81, 73, 73, 70,    // 2
  34, 65, 73, 73, 54,    // 3
  7, 8, 8, 127, 8,       // 4
  47, 73, 73, 73, 49,    // 5
  62, 73, 73, 73, 50,    // 6
  1, 9, 125, 11, 1,      // 7
  54, 73, 73, 73, 54,    // 8
  6, 9, 9, 9, 126,       // 9

  0, 36, 0, 0, 0,        // :
  0, 100, 0, 0, 0,       // ;
  16, 56, 40, 108, 68,   // <
  0, 40, 40, 40, 0,      // =
  68, 108, 40, 56, 16,   // >
  6, 1, 81, 9, 6,        // ?
  62, 65, 73, 85, 10,    // @

  124, 10, 9, 10, 124,   // A
  127, 73, 73, 73, 54,   // B
  62, 65, 65, 65, 34,    // C
  127, 65, 65, 65, 62,   // D
  127, 73, 73, 73, 65,   // E
  127, 9, 9, 9, 1,       // F
  62, 65, 73, 73, 50,    // G
  127, 8, 8, 8, 127,     // H
  0, 65, 127, 65, 0,     // I  was   65,65,127,65,65,
  32, 64, 65, 63, 1,     // J
  127, 8, 20, 34, 65,    // K
  127, 64, 64, 64, 64,   // L
  127, 2, 4, 2, 127,     // M
  127, 4, 8, 16, 127,    // N
  62, 65, 65, 65, 62,    // O
  127, 9, 9, 9, 6,       // P
  62, 65, 81, 33, 94,    // Q
  127, 9, 25, 41, 70,    // R
  38, 73, 73, 73, 50,    // S
  1, 1, 127, 1, 1,       // T
  63, 64, 64, 64, 63,    // U
  3, 28, 96, 28, 3,      // V
  63, 64, 56, 64, 63,    // W
  65, 54, 8, 54, 65,     // X
  3, 4, 120, 4, 3,       // Y
  97, 81, 73, 69, 67,    // Z

  0, 127, 65, 65, 0,     // [
  2, 4, 8, 16, 32,       // back slash
  65, 127, 0, 0, 0,      // ]
  0, 2, 1, 2, 0,         // ^
  128, 128, 128, 128, 128, // _
  0, 0, 1, 2, 0,         // `

  112, 72, 72, 40, 120,   // a
  126, 48, 72, 72, 48,    // b
  48, 72, 72, 72, 0,      // c
  48, 72, 72, 48, 126,    // d
  56, 84, 84, 84, 88,     // e
  0, 8, 124, 10, 0,       // f
  36, 74, 74, 74, 60,     // g
  126, 8, 8, 8, 112,      // h
  0, 0, 122, 0, 0,        // i
  32, 64, 66, 62, 2,      // j
  124, 16, 16, 40, 68,    // k
  0, 124, 64, 64, 0,      // l
  120, 4, 120, 4, 120,    // m
  124, 8, 4, 4, 120,      // n
  48, 72, 72, 48, 0,      // o
  126, 18, 18, 18, 12,    // p
  12, 18, 18, 18, 124,    // q
  124, 8, 4, 4, 8,        // r
  72, 84, 84, 84, 36,     // s
  4, 4, 126, 4, 4,        // t
  60, 64, 64, 64, 60,     // u
  28, 32, 64, 32, 28,     // v
  124, 32, 16, 32, 124,   // w
  68, 40, 16, 40, 68,     // x
  4, 8, 112, 8, 4,        // y
  68, 100, 84, 76, 68,    // z

  0, 0, 0, 62, 73,        // {
  0, 0, 127, 0, 0,        // |
  73, 62, 0, 0, 0,        // }
  16, 8, 24, 16, 8,       // ~
};

const byte GRAPHIC[0][5] = {
};
