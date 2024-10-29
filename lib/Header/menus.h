#ifndef MENUS_H
#define MENUS_H

#include <cstdint>
#include <LiquidCrystal_I2C.h>
#include <vector>
#include <algorithm>

class BaseMenu {
public:
    LiquidCrystal_I2C display; // Change to pointer
    uint8_t cursor;
    std::vector<uint8_t> cursorRange;
    std::vector<char> validKeys;

    BaseMenu(LiquidCrystal_I2C lcd, std::vector<uint8_t> cursor_range, std::vector<char> validKeysList)
        : display(lcd), cursor(1), cursorRange(cursor_range), validKeys(validKeysList) {}

    virtual void check(char key);
    bool validateKey(char key);
    void limitCursorPosition();
    virtual void applyAction(char key) = 0; // Pure virtual function
    virtual void drawMenu() = 0; // Pure virtual function
};

class HomeMenu : public BaseMenu {
public:
    HomeMenu(LiquidCrystal_I2C lcd, std::vector<uint8_t> cursor_range, std::vector<char> validKeysList);
    
    void applyAction(char key) override;
    void drawMenu() override;
};

extern BaseMenu* menu; // Declare as extern
extern std::vector<uint8_t> g_cursor_range; // Declare as extern
extern std::vector<char> g_valid_keys; // Declare as extern

int calculateSpace(String str); // Declaration only

enum specialCharacters {
    LOADING_START_FILLED,
    LOADING_START,
    LOADING_END_FILLED,
    LOADING_END,
    LOADING_MIDDLE_FILLED,
    LOADING_MIDDLE,
    SELECTOR,
    FILLED_SELECTOR
};

#endif
