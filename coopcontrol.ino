// This #include statement was automatically added by the Particle IDE.
#include <RelayShield.h>

// Make our own state enumerator
enum State { open, closed };

// Create an instance of the RelayShield library, so we have something to talk to
RelayShield myRelays;

// Just assume it starts off closed. Probably should be unknown or something else
State currentState = closed;

// Millisecond of delay between relay controls
// I saw problems with the delay being too short and the particle rebooting. (Short circuit?)
#define DELAYMS 100

//STARTUP(WiFi.selectAntenna(ANT_INTERNAL)); // selects the CHIP antenna
STARTUP(WiFi.selectAntenna(ANT_EXTERNAL)); // selects the u.FL antenna
//STARTUP(WiFi.selectAntenna(ANT_AUTO)); // continually switches at high speed between antennas

void setup() {
    myRelays.begin();
    // Setup too many API function hooks
    Particle.function("open",openDoor);
    Particle.function("OPEN",openDoor);
    Particle.function("close",closeDoor);
    Particle.function("CLOSE",closeDoor);
    Particle.function("closed",closeDoor);
    Particle.function("CLOSED",closeDoor);
    Particle.function("toggle",toggle);
    Particle.function("state",state);
    Particle.publish("setupComplete");
}

int openDoor(String args) {
    // Main control relay
    delay(DELAYMS); myRelays.off(4);
    // Reverse polarity
    delay(DELAYMS); myRelays.on(2);
    delay(DELAYMS); myRelays.on(3);
    // Main control relay
    delay(DELAYMS); myRelays.on(4);
    Particle.publish("status", "open");
    currentState = open;
    return(0);
}

int closeDoor(String args) {
    // Main control relay
    delay(DELAYMS); myRelays.off(4);
    // Reverse polarity
    delay(DELAYMS); myRelays.off(3);
    delay(DELAYMS); myRelays.off(2);
    // Main control relay
    delay(DELAYMS); myRelays.on(4);
    Particle.publish("status", "closed");
    currentState = closed;
    return(1);
}

int toggle(String args) {
    Particle.publish("request", "toggle");
    if (currentState == open) {
        closeDoor("");
    } else {
        openDoor("");
    }
    return(0);
}

int state(String args) {
    Particle.publish("request", "state");
    if (currentState == open) {
        return(0);
    } else {
        return(1);
    }
}

void loop() {
    // There is nothing to do in this main loop. Rather than doing nothing, just sleep.
    delay(10000);
}
