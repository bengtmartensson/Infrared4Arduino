# MicroGirs

This "example" implements a [Girs](http://www.harctoolbox.org/Girs.html) server for the Arduino.
It can be used with [IrScrutinizer](https://github.com/bengtmartensson/harctoolboxbundle),
and [Lirc](http://lirc.org) (using the [Girs driver](http://lirc.org/html/girs.html)).
It is an interactive server that can send and receive IR signals, with some bells and whistles.
The interactive server is mainly meant for interact with other programs. communicating over a serial line, likely in USB disguise.

MicroGirs is essentially a simplified version of [AGirs](https://github.com/bengtmartensson/AGirs),
stripped of features seldomly used (LCD and (visible-light) LED support, Ethernet, etc). As opposed to
AGirs, it does not depend on other libraries. For more documentation, see the AGirs project.

There are a number of configuration options. These are all contained in the
file `config.h` and consists of CPP `#defines`.

MicroGirs is essentially functionally equivalent to "GirsLite".
