#include "LedControl.h"

LedControl lc=LedControl(12,11,10,1);

int i,j;

void setup() {

  Serial.begin(9600);
  
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,15);
  /* and clear the display */
  lc.clearDisplay(0);
  for (j=0; j<8; j++)
    {
      lc.setDigit(0,j,5,false);
      Serial.println(j);
    }    

}

void loop() {

//    for (j=0; j<9; j++)
//    {
//      for (i=0; i<8; i++)
//      {
//        lc.setDigit(0,i,j,false);
//        Serial.println(j);
//      }   
//
//       delay(1000);
//    }
//

}
