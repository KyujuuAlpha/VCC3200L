# Virtual CC3200 Library (VCC3200L)
A simple library that allows for a virtualization of the CC3200 Launchpad.
In development, so not all features have been added as of yet.

Depends on pthreads and GTK 3.0

VERY IMPORTANT NOTE: This is still early in development and so far only LEDs, switches, and UART works.  In the near future, will be able to change directives in a config.h to enable other peripherals as well (SPI, Adafruit display, etc.)

To compile, first make sure GTK 3 is installed and run: 
```
cmake .
```
After cmake generates a Makefile run:
```
make
```
And a static library should be generated in the lib directory

Use the generated library as a normal static library, but don't forget to include the headers, GTK-3.0, and libpthreads when compiling your program.