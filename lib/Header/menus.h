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
        : display(lcd), cursor(uint8_t(1)), cursorRange(cursor_range), validKeys(validKeysList) {}

	void drawBack(uint8_t pos);
	void drawHome(uint8_t pos);
    void drawSelector(uint8_t pos, String txt);
	void moveCursor(char key);
	void limitCursorPosition();
    bool validateKey(char key);
    virtual void check(char key);
    virtual void applyAction(char key) = 0; // Pure virtual function
    virtual void drawMenu() = 0; // Pure virtual function
};

class HomeMenu : public BaseMenu {
public:
    HomeMenu(LiquidCrystal_I2C lcd, std::vector<uint8_t> cursor_range, std::vector<char> validKeysList);
    
    void applyAction(char key) override;
    void drawMenu() override;
};

class AboutMenu : public BaseMenu {
public:
    AboutMenu(LiquidCrystal_I2C lcd, std::vector<uint8_t> cursor_range, std::vector<char> validKeysList);
    
    void applyAction(char key) override;
    void drawMenu() override;
};

class ListMenu : public BaseMenu {
public:
    ListMenu(LiquidCrystal_I2C lcd, std::vector<uint8_t> cursor_range, std::vector<char> validKeysList);
    
    void applyAction(char key) override;
    void drawMenu() override;
};

class InputsMenu : public BaseMenu {
public:
    InputsMenu(LiquidCrystal_I2C lcd, std::vector<uint8_t> cursor_range, std::vector<char> validKeysList);
    
    void check(char key) override;
    void applyAction(char key) override;
    void drawMenu() override;
};

extern BaseMenu* menu; // Declare as extern
extern String newMenu; // Declare as extern
extern std::vector<uint8_t> g_cursor_range; // Declare as extern
extern std::vector<char> g_valid_keys; // Declare as extern
extern String status;

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