#include <Arduino.h>
#include <Definitions.h>

void setup() {
    // SPI.begin();
    // pinMode(pins::controls::chipSelectPin, OUTPUT);
    // digitalWrite(pins::controls::chipSelectPin, HIGH);

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
    display->createChar(specialCharacters::LOADING_START, Icons::LOADING_START);
    display->createChar(specialCharacters::LOADING_START_FILLED, Icons::LOADING_START_FILLED);
    display->createChar(specialCharacters::LOADING_END, Icons::LOADING_END);
    display->createChar(specialCharacters::LOADING_END_FILLED, Icons::LOADING_END_FILLED);
    // pins::controls::AIR_CLEANER_TIMER = float(readByte(0x1000)) / 10;
    // pins::controls::MICROSWITCH_TIMER = float(readByte(0x2000)) / 10;
    // xTaskCreate(task1, "UNIT Motor with control phase", 512, NULL, 1, NULL);
    // xTaskCreate(task2, "Inputs, Pumps and AirCleaner", 1024, NULL, 1, NULL);
    xTaskCreate(task3, "Menu & Shift Register", 512, NULL, 1, NULL);
    // xTaskCreate(air_cleaner, "Air Cleaner", 256, NULL, 1, NULL);
    vTaskStartScheduler();
}

void loop() {     
    yield();
}