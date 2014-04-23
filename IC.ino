// include the library code:
#include <LiquidCrystal.h>
#include "StepperIC.h"
#include "MenuIC.h"



#define PulsesPerMM 42.666666
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
//  W\d+ - wash sec              5B    6B    300B-5-6
//                             |     |     |                  A                              B                              C                              D                                E                        F                          E              Final    
prog_char string_0[] PROGMEM = "  101  4020 &H2&H0 &X66.5 &Z-16&I5&H2&P25     &H0&X71.5 &Z-25&W5&H2&P34     &H0&X76.5 &Z-25&I5&H2&P43      &H0&X81.5 &Z-25&W5&H2&P52     &H0&X86.5 &Z-25&W5&H2&P61     &H0&X81.5 &Z-25&I5&H2&P70     &X-5 &Z-25&W5&H2&P69  &X15&   ";
prog_char string_1[] PROGMEM = "  401  4020 &H2&H0 &X66.5 &Z-16&I1800&H2&P25  &H0&X71.5 &Z-25&W120&H2&P34   &H0&X71.5 &Z-25&I1200&H2&P43   &H0&X81.5 &Z-25&W120&H2&P52   &H0&X86.5 &Z-25&W120&H2&P61   &H0&X81.5 &Z-25&I600&H2&P70   &X-5 &Z-25&W60&H2&P69 &X15&   ";
prog_char string_2[] PROGMEM = "  102  1000 &String 1";
prog_char string_3[] PROGMEM = "  103       &String 2";
prog_char string_4[] PROGMEM = "  104       &String 3";
prog_char string_5[] PROGMEM = "  105       &String 4";
prog_char string_6[] PROGMEM = "  201       &String 5";

PROGMEM const char *tests[] =
{
  string_0,
  string_1,
  string_2,
  string_3,
  string_4,
  string_5,
  string_6
};
#define TESTLEN 300
char buffer[TESTLEN];    // make sure this is large enough for the largest string it must hold


#define NUM_ITEMS 37 //length of items array include submenu headers
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
  MItm("Mega Wash", 1012, 2),
  MItm("Test X", 1013, 2),  
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
  
  MItm("CMV IgM", 401, 40),
  MItm("..BACK", -10, 40),
  
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

void move_to(Stepper st, float mm)
{
  st.step(mm - st.pos);
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

void wash(long sec)
{
  long time = millis() + sec*long(1000);
  while (time > millis())
  {
    move(stZ, 6);
    move(stZ, -6);
    move(stX, 2);
    move(stX, -4);
    move(stX, 2);
    timeleft(); // echo time left    
  }
}

void incubate(long sec)
{
  long del = millis();
  long t1 = millis() + sec*long(1000);
  while (t1 > millis())
  {  
    if (del<millis()) {
       wash(30);
       del=millis()+long(180)*long(1000);
     }
     timeleft(); // echo time left
  }
}

int time;
long timestart;
long last_time_left=0;

void timeleft(){
  if (last_time_left<millis())
  {
    last_time_left=millis()+20000;
    lcd.setCursor(0, 1);
    lcd.print("Min left:       ");
    lcd.setCursor(10, 1);  
    lcd.print( (time-(millis()-timestart)/1000)/60 );
  }
}



void menuCallback(int idx) {
  //do something according to index of item selected
//  lcd.setCursor(0,0);
//  lcd.print("@");
//  lcd.print(idx);
  
  if (idx == 1001) stX.home();
  if (idx == 1002) stZ.home();
  if (idx == 1003) {move(stX, 5);
      lcd.setCursor(0,2);
      lcd.print(stX.steps);
      lcd.print(" ");
      lcd.print(stX.pos);
      lcd.print(" ");
      lcd.print(stX.ppm);
      lcd.print(" ");
      lcd.print(stX.pos);
      
      delay(1000);      
  }
  if (idx == 1004) {move(stX, -5);
      lcd.setCursor(0,2);
      lcd.print(stX.steps);
      delay(1000);
  }
  if (idx == 1005) move(stZ, 5);
  if (idx == 1006) move(stZ, -5);

  if (idx == 1007) move(stX, 1);
  if (idx == 1008) move(stX, -1);
  if (idx == 1009) move(stZ, 1);
  if (idx == 1010) move(stZ, -1);
  if (idx == 1011) wash(5);
  if (idx == 1012) 
  for (int i=0; i<100; i++)
    {
      lcd.setCursor(0,2);
      lcd.print(stX.steps);
      lcd.setCursor(0,3);
      lcd.print(stZ.steps);
      wash(5);
      
    }
  if (idx == 1013) {
    while (digitalRead(stX.home_pin) == HIGH)
    {
        float mm = random(1,500)/10;
        move(stX, mm);
        move(stX,-mm);
    }
  }
  
  
 


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
        lcd.setCursor(0,0);
        lcd.print( "TESTING IN PROGRESS" );
        memset(buf,int("0"),6); 
        memcpy( sbuf, &buffer[6], 5 );
        time = atoi(sbuf);
        timestart = millis();
//        lcd.setCursor(0, 0);
//        sprintf(sbuf, "%02d:%02d:%02d", time / 3600 % 60, time / 60 % 60, time % 60);
//        lcd.setCursor(0, 1);
//        lcd.print("Test time: ");
        memset(sbuf, int(" "), sizeof sbuf);
        int j = 13;
        for (i = j; i < TESTLEN; i++)
        {
          timeleft();                  
          lcd.setCursor(0,0);
          lcd.print( "TESTING IN PROGRESS" );
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
              lcd.setCursor(0, 4);
              lcd.print("                    ");
              lcd.setCursor(0, 4);
              lcd.print("Homing ");
              lcd.print(sbuf[1]);
              lcd.print("...");
              if (sbuf[1]=='0') {home(stX);}
              else {home(stZ);}
            }

            if (sbuf[0] == 'X' || sbuf[0] == 'Z')
            {
              memcpy( buf, &sbuf[1], 5 );              
              float mm = atof(buf);
              lcd.setCursor(0, 4);
              lcd.print("                         ");
              lcd.setCursor(0, 4);
              lcd.print("Moving ");
              lcd.print(sbuf[0]);
              lcd.print(mm);
              lcd.print("mm...");            
              move( (sbuf[0]=='X'?stX:stZ), mm);
            }
            if (sbuf[0]=='W') //wash/
            {
              memcpy( buf, &sbuf[1], 5 );              
              int sec = atoi(buf);
              lcd.setCursor(0, 4);
              lcd.print("                         ");
              lcd.setCursor(0, 4);
              lcd.print("Washing ");
              lcd.print(sec/60);                
              lcd.print(" min...");
              wash(sec);
//              wait(RIGHT);
             }
             if (sbuf[0]=='I') //incubate
             {
                memcpy( buf, &sbuf[1], 5 );              
                int sec = atoi(buf);
                lcd.setCursor(0, 4);
                lcd.print("                         ");
                lcd.setCursor(0, 4);
                lcd.print("Incubating ");
                lcd.print(sec/60);                
                lcd.print(" min...");
                incubate(sec);
//                wait(RIGHT);
             }
             if (sbuf[0]=='P') //wipe
             {
                lcd.setCursor(0, 4);
                lcd.print("                         ");
                lcd.print("Wiping...");
                memcpy( buf, &sbuf[1], 5 );              
                float mm = atof(buf);
                move(stX,-mm);                
                move(stZ,-27.5);                
                move(stZ,10);                
                move(stX,2);                
                move(stZ,-10);                
                home(stZ);
                move(stX,mm-2);                
//                wait(RIGHT);  
               timeleft(); // echo time left
           }
            
          }


        }
        break;
      }
    }
  }
 menu.drawSub(); //return to last viewed menu

}


