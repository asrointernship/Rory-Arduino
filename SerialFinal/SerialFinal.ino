#include "MediaWall.h"
#include "Helvetica_World__20.h"
#include "Helvetica_World__12.h"

MediaWall LCD(240,128,6,32, 2);// 240x64 Pixel and 6x8 Font

int incomingByte = 0;	// for incoming serial data
int mode = 0;

//Pixel
boolean xIn = true;

//TextBox
int x1 = 0;
int y1 = 0;
int x2 = 0;
int y2 = 0;
char buffer[300];
const FONT_DEF* font = &Helvetica_World__12;
int textType = 0;

int stage = 0;

void emptyBuffer() {
  buffer[0] = 0;
}

void clearPixels() {
  for (int i = x1; i < x2; i++) {
    for (int j = y1; j < y2; j++) {
      LCD.clearPixel(i, j);
    }
  }
}

void setup(){
  Serial.begin(115200);
  
  LCD.Initialize();
  
  Serial.println("Initialized");
  
  LCD.TextGoTo(0,0);
  emptyBuffer();
}

/*
241 = reset screen

242 followed by number (1-3) = select screen

243 = textbox information will follow
After 243 send font, y1, x1, x2, y2, text in that order
If it's a title or a subtitle we don't need x1, x2, y2
At the end of a textbox stream send 244

245 = pixels 244 to end stream
*/

void loop(){
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();
    if (mode == 0) {
      if (incomingByte == 241) { // reset
        LCD.clearText();
        LCD.TextGoTo(0,0);
        LCD.clearGraphic();
      } else if (incomingByte == 242) { // screen swap
        mode = 3;
      } else if (incomingByte == 243) { // text
        mode = 1; // 244 = end stream
      } else if (incomingByte == 245) { // pixel
        mode = 2;
      }
    } else if (mode == 1) { // text
      if (textType == 0) {
        textType = incomingByte;
      } else if (textType == 3) { // method = textbox
        if (stage == 0) { // x1
          y1 = incomingByte;
          stage++;
        } else if (stage == 1) {
          x1 = incomingByte;
          stage++;
        } else if (stage == 2) {
          x2 = incomingByte;
          stage++;
        } else if (stage == 3) {
          y2 = incomingByte;
          stage++;
        } else if (stage == 4) { // text
          if (incomingByte == 244) {
            stage = 0;
            mode = 0;
            textType = 0;
            //clearPixels();
            LCD.glcd_print1_textbox(x1, y1, x2, y2, buffer, font, 0, 0);
            emptyBuffer();
          } else {
            char a[2] = {incomingByte};
            strcat(buffer, a);
          }
        }
      } else if (textType == 2 || textType == 1) { // method = half font title
        if (stage == 0) { // x1
          x1 = incomingByte;
          stage++;
        } else if (stage == 1) {
          if (incomingByte == 244) {
            stage = 0;
            mode = 0;
            if (textType == 1) {
              LCD.glcd_print1_half_right(x1, buffer, &Helvetica_World__20, 0);
            } else if (textType == 2) {
              LCD.glcd_print1_right(x1, buffer, &Helvetica_World__20, 0, 1);
            } 
            textType = 0;
            emptyBuffer();
          } else {
            char a[2] = {incomingByte};
            strcat(buffer, a);
          }
        }
      }
    } else if (mode == 2) { // pixel
      if (incomingByte == 244) {
        mode = 0;
      }else {
        if (xIn) {
          x1 = incomingByte;
          xIn = false;
        } else {
          y1 = incomingByte;
          LCD.setPixel(x1, y1);
          xIn = true;
        }
      }
    } else if (mode == 3) { // screen
      byte screen;
      if (incomingByte == 1) {
        screen = 2;
      } else if (incomingByte == 2) {
        screen = 4;
      } else if (incomingByte == 3) {
        screen = 5;
      }
      LCD.setCE(screen);
      LCD.Initialize();
      mode = 0;
    }
    Serial.println(incomingByte);
  }
}  

