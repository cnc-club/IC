/* ===============================================
|	//MyMenu library for LCD with 2+ lines of text
|
|   //Usage:
|
|   #define NUM_ITEMS 7 //total number of items in menu include submenu-headers
|	#define NUM_ROWS 4 //total number of rows in your lcd, must be greater than or equal to 2
|
|	MItm = items[NUM_ITEMS] = {
|		MItm("MAIN MENU title",0,0), //main header, always 0,0
|		MItm("Some item",1), //second parameter is submenu-index, which will be opened by this item
|		MItm("Second item",2),
|		MItm("FIRST SUBMENU",1,0), //sub-menu header (two input parameters), 1 is submenu index, any unique integer for submenus, 0 is previous-menu index for right hierarchy
|		MItm("Submenu item one",3),
|		MItm("Sub item two",4),
|		MItm("BACK",0) //optional BACK item
|	};
|
|	LiquidCrystal lcd(...); // your lcd initialization
|	String cursor = "-"; // the menu cursor, by default is ">"
|	Menu menu(items,NUM_ITEMS,&lcd,menuCallback,NUM_ROWS,cursor); //notice &lcd instead of lcd, we will work with pointer
|	menu.goMain(); // shows main menu on display
|
|	//Then you can call anywhere you want (on button press, for example)
|	menu.goDown(); // to list current menu down, this will move cursor from current item to next, but wont select it
|	menu.goUp(); // to list up
|	menu.goNext(); // to go to submenu or run callback (i.e "select item")
|	menu.goBack(); // to go to previous menu
|	menu.goSub(int sub_idx); // to go to submenu with index <sub_idx>
|	menu.goItem(int item_idx); // to go to submenu contains item with index (in "items" array) <item_idx> and move cursor to this item
|
|	//also you need to declare callback, it will be run if there is no submenu with current index
|	//e.g.
|	//If current item is MItm("Title",5) and you call goNext(), then:
|	//1. If you have Submenu-header with index 5 (MItm("SUBMENU",5,0)) it will be opened
|	//2. If you have not, callBack will be run width "5" as input parameter
|
|	void menuCallback(int idx){
|		Serial.println(idx);
|	}
|
| ===============================================
| Made by Denis Peshekhonov, 2013
| Updated by nick from cnc-club.ru 2014
| ===============================================
*/

#define SERIAL 1
#include <LiquidCrystal.h>
#include "MenuIC.h"
#include "Flash.h"
#include <math.h>


FLASH_STRING(menuitems, 
//     id  sub_id  name[20]
//   |1234|    |12345678901234567890|
//        |1234|
"   0   -2 ImmunoComb Tester\n"
"   1    0 Infections\n"
"  10    1 HIV\n"
"  20    1 Hepatitis\n"
"  30    1 Chlamidia\n"
"  40    1 TORCH\n"
"  50    1 Helicobacter\n"
"  60    1 Others\n"
" -10    1 ..BACK\n"
" 201   20 HAV Ab\n"
" 202   20 HAV IgM\n"
" 203   20 HBs Ag'90\n"
" 204   20 HBc IgG\n"
" 205   20 HBc IgM\n"
" 206   20 HCV\n"
" -10   20 ..BACK\n"
"   2    0 Settings\n"
"1001    2 Home X\n"
"1002    2 Home Z\n"
"1003    2 X+5\n"
"1004    2 X-5\n"
"1005    2 Z+5\n"
"1006    2 Z-5\n"
"1007    2 X+1\n"
"1008    2 X-1\n"
"1009    2 Z+1\n"
"1010    4 Z-1\n"
"1011    2 Wash\n"
"-10     2 ..Back\n");


int id = {0,-2 ImmunoComb Tester\n"
"   1    0 Infections\n"
"  10    1 HIV\n"
"  20    1 Hepatitis\n"
"  30    1 Chlamidia\n"
"  40    1 TORCH\n"
"  50    1 Helicobacter\n"
"  60    1 Others\n"
" -10    1 ..BACK\n"
" 201   20 HAV Ab\n"
" 202   20 HAV IgM\n"
" 203   20 HBs Ag'90\n"
" 204   20 HBc IgG\n"
" 205   20 HBc IgM\n"
" 206   20 HCV\n"
" -10   20 ..BACK\n"
"   2    0 Settings\n"
"1001    2 Home X\n"
"1002    2 Home Z\n"
"1003    2 X+5\n"
"1004    2 X-5\n"
"1005    2 Z+5\n"
"1006    2 Z-5\n"
"1007    2 X+1\n"
"1008    2 X-1\n"
"1009    2 Z+1\n"
"1010    4 Z-1\n"
"1011    2 Wash\n"
"-10     2 ..Back\n");




#define menulen 50

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

int row=0;
int current_sub=0;
int num_rows=4;
char buff[30];



Menu::Menu(LiquidCrystal* _lcd, void (*_callback)(int), int _num_rows, String _cursor) {
  fullsize = 0;
  for (int i=0; menuitems[i]; ++i) // inspect each character in the string
    if (menuitems[i] == '\n')
      fullsize++;
  int id[menulen];
  int sub_id[menulen];
  for 
  return;



  int items ;
  num_rows = _num_rows;
  lcd = _lcd;

  callback = _callback;
  cursor = _cursor;
}



void Menu::drawSub()
{
  // get title 
  (*lcd).clear();
  String title = items[0].name;
  for (int i = 0; i < fullsize; i++)
  {
    if (items[i].id == current_sub)
    {
      title = items[i].name;
      break;
    }
  }  

  (*lcd).setCursor(0, 0);
  (*lcd).print(title+":");
#ifdef SERIAL 
  Serial.println("");
  Serial.println(title+":");
#endif 
  int count = 0;
  int line = 1;
  
  
  
  
  for (int i = 1; i < fullsize; i++)
  {
    if (items[i].sub_id == current_sub)
    {

      if ( count/(num_rows-1) == row/(num_rows-1) )
      {
         // echo menu
        (*lcd).setCursor(0, line);
        if (count == row) 
          { 
            (*lcd).print(cursor);
            #ifdef SERIAL   
              Serial.print(cursor);
            #endif 
          }
        else 
          {
            (*lcd).print(" ");
            #ifdef SERIAL   
              Serial.print(" ");
            #endif 
          }
        (*lcd).print(items[i].name);
         #ifdef SERIAL   
            Serial.print(i);
            Serial.print("-");            
            Serial.print(items[i].sub_id);
            Serial.print(" ");            
            Serial.print(items[i].name);
            Serial.println("");
          #endif 
        line++;        
        if (line > num_rows) 
        {
          break;
        }
      }
      count++;    
    }
  }
  for (int i = 1; i < fullsize; i++)
        Serial.println(items[i].name);  
}

int Menu::get_id()
{ int count=0;
  for (int i = 0; i < fullsize; i++)
  if (items[i].sub_id == current_sub)
  {
    if (count==row)
     {return items[i].id;}
    count++;
  }
  return 0;
}

int Menu::get_row(int id)
{
  int sub_id = get_sub(id);
  int count=0;
  for (int i = 0; i < fullsize; i++)
  {
    if (items[i].id = id) return count;
    if (items[i].sub_id == sub_id) count++;
  }
  return 0;
}

int Menu::get_sub(int id)
{
  for (int i = 0; i < fullsize; i++)
  {
    if (items[i].id == id)
      return items[i].sub_id;
  }
  return 0;
}

int Menu::get_count(int sub_id) {
  int count = 0;
  for (int i = 0; i < fullsize; i++)
  {
    if (items[i].sub_id == sub_id)
      count++;
  }
  return count;
}

void Menu::goNext() {
  row++; 
  if ( row>=get_count(current_sub) ) row=0;
  drawSub();
}

void Menu::goPrev() {
  row--; 
  if (row<0) row = get_count(current_sub)-1;
  drawSub();
}

void Menu::goMain() {
  current_sub = 0;
  row=0;
  drawSub();
}

void Menu::goUp() {
  for (int i = 0; i < fullsize; i++) {
    if (items[i].id==current_sub)
    {
      current_sub=items[i].sub_id;
      row = get_row(i);
      drawSub();
      return;
    }
  }
  current_sub = 0;
  row=0;
  drawSub();
  return;
}

void Menu::goDown() {
  int id = get_id();
  if (id == -10) 
  {
     goUp();
     return;
     
  }  //Back!;
  for (int i = 0; i < fullsize; i++) {
    if (items[i].sub_id==id){
      current_sub = id;
      row=0;
      drawSub();
      return;
    }
  }
  (*callback)(get_id());
}

