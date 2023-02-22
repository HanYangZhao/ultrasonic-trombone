# A DIY contoller for Trombone Champ

This controller is made using an ultrasonic sensor, Teensy, a button, three 3D printed parts and 2 pieces of 1 inch PVC tubing. It emulates mouse emulation based on the distance between the ultrasonic sensor and the sliding piece. Unlike the controller that are based on potentiometer, this one offers very low friction just like a real trombone. A button is used to simulate the blowing action.

Demo : 
https://www.youtube.com/watch?v=xJ7Nz3pXyhk


<img src="https://github.com/HanYangZhao/ultrasonic-trombone/blob/main/CAD/render.PNG?raw=true" alt="alt text" width="500"/>

<img src="https://github.com/HanYangZhao/ultrasonic-trombone/blob/main/photos/IMG_2262.JPG?raw=true" alt="alt text" width="500"/>

<img src="https://github.com/HanYangZhao/ultrasonic-trombone/blob/main/photos/IMG_2271.JPG?raw=true" alt="alt text" width="500"/>


# Part List
* 1 x Teensy 4.0 (3.2 should work as well)
* 1 x Ultrasonic Sensor HC-SR04 or equivalent
* 1 x Button
* 1 x Perforated PCB board
* 2 x 40 CM PVC tubing (0.58 inch / 15.8mm OD )
* 3 x 3D printed parts

# Wiring

## Ultrasonic Sensor

```
VCC     ----> 5V
trigPin ----> PIN 1
echoPin ----> PIN 0
GND     ----> GND
```

## Button 
```
MOUSE_LEFTCLICK_PIN ----> PIN 23
```

## Code
The original code was downloaded somewhere on the Internet and modified by me, but I was unable to locate it. Credit goes to whoever made that original code.

The current code use Mouse.move() because Mouse.moveTo() does not work in Linux/Mac. By using Mouse.move() we can use this on a Steam Deck
https://www.pjrc.com/teensy/td_mouse.html

