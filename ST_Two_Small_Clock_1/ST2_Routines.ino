//*******************************************************************************************************************
// 								                        Helper - State incrementor
//*******************************************************************************************************************
// Increments the Main state to the next state upto MAXSTATE
// If you add more States increase MAXSTATE to match.

//*******************************************************************************************************************
//                                                     Button Management
//*******************************************************************************************************************
//called in each loop
void ButtonManager() {
  unsigned long milliTemp = millis();
  if (SetButtonTimer <= milliTemp) {
    SetButton = !digitalRead(SETBUTTON);
    if (!SetButton && SetButtonTriggered) {
      SetButtonTriggered = false;
    }
  }
  if (milliTemp >= ModeButtonTimer) {
    ModeButton = !digitalRead(MODEBUTTON);
    if (!ModeButton && ModeButtonTriggered) {
      ModeButtonTriggered = false;
    }
  }
}

bool getSetButton() {
  if (SetButton && !SetButtonTriggered) {
    SetButtonTriggered = true;
    SetButtonTimer = millis() + debounceTime;
    return (true);
  }
  return (false);
}

bool getModeButton() {
  if (ModeButton && !ModeButtonTriggered) {
    ModeButtonTriggered = true;
    ModeButtonTimer = millis() + debounceTime;
    return (true);
  }
  return (false);
}

//*******************************************************************************************************************
// 								                                    Beep the piezo
//*******************************************************************************************************************
// NOTE: the piezo uses the same pin as the SET Button input
// This routine switches the pin to an output during the beeping process
// (There is a current limited resistor on the board to prevent over current if the Set Button is pressed at
//  the same time as the pin is driving the piezo.)

void beepsound(int freq, int freqlenght)
{
  // freq was 4000
  // freqlenght was 100
  pinMode(SETBUTTON, OUTPUT);
  tone(SETBUTTON, freq, freqlenght);
  delay(freqlenght);
  noTone(SETBUTTON);
  digitalWrite(SETBUTTON, HIGH);

#if ARDUINO >= 101
  pinMode(SETBUTTON, INPUT_PULLUP);
  //  digitalWrite(SETBUTTON, HIGH);
#else
  //  digitalWrite(SETBUTTON, HIGH);
  pinMode(SETBUTTON, INPUT);
#endif

}

//*******************************************************************************************************************
// 								                                        LED tester
//*******************************************************************************************************************

void lamptest()
{
  int lamptestspeed = 250;
  bool on = true;
  clearMatrix();
  do
  {
    //    clearMatrix();
    for (int i = 0; i < 20; i++)
    {
      for (int y = 0; y < 8; y++)
      {
        bitSet(LEDMAT[i], y);
        delay(lamptestspeed / 10);
      }

      on = digitalRead(MODEBUTTON);
      if (!on)
      {
        break;
      }

      delay(lamptestspeed);
      LEDMAT[i] = 0;
      delay(lamptestspeed / 5);
    }
    on = digitalRead(MODEBUTTON);
  }
  while (on);
}

//*******************************************************************************************************************
// 								                      Output 4 Characters to Display
//*******************************************************************************************************************
void displayString(char outText[])
{
  int  cindex = 0;
  //  for (int i = 0; i < sizeof(Str1) - 1; i++){
  for (int i = 0; i < 4; i++) {
    for (int y = 0; y < 5; y++)
    {
      LEDMAT[cindex] = LETTERS[outText[i] - 32][y];
      cindex = cindex + 1;
    }
  }
}

//*******************************************************************************************************************
// 								                  Output Custom Graphic to Display
//*******************************************************************************************************************
void displayGraphic(int index, int pos, int len)
{
  for (int y = 0; y < len; y++)
  {
    LEDMAT[pos] = GRAPHIC[index][y];
    pos = pos + 1;
  }
}

//*******************************************************************************************************************
// 								                                Clear LED Matrix
//*******************************************************************************************************************

void clearMatrix()
{
  for (int i = 0; i < 20; i++)
  {
    LEDMAT[i] = 0;
  }
}


//*******************************************************************************************************************
// 								               Output single character to display
//*******************************************************************************************************************

void filldigit(int dig, int index)        // Where dig is 1 to 4 and index is position of character
{
  for (int y = 0; y < 5; y++)
  {
    LEDMAT[((dig - 1) * 5) + y] = LETTERS[index][y];
  }
}

//*******************************************************************************************************************
//                                Fill single pixel
//*******************************************************************************************************************

void pixel(int x, int y, byte Mode)
{
  uint8_t temp = LEDMAT[x];
  switch (Mode) {
    case 0:
      bitClear(temp, 6 - y);
      break;
    case 1:
      bitSet(temp, 6 - y);
      break;
    case 2:
      bitWrite(temp, 6 - y, !bitRead(temp, 6 - y));
      break;
  }
  LEDMAT[x] = temp;
}

void bounce() {
  if (Xpos == 0) {
    dirX = 1;
  }
  else if (Xpos == 19) {
    dirX = -1;
  }
  if (Ypos == 6) {
    dirY = -1;
  }
}
