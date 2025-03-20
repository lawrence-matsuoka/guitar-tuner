#include <msp430.h>
#include <stdio.h>

void uart_init(void) {
  P3SEL |= 0x18;                 // P3.3, P3.4 = USCI_A0 TXD/RXD
  UCA0CTL1 |= UCSWRST;           // **Put state machine in reset**
  UCA0CTL1 |= UCSSEL_2;          // SMCLK
  UCA0BR0 = 9;                   // 1MHz w/ baud rate of 115200
  UCA0BR1 = 0;                   // 1MHz w/ baud rate of 115200
  UCA0MCTL |= UCBRS_1 + UCBRF_0; // Modulation UCBRSx=1, UCBRFx=0
  UCA0CTL1 &= ~UCSWRST;
}

void uart_send_char(char c) {
  UCA0TXBUF = c;
  while (UCA0STAT & UCBUSY)
    ;
}

void uart_send_string(char *c) {
  while (*c) {
    UCA0TXBUF = *c;
    while (UCA0STAT & UCBUSY)
      ;
    c++;
  }
}

void uart_send_int(int value) {
  char buffer[10];
  sprintf(buffer, "%d", value);
  uart_send_string(buffer);
}
