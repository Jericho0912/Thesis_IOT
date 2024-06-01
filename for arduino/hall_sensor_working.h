
#include <Wire.h> // Wire library is still needed for interrupts

float rev = 0;
int rpm;
int oldtime = 0;
int time;

void isr() // Interrupt service routine
{
  rev++;
}

void setup()
{
  pinMode(3, INPUT_PULLUP); // Enable internal pull-up resistor
  attachInterrupt(0, isr, RISING);
  Serial.begin(9600);
}

void loop()
{
  delay(2000); // 2-second delay
  detachInterrupt(0); 

  time = millis() - oldtime;
  rpm = (rev / time) * 60000;
  oldtime = millis();
  rev = 0;

  Serial.print("RPM: "); 
  Serial.println(rpm); 

  attachInterrupt(0, isr, RISING);
}
