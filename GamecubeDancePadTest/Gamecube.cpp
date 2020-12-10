#include "Gamecube.h"
extern "C" void gc_asm_write_read(uint32_t *buff, uint8_t len, uint32_t *read_buff, uint8_t read_buff_len);

#ifdef DEBUG
    Serial pc(USBTX, USBRX); // tx, rx
#endif
DigitalIn my_in(p10);
DigitalOut p(p19);

void reverse_array(uint32_t *arr, uint8_t count) {
    int temp, i;
    for (i = 0; i < count/2; ++i) {
        temp = arr[count-i-1];
        arr[count-i-1] = arr[i];
        arr[i] = temp;
    }
}

//TODO: translate pin name to assembly layer, it is currently hardcoded at p9 and p10!!!
Gamecube::Gamecube(PinName _data_line)
    :data_line(_data_line)
{
    
}

int Gamecube::get_device_id() {
    uint32_t start_sequence[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int num_bits = 2 * 8;
    uint32_t data[num_bits];
    int ret_val;
    
    gc_write_read(start_sequence, 8, data, num_bits);
    
    for (int i=0; i < num_bits; i++) {
        ret_val <<= 1;
        ret_val |= data[i];
    }
    
#ifdef DEBUG
    for (int i=0; i < num_bits; i++) {
        pc.printf("%d", data[i]!=0);
        if (i>0 && (i+1)%8 == 0) {
            pc.printf(" ");    
        }
    }
    pc.printf("\n\r");
#endif
    
    return ret_val;
}

void Gamecube::rumble(bool r) {
    _rumble = r;
    uint32_t status_sequence[] = {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, _rumble};
    
    // passing in a variable for data because the assembly can't waste the time between writing and reading at the current moment
    //TODO: implement assembly such that an empty data array doesn't have to be passed in
    uint32_t data[1];
    gc_write_read(status_sequence, 24, data, 0);
}

void Gamecube::update(void) {
    int num_bits = 8 * 8;
    uint32_t data[num_bits];
    uint32_t status_sequence[] = {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, _rumble};
    
    gc_write_read(status_sequence, 24, data, num_bits);
    
    // data[0] data[1] data[2] are not used
    START   = data[3] == 1;
    Y       = data[4] == 1;
    X       = data[5] == 1;
    B       = data[6] == 1;
    A       = data[7] == 1;
    
    //data[8] is always 1
    L       = data[9] == 1;
    R       = data[10] == 1;
    Z       = data[11] == 1;
    D_UP    = data[12] == 1;
    D_DOWN  = data[13] == 1;
    D_RIGHT = data[14] == 1;
    D_LEFT  = data[15] == 1;
    
    JOYSTICK_X = 0;
    JOYSTICK_Y = 0;
    C_STICK_X = 0;
    C_STICK_Y = 0;
    LEFT_TRIGGER = 0;
    RIGHT_TRIGGER = 0;
    
    for (int i=0; i < 8; i++) {
        JOYSTICK_X <<= 1;
        JOYSTICK_X |= data[i+16];
        JOYSTICK_Y <<= 1;
        JOYSTICK_Y |= data[i+24];
        
        C_STICK_X <<= 1;
        C_STICK_X |= data[i+32];
        C_STICK_Y <<= 1;
        C_STICK_Y |= data[i+40];
        
        LEFT_TRIGGER <<= 1;
        LEFT_TRIGGER |= data[i+48];
        RIGHT_TRIGGER <<= 1;
        RIGHT_TRIGGER |= data[i+56];
    }
    
#ifdef DEBUG
    for (int i=0; i < num_bits; i++) {
        pc.printf("%d", data[i]!=0);
        if (i>0 && (i+1)%8 == 0) {
            pc.printf(" ");    
        }
    }
    pc.printf("\n\r");
#endif
}

void Gamecube::gc_write_read(uint32_t* wbuff, uint8_t wbuff_len, uint32_t *rbuff, uint8_t rbuff_len) {
    __disable_irq();
    gc_asm_write_read(wbuff, wbuff_len, rbuff, rbuff_len);
    __enable_irq();
}