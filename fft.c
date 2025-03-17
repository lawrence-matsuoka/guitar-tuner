#include "fft.h"
#include <math.h>
// #include <complex.h>

#define pi 3.14159265358979323846

// FFT function
void fft(volatile unsigned int *samples, unsigned int indexSamples,
         double *frequency) {
  unsigned int i, j, k;
  double real, imaginary;
  double realVal[indexSamples], imaginaryVal[indexSamples];

  //
  for (i = 0; i < indexSamples; i++) {
    realVal[i] = (double)samples[i];
    imaginaryVal[i] = 0.0;
  }

  // FFT algorithm
  for (i = 0; i < log2(indexSamples); i++) {
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
        realVal[j + k + stepSize / 2] = evenImaginary - imaginary;
      }
    }
  }

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

  *frequency = (indexPeak * 44100.0) / indexSamples;
}
