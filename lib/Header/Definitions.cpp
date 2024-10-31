//
// Created by HJT2 on 2/2/2024.
//
#include "Definitions.h"
float pins::controls::AIR_CLEANER_TIMER = 1.0;
float pins::controls::MICROSWITCH_TIMER = 5.0;
bool pins::controls::TIMER_TRIGERED = false;
bool pins::controls::air_cleaner = false;
char pins::controls::keyValue = '\0';
LiquidCrystal_I2C* display = new LiquidCrystal_I2C(0x27, 16, 2);

namespace Icons {
    byte LOADING_START[] = {
        0x0F,
        0x18,
        0x10,
        0x10,
        0x10,
        0x10,
        0x18,
        0x0F
    };
    byte LOADING_START_FILLED[] = {
        0x0F,
        0x18,
        0x13,
        0x17,
        0x17,
        0x13,
        0x18,
        0x0F
    };
    byte LOADING_MIDDLE[] = {
        0x1F,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x1F,
    };
    byte LOADING_MIDDLE_FILLED[] = {
        0x1F,
        0x00,
        0x1F,
        0x1F,
        0x1F,
        0x1F,
        0x00,
        0x1F,
    };
    byte LOADING_END[] = {
        0x1E,
        0x03,
        0x01,
        0x01,
        0x01,
        0x01,
        0x03,
        0x1E,
    };
    byte LOADING_END_FILLED[] = {
        0x1E,
        0x03,
        0x19,
        0x1D,
        0x1D,
        0x19,
        0x03,
        0x1E,
    };
    byte selector[] = {
        0x08,
        0x0C,
        0x0A,
        0x09,
        0x09,
        0x0A,
        0x0C,
        0x08
    };
    byte selector_filled[] = {
        0x08,
        0x0C,
        0x0E,
        0x0F,
        0x0F,
        0x0E,
        0x0C,
        0x08
    };
}

inline bool air_cleaner_status() {
    return digitalRead(AIR_CLEANER_OUTPUT);
}
inline bool unit_motor_status() {
    return digitalRead(UNIT_MOTOR_OUTPUT);
}
inline bool pump_up_status() {
    return digitalRead(PUMP_UP_OUTPUT);
}
inline bool pump_down_status() {
    return digitalRead(PUMP_DOWN_OUTPUT);
}
inline MainSelectorStatus main_selector_status() {
    if(!digitalRead(pins::inputs::MAIN_SELECTOR_MANUAL))
        return MainSelectorStatus::MANUAL;
    else if(!digitalRead(pins::inputs::MAIN_SELECTOR_AUTOMATIC))
        return MainSelectorStatus::AUTOMATIC;
    else
        return MainSelectorStatus::MAIN_OFF;
}
inline AutomaticSelectorStatus automatic_selector_status() {
    if(!digitalRead(pins::inputs::AUTO_SELECTOR_PADDLE))
        return AutomaticSelectorStatus::PADDLE;
    else if(!digitalRead(pins::inputs::AUTO_SELECTOR_AUTOMATIC))
        return AutomaticSelectorStatus::AUTO_AUTO;
    else
        return AutomaticSelectorStatus::AUTO_OFF;
}
inline TimerSelector timer_selector_status() {
    if(!digitalRead(pins::inputs::HIGHLEVEL_SELECTOR_TIMER))
        return TimerSelector::Timer;
    else
        return TimerSelector::MICROSWITCH;
}
inline bool start_button_status() {
    return !digitalRead(pins::inputs::MOTOR_START);
}
inline bool stop_button_status() {
    return digitalRead(pins::inputs::MOTOR_STOP);
}
inline bool pump_up_button_status() {
    return !digitalRead(pins::inputs::PUMP_UP);
}
inline bool pump_down_button_status() {
    return !digitalRead(pins::inputs::PUMP_DOWN);
}
inline bool highlevel_microswitch_status() {
    return digitalRead(pins::inputs::HIGHLEVEL_MICTORSWITCH);
}
inline bool lowlevel_microswitch_status() {
    return digitalRead(pins::inputs::LOWLEVEL_MICTORSWITCH);
}
inline bool phase_status() {
    return !digitalRead(pins::inputs::PHASE_CONTROL);
}
inline bool paddle_status() {
    return !digitalRead(pins::inputs::PADDLE);
}

inline void setOutput(OutputPins pin, bool value) {
    digitalWrite(pin, value);
}

inline String set_status() {
    using namespace pins;
    String status = "";
    status += "00";
    status += String(main_selector_status() == MainSelectorStatus::MANUAL);
    status += String(main_selector_status() == MainSelectorStatus::AUTOMATIC);
    status += String(automatic_selector_status() == AutomaticSelectorStatus::PADDLE);
    status += String(automatic_selector_status() == AutomaticSelectorStatus::AUTO_AUTO);
    status += String(!stop_button_status());   
    status += String(start_button_status());   
    status += String(pump_up_button_status());   
    status += String(pump_down_button_status());   
    status += String(!highlevel_microswitch_status());   
    status += String(!lowlevel_microswitch_status());   
    status += String(phase_status());   
    status += String(timer_selector_status() == TimerSelector::MICROSWITCH);
    status += String(timer_selector_status() == TimerSelector::Timer);
    status += String(paddle_status());   
    // status += String(unit_motor_status());   
    // status += String(pump_up_status());   
    // status += String(pump_down_status());   
    // status += String(air_cleaner_status());   
    if(status != controls::status) {
        controls::status = status;
    }
    return controls::status;
}

char readKeypad() {
    // Read the data pins
    uint8_t data = (digitalRead(pins::inputs::KEYPAD_DATA_D) << 3) | 
                   (digitalRead(pins::inputs::KEYPAD_DATA_C) << 2) | 
                   (digitalRead(pins::inputs::KEYPAD_DATA_B) << 1) | 
                    digitalRead(pins::inputs::KEYPAD_DATA_A);
    
    // Map the binary value to corresponding characters
    switch (data) {
        case 0b0000: pins::controls::keyValue = '1'; break;
        case 0b0001: pins::controls::keyValue = '2'; break;
        case 0b0010: pins::controls::keyValue = '3'; break;
        case 0b0011: pins::controls::keyValue = 'A'; break;
        case 0b0100: pins::controls::keyValue = '4'; break;
        case 0b0101: pins::controls::keyValue = '5'; break;
        case 0b0110: pins::controls::keyValue = '6'; break;
        case 0b0111: pins::controls::keyValue = 'B'; break;
        case 0b1000: pins::controls::keyValue = '7'; break;
        case 0b1001: pins::controls::keyValue = '8'; break;
        case 0b1010: pins::controls::keyValue = '9'; break;
        case 0b1011: pins::controls::keyValue = 'C'; break;
        case 0b1100: pins::controls::keyValue = '*'; break;
        case 0b1101: pins::controls::keyValue = '0'; break;
        case 0b1110: pins::controls::keyValue = '#'; break;
        case 0b1111: pins::controls::keyValue = 'D'; break;
        default: pins::controls::keyValue = '\0'; // No valid key pressed
    }

    if (pins::controls::keyValue == 'D' && !digitalRead(pins::inputs::KEYPAD_DAV)) {
        pins::controls::keyValue = '\0';
    }

    if(powerSaver) {
        // 1- Backlight is one: returns the key
        // 2- Backlight is off: returns '\0' and turns on the backlight
        return checkForPowerSaver(pins::controls::keyValue);
    } else {
        return pins::controls::keyValue;
    }
}

char checkForPowerSaver(char key) {
    if(key != '\0') {
        if(!powerSaverStatus) {
            powerSaverStatus = true;
            display->backlight();
            lastActionTime = millis();
            return '\0';
        } else {
            return key;
        }
    } else {
        if(millis() - lastActionTime >= 30000) {
            powerSaverStatus = false;
            display->noBacklight();
        }
        return '\0';
    }
}

inline uint32_t convert_string_to_decimal(String status) {
    uint32_t value = 0;
    uint8_t power = 0;
    for(int8_t i = status.length() - 1; i >= 0; i--) {
        value += (status[i] - '0') * pow(2, power);
        power++;
    }
    return value;
}

template<class data>
void print(data msg, std::vector<uint8_t> cursor, bool clear, unsigned long delay) {
    if(clear) {
        display->clear();
    }
    display->setCursor(cursor[0], cursor[1]);
    display->print(msg);
    vTaskDelay(delay / portTICK_PERIOD_MS);
}

void task1(void* pvParameters) {
    while(true) {
        if(stop_button_status() || !phase_status()) {
            setOutput(UNIT_MOTOR_OUTPUT, LOW);
        }
        if(start_button_status() && phase_status() && !stop_button_status()) {
            setOutput(UNIT_MOTOR_OUTPUT, HIGH);
        } 
    }
}

void task2(void* pvParameters) {
    while(true) {
        /* --------------------------- MAIN SELECTOR MANUAL --------------------------- */    
        if(main_selector_status() == MainSelectorStatus::MANUAL) {
            while(main_selector_status() == MainSelectorStatus::MANUAL && pump_up_button_status() && !highlevel_microswitch_status() && unit_motor_status()) {
                setOutput(PUMP_UP_OUTPUT, HIGH);
                yield();
            }
            setOutput(PUMP_UP_OUTPUT, LOW);
            while(main_selector_status() == MainSelectorStatus::MANUAL && pump_down_button_status() && !lowlevel_microswitch_status() && !digitalRead(pins::inputs::SENSOR) && unit_motor_status()) {
                setOutput(PUMP_DOWN_OUTPUT, HIGH);
                yield();
            }
            setOutput(PUMP_DOWN_OUTPUT, LOW);
        /* --------------------------- MAIN SELECTOR MANUAL --------------------------- */ 
        /* --------------------------- MAIN SELECTOR AUTOMATIC --------------------------- */    
        } else if(main_selector_status() == MainSelectorStatus::AUTOMATIC) {
        /* --------------------------- AUTOMATIC SELECTOR PADDLE --------------------------- */    
            if(automatic_selector_status() == AutomaticSelectorStatus::PADDLE) {
                while(!highlevel_microswitch_status() && unit_motor_status() && !pins::controls::TIMER_TRIGERED && automatic_selector_status() == AutomaticSelectorStatus::PADDLE) {
                    setOutput(PUMP_UP_OUTPUT, HIGH);
                    yield();
                }
                setOutput(PUMP_UP_OUTPUT, LOW);
                if(paddle_status()) {
                    pins::controls::TIMER_TRIGERED = false;
                    while(automatic_selector_status() == AutomaticSelectorStatus::PADDLE && !lowlevel_microswitch_status() && !digitalRead(pins::inputs::SENSOR) && unit_motor_status()) {
                        setOutput(PUMP_DOWN_OUTPUT, HIGH);
                        yield();
                    }
                    setOutput(PUMP_DOWN_OUTPUT, LOW);
                    if(automatic_selector_status() == AutomaticSelectorStatus::PADDLE) {
                        pins::controls::air_cleaner = true;
                    }
                    yield();
                    uint32_t current_time = millis();
                    while(automatic_selector_status() == AutomaticSelectorStatus::PADDLE && !highlevel_microswitch_status() && unit_motor_status()) {
                        if(timer_selector_status() != TimerSelector::Timer) {
                            setOutput(PUMP_UP_OUTPUT, HIGH);
                        } else {
                            if(millis() - current_time <= int(pins::controls::MICROSWITCH_TIMER * 1000)) {
                                setOutput(PUMP_UP_OUTPUT, HIGH);
                            } else {
                                // display->clear();
                                // display->setCursor(0, 0);
                                // display->print("Timer");
                                setOutput(PUMP_UP_OUTPUT, LOW);
                                pins::controls::TIMER_TRIGERED = true;
                                break;
                            }
                        }
                        yield();
                    }
                    // display->clear();
                    // display->setCursor(0, 0);
                    // display->print("End Paddle");
                    setOutput(PUMP_UP_OUTPUT, LOW);
                    // while(paddle_status()){yield();}
                }
        /* --------------------------- AUTOMATIC SELECTOR AUTOMATIC --------------------------- */
            } else if(automatic_selector_status() == AutomaticSelectorStatus::AUTO_AUTO) {
                uint32_t current_time = millis();
                while(automatic_selector_status() == AutomaticSelectorStatus::AUTO_AUTO && !highlevel_microswitch_status() && unit_motor_status()) {
                    if(timer_selector_status() != TimerSelector::Timer) {
                        setOutput(PUMP_UP_OUTPUT, HIGH);
                    } else {
                        if(millis() - current_time <= int(pins::controls::MICROSWITCH_TIMER * 1000)) {
                            setOutput(PUMP_UP_OUTPUT, HIGH);
                        } else {
                            // display->clear();
                            // display->setCursor(0, 0);
                            // display->print("Timer");
                            setOutput(PUMP_UP_OUTPUT, LOW);
                            pins::controls::TIMER_TRIGERED = false;
                            break;
                        }
                    }
                    yield();
                }
                setOutput(PUMP_UP_OUTPUT, LOW);
                pins::controls::TIMER_TRIGERED = false;
                while(automatic_selector_status() == AutomaticSelectorStatus::AUTO_AUTO && !lowlevel_microswitch_status() && !digitalRead(pins::inputs::SENSOR) &&unit_motor_status()) {
                    setOutput(PUMP_DOWN_OUTPUT, HIGH);
                    yield();
                }
                setOutput(PUMP_DOWN_OUTPUT, LOW);
                if(automatic_selector_status() == AutomaticSelectorStatus::AUTO_AUTO) {
                    pins::controls::air_cleaner = true;
                }
                yield();
            } else if(automatic_selector_status() == AutomaticSelectorStatus::AUTO_OFF) {
                yield();
            }
        /* --------------------------- AUTOMATIC SELECTOR OFF --------------------------- */
        }
        if(main_selector_status() == MainSelectorStatus::MAIN_OFF || automatic_selector_status() == AutomaticSelectorStatus::AUTO_OFF) {
            setOutput(PUMP_UP_OUTPUT, LOW);
            setOutput(PUMP_DOWN_OUTPUT, LOW);
            // setOutput(AIR_CLEANER_OUTPUT, LOW);
        }
    }
}

void task3(void* pvParameters) {
    while(true) {
        char key = readKeypad();
        menu->check(key);
        if(newMenu != "none") {
            delete menu;
            if(newMenu == "home") {
                menu = new HomeMenu(*display, {1, 2}, {'A', 'B', 'C'});
            } else if(newMenu == "about") {
                menu = new AboutMenu(*display, {1, 2}, {'A', 'B', 'C', 'D'});
            } else if(newMenu == "list") {
                menu = new ListMenu(*display, {1, 6}, {'A', 'B', 'C', 'D'});
            } else if(newMenu == "inputs") {
                menu = new InputsMenu(*display, {}, {});
            } else if(newMenu == "outputs") {
                menu = new OutputsMenu(*display, {}, {});
            } else if(newMenu == "settings") {
                menu = new SettingsMenu(*display, {1, 6}, {'A', 'B', 'C', 'D'});
            } else if(newMenu == "highlevel_timer") {
                menu = new HTimerMenu(*display, {1, 3}, {'A', 'B', 'C', 'D', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '#', '*'});
            } else if(newMenu == "aircleaner_timer") {
                menu = new ATimerMenu(*display, {1, 3}, {'A', 'B', 'C', 'D', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '#', '*'});
            }

            newMenu = "none";
            menu->display.clear();
            if(returnCursor) {
                menu->cursor = returnCursor;
                returnCursor = 0;
            }
            menu->drawMenu();
        }
        vTaskDelay(150 / portTICK_PERIOD_MS);
        while(key != '\0' && readKeypad() == key){}
    }
}

void air_cleaner(void* pvParameters) {
    while(true) {
        while(!digitalRead(pins::inputs::AIR_CLEANER_BUTTON)) {
            setOutput(AIR_CLEANER_OUTPUT, HIGH);
        }
        setOutput(AIR_CLEANER_OUTPUT, LOW);
        if(pins::controls::air_cleaner) {
            uint32_t current_time = millis();
            while(millis() - current_time <= int(pins::controls::AIR_CLEANER_TIMER * 1000) && unit_motor_status()) {
                setOutput(AIR_CLEANER_OUTPUT, HIGH);
                yield();
            }
            setOutput(AIR_CLEANER_OUTPUT, LOW);
            pins::controls::air_cleaner = false;
        } else {
            setOutput(AIR_CLEANER_OUTPUT, LOW);
        }
    }
}

void saveToEEPROM() {
    writeByte(0x1000, int(pins::controls::AIR_CLEANER_TIMER * 10)); 
    writeByte(0x2000, int(pins::controls::MICROSWITCH_TIMER * 10)); 
    writeByte(0x3000, uint8_t(powerSaver)); 
}

void readFromEEPROM() {
    pins::controls::AIR_CLEANER_TIMER = float(readByte(0x1000)) / 10;
    pins::controls::MICROSWITCH_TIMER = float(readByte(0x2000)) / 10;
    powerSaver = bool(readByte(0x3000));
}
