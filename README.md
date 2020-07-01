# LcdMenu
Simple Arduino LCD menu

Supports multi level submenus

Supports multiple non/excluyent actions on menu item click:
- Execute function or do nothing, then:
- Go to submenu
- Stay on menu item
- Goto hardcoded menu item (upper, same, lower level)
- Goto programatically menu item.
- Goto menu home

Just define menu items in a simple array specifiying for each entry:
- id number for menu item. Must use 0 as id for "menu home".
- Menu item text
- "Can move" flag, "DN": down on same menu level (first item on menu level), "UPDN": can move up/down on same menu level, or "UP": just up (last item on menu level).
- Function to call on menu item "click" or selection. Function can return next item id to set menu "cursor" or just use hardcoded "next item id" for such menu item.
- "Next item id" to set menu "cursor" after function call is executed.


See example, usage is very simple.

# Installation
Download and copy "LcdMenu.h" within Arduino IDE library folders.
