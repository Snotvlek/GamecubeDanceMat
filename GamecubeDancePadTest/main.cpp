//#include "mbed.h"
#include "Gamecube.h"

Serial pc1(USBTX, USBRX); // tx, rx
DigitalOut myled(LED1);
int main() {
    Gamecube g(p9);
    int device_id = g.get_device_id();
    if (device_id != NINTENDO_DEVICE_GC_WIRED) {
        pc1.printf("this device is not a nintendo gamecube wired controller!, it returned an ID of %d\n", device_id);    
    }
    while(1) {
        g.update();
        pc1.printf("A: %d \n\rB: %d \n\rX: %d \n\rY: %d \n\rL: %d \n\rR: %d \n\rZ: %d \n\rSTART: %d \n\rD_UP: %d \n\rD_LEFT: %d \n\rD_DOWN: %d \n\rD_RIGHT: %d \n\rJOYSTICK_X: %d \n\rJOYSTICK_Y: %d \n\rC_STICK_X: %d \n\rC_STICK_Y: %d \n\rLEFT_TRIGGER: %d \n\rRIGHT_TRIGGER: %d \n\r", g.A, g.B, g.X, g.Y, g.L, g.R, g.Z, g.START, g.D_UP, g.D_LEFT, g.D_DOWN, g.D_RIGHT, g.JOYSTICK_X, g.JOYSTICK_Y, g.C_STICK_X, g.C_STICK_Y, g.LEFT_TRIGGER, g.RIGHT_TRIGGER);
        wait(1);
        
        g.rumble(true);
        myled = 1;
        wait(1);
        myled = 0;
        g.rumble(false);
        wait(.2);
    }
}
