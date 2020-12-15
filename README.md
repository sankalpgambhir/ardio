# ardio

An Arduino based implementation of an optimized approximate Fourier transform technique for use in embedded systems.

An audio sample is passed to the arduino, currently via a serial input, easily configurable to be a true mic instead. 
In response, the serial line receives a list a detected frequencies in the sample.

Uses < 2K RAM as configured, limited by the Arduino Uno. Scales logarithmically with resolution, granting great flexibility for 
even marginally faster systems.

Copyright 2020, Pushkar Mohile and Sankalp Gambhir.
