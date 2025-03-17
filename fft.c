#include "fft.h"
#include <math.h>
// #include <complex.h>

#define pi 3.14159265358979323846

// Bit reversal function
unsigned int bitReversed(unsigned int num, unsigned int numBits) {
  unsigned int reversed = 0;
  for (unsigned int i = 0; i < numBits; i++) {
    reversed = (reversed << 1) | (num & 1);
    num >>= 1;
  }
  return reversed;
}

// FFT function
void fft(volatile unsigned int *samples, unsigned int indexSamples,
         double *frequency, double samplingRate) {
  unsigned int i, j, k;
  double real, imaginary;
  double realVal[indexSamples], imaginaryVal[indexSamples];

  // Initialize real and imaginary parts
  for (i = 0; i < indexSamples; i++) {
    realVal[i] = (double)samples[i];
    imaginaryVal[i] = 0.0;
  }

  // Bit-reversal reordering
  unsigned int numIterations = 0;
  while (indexSamples >>= 1) {
    numIterations++;
  }
  for (i = 0; i < indexSamples; i++) {
    unsigned int j = bitReversed(i, numIterations);
    if (i < j) {
      // Swap real and imaginary parts
      double tempReal = realVal[i];
      double tempImaginary = imaginaryVal[i];
      realVal[i] = realVal[j];
      imaginaryVal[i] = imaginaryVal[j];
      realVal[j] = tempReal;
      imaginaryVal[j] = tempImaginary;
    }
  }

  // FFT computation (Cooley-Tukey)
  for (i = 0; i < numIterations; i++) {
    unsigned int stepSize = 1 << (i + 1);
    double angle = 2 * pi / stepSize;

    for (j = 0; j < indexSamples; j += stepSize) {
      for (k = 0; k < (stepSize / 2); k++) {
        double cosineVal = cos(k * angle);
        double sinVal = sin(k * angle);
        double evenReal = realVal[j + k];
        double evenImaginary = imaginaryVal[j + k];
        double oddReal = realVal[j + k + stepSize / 2];
        double oddImaginary = imaginaryVal[j + k + stepSize / 2];

        real = cosineVal * oddReal - sinVal * oddImaginary;
        imaginary = sinVal * oddReal + cosineVal * oddImaginary;

        realVal[j + k] = evenReal + real;
        imaginaryVal[j + k] = evenImaginary + imaginary;
        realVal[j + k + stepSize / 2] = evenReal - real;
        imaginaryVal[j + k + stepSize / 2] = evenImaginary - imaginary;
      }
    }
  }

  // Find the peak frequency
  unsigned int indexPeak = 0;
  double amplitudeMax = 0.0;
  for (i = 0; i < (indexSamples / 2); i++) {
    double amplitude =
        sqrt(realVal[i] * realVal[i] + imaginaryVal[i] * imaginaryVal[i]);
    if (amplitude > amplitudeMax) {
      amplitudeMax = amplitude;
      indexPeak = i;
    }
  }

  *frequency = (indexPeak * samplingRate) / indexSamples;
}
