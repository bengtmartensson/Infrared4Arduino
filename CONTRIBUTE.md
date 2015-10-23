## Types
There are some project specific data typedefs in Infrared.h.
For durations in microseconds, the data type microseconds_t is to be
used. If desired/necessary, this can be either uint16_t or
uint32_t. For durations in milliseconds, use the type
millisecons_t. Likewise, use frequency_t for modulation frequency in
Hz.

Use implementation dependent types like "int" if and _only if_ it is OK for the
compiler to select any implementation allowed by the C++ language.

Use unsigned int for sizes that can "impossibly" be larger than 65535,
like signal lengths etc.

## Error handling
Simple answer: there is none. If you send a function silly data, it just silently ignores the request.
This (unfortunately) seems to be the standard procedure in Arduino programming.

I am used to exception based error handling, for some reason this is not used by
the Arduino community.

Constructive suggestions are welcome.
