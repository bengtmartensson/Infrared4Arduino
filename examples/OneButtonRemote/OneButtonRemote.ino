// Requires a switch connected between the digital input pin "button" and ground.
// Sends a Yamaha volume down (NEC1 D=122, F=27) as long as the button is held
// down. Due to the nature of NEC1, first the payload is sent once, then the simple
// repeat signal (many times).

// Button de-bouncing is not really necessary.

#include <Nec1Renderer.h>
#include <IrSenderPwm.h>

// where the switch is connected
const pin_t button = 12;

const IrSignal *irSignal;
IrSender *irSender;

void setup() {
    // Define switch as input, using pullup to Vcc
    pinMode(button, INPUT_PULLUP);
    
    // Set up the sender
    irSender = IrSenderPwm::getInstance(true);
    
    // Set up the signal
    irSignal = Nec1Renderer::newIrSignal(122, 27); // volume_down for Yahama receivers
}

void loop() {
    if (digitalRead(button) == LOW) {
        // Button is pressed, send
        
        // Send the main signal,...
        irSender->sendIrSignal(*irSignal);
        
        // ... then, for as long as the button is held,
        // send the repeat sequence
        while (digitalRead(button) == LOW) {
            irSender->send(irSignal->getRepeat());
            delay(30); // Make up for deficiency in implementation
        }
    } else {
        // Button is not pressed, do nothing.
    }
}
