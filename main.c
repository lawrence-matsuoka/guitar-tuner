#include "fft.h" // import the Fast Fourier Transform library
#include "uart.h" // import the UART library
#include "adc.h" // import the ADC library
#include <math.h>
#include <msp430.h>

#define GUITAR_STRINGS 6
#define SAMPLE_SIZE 256  //256 Number of samples for the FFT
#define SAMPLE_CHANNEL 0 // Define ADC channel to use (P6.0)
#define SAMPLE_RATE 1000 //1000 ADC sampling rate at 1000 samples per second

// Define the appropriate frequencies for each string of a tuned guitar
double noteFrequencies[GUITAR_STRINGS] = {82.0,  110.0, 147.0,
                                          196.0, 247.0, 330.0};
volatile unsigned char tunedString[GUITAR_STRINGS] = {
    0, 0, 0, 0, 0, 0}; // Set all strings to out-of-tune

void led_init(void);
void led_tuned(void);
int tuneFrequency(double frequency, double targetFrequency, double nearBound,
                  double farBound, unsigned int indexFrequency);

int main(void) {
  // FOR TESTING PURPOSES
  uart_send_string("main\n");

  // Stop the watchdog timer
  WDTCTL = WDTPW | WDTHOLD;

  // Initialize the LEDs and ADC
  led_init();
  adc_init();

  // Initialize UART
  uart_init();

  // Array for audio data samples
  unsigned int samples[SAMPLE_SIZE];
  double frequency;
  // Define variables for in-tune, slightly flat/sharp, and very flat/sharp
  double boundTuned = 2.0;    // Slightly flat/sharp
  double boundNotTuned = 5.0; // Very flat/sharp

  // Infinite loop that samples audio data into an array and performs the FFT
  while (1) {
    // FOR TESTING PURPOSES
    uart_send_string("Loop\n");

    // Collect audio samples
    adc_sampler(samples, SAMPLE_SIZE);

    // Perform FFT
    //fft(samples, SAMPLE_SIZE, &frequency, SAMPLE_RATE);

    // FOR TESTING PURPOSES
    //uart_send_int(frequency);

    // Tune each string based on the calculated frequency
    //unsigned int i;

    //for (i = 0; i < GUITAR_STRINGS; i++) {
    //  int tuned = tuneFrequency(frequency, noteFrequencies[i], boundTuned,
    //                            boundNotTuned, i);
    //  if (tuned == 1) {
    //    led_tuned();
    //  }
    //}
  }
}

void led_init() {
  // Tuned notes LEDs on P1.4, P1.5, P2.2, P2.4, P2.5, P4.3
  // Sharp and Flat LEDs on P2.3, P2.6, P3.7, P8.1, P8.2
  P1DIR |= 0x30;
  P2DIR |= 0x7C;
  P3DIR |= 0x80;
  P4DIR |= 0x08;
  P8DIR |= 0x06;

  // Set LEDs to off
  P1OUT &= 0x00;
  P2OUT &= 0x00;
  P3OUT &= 0x00;
  P4OUT &= 0x00;
  P8OUT &= 0x00;

  // FOR TESTING PURPOSES
  uart_send_string("led_init\n");
}

void led_tuned(void) {

  // FOR TESTING PURPOSES
    uart_send_string("led_tuned\n");

  unsigned int i;
  for (i = 0; i < GUITAR_STRINGS; i++) {
    if (tunedString[i] == 1) {
      switch (i) {
      case 0:
        P4OUT |= 0x08;
        break; // P4.3 = E2
      case 1:
        P1OUT |= 0x10;
        break; // P1.4 = A2
      case 2:
        P1OUT |= 0x20;
        break; // P1.5 = D3
      case 3:
        P2OUT |= 0x04;
        break; // P2.2 = G3
      case 4:
        P2OUT |= 0x10;
        break; // P2.4 = B3
      case 5:
        P2OUT |= 0x20;
        break; // P2.5 = E4
      default:
        break;
      }
    }
  }
}

int tuneFrequency(double frequency, double targetFrequency, double nearBound,
                  double farBound, unsigned int indexFrequency) {

  // FOR TESTING PURPOSES
    uart_send_string("tuneFrequency\n");

  if (frequency >= (targetFrequency - farBound) &&
      frequency <= (targetFrequency + farBound)) {
    // In-tune
    if (frequency >= (targetFrequency - nearBound) &&
        frequency <= (targetFrequency + nearBound)) {
      // Mark the string as tuned
      tunedString[indexFrequency] = 1;
      P3OUT |= 0x80; // Turn on the "in-tune" LED
      return 1;      // Return 1 to indicate that the string is tuned
    }
    // Very flat or sharp
    else if (frequency < (targetFrequency - farBound)) {
      P8OUT |= 0x04; // Very flat
    } else if (frequency > (targetFrequency + farBound)) {
      P2OUT |= 0x04; // Very sharp
    }
    // Slightly flat or sharp
    else if (frequency >= (targetFrequency - nearBound) &&
             frequency < targetFrequency) {
      P8OUT |= 0x02; // Slightly flat
    } else if (frequency > targetFrequency &&
               frequency <= (targetFrequency + nearBound)) {
      P2OUT |= 0x40; // Slightly sharp
    }
  }
  return 0; // Return 0 if the string is not tuned
}

