#include <msp430.h>

void adc_init(void) {
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

void adc_sampler(unsigned int *samples, unsigned int sampleSize) {
  unsigned int i;

  for (i = 0; i < sampleSize; i++) {
    while (!(ADC12IFG & 0x01))
      ;                     // Wait for conversion to complete
    samples[i] = ADC12MEM0; // Store ADC value
    ADC12CTL0 |= ADC12SC;   // Trigger next conversion
  }
}
