#include <JoystickA0.h>
#include <LiquidCrystal_I2C.h>

#define PINCLICK D0

LiquidCrystal_I2C lcd(0x27, 16, 2);

#include <LcdMenu.h> // include AFTER lcd definition

MonitorJoystick monitorJoystick = MonitorJoystick( PINCLICK , 500 );

int displayMillis(LcdMenuItem * menuItem) {
  Serial.println("Menu clickDisplayMillis");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(menuItem->text);
  lcd.setCursor(0,1);
  lcd.print(millis());
  lcd.print("ms");
  delay(3000);
  return -1;
}

int clicks = 0;
char countClicksBuf[30];
const char *clickCountMenuItem(LcdMenuItem * menuItem) {
  strncpy( countClicksBuf, (String("Clicks: ")+clicks).c_str(), sizeof(countClicksBuf)-1 ) ;
  return countClicksBuf;
}

int addClick(LcdMenuItem * menuItem){
  clicks++;
  Serial.println("Menu addClick");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("addClick, now:");
  lcd.setCursor(0,1);
  lcd.print(clicks);
  delay(3000);
  return -1;
}

int displaySecs(LcdMenuItem * menuItem) {
  Serial.println("Menu clickDisplaySecs");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(menuItem->text);
  lcd.setCursor(0,1);
  lcd.print(millis()/1000);
  lcd.print("s");
  delay(3000);
  return -1;
}

int displayRandom(LcdMenuItem * menuItem) {
  Serial.println("Menu displayRandom");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(menuItem->text);
  lcd.setCursor(0,1);
  lcd.print(random(100));
  delay(3000);
  return -1;
}

int backOrStayAtRandom(LcdMenuItem * menuItem) {
  Serial.println("Menu backOrStayAtRandom");
  bool back = random(100) > 50;
  const char *msg = (back ? "Back to '4-More'" : "Stay here");
  Serial.print("Menu backOrStayAtRandom: ");
  Serial.println( msg );
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(menuItem->text);
  lcd.setCursor(0,1);
  lcd.print(msg);
  delay(3000);
  return back ? 4 : -1;
}

// menu items definition
// start with id:0 as menu home.
// 
LcdMenuItem menuItems[] {
  //id        text                updn          doOnClick         cursorIdAfterClick
  { 1,        "Millis",NULL,           LcdMenu::DN,     &displayMillis,  -1},  // on click displayMillis and stay there
  { 2,        "Secs &back home",NULL,  LcdMenu::UPDN,   &displaySecs,     0},  // on click displaySecs and go back to id:0-menu home
  { 3,        NULL,&clickCountMenuItem,       LcdMenu::UPDN,   &addClick,       -1},  // use textFunction, on click do 'addClick' and stay there"
  { 4,        "More",NULL,             LcdMenu::UPDN,   NULL,             40}, // on click show menu items starting on id 40-"(back)"
  { 9,        "Even More",NULL,        LcdMenu::UP,     NULL,             90}, // last menu item of list (UP only), on click show menu items starting on id 90-"(back)"

  // menu item group. id's has to be located consecutive within level/group.
  { 40,       "(back)",NULL,           LcdMenu::DN,     NULL,             4},  // first menu item of list (DN only), on click go back to menu id 4-"More"
  { 41,       "Foo",NULL,              LcdMenu::UPDN,   &LcdMenu::doClick,-1}, // on click do defalut LcdMenu::doClick, and stay there
  { 42,       "Random 0..100",NULL,    LcdMenu::UPDN,   &displayRandom,   -1}, // on click do displayRandom0to100 and stay there
  { 43,       "Random & back",NULL,    LcdMenu::UPDN,   &displayRandom,   4},  // on click doClickRandom and go back to menu id 4-"More"
  { 44,       "Back/Stay random",NULL, LcdMenu::UP,     &backOrStayAtRandom,-1}, // last menu item of list (UP only), on click backOrStayAtRandom (decide random if back to id 4-"More", or stay there)

  // menu item group. id's has to be located consecutive within level/group.
  { 90,       "(back)",NULL,           LcdMenu::DN,     NULL,             9},  // first menu item of list (DN only), on click go back to menu id 5-"Even More"
  { 91,       "Even more 1",NULL,      LcdMenu::UPDN,   NULL,             9},  // on click go back to menu id 5-"Even More"
  { 92,       "Even more 2",NULL,      LcdMenu::UP,     NULL,             9},  // last item of list (UP only), on click go back to menu id 5-"Even More"

  // DO NOT REMOVE NOR CHANGE. End of menu definition. 
  { -1,       NULL,NULL,               LcdMenu::NONE,   NULL,             0 }
};

LcdMenu lcdMenu = LcdMenu(menuItems);

void joystickEvent(int button, int updn) { // joystick event handler
  if (button != JOYSTICK_EMPTY) {
    lcdMenu.click();
  }
  if (updn != JOYSTICK_EMPTY) {
    // my joystick is upside down
    if ( updn == JOYSTICK_DOWN ) { 
      lcdMenu.up();
    } else {
      lcdMenu.down();
    }
  }
}

void setup() {
  Serial.begin(115200);

  lcd.init();
  lcd.backlight();
  lcd.clear();

  monitorJoystick.setEventHandler( &joystickEvent );

  lcdMenu.home();
}

void loop() {
  monitorJoystick.update();
}