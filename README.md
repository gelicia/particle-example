# Particle Example
This is an example node/express server to show how to get data from a Particle Photon and interact with it. It's meant to be as simple of an example as I can make it :) 
Slides from a talk for Girl Develop It: http://files.gelicia.com/internet%20connected%20clothing.pdf

## Hardware
The hardware code was written for a Particle Photon. It has a temperature sensor (DS18B20) and a strand of 5 (WS2812/Neopixel) LEDs to show input and output.
### Setup
1. Wire up the LEDs and temperature sensor (don't forget the pull up resistor on the temperature sensor)
1. Copy code from photon.ino in this repository to your Photon
1. Include DS18B20 and Neopixel libraries
1. Flash to a Photon (You can easily test inputs and see outputs on [console.particle.io](https://console.particle.io))
### Details
- Code exposes the variable temp, which is the last reading of the temperature sensor in F. 
- It also exposes a setLED function, which gets passed a string like `0,0,0,0` which stands for LED index, Red, Green, Blue

## Server
An express / node server runs to expose an API without exposing the Particle access keys and serve a webpage for interaction.
1. Run `npm install`
1. Rename particleInfoTEMP.js to particleInfo.js and add your Photon's device ID and access token.
1. Run `npm start`
1. Go to [http://localhost:3000/](http://localhost:3000/)
### Details
- Two endpoints are exposed that passthrough to the Particle API
    - `temperature` gets the value from the temperature sensor
    - `setLED` sets an LED to a color, using params in a form
        - `ledIdx` is the address of the LED (starting at 0)
        - `ledR` is the red value to send (0-255)
        - `ledG` is the green value to send (0-255)
        - `ledB` is the blue value to send (0-255)
 - The public/ directory is served as a static webpage and is intended to have a UI to interact with the endpoints.