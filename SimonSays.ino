#include <Adafruit_GFX.h>
#include <gfxfont.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

#include <stdio.h>
#include <stdlib.h>

#define TFT_CS     10
#define TFT_RST    9  // you can also connect this to the Arduino reset
                      // in which case, set this #define pin to 0!
#define TFT_DC     8

// Option 1 (recommended): must use the hardware SPI pins
// (for UNO thats sclk = 13 and sid = 11) and pin 10 must be
// an output. This is much faster - also required if you want
// to use the microSD card (see the image drawing example)
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

// Option 2: use any pins but a little slower!
#define TFT_SCLK 13   // set these to be whatever pins you like!
#define TFT_MOSI 11   // set these to be whatever pins you like!
//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);


//definitions of colors not provided in Adafruit_GFX library
#define CYAN      0x07FF
#define MAGENTA   0XF81F
#define MAROON    0x7800
#define PURPLE    0x780F
#define ORANGE    0xFD20
#define PINK      0xF81F


unsigned long randNumber = 0;
unsigned int iterations = 0;
int simonButtons[10];
int mode = 0;

//*************************************************************************//
//*************************Graphics functions******************************//
//*************************************************************************//
void drawtext(int x, int y, String text, uint16_t color, int tsize) {
  tft.setCursor(x, y);
  tft.setTextSize(tsize);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.setRotation(3);
  tft.print(text);
}


void drawchar(int x, int y, char text, uint16_t color, int tsize) {
  tft.setCursor(x, y);
  tft.setTextSize(tsize);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.setRotation(3);
  tft.print(text);
}

void flashingIntro(){
  for(int i = 0; i < 4; ++i){
    drawtext(20, 50, "HOWDY!!", MAROON, 3);
    delay(500);
    tft.fillScreen(ST7735_WHITE);
    delay(500);
  }
  tft.fillScreen(ST7735_BLACK);
}


void projectTitle(){
  drawtext(20, 10, "DUSSEK", ST7735_RED, 3);
  drawtext(0, 35, "Dual", ST7735_WHITE, 2);
  drawtext(9, 50, "Use", ST7735_WHITE, 2);
  drawtext(18, 65, "Symon", ST7735_WHITE, 2);
  drawtext(27, 80, "Says", ST7735_WHITE, 2);
  drawtext(80, 85, "and", ST7735_WHITE, 1);
  drawtext(36, 95, "Electronic", ST7735_WHITE, 2);
  drawtext(45, 110, "Keyboard", ST7735_WHITE, 2);
  delay(5000);
}

int randomColor(){
  int num = random(1, 11);
  switch(num){
    case 1:
      return ST7735_WHITE;
      break;
      
    case 2:
      return ST7735_BLUE;
      break;

    case 3:
      return ST7735_GREEN;
      break;

    case 4:
      return ST7735_YELLOW;
      break;

    case 5:
      return ST7735_RED;
      break;

    case 6:
      return CYAN;
      break;

    case 7:
      return MAGENTA;
      break;

    case 8:
      return MAROON;
      break;

    case 9:
      return PURPLE;
      break;

    case 10:
      return ORANGE;
      break;

    case 11:
      return PINK;
      break;
  }
}


void drawMultiColor(String text){
  int x = 0;
  int y = 0;
  for(int i = 0; i < text.length(); ++i){
    char temp = text.charAt(i);
    drawchar(x, y, temp, randomColor(), 3);
    x += 18;
  }
}
//*************************************************************************//
//*************************************************************************//
//*************************************************************************//


  
void setup() {
  Serial.begin(9600);  
  randomSeed(A5);
  pinMode(1,OUTPUT);
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  
  tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab
  tft.fillScreen(ST7735_WHITE);
    
  flashingIntro();
  projectTitle();
  tft.fillScreen(ST7735_BLACK);
}

void loop() {
  if(mode == 0){
    pianoMode();
  }
  if(mode == 1){
    simonSays();  
  }
}

//*************************************************************************//
//*************************Simon Says Gameplay*****************************//
//*************************************************************************//
void simonSays(){
  randNumber = random(1, 14);
  if(iterations == 10){
    iterations = 0;
  }
  simonButtons[iterations] = (int) randNumber;
  iterations = iterations + 1;
  int counter = 0;

  int i;
  Serial.println("Press Buttons in this Order: ");
  for (i = 0; i < iterations; i = i + 1) {
     Serial.println(simonButtons[i]);
     setPins(simonButtons[i]);
     delay(500);
  }
  
  Serial.println("-------------");
    
  while(counter < iterations){
    int val1 = analogRead(A0);
    int val2 = analogRead(A1);
    int val3 = analogRead(A2);
    int val4 = analogRead(A3);
    while(val1 < 5 && val2 < 5 && val3 < 5 && val4 < 5){
        delay(100);
        val1 = analogRead(A0);
        val2 = analogRead(A1);
        val3 = analogRead(A2);
        val4 = analogRead(A3);
    }
   
      int button_pressed =  determine_button(val1,val2, val3, val4);
      char buttonPressed = convertToKey(val1, val2, val3, val4);
      setPins(button_pressed);
      
       delay(250);
       digitalWrite(1, LOW);
       digitalWrite(2, LOW);
       digitalWrite(3, LOW);
       digitalWrite(4, LOW);        
//        if(button_pressed > 0)
//        { 
//          String phrase = "button ";
//          String output = phrase + button_pressed;
//          Serial.println(output);
//        }
    if(simonButtons[counter] == button_pressed){
      counter = counter + 1;
    }
    else{
      Serial.print("Failed, Final Score: ");
      Serial.println(iterations - 1);
      tft.fillScreen(ST7735_BLACK);
      drawtext(25, 0, "Game Over", ST7735_RED, 2);
      drawtext(10, 50, "Final Score", ST7735_WHITE, 2);
      char buff [sizeof(int) * 8 + 1];
      drawtext(70, 90, itoa(iterations, buff, 10), ST7735_WHITE, 3);
      delay(5000);
      mode = 0;
      iterations = 10;
      counter = 10;
      tft.fillScreen(ST7735_BLACK);
      break;
    }
    delay(250);
    Serial.println(button_pressed);
    char buff [sizeof(int) * 8 + 1];
    tft.fillScreen(ST7735_BLACK);
    drawchar(70, 50, buttonPressed, randomColor(), 5);
  }  
}
//*************************************************************************//
//*************************************************************************//
//*************************************************************************//



//*************************************************************************//
//*************************Free Play (Piano) Mode**************************//
//*************************************************************************//
void pianoMode(){
    drawtext(20, 20, "Piano", randomColor(), 4);
    drawtext(30, 70, "Mode", randomColor(), 4);
    int val1 = analogRead(A0);
    int val2 = analogRead(A1);
    int val3 = analogRead(A2);
    int val4 = analogRead(A3);
    while(val1 < 5 && val2 < 5 && val3 < 5 && val4 < 5){
        delay(100);
        val1 = analogRead(A0);
        val2 = analogRead(A1);
        val3 = analogRead(A2);
        val4 = analogRead(A3);
    }
    
    int button_pressed =  determine_button(val1,val2, val3, val4);
    setPins(button_pressed);
    Serial.println(button_pressed);
    
    delay(250);
    digitalWrite(1, LOW);
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);        
}
//*************************************************************************//
//*************************************************************************//
//*************************************************************************//


char convertToKey( int input1, int input2, int input3, int input4){
  //converts integer value to coressponding key on musical scale
  //used for LCD screen
    if(input1 > 1000)
    {
       return 'A';
    }
    else if(input1 > 500)
    {
       return 'B';
    }
    else if(input1 > 300)
    {
      return 'C';
    }
    else if(input1 > 50)
    {
      return 'D';
    }
    else if(input2 > 1000)
    {
      return 'E';
    }
    else if(input2 > 500)
    {
      return 'F';
    }
    else if(input2 > 300)
    {
      return 'G';
    }
    if(input3 > 1000)
    {
       return 'A';
    }
    else if(input3 > 500)
    {
       return 'B';
    }
    else if(input3 > 300)
    {
      return 'C';
    }
    else if(input3 > 50)
    {
      return 'D';
    }
    else if(input4 > 1000)
    {
      return 'E';
    }
    else if(input4 > 500)
    {
      return 'F';
    }
    else if(input4 > 300)
    {
      return 'G';
    }
    else
    {
      return 'Z';
    }
}


int determine_button(int input1, int input2, int input3, int input4) // determines which button is pressed and outputs the corresponding binary code
{
    if(input1 > 1000)
    {
       return 1;
    }
    else if(input1 > 500)
    {
       return 2;
    }
    else if(input1 > 300)
    {
      return 3;
    }
    else if(input1 > 50)
    {
      return 4;
    }
    else if(input2 > 1000)
    {
      return 5;
    }
    else if(input2 > 500)
    {
      return 6;
    }
    else if(input2 > 300)
    {
      return 7;
    }
    if(input3 > 1000)
    {
       return 8;
    }
    else if(input3 > 500)
    {
       return 9;
    }
    else if(input3 > 300)
    {
      return 10;
    }
    else if(input3 > 50)
    {
      return 11;
    }
    else if(input4 > 1000)
    {
      return 12;
    }
    else if(input4 > 500)
    {
      return 13;
    }
    else if(input4 > 300)
    {
      return 14;
    }
    else
    {
      return 0;
    }
}

void setPins(int x){
  switch (x) {
    case 1:
       digitalWrite(1, LOW);
       digitalWrite(2, LOW);
       digitalWrite(3, LOW);
       digitalWrite(4, HIGH);
      break;
    case 2:
       digitalWrite(1, LOW);
       digitalWrite(2, LOW);
       digitalWrite(3, HIGH);
       digitalWrite(4, LOW);
      break;
    case 3:
       digitalWrite(1, LOW);
       digitalWrite(2, LOW);
       digitalWrite(3, HIGH);
       digitalWrite(4, HIGH);
      break;
    case 4:
       digitalWrite(1, LOW);
       digitalWrite(2, HIGH);
       digitalWrite(3, LOW);
       digitalWrite(4, LOW);
      break;
    case 5:
       digitalWrite(1, LOW);
       digitalWrite(2, HIGH);
       digitalWrite(3, LOW);
       digitalWrite(4, HIGH);
      break;
    case 6:
       digitalWrite(1, LOW);
       digitalWrite(2, HIGH);
       digitalWrite(3, HIGH);
       digitalWrite(4, LOW);
      break;
    case 7:
       digitalWrite(1, LOW);
       digitalWrite(2, HIGH);
       digitalWrite(3, HIGH);
       digitalWrite(4, HIGH);
        break;
    case 8:
       digitalWrite(1, HIGH);
       digitalWrite(2, LOW);
       digitalWrite(3, LOW);
       digitalWrite(4, HIGH);
      break;
    case 9:
       digitalWrite(1, HIGH);
       digitalWrite(2, LOW);
       digitalWrite(3, HIGH);
       digitalWrite(4, LOW);
      break;
    case 10:
       digitalWrite(1, HIGH);
       digitalWrite(2, LOW);
       digitalWrite(3, HIGH);
       digitalWrite(4, HIGH);
      break;
    case 11:
       digitalWrite(1, HIGH);
       digitalWrite(2, HIGH);
       digitalWrite(3, LOW);
       digitalWrite(4, LOW);
      break;
    case 12:
       digitalWrite(1, HIGH);
       digitalWrite(2, HIGH);
       digitalWrite(3, LOW);
       digitalWrite(4, HIGH);
      break;
    case 13:
       digitalWrite(1, HIGH);
       digitalWrite(2, HIGH);
       digitalWrite(3, HIGH);
       digitalWrite(4, LOW);
      break;
    case 14:
       digitalWrite(1, HIGH);
       digitalWrite(2, HIGH);
       digitalWrite(3, HIGH);
       digitalWrite(4, HIGH);
      break;
  }
  if(x < 8)
  {
    digitalWrite(5,LOW);
    digitalWrite(6,HIGH);
  }
  else
  {
    digitalWrite(5,HIGH);
    digitalWrite(6,LOW);
  }
}














