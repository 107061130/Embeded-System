# EXAM1
---
## Test Equipment
![](https://i.imgur.com/SlIBJYd.png)
---
## Result
### Slew rate=1
![](https://i.imgur.com/ncbvG5m.png)
![](https://i.imgur.com/j1EaRif.png)
---

### Slew rate=1/2
![](https://i.imgur.com/6dCx1vG.png)
![](https://i.imgur.com/y3x6VhA.png)
---

### Slew rate=1/4
![](https://i.imgur.com/RWqs29f.png)
![](https://i.imgur.com/6zcr45o.png)
---

### Slew rate=1/8
![](https://i.imgur.com/y7t8L4T.png)
![](https://i.imgur.com/WqsL05r.png)
---

## Code
### 操作
最右邊的button讓Slew Rate * 2

中間的讓Slew Rate / 2

最左邊的確定Slew Rate

按下User_Button印出蒐集的ADC_data

---
### Button Select Slew Rate
```
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
```

### Generate Wave and Store ADC_data
```
float a1;
float b = 80;
b = b * f;
a1 = (1.0f  / (sample * (b / 240))) / 0.24;
int k  = 0;

while(1) {  
    // UP  
    for (float i = 0; i < 1; i = i + a1) {
        aout = i;
        if (k >= sample && k < 2*sample)
            ADC[k-sample] = aout;   // store ADC_data
        k++;
        ThisThread::sleep_for(1ms);
    }
    
    // MIDDLE
    for (float i = 0; i < (240 - 2*b); i = i + 1) {
        aout = aout;
        if (k >= sample && k < 2*sample)
            ADC[k-sample] = aout;   // store ADC_data
        k++;
        ThisThread::sleep_for(1ms);
    }
    
    // DOWN
    for (float i = 1; i > 0; i = i - a1) {
        aout = i;
        if (k >= sample && k < 2*sample)
            ADC[k-sample] = aout;   // store ADC_data
        k++;
        ThisThread::sleep_for(1ms);
    }
}
```

### Thread to Output ADC_data when push USER_BUTTON

```
t.start(callback(&queue, &EventQueue::dispatch_forever)); // Build an eventqueue for the multiple task 
sw3.rise(queue.event(Print_ADC));
    
void Print_ADC() {          
    for (int i = 0; i < 1000; i++) {
        printf("%f\r\n", ADC[i]);
        ThisThread::sleep_for(1ms);
    }
}
```

### FFT.py
```
import matplotlib.pyplot as plt
import numpy as np
import serial
import time

Fs = 500.0;  # sampling rate
Ts = 1.0/Fs; # sampling interval
t = np.arange(0,1,Ts) # time vector; create Fs samples between 0 and 1.0 sec.
y = np.arange(0,1,Ts) # signal vector; create Fs samples

n = len(y) # length of the signal
k = np.arange(n)
T = n/Fs
frq = k/T # a vector of frequencies; two sides frequency range
frq = frq[range(int(n/2))] # one side frequency range

serdev = '/dev/ttyACM0'
s = serial.Serial(serdev)
for x in range(0, int(Fs)):
    line=s.readline() # Read an echo string from B_L4S5I_IOT01A terminated with '\n'
    # print line
    y[x] = float(line)

Y = np.fft.fft(y)/n*2 # fft computing and normalization
Y = Y[range(int(n/2))] # remove the conjugate frequency parts

fig, ax = plt.subplots(2, 1)
ax[0].plot(t,y)
ax[0].set_xlabel('Time')
ax[0].set_ylabel('Amplitude')
ax[1].plot(frq,abs(Y),'r') # plotting the spectrum
ax[1].set_xlabel('Freq (Hz)')
ax[1].set_ylabel('|Y(freq)|')
plt.show()
s.close()
```
