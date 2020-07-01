#ifndef LcdMenu_H
#define LcdMenu_H

typedef struct LcdMenuItem {
  int id;
  char *text;
  int updn;
  int (*doOnClick)(LcdMenuItem * menuItem);
  int cursorIdAfterClick;
} LcdMenuItem;

class LcdMenu {
  public:
    static constexpr char * LCD_EMPTYROW = "                ";
    static const int NONE = 0;
    static const int UP = 1;
    static const int DN = 2;
    static const int UPDN = 3;
    byte homeRow = 1;
    char cursorChar = 0b01111110;
    char backCursorChar = 0b01111111;
    int cursor = 0;
    int line0 = -1;
    LcdMenuItem *items;
    LcdMenu(LcdMenuItem *items) {
      this->items = items;
    }
    void home( int homeRow = 1 ) {
      this->homeRow = homeRow % 2;
      cursor = line0 = 0;
      lcd.setCursor(0, 1 - this->homeRow); // clear line not used for home
      lcd.print( LCD_EMPTYROW );
      displayCurrent();
    }
    void displayCurrent() {
      if (cursor == 0) {
        displayStartMenu();
      } else {
        displayLines();
      }
    }
    void displayStartMenu() {
      lcd.setCursor(0, this->homeRow);
      lcd.print( cursorChar );
      lcd.print( items[getIndex(cursor)].text );
      lcd.print( LCD_EMPTYROW );
    }
    void displayLines() {
      int line0Index = getIndex(line0);
      int line1Index = getIndex(line0+1);
      lcd.setCursor(0, 0);
      lcd.print( cursor == line0 ? ( canMoveUp( items[line0Index].updn ) ? cursorChar : backCursorChar ) : ' ');
      lcd.print( items[line0Index].text );
      lcd.print( LCD_EMPTYROW );
      lcd.setCursor(0, 1);
      lcd.print( cursor == line0 + 1 ? cursorChar : ' ');
      lcd.print( items[line1Index].text );
      lcd.print( LCD_EMPTYROW );
    }
    bool canMoveUp( int updn ) { return (updn & UP) != 0; }
    bool canMoveDown( int updn ) { return (updn & DN) != 0; }
    void up() {
      int indexCursor = getIndex(cursor);
      if ( canMoveUp(items[indexCursor].updn) ) {
        cursor--;
        if (line0 > cursor) {
          line0 = cursor;
        }
      }
      displayCurrent();
    }
    void down() {
      int indexCursor = getIndex(cursor);
      if ( canMoveDown(items[indexCursor].updn) ) {
        cursor++;
        if (line0 < cursor - 1) {
          line0 = cursor - 1;
        }
      }
      displayCurrent();
    }
    void click() {
      int indexCursor = getIndex(cursor);
      int cursorIdAfterClick = -1;
      if ( items[indexCursor].doOnClick != NULL ) {
        // execute click action and keep cursorIdAfterClick menu item
        cursorIdAfterClick = items[indexCursor].doOnClick( &(items[indexCursor]) );
      }
      if (cursorIdAfterClick == -1) {
        // use ".cursorIdAfterClick" from menu definition
        cursorIdAfterClick = items[indexCursor].cursorIdAfterClick;
      }
      if ( cursorIdAfterClick != -1) {
        cursor = cursorIdAfterClick;
        if ( cursor == 0 ) {
          home();
        } else {
          int indexCursor = getIndex(cursor);
          if ( canMoveDown(items[indexCursor].updn) ) {
            line0 = cursor; // from cursor can go down, more items down...
          } else {
            line0 = cursor -1; // cursor is last item on submenu, so line0 has to be previous item
          }
        }
      }
      displayCurrent();
    }
    int getIndex(int id) {
      int index = 0;
      while ( items[index].id != id && items[index].text != NULL ) {
        index++;
      }
      if (items[index].id == id) {
        return index;
      } else {
        return -1;
      }
    }
    static int doClick( LcdMenuItem *menuItem ) {
      Serial.print("LcdMenu::doClick ");
      Serial.println(menuItem->text);
      lcd.setCursor(0,0);
      lcd.print( "clicked" );
      lcd.print( LCD_EMPTYROW );
      lcd.setCursor(0,1);
      lcd.print( menuItem->text );
      lcd.print( LCD_EMPTYROW );
      delay(3000);
      return -1;
    }
};


#endif
