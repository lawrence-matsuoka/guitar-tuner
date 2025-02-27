#include <msp430.h> 


/**
 * main.c
 */

void configureADC(void);
unsigned int readADC(void);

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	P6DIR &= ~0x01;  // set P6.0 as input
	P6SEL |= 0x01;  // Set P6.0 as analog input

	// Feedback LED on P1.3, P1.4, P1.5, P2.4, P2.5, P4.3
	P1DIR |= 0x38;
	P2DIR |= 0x30;
	P4DIR |= 0x08;

	// call the configureADC function
	configureADC();

	// Infinite loop
	while(1) {
	    unsigned int adcValue = readADC();  // Read in the ADC value

	    if (adcValue > 3000) {
	        P1OUT |= 0x08;  // Turn on P1.3
	        P1OUT &= ~0x10;  // Turn off P1.4
	        P1OUT |= 0x20;  // Turn on P1.5
	        P2OUT &= ~0x10;  // Turn off P2.4
	        P2OUT |= 0x20;  // Turn on P2.5
	        P4OUT &= ~0x08;  // Turn off P4.3
	    }
	    else {
	        P1OUT &= ~0x08;  // Turn off P1.3
	        P1OUT |= 0x10;  // Turn on P1.4
	        P1OUT &= ~0x20;  // Turn off P1.5
	        P2OUT |= 0x10;  // Turn on P2.4
	        P2OUT &= ~0x20;  // Turn off P2.5
	        P4OUT |= 0x08;  // Turn on P4.3
	    }
	}

	return 0;
}

void configureADC(void) {
    // Setup ADC12 for single-channel single-conversion mode
    ADC12CTL0 = ADC12SHT0_2 | ADC12ON;             // Enable ADC, set sample time
    ADC12CTL1 = ADC12CSTARTADD_0 | ADC12SHP;       // Use default start address, use sample and hold
    ADC12MCTL0 = ADC12INCH_0;                      // Select input channel (P6.0)
    ADC12CTL0 |= ADC12ENC;                         // Enable the ADC
}

unsigned int readADC(void) {
    ADC12CTL0 |= ADC12SC;  // Start the conversion
    while (ADC12CTL1 & ADC12BUSY);  // Wait for the conversion to finish
    return ADC12MEM0;  // Return the value of the ADC result
}
