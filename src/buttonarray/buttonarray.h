#ifndef buttonarray_h
#define buttonarray_h
#include "Arduino.h"

#define MAX_BUTTONS 8
#define DEBOUNCE_MS 200

#if defined(ARDUINO_ARCH_ESP32)
    #define max_value 4095
#else
    #define max_value 1023
    #define r1_value 10
    #define r2_value 1
#endif

class Button {
    private:
        int32_t low_limit_;
        int32_t high_limit_;
        uint8_t step_;
    public:
        Button(){};
        init_limits(uint8_t step);
        bool pressed(int32_t analog_reading);
        int32_t high_limit();
        int32_t low_limit();
};

class ButtonArray {
    private:
        Button button_array_[MAX_BUTTONS];
        uint8_t pin_;
        uint8_t nb_buttons_;
        int32_t high_limit_;
        int8_t read_buttons();

    public:
        uint32_t last_pressed_ms;
        ButtonArray(uint8_t pin, uint8_t nb): pin_(pin), nb_buttons_(nb){};
        add_all();
        add(uint8_t step, uint8_t index);
        int32_t high_limit();
        int8_t pressed();
        bool held(uint8_t index, uint32_t until );
};


#endif