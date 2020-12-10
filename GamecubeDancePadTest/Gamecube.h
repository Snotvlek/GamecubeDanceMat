#ifndef GAMECUBE_H
#define GAMECUBE_H

////#include <stdint.h>
////#include <mbed.h>

//================================================================================
// Gamecube
//================================================================================

// gamecube controller device status ids
#define NINTENDO_DEVICE_GC_WIRED 0x0900

class Gamecube{
public:
    Gamecube(PinName _data_line);
    int get_device_id();
    void gc_write_read(uint32_t* wbuff, uint8_t wbuff_len, uint32_t *rbuff, uint8_t rbuff_len);
    void rumble(bool r);    
    void update(void);
    
    bool A;
    bool B;
    bool X;
    bool Y;
    bool START;
    bool L;
    bool R;
    bool Z;
    bool D_UP;
    bool D_DOWN;
    bool D_LEFT;
    bool D_RIGHT;
    
    int JOYSTICK_X;
    int JOYSTICK_Y;
    int C_STICK_X;
    int C_STICK_Y;
    int LEFT_TRIGGER;
    int RIGHT_TRIGGER;

private:
    DigitalOut data_line;
    bool _rumble;
};
#endif // include guard