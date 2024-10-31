#include <Arduino.h>
#include <Definitions.h>

void setup() {
    SPI.begin();
    pinMode(pins::controls::chipSelectPin, OUTPUT);
    digitalWrite(pins::controls::chipSelectPin, HIGH);

    pinMode(pins::inputs::MAIN_SELECTOR_MANUAL, INPUT);
    pinMode(pins::inputs::MAIN_SELECTOR_AUTOMATIC, INPUT);
    pinMode(pins::inputs::AUTO_SELECTOR_PADDLE, INPUT);
    pinMode(pins::inputs::AUTO_SELECTOR_AUTOMATIC, INPUT);
    pinMode(pins::inputs::SENSOR, INPUT);
    pinMode(pins::inputs::HIGHLEVEL_SELECTOR_TIMER, INPUT);
    pinMode(pins::inputs::MOTOR_START, INPUT);
    pinMode(pins::inputs::MOTOR_STOP, INPUT);
    pinMode(pins::inputs::PUMP_UP, INPUT);
    pinMode(pins::inputs::PUMP_DOWN, INPUT);
    pinMode(pins::inputs::HIGHLEVEL_MICTORSWITCH, INPUT);
    pinMode(pins::inputs::LOWLEVEL_MICTORSWITCH, INPUT);
    pinMode(pins::inputs::PHASE_CONTROL, INPUT);
    pinMode(pins::inputs::PADDLE, INPUT);
    pinMode(pins::inputs::AIR_CLEANER_BUTTON, INPUT);

    pinMode(pins::inputs::KEYPAD_DAV, INPUT);

    pinMode(pins::inputs::KEYPAD_DATA_A, INPUT);
    pinMode(pins::inputs::KEYPAD_DATA_B, INPUT);
    pinMode(pins::inputs::KEYPAD_DATA_C, INPUT);
    pinMode(pins::inputs::KEYPAD_DATA_D, INPUT);

    pinMode(pins::outputs::OUTPUT1, OUTPUT);
    pinMode(pins::outputs::OUTPUT2, OUTPUT);
    pinMode(pins::outputs::OUTPUT3, OUTPUT);
    pinMode(pins::outputs::OUTPUT4, OUTPUT);
    display->init();
    display->backlight();
    display->createChar(specialCharacters::SELECTOR, Icons::selector);
    display->createChar(specialCharacters::FILLED_SELECTOR, Icons::selector_filled);
    display->createChar(specialCharacters::LOADING_START, Icons::LOADING_START);
    display->createChar(specialCharacters::LOADING_START_FILLED, Icons::LOADING_START_FILLED);
    display->createChar(specialCharacters::LOADING_END, Icons::LOADING_END);
    display->createChar(specialCharacters::LOADING_END_FILLED, Icons::LOADING_END_FILLED);
    display->createChar(specialCharacters::LOADING_MIDDLE, Icons::LOADING_MIDDLE);
    display->createChar(specialCharacters::LOADING_MIDDLE_FILLED, Icons::LOADING_MIDDLE_FILLED);
    g_cursor_range.push_back(1);
    g_cursor_range.push_back(2);
    g_valid_keys.push_back('A');
    g_valid_keys.push_back('B');
    g_valid_keys.push_back('C');
    menu = new HomeMenu(*display, g_cursor_range, g_valid_keys);
    menu->drawMenu();
    readFromEEPROM();
    xTaskCreate(task1, "UNIT Motor with control phase", 512, NULL, 1, NULL);
    xTaskCreate(task2, "Main Inputs Logic", 1024, NULL, 1, NULL);
    xTaskCreate(task3, "Menu", 512, NULL, 1, NULL);
    xTaskCreate(air_cleaner, "Air Cleaner", 256, NULL, 1, NULL);
    vTaskStartScheduler();
}

void loop() {     
    yield();
}