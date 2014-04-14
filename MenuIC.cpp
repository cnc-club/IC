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


#include <LiquidCrystal.h>
#include "MenuIC.h"
#include <math.h>

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

int row=0;
int current_sub=0;
int num_rows=4;
MItm::MItm(String _name, int _id, int _sub_id) {
  this->id = _id;
  this->sub_id = _sub_id;
  this->name = _name;
}



Menu::Menu(MItm *_items, int _num_items, LiquidCrystal* _lcd, void (*_callback)(int), int _num_rows, String _cursor) {
  items = _items;
  num_rows = _num_rows;
  lcd = _lcd;
  fullsize = _num_items;
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
          {(*lcd).print(cursor);}
        else 
          {(*lcd).print(" ");}
        (*lcd).print(items[i].name);
        (*lcd).print(items[i].sub_id);        
        (*lcd).print(".");                
        (*lcd).print(items[i].id);                
        line++;        
        if (line > num_rows) 
        {
          break;
        }
      }
      count++;    
    }
  }
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
      if (current_sub<0) {current_sub=0;}
      
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

