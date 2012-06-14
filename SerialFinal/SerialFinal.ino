#include "MediaWall.h"
#include "Helvetica__10.h"
#include "Helvetica__11.h"
#include "Helvetica__12.h"
#include "Helvetica__14.h"
#include "Helvetica__16.h"
#include "Helvetica__18.h"
#include "Helvetica__22.h"
#include "Helvetica_World__20.h"
#include "Helvetica_World__12.h"
#include "Helvetica_World__24.h"

MediaWall LCD(240,128,6,32, 2);// 240x64 Pixel and 6x8 Font

int incomingByte = 0;	// for incoming serial data
int mode = 0;

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

void setup(){
  Serial.begin(115200);
  /*Serial.print("TH: ");
  Serial.println(LCD.getTH());
  Serial.print("GH: ");
  Serial.println(LCD.getGH());*/
  
  //Serial.println((int)LCD.checkStatus());
  
  LCD.Initialize();
  
  //Serial.println((int)LCD.checkStatus());
  //LCD1.Initialize();
  //LCD2.Initialize();
  
  Serial.println("Initialized");
  
  LCD.TextGoTo(0,0);
  emptyBuffer();
}

void loop(){
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();
    if (mode == 0) {
      if (incomingByte == 241) {
        LCD.clearText();
        LCD.TextGoTo(0,0);
        LCD.clearGraphic();
      } else if (incomingByte == 243) {
        mode = 1;
      } else if (incomingByte == 245) {
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
    }
    Serial.println(incomingByte);
  }
}  

