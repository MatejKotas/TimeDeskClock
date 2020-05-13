//*******************************************************************************************************************
// 								                                 Setup
//*******************************************************************************************************************
void setup()
{
#if ARDUINO >= 101
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  digitalWrite(2, HIGH);                                   // Write a high to pin, acts as weak pull-up
  digitalWrite(3, HIGH);
#else
  digitalWrite(2, HIGH);                                   // Write a high to pin, acts as weak pull-up
  digitalWrite(3, HIGH);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
#endif

  // Column address bits 4 to 16 decode
  pinMode(4, OUTPUT);                                      // DeMux A
  pinMode(5, OUTPUT);                                      // DeMux B
  pinMode(6, OUTPUT);                                      // DeMux C
  pinMode(7, OUTPUT);                                      // DeMux D

  //test with
  DDRB = (1 << DDB7) | (1 << DDB6) | (1 << DDB5) | (1 << DDB4) | (1 << DDB3) | (1 << DDB2) | (1 << DDB1) | (1 << DDB0);


  // Make these pins outputs (analog 0 to 3)
  DDRC = DDRC | 1 << PORTC0;                                      // Column 17
  DDRC = DDRC | 1 << PORTC1;                                      // Column 18
  DDRC = DDRC | 1 << PORTC2;                                      // Column 19
  DDRC = DDRC | 1 << PORTC3;                                      // Column 20

  // attachInterrupt(0, MinuteUP, FALLING);
  // attachInterrupt(1, MinuteDOWN, FALLING);

  // Turn one Interupts, used to update the displayed LED matrix
  Timer1.initialize(100);                                          // was 100
  //   Timer1.attachInterrupt(LEDupdate);
  Timer1.attachInterrupt(LEDupdateTHREE);


  // I2C Inits
  Wire.begin();

  // Power Reduction - S
  /*power_adc_disable();
  power_spi_disable();
  power_usart0_disable();*/

  //wdt_disable();

  if (!digitalRead(SETBUTTON)) {
    lamptest();
    clearMatrix();
  }
}
