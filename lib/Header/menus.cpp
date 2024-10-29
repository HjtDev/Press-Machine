#include "menus.h"

BaseMenu* menu = nullptr; // Define the global pointer here
std::vector<uint8_t> g_cursor_range; // Define the global vector here
std::vector<char> g_valid_keys; // Define the global vector here

void BaseMenu::check(char key) {
    if (this->validateKey(key)) {
        this->applyAction(key);
        this->limitCursorPosition();
        this->display.clear();
        this->drawMenu();
    }
}

bool BaseMenu::validateKey(char key) {
    return std::find(this->validKeys.begin(), this->validKeys.end(), key) != this->validKeys.end();
}

void BaseMenu::limitCursorPosition() {
    if (this->cursor < this->cursorRange[0]) {
        this->cursor = this->cursorRange[1];
    } else if (this->cursor > this->cursorRange[1]) {
        this->cursor = this->cursorRange[0];
    }
}

HomeMenu::HomeMenu(LiquidCrystal_I2C lcd, std::vector<uint8_t> cursor_range, std::vector<char> validKeysList)
    : BaseMenu(lcd, cursor_range, validKeysList) {}

void HomeMenu::applyAction(char key) {
    if (key == 'A') {
        digitalWrite(PB12, HIGH);
    }
    if (key == 'B') {
        digitalWrite(PB12, LOW);
    }
}

void HomeMenu::drawMenu() {
	display.setCursor(0, 0);
	display.write(specialCharacters::SELECTOR);
    display.setCursor(calculateSpace("HOME"), 0);
	display.print("HOME");
	display.setCursor(0, 1);
	display.write(specialCharacters::FILLED_SELECTOR);
}

int calculateSpace(String str) {
    return ((16 - str.length()) / 2);
}
