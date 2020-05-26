//*******************************************************************************************************************
// 								                                         Main Loop
//*******************************************************************************************************************
void loop()
{
  ButtonManager();
  unsigned long milliTemp = millis();

  //From original code
#if ARDUINO >= 101
  pinMode(SETBUTTON, INPUT_PULLUP);
  //    digitalWrite(SETBUTTON, HIGH);
#else
  //    digitalWrite(SETBUTTON, HIGH);
  pinMode(SETBUTTON, INPUT);
#endif

  if (inGame) {
    switch (gameIndex) {
      case 0:
        if (getSetButton ()) {
          BNCplayerPos++;
          BNCplayerUpdate = true;
          if (BNCplayerPos == 19) {
            BNCplayerPos = 0;
            pixel(19, 0, 0);
            pixel(18, 0, 0);
          }
        }
        else if (getModeButton()) {
          BNCplayerPos--;
          BNCplayerUpdate = true;
          if (BNCplayerPos == -1) {
            BNCplayerPos = 18;
            pixel(0, 0, 0);
            pixel(1, 0, 0);
          }
        }
        if (BNCplayerUpdate) {
          BNCplayerUpdate = false;
          pixel(BNCplayerPos, 0, 1);
          pixel(BNCplayerPos + 1, 0, 1);
          if (BNCplayerPos != 0) {
            pixel(BNCplayerPos - 1, 0, 0);
          }
          if (BNCplayerPos != 18) {
            pixel(BNCplayerPos + 2, 0, 0);
          }
        }
        if (milliTemp > BNCballTimer) {
          bounce();
          if (Ypos == -2) {
            clearMatrix();
            displayString("Scor");
            delay(1000);
            displayString("e:  ");
            delay(1000);
            char temp[4] = "    ";
            temp[3] = score % 10;
            score -= temp[3];
            temp[2] = score % 100 / 10;
            score -= temp[2];
            temp[1] = score % 1000 / 100;
            score -= temp[1];
            temp[0] = score / 1000;
            for (int i = 0; i < 4; i++) {
              temp[i] += '0';
            }
            for (int i = 0; i < 3; i++) {
              if (temp[i] == '0') {
                temp[i] = ' ';
              }
              else {
                break;
              }
            }

            displayString(temp);
            delay(1500);
            inGame = false;
            BNCplayerPos = 9;
            BNCplayerUpdate = true;
          }

          if (Ypos == 1) {
            if (Xpos - 1 == BNCplayerPos && dirX == -1) {
              dirY = 1;
              score++;
            }
            else if (Xpos == BNCplayerPos && dirX == 1) {
              dirY = 1;
              score++;
            }
            else if (Xpos + 1 == BNCplayerPos && dirX == 1) {
              dirY = 1;
              dirX = -1;
              score++;
            }
            else if (Xpos - 1 == BNCplayerPos + 1 && dirX == -1) {
              dirY = 1;
              dirX = 1;
              score++;
            }
            bounce();
          }

          pixel(Xpos, Ypos, 1);
          pixel(Xposprev, Yposprev, 0);
          Xposprev = Xpos;
          Yposprev = Ypos;

          Xpos += dirX;
          Ypos += dirY;

          BNCballTimer = milliTemp + BNCballSpeed;
        }
        break;
    }
  }
  else {
    displayString(gameNames[gameIndex]);
    if (getModeButton()) {
      gameIndex++;
      if (gameIndex == games) {
        gameIndex = 0;
      }
    }
    if (getSetButton()) {
      inGame = true;
      score = 0;
      switch (gameIndex) {
        case 0:
          Xpos = BNCXposDefault;
          Ypos = BNCTposDefault;

          Xposprev = 0;
          Yposprev = 2;

          dirX = BNCdirXdefault;
          dirY = BNCdirYdefault;
          break;
      }
      clearMatrix();
    }
  }
}


//*******************************************************************************************************************
// 								                                 End of Main Loop
//*******************************************************************************************************************
