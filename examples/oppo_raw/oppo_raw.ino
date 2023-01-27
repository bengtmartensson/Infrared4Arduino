// This sketch can interactivelly send all the commands to an OPPO BDP-93
// BluRay player. It contains a large number of raw signals. To save RAM memory
// (which is a precious resource on "standard" Arduinos), the data is stored
// in the flash memory, "PROGMEM".

// The sketch is a simplified versions of the "Arduino/Raw" export from
// IrScrutinizer 1.2.

#include <IrSenderPwm.h>

#if ! HAS_FLASH_READ
#error This example will not run on your board, since it lacks the flash reading functions.
#endif

// Constants
static const long BAUD = 115200UL; // Change if desired

IrSender *irsend = IrSenderPwm::getInstance(true);

// Command #1: power_toggle
// Protocol: nec1, Parameters: D=73 F=26
const microseconds_t intro_power_toggle[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_power_toggle[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #2: open_close
// Protocol: nec1, Parameters: D=73 F=27
const microseconds_t intro_open_close[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_open_close[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #3: setup
// Protocol: nec1, Parameters: D=73 F=0
const microseconds_t intro_setup[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_setup[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #4: power_on
// Protocol: nec1, Parameters: D=73 F=90
const microseconds_t intro_power_on[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_power_on[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #5: power_off
// Protocol: nec1, Parameters: D=73 F=91
const microseconds_t intro_power_off[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_power_off[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #6: source
// Protocol: nec1, Parameters: D=73 F=17
const microseconds_t intro_source[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_source[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #7: play
// Protocol: nec1, Parameters: D=73 F=86
const microseconds_t intro_play[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_play[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #8: pause_toggle
// Protocol: nec1, Parameters: D=73 F=20
const microseconds_t intro_pause_toggle[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_pause_toggle[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #9: stop
// Protocol: nec1, Parameters: D=73 F=82
const microseconds_t intro_stop[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_stop[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #10: rewind
// Protocol: nec1, Parameters: D=73 F=85
const microseconds_t intro_rewind[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_rewind[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #11: fast_forward
// Protocol: nec1, Parameters: D=73 F=81
const microseconds_t intro_fast_forward[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_fast_forward[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #12: next
// Protocol: nec1, Parameters: D=73 F=80
const microseconds_t intro_next[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_next[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #13: previous
// Protocol: nec1, Parameters: D=73 F=84
const microseconds_t intro_previous[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_previous[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #14: a_b
// Protocol: nec1, Parameters: D=73 F=77
const microseconds_t intro_a_b[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_a_b[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #15: repeat
// Protocol: nec1, Parameters: D=73 F=76
const microseconds_t intro_repeat[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_repeat[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #16: title
// Protocol: nec1, Parameters: D=73 F=72
const microseconds_t intro_title[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_title[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #17: menu
// Protocol: nec1, Parameters: D=73 F=64
const microseconds_t intro_menu[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_menu[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #18: cmd_goto
// Protocol: nec1, Parameters: D=73 F=74
const microseconds_t intro_cmd_goto[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_cmd_goto[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #19: cmd_return
// Protocol: nec1, Parameters: D=73 F=66
const microseconds_t intro_cmd_return[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_cmd_return[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #20: cmd_0
// Protocol: nec1, Parameters: D=73 F=4
const microseconds_t intro_cmd_0[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_cmd_0[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #21: cmd_1
// Protocol: nec1, Parameters: D=73 F=11
const microseconds_t intro_cmd_1[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_cmd_1[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #22: cmd_2
// Protocol: nec1, Parameters: D=73 F=7
const microseconds_t intro_cmd_2[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_cmd_2[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #23: cmd_3
// Protocol: nec1, Parameters: D=73 F=3
const microseconds_t intro_cmd_3[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_cmd_3[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #24: cmd_4
// Protocol: nec1, Parameters: D=73 F=10
const microseconds_t intro_cmd_4[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_cmd_4[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #25: cmd_5
// Protocol: nec1, Parameters: D=73 F=6
const microseconds_t intro_cmd_5[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_cmd_5[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #26: cmd_6
// Protocol: nec1, Parameters: D=73 F=2
const microseconds_t intro_cmd_6[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_cmd_6[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #27: cmd_7
// Protocol: nec1, Parameters: D=73 F=9
const microseconds_t intro_cmd_7[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_cmd_7[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #28: cmd_8
// Protocol: nec1, Parameters: D=73 F=5
const microseconds_t intro_cmd_8[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_cmd_8[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #29: cmd_9
// Protocol: nec1, Parameters: D=73 F=1
const microseconds_t intro_cmd_9[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_cmd_9[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #30: cancel
// Protocol: nec1, Parameters: D=73 F=70
const microseconds_t intro_cancel[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_cancel[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #31: up
// Protocol: nec1, Parameters: D=73 F=24
const microseconds_t intro_up[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_up[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #32: down
// Protocol: nec1, Parameters: D=73 F=16
const microseconds_t intro_down[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_down[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #33: left
// Protocol: nec1, Parameters: D=73 F=87
const microseconds_t intro_left[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_left[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #34: right
// Protocol: nec1, Parameters: D=73 F=79
const microseconds_t intro_right[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_right[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #35: ok
// Protocol: nec1, Parameters: D=73 F=83
const microseconds_t intro_ok[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_ok[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #36: red
// Protocol: nec1, Parameters: D=73 F=28
const microseconds_t intro_red[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_red[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #37: green
// Protocol: nec1, Parameters: D=73 F=29
const microseconds_t intro_green[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_green[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #38: blue
// Protocol: nec1, Parameters: D=73 F=31
const microseconds_t intro_blue[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_blue[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #39: yellow
// Protocol: nec1, Parameters: D=73 F=30
const microseconds_t intro_yellow[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_yellow[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #40: subtitle
// Protocol: nec1, Parameters: D=73 F=75
const microseconds_t intro_subtitle[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_subtitle[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #41: audio
// Protocol: nec1, Parameters: D=73 F=71
const microseconds_t intro_audio[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_audio[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #42: angle
// Protocol: nec1, Parameters: D=73 F=73
const microseconds_t intro_angle[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_angle[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #43: zoom
// Protocol: nec1, Parameters: D=73 F=69
const microseconds_t intro_zoom[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_zoom[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #44: SAP
// Protocol: nec1, Parameters: D=73 F=18
const microseconds_t intro_SAP[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_SAP[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #45: PIP
// Protocol: nec1, Parameters: D=73 F=12
const microseconds_t intro_PIP[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_PIP[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #46: osd
// Protocol: nec1, Parameters: D=73 F=68
const microseconds_t intro_osd[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_osd[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #47: home
// Protocol: nec1, Parameters: D=73 F=92
const microseconds_t intro_home[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_home[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #48: dimmer
// Protocol: nec1, Parameters: D=73 F=95
const microseconds_t intro_dimmer[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_dimmer[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #49: light_on
// Protocol: nec1, Parameters: D=73 F=25
const microseconds_t intro_light_on[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_light_on[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #50: page_up
// Protocol: nec1, Parameters: D=73 F=94
const microseconds_t intro_page_up[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_page_up[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #51: page_down
// Protocol: nec1, Parameters: D=73 F=93
const microseconds_t intro_page_down[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_page_down[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #52: volume_up
// Protocol: nec1, Parameters: D=73 F=19
const microseconds_t intro_volume_up[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_volume_up[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #53: volume_down
// Protocol: nec1, Parameters: D=73 F=23
const microseconds_t intro_volume_down[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_volume_down[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #54: mute_toggle
// Protocol: nec1, Parameters: D=73 F=67
const microseconds_t intro_mute_toggle[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_mute_toggle[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #55: audio_only
// Protocol: nec1, Parameters: D=73 F=65
const microseconds_t intro_audio_only[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_audio_only[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #56: pal_ntsc
// Protocol: nec1, Parameters: D=73 F=22
const microseconds_t intro_pal_ntsc[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_pal_ntsc[] PROGMEM = { 9024U, 2256U, 564U, 65535U };

// Command #57: resolution
// Protocol: nec1, Parameters: D=73 F=21
const microseconds_t intro_resolution[] PROGMEM = { 9024U, 4512U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 564U, 564U, 1692U, 564U, 1692U, 564U, 1692U, 564U, 39756 };
const microseconds_t repeat_resolution[] PROGMEM = { 9024U, 2256U, 564U, 65535U };


static void sendRaw(const microseconds_t intro_P[], size_t lengthIntro, const microseconds_t repeat_P[],
             size_t lengthRepeat, frequency_t frequency, unsigned times) {
    IrSequence* intro = IrSequence::readFlash(intro_P, lengthIntro);
    IrSequence* repeat = IrSequence::readFlash(repeat_P, lengthRepeat);
    IrSequence* ending = new IrSequence();
    IrSignal irSignal(*intro, *repeat, *ending, frequency);
    irsend->sendIrSignal(irSignal, times);
    delete intro;
    delete repeat;
    delete ending;
}

void setup() {
    Serial.begin(BAUD);
    Serial.setTimeout(60000UL);
}

// A pretty silly main loop; just intended as an example.
void loop() {
    Serial.println(F("Enter number of signal to send (1 .. 57)"));
    long commandno = Serial.parseInt();
    Serial.println(F("Enter number of times to send it"));
    long times = Serial.parseInt();
    switch (commandno) {
    case 1U:
        sendRaw(intro_power_toggle, 68U, repeat_power_toggle, 4U, 38400U, times);
        break;
    case 2U:
        sendRaw(intro_open_close, 68U, repeat_open_close, 4U, 38400U, times);
        break;
    case 3U:
        sendRaw(intro_setup, 68U, repeat_setup, 4U, 38400U, times);
        break;
    case 4U:
        sendRaw(intro_power_on, 68U, repeat_power_on, 4U, 38400U, times);
        break;
    case 5U:
        sendRaw(intro_power_off, 68U, repeat_power_off, 4U, 38400U, times);
        break;
    case 6U:
        sendRaw(intro_source, 68U, repeat_source, 4U, 38400U, times);
        break;
    case 7U:
        sendRaw(intro_play, 68U, repeat_play, 4U, 38400U, times);
        break;
    case 8U:
        sendRaw(intro_pause_toggle, 68U, repeat_pause_toggle, 4U, 38400U, times);
        break;
    case 9U:
        sendRaw(intro_stop, 68U, repeat_stop, 4U, 38400U, times);
        break;
    case 10U:
        sendRaw(intro_rewind, 68U, repeat_rewind, 4U, 38400U, times);
        break;
    case 11U:
        sendRaw(intro_fast_forward, 68U, repeat_fast_forward, 4U, 38400U, times);
        break;
    case 12U:
        sendRaw(intro_next, 68U, repeat_next, 4U, 38400U, times);
        break;
    case 13U:
        sendRaw(intro_previous, 68U, repeat_previous, 4U, 38400U, times);
        break;
    case 14U:
        sendRaw(intro_a_b, 68U, repeat_a_b, 4U, 38400U, times);
        break;
    case 15U:
        sendRaw(intro_repeat, 68U, repeat_repeat, 4U, 38400U, times);
        break;
    case 16U:
        sendRaw(intro_title, 68U, repeat_title, 4U, 38400U, times);
        break;
    case 17U:
        sendRaw(intro_menu, 68U, repeat_menu, 4U, 38400U, times);
        break;
    case 18U:
        sendRaw(intro_cmd_goto, 68U, repeat_cmd_goto, 4U, 38400U, times);
        break;
    case 19U:
        sendRaw(intro_cmd_return, 68U, repeat_cmd_return, 4U, 38400U, times);
        break;
    case 20U:
        sendRaw(intro_cmd_0, 68U, repeat_cmd_0, 4U, 38400U, times);
        break;
    case 21U:
        sendRaw(intro_cmd_1, 68U, repeat_cmd_1, 4U, 38400U, times);
        break;
    case 22U:
        sendRaw(intro_cmd_2, 68U, repeat_cmd_2, 4U, 38400U, times);
        break;
    case 23U:
        sendRaw(intro_cmd_3, 68U, repeat_cmd_3, 4U, 38400U, times);
        break;
    case 24U:
        sendRaw(intro_cmd_4, 68U, repeat_cmd_4, 4U, 38400U, times);
        break;
    case 25U:
        sendRaw(intro_cmd_5, 68U, repeat_cmd_5, 4U, 38400U, times);
        break;
    case 26U:
        sendRaw(intro_cmd_6, 68U, repeat_cmd_6, 4U, 38400U, times);
        break;
    case 27U:
        sendRaw(intro_cmd_7, 68U, repeat_cmd_7, 4U, 38400U, times);
        break;
    case 28U:
        sendRaw(intro_cmd_8, 68U, repeat_cmd_8, 4U, 38400U, times);
        break;
    case 29U:
        sendRaw(intro_cmd_9, 68U, repeat_cmd_9, 4U, 38400U, times);
        break;
    case 30U:
        sendRaw(intro_cancel, 68U, repeat_cancel, 4U, 38400U, times);
        break;
    case 31U:
        sendRaw(intro_up, 68U, repeat_up, 4U, 38400U, times);
        break;
    case 32U:
        sendRaw(intro_down, 68U, repeat_down, 4U, 38400U, times);
        break;
    case 33U:
        sendRaw(intro_left, 68U, repeat_left, 4U, 38400U, times);
        break;
    case 34U:
        sendRaw(intro_right, 68U, repeat_right, 4U, 38400U, times);
        break;
    case 35U:
        sendRaw(intro_ok, 68U, repeat_ok, 4U, 38400U, times);
        break;
    case 36U:
        sendRaw(intro_red, 68U, repeat_red, 4U, 38400U, times);
        break;
    case 37U:
        sendRaw(intro_green, 68U, repeat_green, 4U, 38400U, times);
        break;
    case 38U:
        sendRaw(intro_blue, 68U, repeat_blue, 4U, 38400U, times);
        break;
    case 39U:
        sendRaw(intro_yellow, 68U, repeat_yellow, 4U, 38400U, times);
        break;
    case 40U:
        sendRaw(intro_subtitle, 68U, repeat_subtitle, 4U, 38400U, times);
        break;
    case 41U:
        sendRaw(intro_audio, 68U, repeat_audio, 4U, 38400U, times);
        break;
    case 42U:
        sendRaw(intro_angle, 68U, repeat_angle, 4U, 38400U, times);
        break;
    case 43U:
        sendRaw(intro_zoom, 68U, repeat_zoom, 4U, 38400U, times);
        break;
    case 44U:
        sendRaw(intro_SAP, 68U, repeat_SAP, 4U, 38400U, times);
        break;
    case 45U:
        sendRaw(intro_PIP, 68U, repeat_PIP, 4U, 38400U, times);
        break;
    case 46U:
        sendRaw(intro_osd, 68U, repeat_osd, 4U, 38400U, times);
        break;
    case 47U:
        sendRaw(intro_home, 68U, repeat_home, 4U, 38400U, times);
        break;
    case 48U:
        sendRaw(intro_dimmer, 68U, repeat_dimmer, 4U, 38400U, times);
        break;
    case 49U:
        sendRaw(intro_light_on, 68U, repeat_light_on, 4U, 38400U, times);
        break;
    case 50U:
        sendRaw(intro_page_up, 68U, repeat_page_up, 4U, 38400U, times);
        break;
    case 51U:
        sendRaw(intro_page_down, 68U, repeat_page_down, 4U, 38400U, times);
        break;
    case 52U:
        sendRaw(intro_volume_up, 68U, repeat_volume_up, 4U, 38400U, times);
        break;
    case 53U:
        sendRaw(intro_volume_down, 68U, repeat_volume_down, 4U, 38400U, times);
        break;
    case 54U:
        sendRaw(intro_mute_toggle, 68U, repeat_mute_toggle, 4U, 38400U, times);
        break;
    case 55U:
        sendRaw(intro_audio_only, 68U, repeat_audio_only, 4U, 38400U, times);
        break;
    case 56U:
        sendRaw(intro_pal_ntsc, 68U, repeat_pal_ntsc, 4U, 38400U, times);
        break;
    case 57U:
        sendRaw(intro_resolution, 68U, repeat_resolution, 4U, 38400U, times);
        break;
    default:
        Serial.println(F("Invalid number entered, try again"));
        break;
    }
}
