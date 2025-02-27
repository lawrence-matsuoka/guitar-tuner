# guitar-tuner

A guitar tuner using an MSP430F5529 development board and an MAX4466 microphone amplifier module.

The goal is to create a guitar tuner with a custom 3D printed enclosure to gift to my girlfriend on her birthday.

- The mic module will take in an analog signal
- The dev board processes the analog signal into a digital signal
- A Fast Fourier Transform (FFT) is performed to determine the dominant frequency
- Each note of the guitar has a corresponding dominant frequency
- LEDs keep track of which string is currently being tuned and which need to be tuned/have been tuned

## Learning Outcomes
- Protoboard soldering
- Audio signal processing
- Application of FFT
- Basic 3D modeling
