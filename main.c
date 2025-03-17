#include <msp430.h> 
#include <math.h>
#include "fft.h"  // import the Fast Fourier Transform library

#define SAMPLE_SIZE 256  // Number of samples for the FFT
#define SAMPLE_CHANNEL 0  // Define ADC channel to use (P6.0)
#define SAMPLE_RATE 1000  // ADC sampling rate at 1000 samples per second

double noteFrequencies[6] = {82.0, 110.0, 147.0, 196.0, 247.0, 330.0};

//void configureADC(void);
//unsigned int readADC(void);
void initLED();

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	P6DIR &= ~0x01;  // set P6.0 as input
	P6SEL |= 0x01;  // Set P6.0 as analog input

	initLED();

	// call the configureADC function
	configureADC();

	// Infinite loop
	while(1) {
	    unsigned int adcValue = readADC();  // Read in the ADC value

	    if (adcValue > 3000) {
	        P2OUT |= 0x04;  // Turn on P2.2
	        P1OUT &= ~0x10;  // Turn off P1.4
	        P1OUT |= 0x20;  // Turn on P1.5
	        P2OUT &= ~0x10;  // Turn off P2.4
	        P2OUT |= 0x20;  // Turn on P2.5
	        P4OUT &= ~0x08;  // Turn off P4.3

	        // Flat/sharp LEDs
	        P2OUT |= 0x04;  // Turn on P2.3
	        P2OUT |= 0x40;  // Turn on P2.6
	        P3OUT |= 0x80;  // Turn on P3.7
	        P8OUT |= 0x02;  // Turn on P8.1
	        P8OUT |= 0x04;  // Turn on P8.2
	    }
	    else {
	        P2OUT &= ~0x04;  // Turn off P2.2
	        P1OUT |= 0x10;  // Turn on P1.4
	        P1OUT &= ~0x20;  // Turn off P1.5
	        P2OUT |= 0x10;  // Turn on P2.4
	        P2OUT &= ~0x20;  // Turn off P2.5
	        P4OUT |= 0x08;  // Turn on P4.3


	    }
	}

//	return 0;
}

void initLED() {
    // Tuned notes LEDs on P1.4, P1.5, P2.2, P2.4, P2.5, P4.3
    // Sharp and Flat LEDs on P2.3, P2.6, P3.7, P8.1, P8.2
    P1DIR |= 0x30;
    P2DIR |= 0x7C;
    P3DIR |= 0x80;
    P4DIR |= 0x08;
    P8DIR |= 0x06;

    // Set LEDs to off
    //P1OUT &= 0x38;
    //P2OUT &= 0x78;
    //P3OUT &= 0x80;
    //P4OUT &= 0x08;
    //P8OUT &= 0x06;
}

//void configureADC(void) {
    // Setup ADC12 for single-channel single-conversion mode
  //  ADC12CTL0 = ADC12SHT0_2 | ADC12ON;             // Enable ADC, set sample time
  //  ADC12CTL1 = ADC12CSTARTADD_0 | ADC12SHP;       // Use default start address, use sample and hold
  //  ADC12MCTL0 = ADC12INCH_0;                      // Select input channel (P6.0)
  //  ADC12CTL0 |= ADC12ENC;                         // Enable the ADC
//}

//unsigned int readADC(void) {
  //  ADC12CTL0 |= ADC12SC;  // Start the conversion
  //  while (ADC12CTL1 & ADC12BUSY);  // Wait for the conversion to finish
  //  return ADC12MEM0;  // Return the value of the ADC result
//}
