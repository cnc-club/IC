/* ===============================================
|	MyMenu library for LCD with 2+ lines of text
|
|   Usage example is in *.cpp
|
|	Made by Denis Peshekhonov, 2013
| ===============================================
*/

#ifndef MYMENU_H
#define MYMENU_H

#include <LiquidCrystal.h>

class MItm
{
  public:
    MItm(String _name, int _id, int _sub_id=-1);
    String name;
    int id;
    int sub_id;
};

class Menu
{
  public:
    Menu(MItm *_items, int _num_items, LiquidCrystal* _lcd, void (*_callback)(int), int _num_rows = 4, String _cursor = ">");
    void drawSub(int id, int row);
    int get_id();
    int get_row(int id);
    int get_sub(int id);
    int get_count(int sub_id);
    void goNext();
    void goPrev();
    void goMain();
    void goUp();
    void goDown();
    void drawSub();

  private:
    void (*callback)(int);
    MItm *items;
    LiquidCrystal *lcd;
    int num_rows;
    int current_sub;
    int fullsize;
    String cursor;
};

#endif
