/*
This program will make a light on the mbed blink when sending data
*/

// Official library for using mbed microcontroller
#include "mbed.h"

// Create objects to represent used accesories like led lights, button, xbee, a computer connected to robot through usb
DigitalIn button(p21);
DigitalOut myled(LED1);
Serial pc(USBTX, USBRX);
Serial xbee(p28, p27);
DigitalOut rst(p26);

// Allow xbee to recieve data from computer for configuration in command mode
void atCommandMode();

int main()
{
    // Set button mode according to its wiring; in our case PullUp mode
    button.mode(PullUp);
    
    // xbee must be reset before use
    rst = 0;
    wait(0.5);
    rst = 1;
    wait(0.5);
    
    pc.printf("xbee start putting characters\n");
    myled = 1; // turn led1 on
    wait(0.1);

    while (1) {
        if (!button) { // if button is pressed
            wait(0.2);
            atCommandMode();
        }
        
        // blink light to signify xbee sending out characters
        myled = 0;
        wait(0.1);
        myled = 1;
        wait(0.1);

        xbee.putc(0x7E);
        wait(0.5); // a call to wait function to see if it affects transmision
        xbee.putc(0x00);
        wait(0.5); // a call to wait function to see if it affects transmision
        xbee.putc(0x02);
        xbee.putc(0x01);
        xbee.putc(0xDD);
        xbee.putc(0xFF);
        xbee.putc(0xFF);
        xbee.putc(0x00);
        xbee.putc(0x48);
        xbee.putc(0x49);
        wait(2.0);
    }
}

// Allow xbee to recieve data from computer for configuration in command mode
void atCommandMode()
{
    pc.printf("start serial communication\n");
    // Allow at least 1 second to pass so xbee does not recieve or transmist any data to prevent problems if entering command mode
    wait(1.2);

    while (1) {
        // If terminal data available send through xbee
        if(pc.readable())
            xbee.putc(pc.getc());

        // If xbee gets data send to terminal
        while(xbee.readable())
            pc.printf("%c", xbee.getc());

        if (!button) { // exit the function
            wait(0.2);
            break;
        }
    }
}