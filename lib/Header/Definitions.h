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

//  Main Namespace
namespace pins {
    namespace outputs {
        const uint8_t OUTPUT1 = PB12;
        const uint8_t OUTPUT2 = PB13;
        const uint8_t OUTPUT3 = PB14;
        const uint8_t OUTPUT4 = PB15;
    }
    namespace inputs {
        const uint8_t MAIN_SELECTOR_MANUAL = PB9; // 1
        const uint8_t MAIN_SELECTOR_AUTOMATIC = PB8; // 2
        const uint8_t AUTO_SELECTOR_PADDLE = PB5; // 3
        const uint8_t AUTO_SELECTOR_AUTOMATIC = PB4; // 4
        const uint8_t MOTOR_START = PB3; // 5
        const uint8_t MOTOR_STOP = PA15; // 6
        const uint8_t PUMP_UP = PA12; // 7
        const uint8_t PUMP_DOWN = PA11; // 8
        const uint8_t AIR_CLEANER_BUTTON = PA10;  // 9
        const uint8_t HIGHLEVEL_MICTORSWITCH = PA9; // 10
        const uint8_t LOWLEVEL_MICTORSWITCH = PA8; // 11
        const uint8_t HIGHLEVEL_SELECTOR_MICROSWITCH = PB15; // 14
        const uint8_t PHASE_CONTROL = PB14; // 12
        const uint8_t HIGHLEVEL_SELECTOR_TIMER = PB13; // 13
        const uint8_t PADDLE = PA2; // 15
        const uint8_t MENU_DOWN = PC13; // 16
        const uint8_t MENU_UP = PC15; // 17
        // const uint8_t MENU_ENTER = PC13; // 18
        const uint8_t MENU_NEXT = PC14; // 19
        const uint8_t DATA = PB7; // 20
        const uint8_t CLK = PB6; // 21
        const uint8_t NTC_INPUT = PA3;
    }
    namespace controls {
        extern float AIR_CLEANER_TIMER;
        extern float MICROSWITCH_TIMER;
        extern bool TIMER_TRIGERED;
        extern int8_t menu;
        extern bool air_cleaner;
        extern String status;
        const uint8_t chipSelectPin = PA4;
    }
    // namespace outputs {
    // 1- const uint8_t OUTPUT1 = PB11;
    // 2- const uint8_t OUTPUT2 = PB10;
    // 3- const uint8_t OUTPUT3 = PB1;
    // 4- const uint8_t OUTPUT4 = PB0;
    // 5- const uint8_t BUZZER = PB12;
    // }

    // namespace inputs {
    //     const uint8_t MAIN_SELECTOR_MANUAL = PB9; // 1
    //     const uint8_t MAIN_SELECTOR_AUTOMATIC = PB8; // 2
    //     const uint8_t AUTO_SELECTOR_PADDLE = PB5; // 3
    //     const uint8_t AUTO_SELECTOR_AUTOMATIC = PB4; // 4
    //     const uint8_t MOTOR_START = PB3; // 5
    //     const uint8_t MOTOR_STOP = PA15; // 6
    //     const uint8_t PUMP_UP = PA12; // 7
    //     const uint8_t PUMP_DOWN = PA11; // 8
    //     const uint8_t AIR_CLEANER_BUTTON = PA2;  // 9
    //     const uint8_t HIGHLEVEL_MICTORSWITCH = PA10; // 10
    //     const uint8_t LOWLEVEL_MICTORSWITCH = PA9; // 11
    //     const uint8_t HIGHLEVEL_SELECTOR_MICROSWITCH = PA8; // 12
    //     const uint8_t PHASE_CONTROL = PB15; // 13
    //     const uint8_t HIGHLEVEL_SELECTOR_TIMER = PB14; // 14
    //     const uint8_t PADDLE = PB13; // 15
    //     const uint8_t MENU_UP = PA0;
    //     const uint8_t MENU_DOWN = PC14;
    //     const uint8_t MENU_ENTER = PC13;
    //     const uint8_t MENU_NEXT = PC15;
    //     const uint8_t DATA = PB7; // 19
    //     const uint8_t CLK = PB6; // 20
    // }
    
    // namespace controls {
    // 1- extern float AIR_CLEANER_TIMER;
    // 2- extern float MICROSWITCH_TIMER;
    // 3- extern bool TIMER_TRIGERED;
    // 4- extern bool menu;
    // 5- extern bool air_cleaner;
    // 6- extern String status;
    // 7- const uint8_t latch = PA2;
    // 8- const uint8_t data_pin = PA3;
    // 9- const uint8_t clock_pin = PA1;
    // 10- const uint8_t chipSelectPin = PA4;
    // }
}

namespace Icons {
    extern byte LOADING_START[];
    extern byte LOADING_START_FILLED[];
    extern byte LOADING_MIDDLE[];
    extern byte LOADING_MIDDLE_FILLED[];
    extern byte LOADING_END[];
    extern byte LOADING_END_FILLED[];
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

enum specialCharacters {
    LOADING_START_FILLED,
    LOADING_START,
    LOADING_END_FILLED,
    LOADING_END,
    LOADING_MIDDLE_FILLED,
    LOADING_MIDDLE,
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
inline MenuKey get_menu_key();
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
