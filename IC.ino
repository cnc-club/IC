// include the library code:
#include <LiquidCrystal.h>
#include "StepperIC.h"
#include "MenuIC.h"



#define PulsesPerMM 42666
Stepper stX(PulsesPerMM, 8, 7, A0, -1);
Stepper stZ(PulsesPerMM, 10, 9, A1, 1);
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


int UP, DOWN, RIGHT, LEFT, LIMIT_X, LIMIT_Z;
#define TESTSLEN 6
// Procedure = &command&command&...
// command = [char-type][array char]
//                              <    ><    ><        ...>
//                              TestId Time  Procedure
//  Procedure: (X|Z)\d+ - move
//  I\d+ - incubate sec
//  W\d+ - wash sec
prog_char string_0[] PROGMEM = "  101  1000 &H2&H0&X66 &Z-20&I5&Z20&X5 &Z-20&I5&Z20&X5 &Z-20&I5&Z20&X5 &Z-20&I5&Z20&X5 &Z-20&I5&Z20&X5 &Z-20&I5&Z20&X5 &";
prog_char string_1[] PROGMEM = "  102  1000 &String 1";
prog_char string_2[] PROGMEM = "  103       &String 2";
prog_char string_3[] PROGMEM = "  104       &String 3";
prog_char string_4[] PROGMEM = "  105       &String 4";
prog_char string_5[] PROGMEM = "  201       &String 5";

PROGMEM const char *tests[] =
{
  string_0,
  string_1,
  string_2,
  string_3,
  string_4,
  string_5
};
#define TESTLEN 300
char buffer[TESTLEN];    // make sure this is large enough for the largest string it must hold


#define NUM_ITEMS 33 //length of items array include submenu headers
// DO NOT FORDGET UPDATE LENGTH
MItm items[NUM_ITEMS] = {
  MItm("ImmunoComb Tester", 0, -2), //main header always 0,0
  MItm("Infections", 1, 0), 
  MItm("HIV", 10, 1), 
  MItm("Hepatitis", 20, 1),
  MItm("Chlamidia", 30, 1),
  MItm("TORCH", 40, 1),
  MItm("Helicobacter", 50, 1),
  MItm("Others", 60, 1),
  MItm("..BACK", -10, 1),

  MItm("Settings", 2, 0),
  MItm("Home X", 1001, 2),
  MItm("Home Z", 1002, 2),
  MItm("X+5", 1003, 2),
  MItm("X-5", 1004, 2),
  MItm("Z+5", 1005, 2),
  MItm("Z-5", 1006, 2),
  MItm("X+1", 1007, 2),
  MItm("X-1", 1008, 2),
  MItm("Z+1", 1009, 2),
  MItm("Z-1", 1010, 2),
  MItm("Wash", 1011, 2),
  MItm("..Back", -10, 2),


  MItm("HIV BiSpot", 101, 10),
  MItm("HIV Combfirm", 102, 10),
  MItm("HIV TriSpot", 103, 10),
  MItm("..BACK", -10, 10),

  MItm("HAV Ab", 201, 20),
  MItm("HAV IgM", 202, 20),
  MItm("HBs Ag'90", 203, 20),
  MItm("HBc IgG", 204, 20),
  MItm("HBc IgM", 205, 20),
  MItm("HCV", 206, 20),
  MItm("..BACK", -10, 20),
};
// DO NOT FORDGET UPDATE LENGTH
Menu menu(items, NUM_ITEMS, &lcd, menuCallback);

void home(Stepper st) {
    st.home();  
}

int move(Stepper st, float mm)
{
  st.step(mm);
}

void move_to(Stepper st, long mm)
{
  st.step(mm * st.ppm / 1000 - st.pos);
}


void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(20, 4);
  RIGHT = A2;
  UP = A3;
  DOWN = A4;
  LEFT = A1;
  LIMIT_X = 0;
  LIMIT_Z = 1;
  pinMode(RIGHT, INPUT);
  pinMode(UP, INPUT);
  pinMode(DOWN, INPUT);
  pinMode(LEFT, INPUT);
  pinMode(LIMIT_X, INPUT);
  pinMode(LIMIT_Z, INPUT);
  menu.goMain();
  stX.setSpeed(1.5); // just max speed
  stZ.setSpeed(1);
  stX.dir_inv = -1;
  stZ.dir_inv = -1;
  

}


void loop() {
//  menuCallback(101) ;
  if (digitalRead(LEFT) == HIGH) {
    //menu.goBack();
    //delay(100);
  }
  if (digitalRead(DOWN) == HIGH) {
    menu.goNext();
    delay(100);
  }
  if (digitalRead(UP) == HIGH) {
    menu.goPrev();
    delay(100);
  }
  if (digitalRead(RIGHT) == HIGH) {
    menu.goDown();
    delay(100);
  }

  delay(100);
}

void wait(int button)
{
  while ( digitalRead(button) == HIGH ) delay(100);
  while ( digitalRead(button) == LOW ) delay(100);
}

void wash(int sec)
{
  long time = millis() + sec*1000;
  while (time > millis())
  {
    move(stZ, 6);
    move(stZ, -6);
    move(stX, 2);
    move(stX, -4);
    move(stX, 2);
  }
}

void incubate(int sec)
{
  long del = millis();
  long time = millis() + sec*1000;
  while (time > millis())
  {  
    if (del<millis()) {
       wash(5);
       del=millis()+120*1000;

    }
  }
}

void menuCallback(int idx) {
  //do something according to index of item selected
  lcd.setCursor(0,0);
  lcd.print("@");
  lcd.print(idx);
  
  if (idx == 1001) stX.home();
  if (idx == 1002) stZ.home();
  if (idx == 1003) move(stX, 5);
  if (idx == 1004) move(stX, -5);
  if (idx == 1005) move(stZ, 5);
  if (idx == 1006) move(stZ, -5);

  if (idx == 1007) move(stX, 1);
  if (idx == 1008) move(stX, -1);
  if (idx == 1009) move(stZ, 1);
  if (idx == 1010) move(stZ, -1);
  if (idx == 1011) wash(5);



  if (idx > 100 & idx < 900)
  {
    // Run test!!!
    int i;
    char buf[6];

    for (i = 0; i < TESTSLEN; i++)
    {
      strcpy_P(buffer, (char*)pgm_read_word(&(tests[i])));
      char sbuf[15];
      memcpy( sbuf, &buffer[0], 5 );
      if (idx == atoi(sbuf) )
      {
        lcd.clear();
        lcd.print( "Start testing!" );
        memset(buf,int("0"),6); 
        memcpy( sbuf, &buffer[6], 5 );
        int time = atoi(sbuf);
        lcd.setCursor(0, 0);
        sprintf(sbuf, "%02d:%02d:%02d", time / 3600 % 60, time / 60 % 60, time % 60);
        lcd.setCursor(0, 1);
        lcd.print("Test time: ");
        
        lcd.print(sbuf);
        
        memset(sbuf, int(" "), sizeof sbuf);
        int j = 13;
        for (i == j; i < TESTLEN; i++)
        {
          if (buffer[i] != '&')
          {
            // store command
            sbuf[i - j] = buffer[i];
            sbuf[i - j + 1] = char(0);
          }
          else //(buffer[i] == '&')
          { // exec sbuf;
//            wait(RIGHT);
            j = i + 1;
            if (sbuf[0] == 'H')
            {
              if (sbuf[1]=='0') {home(stX);}
              else {home(stZ);}
            }

            if (sbuf[0] == 'X' || sbuf[0] == 'Z')
            {
              memcpy( buf, &sbuf[1], 5 );              
              float mm = atof(buf);
              lcd.setCursor(0, 4);
              lcd.print("Move ");
              lcd.print(sbuf[0]);
              lcd.print(mm);
              
              move( (sbuf[0]=='X'?stX:stZ), mm);
//              wait(RIGHT);
            }
            if (sbuf[0]=='C') //wash/
            {
              memcpy( buf, &sbuf[1], 5 );              
              int sec = atoi(buf);
              lcd.setCursor(0, 4);
              lcd.print("Washing ");
              lcd.print(sec);
              wash(sec);
//              wait(RIGHT);
             }
             if (sbuf[0]=='I') //incubate
             {
                memcpy( buf, &sbuf[1], 5 );              
                int sec = atoi(buf);
                lcd.setCursor(0, 4);
                lcd.print("Incubating ");
                lcd.print(sec);
                incubate(sec);
//                wait(RIGHT);
             }
             if (sbuf[0]=='P') //wipe
             {
                lcd.print("Wiping ");
                move(stZ,-10);
                move(stZ,5);
                move(stX,1);
                move(stZ,-5);
                move(stZ,5);
                move(stX,1);
                move(stZ,-5);
                move(stZ,5);
                move(stX,1);
                move(stZ,10);
                move(stX,-3);                
//                wait(RIGHT);  
           }
            
          }


        }
        break;
      }
    }
  }
 menu.drawSub(); //return to last viewed menu

}


