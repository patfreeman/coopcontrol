#include "DHT.h"

// This #include statement was automatically added by the Particle IDE.
#include <RelayShield.h>

// Make our own state enumerator
enum State { open, closed };

// Create an instance of the RelayShield library, so we have something to talk to
RelayShield myRelays;

// Just assume it starts off closed. Probably should be unknown or something else
State currentState = closed;

// Pins
int light_sensor_pin = A0;
#define DHTPIN_ONE 1
#define DHTPIN_TWO 2

// DHT parameters
#define DHTTYPE_ONE DHT22
#define DHTTYPE_TWO DHT22
String DHTNAME_ONE = "inside";
String DHTNAME_TWO = "outside";

// Variables
double one_temperature = 0;
int one_humidity = 0;
double two_temperature = 0;
int two_humidity = 0;
int light = 0;

// DHT sensor
DHT dht_one(DHTPIN_ONE, DHTTYPE_ONE);
DHT dht_two(DHTPIN_TWO, DHTTYPE_TWO);

// Millisecond of delay between relay controls
// I saw problems with the delay being too short and the particle rebooting. (Short circuit?)
#define RELAY_DELAY 100
#define PUBLISH_DELAY 10000

//STARTUP(WiFi.selectAntenna(ANT_INTERNAL)); // selects the CHIP antenna
STARTUP(WiFi.selectAntenna(ANT_EXTERNAL)); // selects the u.FL antenna
//STARTUP(WiFi.selectAntenna(ANT_AUTO)); // continually switches at high speed between antennas

void setup() {
    myRelays.begin();
    // Setup too many API function hooks
    Particle.function("open", openDoor);
    Particle.function("OPEN", openDoor);
    Particle.function("close", closeDoor);
    Particle.function("CLOSE", closeDoor);
    Particle.function("closed", closeDoor);
    Particle.function("CLOSED", closeDoor);
    Particle.function("toggle", toggle);
    Particle.function("state", state);
    Particle.publish("setupComplete");
    // Variables
    Particle.variable("light", light);
    Particle.variable(String(DHTNAME_ONE) + "_temp", one_temperature);
    Particle.variable(String(DHTNAME_ONE) + "_hum", one_humidity);
    Particle.variable(String(DHTNAME_TWO) + "_temp", two_temperature);
    Particle.variable(String(DHTNAME_TWO) + "_hum", two_humidity);
    // Start DHT sensor
    dht_one.begin();
    dht_two.begin();
}

int openDoor(String args) {
    // Main control relay
    delay(RELAY_DELAY); myRelays.off(4);
    // Reverse polarity
    delay(RELAY_DELAY); myRelays.on(2);
    delay(RELAY_DELAY); myRelays.on(3);
    // Main control relay
    delay(RELAY_DELAY); myRelays.on(4);
    Particle.publish("status", "open");
    currentState = open;
    return(0);
}

int closeDoor(String args) {
    // Main control relay
    delay(RELAY_DELAY); myRelays.off(4);
    // Reverse polarity
    delay(RELAY_DELAY); myRelays.off(3);
    delay(RELAY_DELAY); myRelays.off(2);
    // Main control relay
    delay(RELAY_DELAY); myRelays.on(4);
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
    // Light level measurement
    float light_measurement = analogRead(light_sensor_pin);
    light = (int)(light_measurement/4096*100);
    one_temperature = dht_one.getTempCelcius();
    one_humidity = dht_one.getHumidity();
    two_temperature = dht_two.getTempCelcius();
    two_humidity = dht_two.getHumidity();

    Spark.publish("light", String(light) + "%", PRIVATE);
    Spark.publish(String(DHTNAME_ONE) + " temperature", String(one_temperature) + " °C", PRIVATE);
    Spark.publish(String(DHTNAME_ONE) + " humidity", String(one_humidity) + "%", PRIVATE);
    delay(PUBLISH_DELAY);
    Spark.publish(String(DHTNAME_TWO) + " temperature", String(two_temperature) + " °C", PRIVATE);
    Spark.publish(String(DHTNAME_TWO) + " humidity", String(two_humidity) + "%", PRIVATE);
    delay(PUBLISH_DELAY);
}
