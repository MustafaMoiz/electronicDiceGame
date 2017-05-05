/*

The Electronic Dice Game

Last modified on December 13, 2016 by
Mustafa Lokhandwala, Fresher Foundry. 

*/

#include "LedControl.h"

//Read all 8 button status

#define button2 digitalRead(2)
#define button3 digitalRead(3)
#define button4 digitalRead(4)
#define button5 digitalRead(5)
#define button6 digitalRead(6)
#define button7 digitalRead(7)
#define button8 digitalRead(8)
#define button9 digitalRead(9)

LedControl lc=LedControl(12,11,10,1); //Create LedControl library object

//Declare variables

int i, j;
int roundCount = 0;
int playerRoll[8]={0};
int playerPass[8] = {0};
int playerPresent[8];
float numberInSystem[27]={0};
int throughput=0;
float avgthroughput=0;
int roundInput = 0;
int roundOutput = 0;
int totalMoving = 0;
int roundNumber =0;
int totalNumber=0;
char val;
int buttonFlag[8];

//Function to roll dice

void rollDice()
{
    // Display random numbers for a while

    for (int i=0; i<20; i++)
  {
    for (j=0; j<8; j++)
    {
      lc.setDigit(0,j,random(1,7),false);
    }    
    delay(100);
  }

  //Store die roll value for all 8 players
  
  for(i=0; i<8; i++)
  {
    playerRoll[i]=random(1,7);
  }

  // Blink die roll 3 times
  
  for (int i=0; i<3; i++)
  {
    lc.clearDisplay(0);

    delay(250);

    for (j=0; j<8; j++)
    {
      lc.setDigit(0,j,playerRoll[j],false);
    }
    
    delay(250);
  }

  //Clear button flags for next round

  for(i=0; i<8; i++)
  {
    buttonFlag[i]=0;
  }
  
}

//Function for doing game stats calculations

void calculate()
{
  
  //Calculate pass number for each player
  
  totalMoving=0;

  for (i = 0; i < 8; i++)
  {
    if (playerPresent[i] <= playerRoll[i])
    {
      playerPass[i] = playerPresent[i];
    }
    else playerPass[i] = playerRoll[i];

  //Calculate total parts moving through assembly/total number of chips passed

    totalMoving += playerPass[i];
  }

  playerPresent[0] = playerPresent[0] - playerPass[0] + playerRoll[0];

  //Calculate present number for each player

  for (i = 1; i < 8; i++)
  {
    playerPresent[i] = playerPresent[i] - playerPass[i] + playerPass[i - 1];
  }

  //Calculate total number in system in this round

  for (i = 0; i < 8; i++)
  {
    numberInSystem[roundCount] += playerPresent[i];
  }

  //Set other stats values

  roundInput = playerRoll[0];
  roundOutput = playerPass[7];
  throughput +=roundOutput;
  roundNumber=roundCount+1;
  totalNumber=numberInSystem[roundCount];
  avgthroughput=throughput/roundNumber;

}

//Upload to processing

void upload()
{

  //Store calculations in the latter part of array and send a single array to processing
  
  numberInSystem[20]=roundNumber;
  numberInSystem[21]=roundInput;
  numberInSystem[22]=roundOutput;
  numberInSystem[23]=totalMoving;
  numberInSystem[24]=totalNumber;
  numberInSystem[25]=throughput;
  numberInSystem[26]=avgthroughput;

  //Send newline character to processing
  
  Serial.println();
  delay(50);

  
  if (Serial.available() > 0)
  {
    val = Serial.read();

    if (val == 'A')              //Check for A sent by processing
    {
      for (i = 0; i < 26; i++)   //Send array to processing
      {
        Serial.print(numberInSystem[i], DEC);
        Serial.print(',');
      }
      Serial.print(numberInSystem[26]);
      Serial.println();
      roundCount++;               //Increment round count
    }
  }

  //Reset data at end of round 20
  
  if (roundNumber==20)
  {
    roundCount=0;
    roundNumber=0;
    playerPass[8] = {0};
    for (i=0; i<25; i++)
    {
      numberInSystem[i]=0;
    } 
    playerRoll[8]={0};

    for (i=0; i<8; i++)
      {
        playerPresent[i]=4;
      }
    
    roundInput = 0;
    roundOutput = 0;
    totalMoving = 0;
    totalNumber=0;
    throughput=0;
    avgthroughput=0;
  }
}

void setup() {

  randomSeed(analogRead(A0));

  Serial.begin(9600);

  for(i=0; i<8; i++)
  {
    buttonFlag[i]=0;
  }

  //Set initial value of chips to 4 for each player

  for (i=0; i<8; i++)
  {
    playerPresent[i]=4;
  }
  
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,15);
  /* and clear the display */
  lc.clearDisplay(0);

}

//Continuously check for button press and roll dice when all 8 are pressed

void loop() {

  if(button8==HIGH || buttonFlag[0]==1)
  {
    lc.setDigit(0,5,random(1,7),false);
    buttonFlag[0]=1;
  }

  if(button9==HIGH || buttonFlag[1]==1)
  {
    lc.setDigit(0,2,random(1,7),false);
    buttonFlag[1]=1;
  }

  if(button2==HIGH || buttonFlag[2]==1)
  {
    lc.setDigit(0,7,random(1,7),false);
    buttonFlag[2]=1;
  }

  if(button3==HIGH || buttonFlag[3]==1)
  {
    lc.setDigit(0,0,random(1,7),false);
    buttonFlag[3]=1;
  }

  if(button4==HIGH || buttonFlag[4]==1)
  {
    lc.setDigit(0,6,random(1,7),false);
    buttonFlag[4]=1;
  }

  if(button5==HIGH || buttonFlag[5]==1)
  {
    lc.setDigit(0,3,random(1,7),false);
    buttonFlag[5]=1;
  }

  if(button6==HIGH || buttonFlag[6]==1)
  {
    lc.setDigit(0,1,random(1,7),false);
    buttonFlag[6]=1;
  }

  if(button7==HIGH || buttonFlag[7]==1)
  {
    lc.setDigit(0,4,random(1,7),false);
    buttonFlag[7]=1;
  }

  if(buttonFlag[0]&&buttonFlag[1]&&buttonFlag[2]&&buttonFlag[3]&&buttonFlag[4]&&buttonFlag[5]&&buttonFlag[6]&&buttonFlag[7])  //roll dice, calculate and upload data to processing only if all 8 buttons are pressed
  {
    rollDice();
    calculate();
    upload();
  }
  
  delay(150);

}
