#include "arduino.h"
#include "buttonarray.h"

Button::InitLimits(uint8_t step){

    int32_t button_value;
    int32_t lower_button_value;
    int32_t higher_button_value;

    button_value = uint32_t(max_value * (float(step * r2_value)/float((step * r2_value) + r1_value)));
    lower_button_value = uint32_t(max_value * (float((step-1) * r2_value)/float(((step-1) * r2_value) + r1_value)));
    higher_button_value = uint32_t(max_value * (float((step+1) * r2_value)/float(((step+1) * r2_value) + r1_value)));
    low_limit_= (button_value - ((button_value - lower_button_value)/2));
    high_limit_= (button_value + ((higher_button_value - button_value)/2));
    step_ = step;
}

bool Button::Pressed(int32_t analog_reading)
{
    if ((analog_reading > low_limit_) && (analog_reading <=  high_limit_)) {
        return true;
    } 
    return false;
}
int32_t Button::high_limit()
{
    return high_limit_;
}

int32_t Button::low_limit()
{
    return low_limit_;
}

// Add all buttons in incremental order
ButtonArray::AddAll()
{
    Serial.println("Add All");
    for(size_t i = 0; i < nb_buttons_; i++)
    {
        ButtonArray::Add(i, i);
    }
    Serial.print("High Limit: ");
    Serial.println(high_limit_);

}


// Adds a single button to the array
// Takes a step (rank in the resistor ladder), and the index in the button array.
ButtonArray::Add(uint8_t step, uint8_t index){
    Button button;
    button.init_limits(step);
    button_array_[index] = button;
    if (button.high_limit() > high_limit_) {
        high_limit_ = button.high_limit();
    }
}

int32_t ButtonArray::high_limit(){
    return high_limit_;
}
int8_t ButtonArray::ReadButtons(){
    uint32_t analog_read_temp =0;
    uint32_t analog_line_read_average=0;

    for (byte x = 0; x < 19; x++){
        analog_read_temp = analogRead(pin_);
        if (analog_read_temp <= high_limit_){
            analog_line_read_average = (analog_line_read_average + analog_read_temp) / 2;
        }
    }
    Serial.print("Average ");
    Serial.println(analog_line_read_average);
    for (size_t x = 0; x < nb_buttons_; x++) {
        Button button = button_array_[x];
        if (button.Pressedssed(analog_line_read_average)) {
            last_pressed_ms = millis();
            Serial.print("Returning ");
            Serial.println(x);
            return x;
        }
    }
    return -1;
}

int8_t ButtonArray::Pressed() 
{
    if ((analogRead(pin_) <= high_limit_) && ((millis() - last_pressed_ms) > DEBOUNCE_MS )){
        return ReadButtons();
    }
    return -1;
}
// Return true as long as the indicated button is held and the deadline not reached.
bool ButtonArray::Held(uint8_t index, uint32_t deadline) 
{
    if ((analogRead(pin_) <= high_limit_) && 
    (millis() < deadline) && 
    (ReadButtons() == index)) {
        return true;
    }
    return false;
}

// Return true as long as the indicated button is held
bool ButtonArray::Held(uint8_t index,) 
{
    if ((analogRead(pin_) <= high_limit_) && (ReadButtons() == index)) {
        return true;
    }
    return false;
}