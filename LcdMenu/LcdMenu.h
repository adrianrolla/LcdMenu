#ifndef LcdMenu_H
#define LcdMenu_H

typedef struct LcdMenuItem {
  int id;
  const char *text;
  const char *(*textFunction)(LcdMenuItem * menuItem);
  int updn;
  int (*doOnClick)(LcdMenuItem * menuItem);
  int cursorIdAfterClick;
} LcdMenuItem;

class LcdMenu {
    int cursor = 0;
    int line0 = -1;
    bool canMoveUp( int updn ) { return (updn & UP) != 0; }
    bool canMoveDown( int updn ) { return (updn & DN) != 0; }
    void lcdPrintText( LcdMenuItem *item ){
      if ( item->text != NULL ){
         lcd.print( item->text );
      } else if ( item->textFunction != NULL ){
         lcd.print( item->textFunction( item ) );
      } else {
         lcd.print( "menu-id-" );
         lcd.print( item->id );
      }
    }
    int getIndex(int id) {
      int index = 0;
      while ( items[index].id != id && items[index].id >= 0 ) {
        index++;
      }
      if (items[index].id == id) {
        return index;
      } else {
        return -1;
      }
    }
  public:
    static constexpr char * LCD_EMPTYROW = "                ";
    static const int NONE = 0;
    static const int UP = 1;
    static const int DN = 2;
    static const int UPDN = 3;

    char cursorChar = 0b01111110;
    char backCursorChar = 0b01111111;
    LcdMenuItem *items;
    LcdMenu(LcdMenuItem *items) {
      this->items = items;
    }
    void home( int homeRow = 1 ) {
      cursor = line0 = items[0].id;        // first row is home
      displayMenu();
    }
    void displayMenu() {
      int line0Index = getIndex(line0);
      lcd.setCursor(0, 0);
      lcd.print( cursor == line0 ? ( (canMoveUp( items[line0Index].updn ) || line0Index == 0) ? cursorChar : backCursorChar ) : ' ');
      lcdPrintText( &items[line0Index] );
      lcd.print( LCD_EMPTYROW );
      lcd.setCursor(0, 1);
      lcd.print( cursor == items[line0Index+1].id ? cursorChar : ' ');
      lcdPrintText( &items[line0Index+1] );
      lcd.print( LCD_EMPTYROW );
    }
    void up() {
      Serial.print("up cursor:");
      Serial.print(cursor);
      int indexCursor = getIndex(cursor);
      Serial.print(", ix:");
      Serial.print(indexCursor);
      if ( canMoveUp(items[indexCursor].updn) ) {
        Serial.print(", can UP");
        cursor = items[indexCursor-1].id; // move to previous item
        Serial.print(", new cursor:");
        Serial.print(cursor);
        Serial.print(", line0:");
        Serial.print(indexCursor);
        if (line0 != cursor) {
          line0 = cursor;
        }
        Serial.print(", new line0:");
        Serial.print(indexCursor);
      }
      Serial.println();
      Serial.flush();
      displayMenu();
    }
    void down() {
      int indexCursor = getIndex(cursor);
      if ( canMoveDown(items[indexCursor].updn) ) {
        if (line0 != cursor) {
          line0 = cursor;
        }
        cursor = items[indexCursor+1].id;
      }
      displayMenu();
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
            line0 = items[ indexCursor -1 ].id; // cursor is last item on submenu, so line0 has to be previous item
          }
        }
      }
      displayMenu();
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
