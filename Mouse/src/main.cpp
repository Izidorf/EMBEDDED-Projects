#include "USBDevice.h"
#include "USBMouse.h"
#include "MMA8451Q.h"
#include "TSISensor.h"

PinName const SDA = PTE25;
PinName const SCL = PTE24;
#define MMA8451_I2C_ADDRESS (0x1d<<1)

DigitalOut led_red(LED_RED);
DigitalOut led_green(LED_GREEN);
DigitalIn left(SW1);
DigitalIn right(SW3);
Serial pc(USBTX, USBRX);

MMA8451Q acc(SDA, SCL, MMA8451_I2C_ADDRESS);
TSISensor touch;

USBMouse mouse;

struct touch_struct{
    float curr;
    float prev;
}touch_val;

void init()
{
    pc.baud(115200);
    
    touch_val.curr = touch.readPercentage();
    
    led_green = 1;
    led_red = 1;
    
    return;
}

void check_right(void)
{
    if (right == 0) {
        mouse.press(MOUSE_RIGHT);
        pc.printf("Mouse press right \n");
        
        led_green = 0;
        led_red = 1;

    }
    else
    {
        led_green = 1;
        led_red = 1;
        mouse.release(MOUSE_RIGHT);
        
    }
}

void check_left(void)
{
    if (left == 0) {

        led_green = 1;
        led_red = 0;
        
        mouse.press(MOUSE_LEFT);
        pc.printf("Mouse press left \n");
        
    }
    else
    {
        led_green = 1;
        led_red = 1;
        mouse.release(MOUSE_LEFT);
    }
}

void check_move()
{
    int n_samples = 10;
    float x;
    float y;
    float thresh = 0.1;
    float sensitivity = 0.75;
    
    for(int i = 0; i<n_samples; i++)
    {
        x += acc.getAccX();
        y += acc.getAccY();
    }
    
    x /= n_samples;
    y /= n_samples;
    

    
    if(x >= thresh  || x <= -thresh)
    {
        for(int i = 0; i<(sensitivity*10); i++)  
            mouse.move((x*10*sensitivity),(0));
    } 
    
    
    if(y >= thresh  || y <= -thresh)
    {
        for(int i = 0; i<(sensitivity*10); i++) 
            mouse.move(0,(y*10*sensitivity));
    }
    
    return;    
}

void check_scroll()
{
    float diff;
    float sensitivity = 1.0;
    
    touch_val.prev = touch_val.curr;
    touch_val.curr = touch.readPercentage();
    
    diff = touch_val.prev - touch_val.curr;
    mouse.scroll(diff*100*sensitivity); 
     
}

int main()
{
    init();

    while(true)
    {
        check_right();
        check_left();
        check_move();
        check_scroll();
        
       // wait(0.005);
    }
}
