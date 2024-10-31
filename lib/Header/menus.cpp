#include "menus.h"

BaseMenu* menu = nullptr;
String newMenu = "none";
std::vector<uint8_t> g_cursor_range;
std::vector<char> g_valid_keys;
String status = "0";
bool powerSaver = false;
bool powerSaverStatus = true;
ulong lastActionTime = 0;
uint8_t returnCursor = 0;


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
        this->cursor--;
    }
    if (key == 'B') {
        this->cursor++;
    }
}

void BaseMenu::loading() {
    display.setCursor(2, 1); 
    display.write(specialCharacters::LOADING_START);
    for(uint8_t col = 3; col < 13; col++) {
        display.setCursor(col, 1);
        display.write(specialCharacters::LOADING_MIDDLE);
    }
    display.write(specialCharacters::LOADING_END);
    makeDelay();
    display.setCursor(2, 1);
    display.write(specialCharacters::LOADING_START_FILLED);
    for(uint8_t col = 3; col < 13; col++) {
        makeDelay();
        display.setCursor(col, 1);
        display.write(specialCharacters::LOADING_MIDDLE_FILLED);
    }
    makeDelay();
    display.write(specialCharacters::LOADING_END_FILLED);
    makeDelay();
};
//
void BaseMenu::makeDelay() {
    vTaskDelay(100 / portTICK_PERIOD_MS);
    ulong* rand_input = new ulong(random(0, 750));
    vTaskDelay(*rand_input / portTICK_PERIOD_MS);
    delete rand_input;
};

void BaseMenu::stage(String str) {
    display.clear();
    uint8_t space = calculateSpace(str);
    display.setCursor(space, 0);
    display.print(str);
    loading();
};

void writeByte(uint32_t address, uint8_t value) {
    uint8_t r = readByte(address);
    if(r == value) {
        return;
    }
    eraseSector(address);
    digitalWrite(PA4, LOW);  // Select the chip

    SPI.transfer(0x06);  // Write Enable command
    digitalWrite(PA4, HIGH);  // Deselect the chip
    delayMicroseconds(10);

    digitalWrite(PA4, LOW);  // Select the chip
    SPI.transfer(0x02);  // Page Program command
    SPI.transfer((address >> 16) & 0xFF);  // Send the address bytes
    SPI.transfer((address >> 8) & 0xFF);
    SPI.transfer(address & 0xFF);

    SPI.transfer(value);

    digitalWrite(PA4, HIGH);  // Deselect the chip
    delay(5);  // Page program time
}

uint8_t readByte(uint32_t address) {
    digitalWrite(PA4, LOW);  // Select the chip

    SPI.transfer(0x03);  // Read command
    SPI.transfer((address >> 16) & 0xFF);  // Send the address bytes
    SPI.transfer((address >> 8) & 0xFF);
    SPI.transfer(address & 0xFF);

    uint8_t readValue = SPI.transfer(0x00);  // Send dummy byte to read data

    digitalWrite(PA4, HIGH);  // Deselect the chip
    return readValue;
}

void eraseSector(uint32_t sectorAddress) {
    digitalWrite(PA4, LOW);  // Select the chip

    SPI.transfer(0x06);  // Write Enable command
    digitalWrite(PA4, HIGH);  // Deselect the chip
    delayMicroseconds(10);

    digitalWrite(PA4, LOW);  // Select the chip
    SPI.transfer(0x20);  // Sector Erase command
    SPI.transfer((sectorAddress >> 16) & 0xFF);  // Send the sector address bytes
    SPI.transfer((sectorAddress >> 8) & 0xFF);
    SPI.transfer(sectorAddress & 0xFF);

    digitalWrite(PA4, HIGH);  // Deselect the chip
    delay(100);  // Sector erase time
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
	if(key == 'C' || key == 'D') {
		newMenu = "home";
		returnCursor = 2;
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
	} else if(key == 'C') {
		if(this->cursor == 1) {
			newMenu = "inputs";
		} else if(this->cursor == 2) {
			newMenu = "outputs";
		} else if(this->cursor == 3) {
			newMenu = "settings";
		} else if(this->cursor == 4) {
			powerSaver = !powerSaver;
			lastActionTime = millis(); // to prevent unwanted turn off
			saveToEEPROM();
		} else if(this->cursor == 5 || this->cursor == 6) {
			newMenu = "home";
		}
	} else if(key == 'D') {
		newMenu = "home";
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
		drawSelector(4, "Power Saver " + String(powerSaver ? "ON " : "OFF"));
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

// Start Outputs Menu

OutputsMenu::OutputsMenu(LiquidCrystal_I2C lcd, std::vector<uint8_t> cursor_range, std::vector<char> validKeysList)
    : BaseMenu(lcd, cursor_range, validKeysList) {}

void OutputsMenu::check(char key) {
	if (key != '\0') {
        this->applyAction(key);
    }
	this->drawMenu();
}

void OutputsMenu::applyAction(char key) {
	newMenu = "list";
	returnCursor = 2;
}

void OutputsMenu::drawMenu() {
	display.setCursor(0, 0);
	display.print("UNIT:");
	display.print(digitalRead(PB12) ? "ON " : "OFF");
	
	display.setCursor(10, 0);
	display.print("UP:");
	display.print(digitalRead(PB13) ? "ON " : "OFF");

	display.setCursor(0, 1);
	display.print("AIR:");
	display.print(digitalRead(PB15) ? "ON " : "OFF");

	display.setCursor(8, 1);
	display.print("DOWN:");
	display.print(digitalRead(PB14) ? "ON " : "OFF");
}

// End Outputs Menu

// Start Settings Menu

SettingsMenu::SettingsMenu(LiquidCrystal_I2C lcd, std::vector<uint8_t> cursor_range, std::vector<char> validKeysList)
    : BaseMenu(lcd, cursor_range, validKeysList) {}

void SettingsMenu::applyAction(char key) {
	if(key == 'A' || key == 'B') {
		moveCursor(key);
	} else if(key == 'C') {
		switch (this->cursor)
		{
		case 1:
			newMenu = "highlevel_timer";
			break;
		case 2:
			newMenu = "aircleaner_timer";
			break;
		case 3:
			stage("Resetting Data");
			pins::controls::AIR_CLEANER_TIMER = 1.0;
			pins::controls::MICROSWITCH_TIMER = 3.0;
			pins::controls::change = true;
			newMenu = "settings";
			returnCursor = 3;
			break;
		case 4:
			newMenu = "list";
			returnCursor = 3;
			break;
		case 5:
			newMenu = "home";
			break;
		default:
			break;
		}
	} else if(key == 'D') {
		newMenu = "list";
		returnCursor = 3;
	}
}

void SettingsMenu::drawMenu() {
	if(this->cursor == 1 || this->cursor == 2) {
		display.setCursor(0, 0);
		drawSelector(1, "Highlevel Timer");
		display.setCursor(0, 1);
		drawSelector(2, "Air Timer");
	} else if(this->cursor == 3 || this->cursor == 4) {
		display.setCursor(0, 0);
		drawSelector(3, "Reset Settings");
		display.setCursor(0, 1);
		drawSelector(4, "Back");
	} else if(this->cursor == 5) {
		display.setCursor(0, 0);
		drawSelector(5, "Home");
	}
}

// End Settings Menu

// Start HIGHLEVEL TIMER Menu

HTimerMenu::HTimerMenu(LiquidCrystal_I2C lcd, std::vector<uint8_t> cursor_range, std::vector<char> validKeysList)
    : BaseMenu(lcd, cursor_range, validKeysList), txt_cursor(uint8_t(1)) {}

void HTimerMenu::applyAction(char key) {
	if(key == 'A' || key == 'B') {
		moveCursor(key);
	} else if(key == 'C') {
		if(this->cursor == 2) {
			newMenu = "settings";
		} else if(this->cursor == 3) {
			newMenu = "home";
		}
	} else if(key == 'D') {
		newMenu = "settings";
	} else if(key >= '0' && key <= '9' && this->cursor == 1) {
		if(this->txt_cursor == 1) {
			pins::controls::MICROSWITCH_TIMER = float(int(key - '0') + (pins::controls::MICROSWITCH_TIMER - int(pins::controls::MICROSWITCH_TIMER)));
			this->txt_cursor = 2;
		} else if(this->txt_cursor == 2) {
			pins::controls::MICROSWITCH_TIMER = float(int(pins::controls::MICROSWITCH_TIMER)) + float(int(key - '0')) / 10.0f;
			this->txt_cursor = 1;
    	}
		pins::controls::change = true;
	} else if(key == '*' && this->cursor == 1) {
		pins::controls::MICROSWITCH_TIMER += .1;
		controlVariable(pins::controls::MICROSWITCH_TIMER, .1, 9.9);
		pins::controls::change = true;
	} else if(key == '#' && this->cursor == 1) {
		pins::controls::MICROSWITCH_TIMER -= .1;
		controlVariable(pins::controls::MICROSWITCH_TIMER, .1, 9.9);
		pins::controls::change = true;
	}
}

void HTimerMenu::drawMenu() {
	display.setCursor(0, 0);
	drawSelector(1, "Value:");
	display.setCursor(12, 0);
	display.print(pins::controls::MICROSWITCH_TIMER, 1);
	display.print('s');
	drawBack(2);
	drawHome(3);
}

// End HIGHLEVEL TIMER Menu

// Start AIR TIMER Menu

ATimerMenu::ATimerMenu(LiquidCrystal_I2C lcd, std::vector<uint8_t> cursor_range, std::vector<char> validKeysList)
    : BaseMenu(lcd, cursor_range, validKeysList), txt_cursor(uint8_t(1)) {}

void ATimerMenu::applyAction(char key) {
	if(key == 'A' || key == 'B') {
		moveCursor(key);
	} else if(key == 'C') {
		if(this->cursor == 2) {
			newMenu = "settings";
			returnCursor = 2;
		} else if(this->cursor == 3) {
			newMenu = "home";
		}
	} else if(key == 'D') {
		newMenu = "settings";
		returnCursor = 2;
	} else if(key >= '0' && key <= '9' && this->cursor == 1) {
		if(this->txt_cursor == 1) {
			pins::controls::AIR_CLEANER_TIMER = float(int(key - '0') + (pins::controls::AIR_CLEANER_TIMER - int(pins::controls::AIR_CLEANER_TIMER)));
			this->txt_cursor = 2;
		} else if(this->txt_cursor == 2) {
			pins::controls::AIR_CLEANER_TIMER = float(int(pins::controls::AIR_CLEANER_TIMER)) + float(int(key - '0')) / 10.0f;
			this->txt_cursor = 1;
    	}
		pins::controls::change = true;
	} else if(key == '*' && this->cursor == 1) {
		pins::controls::AIR_CLEANER_TIMER += .1;
		controlVariable(pins::controls::AIR_CLEANER_TIMER, .1, 9.9);
		pins::controls::change = true;
	} else if(key == '#' && this->cursor == 1) {
		pins::controls::AIR_CLEANER_TIMER -= .1;
		controlVariable(pins::controls::AIR_CLEANER_TIMER, .1, 9.9);
		pins::controls::change = true;
	}
}

void ATimerMenu::drawMenu() {
	display.setCursor(0, 0);
	drawSelector(1, "Value:");
	display.setCursor(12, 0);
	display.print(pins::controls::AIR_CLEANER_TIMER, 1);
	display.print('s');
	drawBack(2);
	drawHome(3);
}

// End AIR TIMER Menu

int calculateSpace(String str) {
    return ((16 - str.length()) / 2);
}