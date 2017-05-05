/*

  The Electronic Dice Game

  Last modified on December 13, 2016 by
  Mustafa Lokhandwala, Fresher Foundry.

*/

#include "LedControl.h"

#define numberOfPlayers 6

//Read all numberOfPlayers button status

int buttonArray[] = {2,3,4,5,6,7,8,9};

LedControl lc = LedControl(12, 11, 10, 1); //Create LedControl library object

//Declare variables

int i, j;
int roundCount = 0;
int playerRoll[numberOfPlayers] = {0};
int playerPass[numberOfPlayers] = {0};
int playerPresent[numberOfPlayers];
float numberInSystem[27] = {0};
int throughput = 0;
float avgthroughput = 0;
int roundInput = 0;
int roundOutput = 0;
int totalMoving = 0;
int roundNumber = 0;
int totalNumber = 0;
char val;
bool buttonFlag[numberOfPlayers]={0};
bool flag=1;

//Function to roll dice

void rollDice()
{
  // Display random numbers for a while

  for (int i = 0; i < 20; i++)
  {
    for (j = 0; j < numberOfPlayers; j++)
    {
      lc.setDigit(0, j, random(1, 7), false);
    }
    delay(100);
  }

  //Store die roll value for all numberOfPlayers players

  for (i = 0; i < numberOfPlayers; i++)
  {
    playerRoll[i] = random(1, 7);
  }

  // Blink die roll 3 times

  for (int i = 0; i < 3; i++)
  {
    lc.clearDisplay(0);

    delay(250);

    for (j = 0; j < numberOfPlayers; j++)
    {
      lc.setDigit(0, j, playerRoll[j], false);
    }

    delay(250);
  }

  //Clear button flags for next round

  for (i = 0; i < numberOfPlayers; i++)
  {
    buttonFlag[i] = 0;
  }

}

//Function for doing game stats calculations

void calculate()
{

  //Calculate pass number for each player

  totalMoving = 0;

  for (i = 0; i < numberOfPlayers; i++)
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

  for (i = 1; i < numberOfPlayers; i++)
  {
    playerPresent[i] = playerPresent[i] - playerPass[i] + playerPass[i - 1];
  }

  //Calculate total number in system in this round

  for (i = 0; i < numberOfPlayers; i++)
  {
    numberInSystem[roundCount] += playerPresent[i];
  }

  //Set other stats values

  roundInput = playerRoll[0];
  roundOutput = playerPass[numberOfPlayers-1];
  throughput += roundOutput;
  roundNumber = roundCount + 1;
  totalNumber = numberInSystem[roundCount];
  avgthroughput = throughput / roundNumber;

}

//Upload to processing

void upload()
{

  //Store calculations in the latter part of array and send a single array to processing

  numberInSystem[20] = roundNumber;
  numberInSystem[21] = roundInput;
  numberInSystem[22] = roundOutput;
  numberInSystem[23] = totalMoving;
  numberInSystem[24] = totalNumber;
  numberInSystem[25] = throughput;
  numberInSystem[26] = avgthroughput;

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

  if (roundNumber == 20)
  {
    roundCount = 0;
    roundNumber = 0;
    playerPass[numberOfPlayers] = {0};
    for (i = 0; i < 25; i++)
    {
      numberInSystem[i] = 0;
    }
    playerRoll[numberOfPlayers] = {0};

    for (i = 0; i < numberOfPlayers; i++)
    {
      playerPresent[i] = 4;
    }

    roundInput = 0;
    roundOutput = 0;
    totalMoving = 0;
    totalNumber = 0;
    throughput = 0;
    avgthroughput = 0;
  }
}

void setup() {

  randomSeed(analogRead(A0));

  Serial.begin(9600);

  for (i = 0; i < numberOfPlayers; i++)
  {
    buttonFlag[i] = 0;
  }

  //Set initial value of chips to 4 for each player

  for (i = 0; i < numberOfPlayers; i++)
  {
    playerPresent[i] = 4;
  }

  lc.shutdown(0, false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0, 15);
  /* and clear the display */
  lc.clearDisplay(0);

}

//Continuously check for button press and roll dice when all numberOfPlayers are pressed

void loop() {

  for (i = 0; i < numberOfPlayers; i++)
  {
    if (digitalRead(buttonArray[i]) == HIGH || buttonFlag[i] == 1)
    {
      lc.setDigit(0, i, random(1, 7), false);
      buttonFlag[i] = 1;
    }
  }

  for (i = 0; i < numberOfPlayers; i++)
  {
    Serial.print(buttonFlag[i]);
  }

  Serial.println();

  flag=1;

  for (i = 0; i < numberOfPlayers; i++)
  {
    flag&=buttonFlag[i];
  }
      
  
  if (flag==1) //roll dice, calculate and upload data to processing only if all buttons are pressed
  {
    rollDice();
    calculate();
    upload();
  }

  delay(150);

}
