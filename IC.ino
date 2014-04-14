// include the library code:
#include <LiquidCrystal.h>
#include "MyMenu.h"
#include "StepperIC.h"


#define PulsesPerMM 42666
Stepper stX(PulsesPerMM, 8, 7, A0, 1);
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
prog_char string_0[] PROGMEM = "  101  1000 &X100&Z-10&W30&I30&Z100&X100&";
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
  MItm("ImmunoComb Tester", 0, 0), //main header always 0,0
  MItm("Select test", 1), //1 is target submenu index
  MItm("Settings", 2),

  MItm("Settings", 2, 0),
  MItm("Home X", 1001),
  MItm("Home Z", 1002),
  MItm("X+1", 1003),
  MItm("X-1", 1004),
  MItm("Z+1", 1005),
  MItm("Z-1", 1006),
  MItm(" <<Back", 0),



  MItm("Infection", 1, 0), //this is the submenu with index 1 (0 is previous menu index)
  MItm("HIV", 10), // if there is no "50" submenu callback will run
  MItm("Hepatitis", 20),
  MItm("Chlamidia", 30),
  MItm("TORCH", 40),
  MItm("Helicobacter", 50),
  MItm("Others", 60),
  MItm(" <<BACK", 0),

  MItm("HIV", 10, 1), // if there is no "50" submenu callback will run
  MItm("HIV BiSpot", 101),
  MItm("HIV Combfirm", 102),
  MItm("HIV TriSpot", 103),
  MItm(" <<BACK", 1),

  MItm("HEPATITIS", 20, 1), // if there is no "50" submenu callback will run
  MItm("HAV Ab", 201),
  MItm("HAV IgM", 202),
  MItm("HBs Ag'90", 203),
  MItm("HBc IgG", 204),
  MItm("HBc IgM", 205),
  MItm("HCV", 206),
  MItm(" <<BACK", 1),

  MItm(" <<MAIN", 0),

};
// DO NOT FORDGET UPDATE LENGTH
Menu menu(items, NUM_ITEMS, &lcd, menuCallback);

void home(int axis) {
  if ( axis == 0 )
  {
    while (digitalRead(LIMIT_X) == LOW)
    { stX.step(-10);
      lcd.setCursor(0, 1);
    }
    while (digitalRead(LIMIT_X) == HIGH)
    {
      stX.step(1);
    }
  }
}

int move(Stepper st, float mm)
{
  st.step(round(mm * float(st.ppm) / 1000));
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
  stX.setSpeed(100000); // just max speed
  stZ.setSpeed(100000);

}


void loop() {
//  menuCallback(101) ;
  if (digitalRead(LEFT) == HIGH) {
    //menu.goBack();
    //delay(100);
  }
  if (digitalRead(DOWN) == HIGH) {
    menu.goDown();
    delay(100);
  }
  if (digitalRead(UP) == HIGH) {
    menu.goUp();
    delay(100);
  }
  if (digitalRead(RIGHT) == HIGH) {
    menu.goNext();
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
    move(stZ, 7);
    move(stZ, -7);
    move(stX, 5);
    move(stX, -10);
    move(stX, 5);
  }
}

void incubate(int sec)
{
  long del = millis();
  long time = millis() + sec*1000;
  while (time > millis())
  {  
    if (del<millis()) {
       wash(15);
       del=millis()+120*1000;

    }
  }
}

void menuCallback(int idx) {
  //do something according to index of item selected
  if (idx == 1001) stX.home();
  if (idx == 1002) home(2);
  if (idx == 1003) move(stX, 50);
  if (idx == 1004) move(stX, -50);
  if (idx == 1005) move(stZ, 22.5);
  if (idx == 1006) move(stZ, -22.5);

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
            j = i + 1;
            if (sbuf[0] == 'X' || sbuf[0] == 'Z')
            {
              memcpy( buf, &sbuf[1], 5 );              
              float mm = atof(buf);
              lcd.setCursor(0, 4);
              lcd.print("Move ");
              lcd.print(sbuf[0]);
              lcd.print(mm);
              
              move( (sbuf[0]=='X'?stX:stZ), mm);
              wait(RIGHT);
            }
            if (sbuf[0]=='C') //wash/
            {
              memcpy( buf, &sbuf[1], 5 );              
              int sec = atoi(buf);
              lcd.setCursor(0, 4);
              lcd.print("Washing ");
              lcd.print(sec);
              wash(sec);
              wait(RIGHT);
             }
             if (sbuf[0]=='I') //incubate
             {
                memcpy( buf, &sbuf[1], 5 );              
                int sec = atoi(buf);
                lcd.setCursor(0, 4);
                lcd.print("Incubating ");
                lcd.print(sec);
                incubate(sec);
                wait(RIGHT);
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
                wait(RIGHT);  
           }
            
          }


        }
        break;
      }
    }
  }
  menu.goLast(); //return to last viewed menu

}


