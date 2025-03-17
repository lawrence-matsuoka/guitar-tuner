#include "fft.h" // import the Fast Fourier Transform library
#include <math.h>
#include <msp430.h>

#define SAMPLE_SIZE 256  // Number of samples for the FFT
#define SAMPLE_CHANNEL 0 // Define ADC channel to use (P6.0)
#define SAMPLE_RATE 1000 // ADC sampling rate at 1000 samples per second

// Define the appropriate frequencies for each string of a tuned guitar
double noteFrequencies[6] = {82.0, 110.0, 147.0, 196.0, 247.0, 330.0};

void initLED(void);
void initADC(void);
void tuneFrequency(double frequency);

int main(void) {
  // Stop the watchdog timer
  WDTCTL = WDTPW | WDTHOLD;

  // Initialize the LEDs and ADC
  initLED();
  initADC();

  // Infinite loop that samples audio data into an array and performs the FFT
  while (1) {

    P2OUT |= 0x04;  // Turn on P2.2
    P1OUT &= ~0x10; // Turn off P1.4
    P1OUT |= 0x20;  // Turn on P1.5
    P2OUT &= ~0x10; // Turn off P2.4
    P2OUT |= 0x20;  // Turn on P2.5
    P4OUT &= ~0x08; // Turn off P4.3

    _delay_cycles(1000000); // temp delay function

    P2OUT &= ~0x04; // Turn off P2.2
    P1OUT |= 0x10;  // Turn on P1.4
    P1OUT &= ~0x20; // Turn off P1.5
    P2OUT |= 0x10;  // Turn on P2.4
    P2OUT &= ~0x20; // Turn off P2.5
    P4OUT |= 0x08;  // Turn on P4.3

    _delay_cycles(1000000); // temp delay function
  }
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
  // P1OUT &= 0x30;
  // P2OUT &= 0x7C;
  // P3OUT &= 0x80;
  // P4OUT &= 0x08;
  // P8OUT &= 0x06;
}

void initADC(void) {
  // Disable the ADC to avoid conflicts during configuration
  ADC12CTL0 = ADC12CTL0 & ~ADC12ENC;

  ADC12CTL0 =
      ADC12SHT0_8 | ADC12ON; // Set sampling time to 512 clocks and turn ADC on
  ADC12CTL1 =
      ADC12CSTARTADD_0 |
      ADC12SHP; // Single channel, single conversion, and SHP for internal clock
  ADC12CTL2 = ADC12RES_2; // 12-bit resolution

  P6DIR &= ~0x01; // set P6.0 as input
  P6SEL |= 0x01;  // Set P6.0 as analog input

  //    ADC12IE0 = 0x01;  // Enable P6.0 interrupt

  ADC12CTL0 |= ADC12ENC | ADC12SC; // Turn on ADC12
}

void tuneFrequency(double frequency) {
  unsigned int i;
  double nearBound = 2.0; // Frequency accuracy for slightly flat/sharp
  double farBound = 5.0;  // Frequency accuracy for very flat/sharp

  // Reset the "tuning" LEDs,
  P2OUT &= 0x04; // Turn off P2.3, "very sharp"
  P2OUT &= 0x40; // Turn off P2.6, "slightly sharp"
  P3OUT &= 0x80; // Turn off P3.7, "in-tune"
  P8OUT &= 0x02; // Turn off P8.1, "slightly flat"
  P8OUT &= 0x04; // Turn off P8.2, "very flat"

  // Determine how flat or sharp each frequency is by iterating through
  for (i = 0; i < 6; i++) {
    if (frequency >= (noteFrequencies[i] - farBound) &&
        frequency <= (noteFrequencies[i] + farBound)) {
      // Very flat
      if (frequency < (noteFrequencies[i] - farBound)) {
        P8OUT |= 0x04; // Turn on the "very flat" LED
      } else if (frequency >= (noteFrequencies[i] - farBound) &&
                 frequency < (noteFrequencies[i] - nearBound)) {
        P8OUT |= 0x02; // Turn on the "slightly flat" LED
      } else if (frequency >= (noteFrequencies[i] - nearBound) &&
                 frequency <= (noteFrequencies[i] + nearBound)) {
        P3OUT |= 0x80; // Turn on the "in-tune" LED
      } else if (frequency > (noteFrequencies[i] + nearBound) &&
                 frequency <= (noteFrequencies[i] + farBound)) {
        P2OUT |= 0x40; // Turn on the "slightly sharp" LED
      } else if (frequency > (noteFrequencies[i] + farBound)) {
        P2OUT |= 0x04; // Turn on the "very sharp" LED
      }
    }
  }
}
