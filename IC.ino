// include the library code:
#include <LiquidCrystal.h>
#include <MyMenu.h>
#include "StepperIC.h"

#define PulsesPerMM 43 
Stepper stX(PulsesPerMM, 10, 9, A0, 1);
Stepper stZ(PulsesPerMM, 8, 7, A1, 1);

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
prog_char string_0[] PROGMEM = "  101  1000 &X100&Y100&W10";
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
  string_5};
#define TESTLEN 300
char buffer[TESTLEN];    // make sure this is large enough for the largest string it must hold


#define NUM_ITEMS 32 //length of items array include submenu headers
// DO NOT FORDGET UPDATE LENGTH
MItm items[NUM_ITEMS] = {
  MItm("ImmunoComb Tester",0,0), //main header always 0,0
  MItm("Select test",1), //1 is target submenu index
  MItm("Settings",2),
  
  MItm("Settings",2,0), 
  MItm("Home X",1001),
  MItm("Home Z",1002),
  MItm("X+1",1003),
  MItm("X-1",1004),
  MItm("Z+1",1005),
  MItm("Z-1",1006),
  
  

  MItm("Infection",1,0), //this is the submenu with index 1 (0 is previous menu index)
  MItm("HIV",10), // if there is no "50" submenu callback will run
  MItm("Hepatitis",20),
  MItm("Chlamidia",30),
  MItm("TORCH",40),
  MItm("Helicobacter",50),
  MItm("Others",60),
  MItm(" <<BACK",0),  
  
  MItm("HIV",10,1), // if there is no "50" submenu callback will run
  MItm("HIV BiSpot",101), 
  MItm("HIV Combfirm",102),
  MItm("HIV TriSpot",103),
  MItm(" <<BACK",1),  
 
  MItm("HEPATITIS",20,1), // if there is no "50" submenu callback will run
  MItm("HAV Ab",201), 
  MItm("HAV IgM",202),
  MItm("HBs Ag'90",203),
  MItm("HBc IgG",204),
  MItm("HBc IgM",205),
  MItm("HCV",206),
  MItm(" <<BACK",1),  
 
  MItm(" <<MAIN",0),  

};
// DO NOT FORDGET UPDATE LENGTH
Menu menu(items,NUM_ITEMS,&lcd,menuCallback);

void home(int axis){
  if ( axis == 0 )
  {
    while (digitalRead(LIMIT_X)==LOW)
    {stX.step(-10);
      lcd.setCursor(0,1);
    }
    while (digitalRead(LIMIT_X)==HIGH)
    {stX.step(1);}
  }
}

long ppm = 42666;// pulses per mm * 1000

int move(Stepper st, int steps)
{st.step(steps*ppm/1000);}

void move_to(Stepper st, int steps)
{st.step(steps*ppm/1000-st.pos);}


void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(20, 4);
  RIGHT = A2;
  UP = A6;
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
  if(digitalRead(LEFT) == HIGH){
    menu.goBack();
    delay(100); 
  }
  if(digitalRead(DOWN) == HIGH){
    menu.goDown();
    delay(100); 
  }
  if(digitalRead(UP) == HIGH){
    //menu.goUp();
    //delay(100); 
  }
  if(digitalRead(RIGHT) == HIGH){
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

void menuCallback(int idx){
  //do something according to index of item selected
  lcd.setCursor(0,0);
  lcd.print(1);
  if (idx==1001) stX.home();
  if (idx==1002) home(2);
  if (idx==1003) move(stX, 10);
  if (idx==1004) move(stX,-10);
  if (idx==1005) move(stZ, 10);
  if (idx==1006) move(stZ,-10);
  
  if (idx>100 & idx>900)
  { 
   // Run test!!!    
   int i;
   char buf[6];
   
   for (i=0; i<TESTSLEN; i++)
   {
      strcpy_P(buffer, (char*)pgm_read_word(&(tests[i])));
      char sbuf[50];
      memcpy( sbuf, &buffer[0], 5 );
      if (idx == atoi(sbuf) ) 
      { 
        lcd.clear();
        lcd.print( "Start testing!" );
        memcpy( sbuf, &buffer[6], 5 );        
        int time = atoi(sbuf);
        lcd.print(time/60);
        lcd.print(", ");
        lcd.setCursor(0,1);
        sprintf(sbuf, "Time %02d:%02d:%02d",time/3600%60,time/60%60,time%60);
        lcd.print(sbuf);  
        memset(sbuf, 0, sizeof sbuf);
        int j = 12;
        lcd.setCursor(0,3);
        lcd.print((int)millis);  
        
        for (i==j;i<TESTLEN;i++)  
        {
          bool get_num = false;
          lcd.setCursor(0,3);
          lcd.print((int)millis);  
          
          if (buffer[i]=='&')
          {// exec sbuf;
            j=i;
            lcd.setCursor(0,0);
            lcd.print("!!!");  
            lcd.setCursor(0,2);
            lcd.print(sbuf);  
            
            wait(RIGHT);
            if (sbuf[0]=='H')
            {
              
            }
          }else
          {
            // store command
            sbuf[i-j] = buffer[i];
            lcd.setCursor(0,2);
            lcd.print(sbuf);  
            sbuf[i-j+1] = char(0);
          }
          delay(400);
          
        }
        delay(4000);
        break;
      }
   }
  }
  menu.goLast(); //return to last viewed menu
  
}


