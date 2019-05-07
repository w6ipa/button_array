#include "src/buttonarray/buttonarray.h"

ButtonArray button_array(A0, 1);
long button_depress_time;

void setup(void) {
    Serial.begin(115200);
    while (!Serial) {}
    button_array.add_all();
}

void check_button(ButtonArray buttons) {
    int8_t temp = buttons.pressed();

    if (temp >= 0){
        button_depress_time = buttons.last_pressed_ms;
        Serial.print("Button pressed ");
        Serial.println(temp);
        while (buttons.held(temp, button_depress_time + 1000)) {
            Serial.println("check paddle");
        }

        if ((millis() - button_depress_time) < 500) {
            if (temp == 0) {
                Serial.println("command");
            }
        }
    }
}
void loop() {
    check_button(button_array);
}