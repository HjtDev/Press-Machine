#include "menus.h"

BaseMenu* menu = nullptr; // Define the global pointer here
String newMenu = "none"; // Define the global pointer here
std::vector<uint8_t> g_cursor_range; // Define the global vector here
std::vector<char> g_valid_keys; // Define the global vector here
String status = "0";

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
		if(cursor == 1) {
			newMenu = "list";
		} else if(cursor == 2) {
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

// Start List Menu

ListMenu::ListMenu(LiquidCrystal_I2C lcd, std::vector<uint8_t> cursor_range, std::vector<char> validKeysList)
    : BaseMenu(lcd, cursor_range, validKeysList) {}

void ListMenu::applyAction(char key) {
	if(key == 'A' || key == 'B') {
		moveCursor(key);
	}
	if(key == 'C') {
		if(this->cursor == 1) {
			newMenu = "inputs";
		} else if(this->cursor == 5 || this->cursor == 6) {
			newMenu = "home";
		}
	}
}

void ListMenu::drawMenu() {
	if(this->cursor == 1 || this->cursor == 2) {
		display.setCursor(0, 0);
		drawSelector(1, "Inputs");
		display.setCursor(0, 1);
		drawSelector(2, "Outputs");
	} else if(this->cursor == 3 || this->cursor == 4) {
		display.setCursor(0, 0);
		drawSelector(3, "Settings");
		display.setCursor(0, 1);
		drawSelector(4, "Power Saver");
	} else if(this->cursor == 5 || this->cursor == 6) {
		display.setCursor(0, 0);
		drawSelector(5, "Back");
		display.setCursor(0, 1);
		drawSelector(6, "Home");
	}
}

// End List Menu

// Start Inputs Menu

InputsMenu::InputsMenu(LiquidCrystal_I2C lcd, std::vector<uint8_t> cursor_range, std::vector<char> validKeysList)
    : BaseMenu(lcd, cursor_range, validKeysList) {}

void InputsMenu::check(char key) {
	if (key != '\0') {
        this->applyAction(key);
    }
	this->drawMenu();
}

void InputsMenu::applyAction(char key) {
	newMenu = "list";
}

void InputsMenu::drawMenu() {
	String akbar = ""; 
	akbar += String(!digitalRead(PA8)); 
	akbar += String(!digitalRead(PA9)); 
	akbar += String(!digitalRead(PA10)); 
	akbar += String(!digitalRead(PA11)); 
	akbar += String(!digitalRead(PA12)); 
	akbar += String(!digitalRead(PA15)); 
	akbar += String(!digitalRead(PB3)); 
	akbar += String(!digitalRead(PB4)); 
	akbar += String(!digitalRead(PB5)); 
	akbar += String(!digitalRead(PB8)); 
	akbar += String(!digitalRead(PB9)); 
	akbar += String(!digitalRead(PC13)); 
	akbar += String(!digitalRead(PA0)); 
	akbar += String(!digitalRead(PA1)); 
	akbar += String(!digitalRead(PA2)); 
	if(akbar != status) {
		this->display.setCursor(0, 0);
		this->display.print("XTCELHKDUOSZPAM");
		display.setCursor(0, 1);
		display.print(akbar);
		status = akbar;
	}
}

// End Inputs Menu

int calculateSpace(String str) {
    return ((16 - str.length()) / 2);
}