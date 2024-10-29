//
// Created by HJT2 on 2/2/2024.
//
// 
#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include "Arduino.h"
#include <cstdint>
#include "FreeRTOS.h"
#include "task.h"
#include <SevenSegmentTM1637.h>
#include <SPI.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <vector>
#include "menus.h"

//  Main Namespace
namespace pins {
    namespace outputs {
        const uint8_t OUTPUT1 = PB12;
        const uint8_t OUTPUT2 = PB13;
        const uint8_t OUTPUT3 = PB14;
        const uint8_t OUTPUT4 = PB15;
    }
    namespace inputs {
        const uint8_t MAIN_SELECTOR_MANUAL = PA2; // 1
        const uint8_t MAIN_SELECTOR_AUTOMATIC = PA1; // 2
        const uint8_t AUTO_SELECTOR_PADDLE = PA0; // 3
        const uint8_t AUTO_SELECTOR_AUTOMATIC = PC13; // 4
        const uint8_t MOTOR_START = PB9; // 5
        const uint8_t MOTOR_STOP = PB8; // 6
        const uint8_t PUMP_UP = PB5; // 7
        const uint8_t PUMP_DOWN = PB4; // 8
        const uint8_t AIR_CLEANER_BUTTON = PB3;  // 9
        const uint8_t HIGHLEVEL_MICTORSWITCH = PA15; // 10
        const uint8_t LOWLEVEL_MICTORSWITCH = PA12; // 11
        const uint8_t SENSOR = PA11; // 14
        const uint8_t PHASE_CONTROL = PA10; // 12
        const uint8_t HIGHLEVEL_SELECTOR_TIMER = PA9; // 13
        const uint8_t PADDLE = PA8; // 15
        const uint8_t DATA = PB7; // 20
        const uint8_t CLK = PB6; // 21
        const uint8_t KEYPAD_DATA_A = PB0;
        const uint8_t KEYPAD_DATA_B = PB1;
        const uint8_t KEYPAD_DATA_C = PB2;
        const uint8_t KEYPAD_DATA_D = PB10;
        const uint8_t KEYPAD_DAV = PB11;
    }
    namespace controls {
        extern float AIR_CLEANER_TIMER;
        extern float MICROSWITCH_TIMER;
        extern bool TIMER_TRIGERED;
        extern bool air_cleaner;
        extern String status;
        // const uint8_t chipSelectPin = PA4;
        extern char keyValue;
    }
}

namespace Icons {
    extern byte LOADING_START[];
    extern byte LOADING_START_FILLED[];
    extern byte LOADING_MIDDLE[];
    extern byte LOADING_MIDDLE_FILLED[];
    extern byte LOADING_END[];
    extern byte LOADING_END_FILLED[];
    extern byte selector[];
    extern byte selector_filled[];
}

//  Enumerations
enum MainSelectorStatus {
    MANUAL,
    AUTOMATIC,
    MAIN_OFF
};
enum AutomaticSelectorStatus {
    PADDLE,
    AUTO_AUTO,
    AUTO_OFF
};
enum TimerSelector {
    MICROSWITCH,
    Timer,
    TIMER_OFF
};

enum OutputPins {
    UNIT_MOTOR_OUTPUT = pins::outputs::OUTPUT1,
    PUMP_UP_OUTPUT = pins::outputs::OUTPUT2,
    PUMP_DOWN_OUTPUT = pins::outputs::OUTPUT3,
    AIR_CLEANER_OUTPUT = pins::outputs::OUTPUT4,
};

enum MenuKey {
    UP,
    DOWN,
    ENTER,
    NEXT,
    NO_KEY
};

// Prototypes
extern LiquidCrystal_I2C* display;
// Outputs setter
inline void setOutput(OutputPins pin, bool value);

// Inputs & Outputs status
inline bool air_cleaner_status();
inline bool unit_motor_status();
inline bool pump_up_status();
inline bool pump_down_status();
inline bool start_button_status();
inline bool stop_button_status();
inline bool pump_up_button_status();
inline bool pump_down_button_status();
inline bool highlevel_microswitch_status();
inline bool lowlevel_microswitch_status();
inline bool phase_status();
inline bool paddle_status();
inline MainSelectorStatus main_selector_status();
inline AutomaticSelectorStatus automatic_selector_status();
inline TimerSelector timer_selector_status();
char readKeypad();
void update_display();

inline String set_status();
inline uint32_t convert_string_to_decimal(String status);

template<class data>
void print(data msg, std::vector<uint8_t> cursor, bool clear=true, unsigned long delay=0);

void loading();
void makeDelay();
void stage(String str);


template<typename A>
inline void controlVariable(A &var, const int min, const int max){if(var < min) var = max; else if(var > max) var = min;};;

// EEPROM
void writeByte(uint32_t address, uint8_t value);
uint8_t readByte(uint32_t address);
void eraseSector(uint32_t sectorAddress);

// Synchronous tasks
void task1(void* pvParameters);  // UNIT Motor with phase control
void task2(void* pvParameters);
void task3(void* pvParameters);
void air_cleaner(void* pvParameters);

#endif // DEFINITIONS_H
