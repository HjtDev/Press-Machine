#include "menus.h"

BaseMenu* menu = nullptr; // Define the global pointer here
String newMenu = "none"; // Define the global pointer here
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

void BaseMenu::drawSelector(uint8_t pos, String txt) {
	if(this->cursor == pos) {
		this->display.write(specialCharacters::FILLED_SELECTOR);
	} else {
		this->display.write(specialCharacters::SELECTOR);
	}
	this->display.print(txt);
}

void BaseMenu::drawBack(uint8_t pos) {
	this->display.setCursor(0, 1);
	if(this->cursor == pos) {
		this->display.write(specialCharacters::FILLED_SELECTOR);
	} else {
		this->display.write(specialCharacters::SELECTOR);
	}
	this->display.print("Back");
}

void BaseMenu::drawHome(uint8_t pos) {
	this->display.setCursor(11, 1);
	if(this->cursor == pos) {
		this->display.write(specialCharacters::FILLED_SELECTOR);
	} else {
		this->display.write(specialCharacters::SELECTOR);
	}
	this->display.print("Home");
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

void BaseMenu::moveCursor(char key) {
	if (key == 'A') {
        this->cursor++;
    }
    if (key == 'B') {
        this->cursor--;
    }
}

// Start Home Menu

HomeMenu::HomeMenu(LiquidCrystal_I2C lcd, std::vector<uint8_t> cursor_range, std::vector<char> validKeysList)
    : BaseMenu(lcd, cursor_range, validKeysList) {}

void HomeMenu::applyAction(char key) {
    if(key == 'A' || key == 'B') {
		moveCursor(key);
	}
	if(key == 'C') {
		if(cursor == 2) {
			newMenu = "about";
		}
	}
}

void HomeMenu::drawMenu() {
    display.setCursor(calculateSpace("HOME"), 0);
	display.print("HOME");
	display.setCursor(0, 1);
	drawSelector(1, "List");
	display.setCursor(10, 1);
	drawSelector(2, "About");
}

// End Home Menu

// Start About Menu

AboutMenu::AboutMenu(LiquidCrystal_I2C lcd, std::vector<uint8_t> cursor_range, std::vector<char> validKeysList)
    : BaseMenu(lcd, cursor_range, validKeysList) {}

void AboutMenu::applyAction(char key) {
	if(key == 'A' || key == 'B') {
		moveCursor(key);
	}
	if(key == 'C') {
		newMenu = "home";
	}
}

void AboutMenu::drawMenu() {
	display.setCursor(calculateSpace("Mr.Fotohi"), 0);
	display.print("Mr.Fotohi");
	drawBack(1);
	drawHome(2);
}

// End About Menu

int calculateSpace(String str) {
    return ((16 - str.length()) / 2);
}