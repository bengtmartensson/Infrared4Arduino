// Requires a switch connected between the digital input pin "button" and ground.
// Sends a Yamaha volume down (NEC1 D=122, F=27) as long as the button is held
// down. Due to the nature of NEC1, first the payload is sent once, then the simple
// repeat signal (many times).

// Button de-bouncing is not really necessary.

#include <Nec1Renderer.h>
#include <IrSenderPwm.h>

// where the switch is connected
static constexpr pin_t button = 4U;

static const IrSignal *irSignal;
static const IrSender *irSender;

static bool buttonIsPressed() {
    return digitalRead(button) == LOW;
}

void setup() {
    // Define switch as input, using pullup to Vcc
    pinMode(button, INPUT_PULLUP);

    // Set up the sender
    irSender = IrSenderPwm::getInstance(true);

    // Set up the signal
    irSignal = Nec1Renderer::newIrSignal(122, 27); // volume_down for Yamaha receivers
}

void loop() {
    // Send the signal when and while button is pressed,
    // according to IrSender::sendWhile.
    irSender->sendWhile(*irSignal, buttonIsPressed);
}
