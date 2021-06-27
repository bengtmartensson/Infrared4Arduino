# Infrared4Arduino
Infrared4Arduino is an object-oriented infrared library for the Arduino.

## NEWS

__Version 1.2.0__ constitutes a rather extensive refactoring, in particular regarding memory allocation.
It now uses some of the features of "modern C++" (restricted to C++-11 for Arduino compatibility)
(#16 and #17), like move semantics. For the API user, this means that the duration data
used to call the constructors of IrSequence and IrSignal are to be allocated using `new`,
and are "moved" into the constructed objects.  Also, MicroGirs has been removed.
(The user should use [AGirs](https://github.com/bengtmartensson/AGirs) instead.)

## Introduction
The library was (with some exceptions detailed below) written from scratch by myself.
It uses Michael Dreher's
IrWidget [(article in
German)](http://www.mikrocontroller.net/articles/High-Speed_capture_mit_ATmega_Timer),
see also [this forum
contribution](http://www.hifi-remote.com/forums/viewtopic.php?p=111876#111876).

The main inspiration for the present work was the well-known library called IRremote, published by
Ken Shirriff in [his blog](http://www.righto.com/2009/08/multi-protocol-infrared-remote-library.html),
now [hosted on GitHub](https://github.com/z3t0/Arduino-IRremote).
[Chris Young's IRLib](http://tech.cyborg5.com/irlib/), ([GitHub repo](https://github.com/cyborg5/IRLib)),
should also be mentioned as a source of inspiration, but not of code.
The SAMD21 support (Arduino Zero and others) is based on issues in the IRremote repository,
and in particular [this pull request](https://github.com/z3t0/Arduino-IRremote/pull/437).
I am also indebted to [this thread in the Arduino forum](https://forum.arduino.cc/index.php?topic=346731.0).
The ATMega4809 support (Arduino WiFi Rev 2 and Arduino Nano Every) is based upon
[this PR](https://github.com/z3t0/Arduino-IRremote/pull/657).
The Arduino Due support is basically taken from
[this project](https://github.com/enternoescape/Arduino-IRremote-Due),
which is licensed under the LGPL 2.1 license.
The Teensy 3.x support is based upon code in IRremote.
The ESP32 support is based upon [this pull request](https://github.com/z3t0/Arduino-IRremote/pull/680).

The classes `IrWidget` and `IrWidgetAggregating` are based on Michael's code.
The classes `IrReceiverSampler` and `IrSenderPwm`,
are adapted from Kevin's work. The remaining files are almost completely written
from scratch.

This work is a low-level library (like `IRremote` and `IRLib`) that can be used in other projects,
like [AGirs](https://github.com/bengtmartensson/AGirs), which is an high-level program taking commands interactively
from a user or a program through a bi-directional `Stream`.
The goal has been to provide a sound, object oriented basis for the fundamental basis, not
to provide maximal functionality, the maximal number of protocols supported, or the most complete support of
different hardware. A clean design and high readability, without being "too" inefficient,
has been the highest priority. Dynamic memory allocation with `new` and `delete` is used extensively.
The user who is afraid of this can create his required objects at the start of the run, and keep
them. Most classes are immutable. The classes are `const`-correct.

## Comparison with other infrared libraries
Here we list a (subjective!) selection of generic IR libraries for Arduino, with some (very personal) comments.

### [IRremote](https://github.com/Arduino-IRremote/Arduino-IRremote) by Ken Shirriff and others.
The original, that spawned all the rest! Now hosted [here](https://github.com/Arduino-IRremote/Arduino-IRremote/).
Was "stale" for several years, presently changes daily.
License: MIT.

### [IRlib version 1](https://github.com/cyborg5/IRLib) and [version 2](https://github.com/cyborg5/IRLib2) by Ken Shirriff and Chris Young.
Subjectively, this is a cleaned-up and modernized version of IRremote.
The basic structure is very similar, and it thus suffers from many of the basic
problems of IRremote.
It comes with a very ambitious PDF manual (125 pages!) entirely written by hand (not using a documentation system like Doxygen).
It supports most 8 bit AVRs and SAMDs, but not ESPs. The libraries are not available in the Arduino library manager,
however they are available on [PlatformIO](https://platformio.org/lib/show/1735/IRLib2).
Licenses: LGPL2.1 (IRLib), GPL3 (IRLib2).

### [IRremoteESP8266](https://github.com/crankyoldgit/IRremoteESP8266) by David Conran, Ken Shirriff, and others.
This library, as the name suggested, was created as a fork of IRremote, supporting ESP8266 and ESP32 only.
Has evolved considerably since the fork new features, in particular,
it also contains support for a large number of specialized protocols, like air conditioning protocols. License: LGPL2.1.

### [IRLremote](https://github.com/NicoHood/IRLremote) by Nico Hood.
This "lightweight" library is written with the aim of minimal foot print on very small systems, like the ATTiny.
License: MIT-like.

## Forum
Feel free to open [discussions](https://github.com/bengtmartensson/Infrared4Arduino/discussions)
and [issues at Github](https://github.com/bengtmartensson/Infrared4Arduino/issues) should
the need or desire arise. Other possible place to discuss are the
[Home automation](https://forum.arduino.cc/index.php?board=16.0) or the
[Sensor](https://forum.arduino.cc/index.php?board=10.0) sub-forums at the Arduino forum.

## Examples
A fairly large numbers of examples are provided. Most of these are a straight-forward
demonstration of a particular class, as indicated by the name.

# API
admin
[Up-to-date API, generated by Doxygen](http://bengtmartensson.github.io/Infrared4Arduino).

## Types
There are some project specific data typedefs in `InfraredTypes.h`.
For durations in microseconds, the data type `microseconds_t` is to be
used. If desired/necessary, this can be either `uint16_t` or
`uint32_t`. For durations in milliseconds, use the type
`millisecons_t`. Likewise, use `frequency_t` for modulation frequency in
Hz (_not_ kHz as in the IRremote/IRLib).

For "sizes", `size_t`, the standard C type, is used.

Implementation dependent types like `int` are used if and _only if_ it is OK for the
compiler to select any implementation allowed by the C++ language.

`unsigned int` is used for quantities that can "impossibly" be larger than 65535.
It is believed that (unsigned) int is the fastest type for doing arithmetics,
containing 16 bits or more.

## IrSequences and IrSignals
An `IrSequence` is a vector of durations, i.e. sequence of interleaving on-periods (sometimes called _marks_ or _flashes_)
and off-periods (sometimes called _spaces_ or _gaps_).
It does not contain the modulation frequence. As opposed to IRremote and IRLib, our sequences always start with
a on-period and end with an off-period. It is claimed to be a more relevant representation than the one of IRremote and IRLib.

An `IrSignal` consists of a modulation frequency and three `IrSequence`s: intro-, repeat-, and ending sequence. All of these, but not all, can be
empty. If repeat is empty, intro has to be non-empty and ending empty. The intro sequence is always sent first,
then comes a zero or more repeat sequences, and finally the ending sequence. To send a signal _n_ > 0 times shall mean
the following: If the intro  is non-empty, send intro, _n_ - 1 repeats, and then the ending. If the intro is empty,
send _n_ repeats, and then then ending.

## Signal data in flash memory.
The examples `oppo_cooked` and `oppo_raw` consist of [IrScrutinizer](https://github.com/bengtmartensson/harctoolboxbundle)
exports of the infrared command set of the Oppo Bluray players, in parametrized and raw form respectively.
The latter stores the raw signals in the flash area, "`PROGMEM`", (i.e. not taking up any (permanent) RAM storage)
for which the API support is required. This is unfortunately not available with all architectures.

## Class construction
For some receiving and transmitting classes, multiple instantiations are not sensible,
for others, it may be. In this library, the classes that should only be instantiated
once are implemented as singleton classes, i.e. with no public constructor, but instead
a static "factory" method (`newThing()`) that delivers a pointer to a newly constructed instance
of `Thing`,
provided that `Thing` has not been instantiated before. The classes, where multiple instances
is sensible, come with public constructors. (However, the user still has to take responsibility
for avoiding pin- and timer-conflicts.)

## Hardware configuration
Board specific hardware configuration is located in the class Board, and in the board specific files in `src/boards`.

However, IrWidgetAggregating is currently supported on the boards Uno/Nano (ATmega328P), Leonardo/Micro (ATmega32U4),
and Mega2560 (ATmega2560).

Several of the sending and receiving classes take a GPIO pin as argument to the constructor.
However, the sending pin of `IrSenderPwm` and the capture pin of `IrWidgetAggregating`
are not configurable, but (due to hardware limitations) have to be taken from the following table:

                                           Sender Pin      Capture Pin
    Uno/Nano (ATmega328P)                       3             8
    Leonardo/Micro (ATmega32U4)                 9             4
    Mega2560 (ATmega2560)                       9            49
    Due                                         7           n/a
    Uno WiFi Rev2/Nano Every (ATmega4809)       6           n/a
    Zero/M0 Pro/Nano Every IOT (ATSAMD21)       8           n/a
    Teensy 3.x                                  5           n/a
    ESP32                                       5           n/a

In some cases, this can be relatively easily changed in the code.
A later version of the library may support more elaborate configuration.

The pin used by the the receivers (not to be confused with capture pins) can
in general be selected freely, as long as it does not conflict with other
requirements.

### ESP8266
The ESP8266 is not a board very well suited for sending and receiving IR signals,
since it lacks important features, as documented in
[this issue](https://github.com/bengtmartensson/Infrared4Arduino/issues/5#issuecomment-589957060).
Only the generic IR sender IrSenderPwmSoftDelay and IrSenderPwmSpinWait as well as the generic
IR receiver IrReceiverPoll are available. For IR, please select another board if at all possible!

## Timeouts
All the receiving classes adhere to the following conventions: When initialized, it waits
up to the time `beginningTimeout` for the first on-period. If not received within that period,
it returns with a timeout. Otherwise, is starts collecting data. It will collect data until one of the
following occurs:
* A silence of length `endingTimeout` has been detected. This is the normal ending. The detected last gap is returned with the data.
* The buffer gets full. Reception stops.

## User parameters
As opposed to other infrared libraries, there are no user changeable parameters as CPP symbols.
However, the timer
configuration is compiled in, depending on the CPP processors given to the compiler, see
the file `IRremoteInt.h`.

## Files
As opposed to the predecessor projects, this project has a header (`*.h`) file and an implementation file
(`*.cpp`, sometimes missing) for each public class.

## Error handling
Simple answer: there is none. If a function is sent erroneous data,
it just silently ignores the request, or does something else instead.
This (unfortunately) seems to be the standard procedure in Arduino programming.

I am used to exception based error handling, for some reason this is not used by
the Arduino community.

Constructive suggestions are welcome.

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

## Hash codes
Hash-based decoding is implemented in the class HashDecoder. For an example of use,
see the example of LearningHashDecoder.

## Event driven reception
IRLib and IRremoteESP8266 implements interrupt driven reception, but currently
not this libary. It is [planned](https://github.com/bengtmartensson/Infrared4Arduino/issues/22).

# Blinking during receive.
This library does not, as opposed to some of its competitors, implement blinking.
I do not consider this to be a sensible method of debugging -- using a logical analyzer
together with Sigrok and Pulseview is a much better idea to do systematic debugging.
Also for decoration and other "cool" light effects, there are better alternatives.

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

# Cooperation
Your cooperation is solicited! Starting with testing, usage reports, bug reports and feedback
(both code and documentation), over enhancements suggestions, to code and documentation
contributions.

# Coding style

My goal is to write excellent code, even though I do not always succeed :-).
"Everything as simple as possible, but not simpler."
Cleanliness, logical structure, readability and maintainability are the most important
requirements. Efficiency (runtime and/or space) is also important, although it normally
comes on second place. [The Arduino Style Guide](https://www.arduino.cc/en/Reference/APIStyleGuide)
has different goals (essentially optimizing for novice programmers,
"Some of these run counter to professional programming practice"). It is therefore
not given priority in this project.

# Documentation
The main documentation for the classes is found in the source files themselves. It can be
extracted to a browse-able documentation using the program [Doxygen](http://www.doxygen.org).
After installing the program, fire up the program in the source directory. It will generate documentation in
a subdirectory `html`. To browse, open `html/index.html` in a browser.

The documentation is written for the _user_ of the library, not the developer.
For this reason, the file `Arduino.h` has been deliberately excluded from the documentation,
to keep it centered on the main issues for the programming on the target system.

# Multi platform coding
For someone used to, e.g., Netbeans or Eclipse, the Arduino IDE
feels "somewhat" primitive and limited. In particular, it does not support debugging. Mainly
for this reason, the code in the present library is designed to compile, and at least
to some extent, run in a normal C++ environment on the host compiler. For this,
some code modifications, in particular, a customized `Arduino.h` was needed.
If the preprocessor symbol `ARDUINO` is defined,
just includes the standard Arduino `Arduino.h` is included, otherwise (i.e. for compiling for the host),
some more-or-less dummy stuff are defined, allowing compiling for, and execution/debugging
on the host.

This way, certain types of problems can be solved much faster. The drawback is that the code
is "polluted" with ugly `#ifdef ARDUINO` statements, which decreases readability and
makes maintenance harder.

The subdirectory `tests` contains test(s) that run on the host. The supplied `Makefile`
is intended for compiling for the host as target. It creates a library in the
standard sense (`*.a`), and can be used to build and run tests in subdirectory `tests`.

With the provided `Doxyfile`, Doxygen will document only the (strict) Arduino parts,
not the "portable C++".

# License
The entire work is licensed under the GPL2 "or later" license. Ken's
code is licensed under the LGPL 2.1-license. Michael's code carries the
GPL2-license, although he is [willing to agree to "or later
versions"](http://www.hifi-remote.com/forums/viewtopic.php?p=112586#112586).
