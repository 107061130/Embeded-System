#include "mbed.h"
#include "uLCD_4DGL.h"

uLCD_4DGL uLCD(D1, D0, D2); // serial tx, serial rx, reset pin;
AnalogOut aout(PA_4);
InterruptIn sw3(USER_BUTTON);
EventQueue queue(32 * EVENTS_EVENT_SIZE);
DigitalIn B1(D8);
DigitalIn B2(D9);
DigitalIn B3(D10);

Thread t;
float ADC[1000];
void Print_ADC();

int main(){

    float f = 0.5;
    int sample = 1000;  
    int f1 = 2;
   while(1) {
        if (B1 == 1 && f < 1){
            f *= 2;
            f1 /= 2;
        }
        else if (B2 == 1 && f > 0.125){
            f /= 2;
            f1 *= 2;
        }
        else if (B3 == 1){
            break;
        }
        uLCD.color(BLUE);  
        uLCD.background_color(WHITE);
        uLCD.textbackground_color(WHITE);
        uLCD.cls();
        // basic printf demo = 16 by 18 characters on screen
        uLCD.locate(1, 1);
        uLCD.text_width(4); //4X size text
        uLCD.text_height(4);
        uLCD.color(GREEN);
        uLCD.printf("\n1/%d\n", f1); //Default Green on black text
   }

    float a1;
    float b = 80;
    b = b * f;
    a1 = (1.0f  / (sample * (b / 240))) / 0.24;
    int k  = 0;

    t.start(callback(&queue, &EventQueue::dispatch_forever)); // Build an eventqueue for the multiple task 
    sw3.rise(queue.event(Print_ADC));
                        
    while(1) {                                             
        for (float i = 0; i < 1; i = i + a1) {
            aout = i;
            if (k >= sample && k < 2*sample)
                ADC[k-sample] = aout;   // store ADC_data
            k++;
            ThisThread::sleep_for(1ms);
        }
        for (float i = 0; i < (240 - 2*b); i = i + 1) {
            aout = aout;
            if (k >= sample && k < 2*sample)
                ADC[k-sample] = aout;   // store ADC_data
            k++;
            ThisThread::sleep_for(1ms);
        }
        for (float i = 1; i > 0; i = i - a1) {
            aout = i;
            if (k >= sample && k < 2*sample)
                ADC[k-sample] = aout;   // store ADC_data
            k++;
            ThisThread::sleep_for(1ms);
        }
    }

}

void Print_ADC() {          
    for (int i = 0; i < 1000; i++) {
        printf("%f\r\n", ADC[i]);
        ThisThread::sleep_for(1ms);
    }
}