/*

The Electronic Dice Game

Last modified on December 13, 2016 by
Mustafa Lokhandwala, Fresher Foundry. 

*/


//Import the necessary libraries

import org.gicentre.utils.stat.*;  
import processing.serial.*;
import java.awt.*; 

//Declare serial object and barchart object

Serial port;
BarChart barChart;

//Declare variables and arrays
float total[]=new float[27];
float numberInSystem[]=new float[20];
float avgthroughput;
String message[]=new String[60];
String a[]=new String[27];
boolean flag=false;
PFont mono;
PFont mono_bold;

void setup()
{ 
  fullScreen();                                  //For displaying graph fullscreen
  port = new Serial(this,"/dev/ttyUSB0", 9600);  //creating new serial object
  
  //Wait until newline character, then call serialEvent()
  if (flag==false)
  {
    port.bufferUntil('\n');
  }
}

void serialEvent(Serial port)
{
  flag=true;
  port.write('A');                               //send A to Arduino
  println("Serial Event Triggered");
  while (port.available()>0)
  {     
    message[0] = port.readStringUntil('\n');     //read and store serial data as string in array until newline character is encountered
  } 

}

//Function to draw bar chart

void drawBarChart()
{
  avgthroughput = total[25]/total[20];           //Calculate average throughput (total throughput/round Number)
  
  //Typecast all float numbers into string with fixed decimal place
  
  String avgThroughput = String.format("%.02f", avgthroughput);
  String roundNumber = String.format("%.0f", total[20]);
  String input = String.format("%.0f", total[21]);
  String output = String.format("%.0f", total[22]);
  String totalThroughput = String.format("%.0f", total[25]);
  String numberSystem = String.format("%.0f", total[24]);
  String partsMoving = String.format("%.0f", total[23]);
  
  //Create new barchart object
  
  barChart = new BarChart(this);
  barChart.setData(numberInSystem);
  barChart.setBarLabels(new String[] {"Round 1","Round 2","Round 3",
                                       "Round 4","Round 5","Round 6","Round 7","Round 8",
                                       "Round 9","Round 10","Round 11","Round 12","Round 13",
                                       "Round 14","Round 15","Round 16","Round 17","Round 18",
                                       "Round 19","Round 20"});
                                       
  //Misc graph formatting
  
  mono = createFont("TT0248M_.TTF",10);
  mono_bold = createFont("TT0249M_.TTF",10);
  barChart.showValueAxis(true);
  barChart.showCategoryAxis(true);
  barChart.setMinValue(0);
  barChart.setMaxValue(160);
  textFont(mono);
  textSize(12);
  fill(6,6,6);
  barChart.setBarColour(color(247,143,40));
  barChart.setBarGap(10);
  background(255);
  barChart.draw(15,15,width-50,height-150);
  textFont(mono_bold);
  fill(6,6,6);
  textSize(50);
  text("The Electronic Dice Game", 420,60);
  textFont(mono);
  textSize(20);
  text("Round No.= "+roundNumber, 70,120);
  textSize(20);
  text("Input this round= "+input, 70,150);
  textSize(20);
  text("Output this round= "+output, 70,180);
  textSize(20);
  text("No. of parts moving through assembly= "+partsMoving, 70,210);
  textSize(20);
  text("Number in system= "+numberSystem, 70,240);
  textSize(20);
  text("© 2016 Fresher Foundry. All Rights Reserved.", 500,670);
  textFont(mono_bold);
  fill(255,0,0);
  textSize(30);
  text("Total throughput= "+totalThroughput, 950,160);
  textSize(30);
  text("Average throughput= "+avgThroughput, 950,210);
}

void draw()
{ 
  if(message[0] != null && message.length != 0)  //Only process string array from serial data if 
   {
      println("Message obtained");
      a=split(message[0], ',');
      if (a != null && a.length == 27 )          //Only save string array as float array if string length is 27
      {
        for (int i=0; i<27; i++)
        {
          total [i] =Float.parseFloat(a[i]);
        }
        println("Array obtained");
        System.arraycopy(total, 0, numberInSystem, 0 , numberInSystem.length);   //copy first 20 elements of float array to another array for making graph
        drawBarChart();                                                          //draw bar chart
      }
      else flag=false;                                                           //only read serial port if correct message is not obtained
   }
}