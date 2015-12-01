# Infrared4Arduino
This is yet another infrared library for the Arduino. (Although its name does not
start with YA...)

It is a major rewrite of [Chris Young's
IRLib](http://tech.cyborg5.com/irlib/,
https://github.com/cyborg5/IRLib), which itself is a major
rewrite of a library called IRremote, published by
Ken Shirriff in [his blog](http://www.righto.com/2009/08/multi-protocol-infrared-remote-library.html),
now maintained [here](https://github.com/z3t0/Arduino-IRremote).
It uses Michael Dreher's
IrWidget [(article in
German)](http://www.mikrocontroller.net/articles/High-Speed_capture_mit_ATmega_Timer),
see also [this forum
contribution](http://www.hifi-remote.com/forums/viewtopic.php?p=111876#111876).


The classes `IrWidget` and `IrWidgetAggregating` are based on Michael's code.
The classes `IrReceiverSampler` and `IrSenderPwm`, and in particular the file `IRLibTimer.h`,
are adapted from Kevin's and Chris' work. The remaining files are almost completely written
from scratch, although the influence of Kevin and Chris is gratefully acknowledged.

## Class construction
For some receiving/transmitting classes, multiple instantiations is not sensible,
for other it may be. In this library, the classes that should only be instantiated
once are implemented as singleton classes, i.e. with no public constructor, but instead
a static factory method (`new*()`) that delivers a pointer to a newly constructed instance,
provided that it has not been instantiated before. The classes, where multiple instances
is sensible, come with public constructors. (However, the user may still have to take responsibility
for avoiding pin- and timer-conflicts.)

## Hardware configuration
At the time of this writing, The boards Uno/Nano (ATmega328P), Leonardo/Micro (ATmega32U4),
and Mega2560 (ATmega2560) are supported.

Several of the sending and receiving classes take a GPIO pin as argument to the constructor.
However, the sending pin of `IrSenderPwm` and the capture pin of `IrWidgetAggregating`
are not configurable, but (due to hardware limitations) have to be taken from the following table:

                              Sender Pin      Capture Pin
    Uno/Nano (ATmega328P)          3             8
    Leonardo/Micro (ATmega32U4)    9             4
    Mega2560 (ATmega2560)          9            49


## User parameters
As opposed to other infrared libraries, there are no user changeable parameters as CPP symbols.
However, the timer
configuration is compiled in, depending on the CPP processors given to the compiler, see
the file `IRLibTimer.h`.

## Protocols
Comparing with the predecessor works, this project may look meager, currently supporting only
two protocols (NEC1 and RC5). It is [planned](https://github.com/bengtmartensson/IrpTransmogrifier)
to generate the corresponding C++ code automatically from the IRP notation. (For this reason,
contributed implementations of more protocols are not solicited.)

## Sending non-modulated signals.
RF signals (433 MHz and other carrier frequencies) do not use the IR
typical modulation. Also there are a few IR protocols (like [Revox, Barco,
Archer](http://www.hifi-remote.com/forums/viewtopic.php?t=14186&start=40))
not using modulation. These signals can be sent by the class `IrSenderNonMod`,
after connecting suitable hardware capable of sending non-modulated (IR- _or_ RF-) signals
to the GPIO pin given as argument to the constructor.

## Dependencies

This library does not depend on any other libraries; only the standard Arduino environment.

## Questions and answers

* What is the difference between the `IrReceiver*` and the `IrWidget*` classes?

They are intended for two different use cases,
[receiving](http://www.harctoolbox.org/Glossary.html#ReceivingIrSignals)
and [capturing](http://www.harctoolbox.org/Glossary.html#Capturing).
Differently put, "receive" uses a
demodulating receiver (TSOPxxx, etc.),
"capture" a non-demodulating decoder (TSMPxxx, OPLxxx QSExxx, etc.). Note that
this terminology is not universally accepted (yet!).

# Coding style

## General
My goal is to write excellent code, even though I do not always succeed :-).
Cleanliness, logical structure, readability and maintainability are the most important
requirements. Efficiency (runtime and/or space) is also important, although it normally
comes on second place. [The Arduino Style Guide](https://www.arduino.cc/en/Reference/APIStyleGuide)
has different goals (essentially optimizing for novice programmers). I therefore
do not recognize it.

## Documentation
The main documentation for the classes is found in the source files themselves. It can be
extracted to a browse-able documentation using the program [Doxygen](http://www.doxygen.org).
After installing the program, fire up the program in the source directory. It will generate documentation in
a subdirectory `html`. To browse, open `html/index.html` in a browser.

## Files
As opposed to the predecessor projects, this project has a header (`*.h`) file and an implementation file
(`*.cpp`, sometimes missing) for each public class.

## Types
There are some project specific data typedefs in `InfraredTypes.h`.
For durations in microseconds, the data type `microseconds_t` is to be
used. If desired/necessary, this can be either `uint16_t` or
`uint32_t`. For durations in milliseconds, use the type
`millisecons_t`. Likewise, use `frequency_t` for modulation frequency in
Hz.

For "sizes", use `size_t`, which is a standard C type.

Use implementation dependent types like `int` if and _only if_ it is OK for the
compiler to select any implementation allowed by the C++ language.

Use `unsigned int` for quantities that can "impossibly" be larger than 65535.

## Error handling
Simple answer: there is none. If a function is sent erroneous data,
it just silently ignores the request, or does something else instead.
This (unfortunately) seems to be the standard procedure in Arduino programming.

I am used to exception based error handling, for some reason this is not used by
the Arduino community.

Constructive suggestions are welcome.

# Multi platform coding
For someone used to, e.g., Netbeans or Eclipse, the Arduino IDE
feels "somewhat" primitive and limited. In particular, it does not support debugging. Mainly
for this reason, the code in the present library is designed to compile, and at least
to some extent, run in a normal C++ environment on the host compiler. For this,
some code modifications, in particular, a customized `Arduino.h` was needed.
If the preprocessor symbol `ARDUINO` is defined,
it just includes the standard Arduino `Arduino.h`, otherwise (i.e. for compiling for the host),
it defines some more-or-less dummy stuff for allowing compiling for, and execution/debugging
on the host.

This way, certain types of problems can be solved much faster. The drawback is that the code
is "polluted" with ugly `#ifdef ARDUINO` statements, which decreases readability and
makes maintenance harder.

The subdirectory `tests` contains test(s) that run on the host. The supplied `Makefile`
is intended for compiling for the host as target. It creates a library in the
standard sense (`*.a`), and can be used to build and run tests in subdirectory `tests`.
Tested on Linux only.

With the provided `Doxyfile`, Doxygen will document only the (strict) Arduino parts,
not the "portable C++".

# License
The entire work is licensed under the GPL3 license. Chris' as well as Ken's
code is licensed under the LGPL 2.1-license. Michael's code carries the
GPL2-license, although he is [willing to agree to "or later
versions"](http://www.hifi-remote.com/forums/viewtopic.php?p=112586#112586).
