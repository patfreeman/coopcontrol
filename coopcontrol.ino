#include "DHT.h"
#include <RelayShield.h>

// Make our own state enumerator
enum State { open, closed, open_wait, close_wait, unknown };

// Create an instance of the RelayShield library to talk to
RelayShield myRelays;

State currentState = unknown;

/********************************* <CONFIGS> *********************************/
// Hardware Pins
int light_sensor_pin = A0;
#define DHTPIN_ONE 1
#define DHTPIN_TWO 2

// DHT Types and Names
#define DHTTYPE_ONE DHT22
#define DHTTYPE_TWO DHT22
String DHTNAME_ONE = "inside";
String DHTNAME_TWO = "outside";

// Delay after light sensor before toggling door in milliseconds
#define CLOSE_DELAY 300000 // 5 minutes, 1800000 // 30 minutes
//#define OPEN_DELAY 5400000 // 90 minutes

// Milliseconds of delay between relay controls
// I saw problems with the relay delay being too short and the particle
// rebooting. (Short circuit?)
#define RELAY_DELAY 100
#define PUBLISH_DELAY 10000

//STARTUP(WiFi.selectAntenna(ANT_INTERNAL)); // selects the CHIP antenna
STARTUP(WiFi.selectAntenna(ANT_EXTERNAL)); // selects the u.FL antenna
//STARTUP(WiFi.selectAntenna(ANT_AUTO)); // continually switch between antennas

/********************************* </CONFIGS> *********************************/

// Variables
double one_temperature = -255;
int one_humidity = -1;
double two_temperature = -255;
int two_humidity = -1;
int light = 0;
unsigned long toggle_at;

// DHT sensor
DHT dht_one(DHTPIN_ONE, DHTTYPE_ONE);
DHT dht_two(DHTPIN_TWO, DHTTYPE_TWO);

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
    // Variables
    Particle.variable("light", light);
    Particle.variable(String(DHTNAME_ONE) + "_temp", one_temperature);
    Particle.variable(String(DHTNAME_ONE) + "_hum", one_humidity);
    Particle.variable(String(DHTNAME_TWO) + "_temp", two_temperature);
    Particle.variable(String(DHTNAME_TWO) + "_hum", two_humidity);
    // Start DHT sensor
    dht_one.begin();
    dht_two.begin();
    // Set door state
    float light_measurement = analogRead(light_sensor_pin);
    light = (int)(light_measurement/4096*100);
    if(light) {
        openDoor("");
    } else {
        currentState = close_wait;
        toggle_at = millis() + CLOSE_DELAY;
        Particle.publish("status", "close_wait");
    }
    // Set initial humidity
    if (one_humidity > 100 || one_humidity < 0) {
        one_humidity = dht_one.getHumidity();
    }
    if (two_humidity > 100 || two_humidity < 0) {
        two_humidity = dht_two.getHumidity();
    }
    Particle.publish("setupComplete");
}

int openDoor(String args) {
    Particle.publish("request", "open");
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
    Particle.publish("request", "close");
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
    Particle.publish("status", String(currentState));
    return(currentState);
}

void loop() {
    // Light level measurement
    float light_measurement = analogRead(light_sensor_pin);
    light = (int)(light_measurement/4096*100);

    one_temperature = dht_one.getTempCelcius();
    two_temperature = dht_two.getTempCelcius();

    // Error correct humidity and smooth graph
    int tmp_humidity = 0;
    tmp_humidity = dht_one.getHumidity();
    if (abs(one_humidity - tmp_humidity) < 5) {
        one_humidity = tmp_humidity;
    }
    tmp_humidity = dht_two.getHumidity();
    if (abs(two_humidity - tmp_humidity) < 5) {
        two_humidity = tmp_humidity;
    }

    Spark.publish("light", String(light) + "%", PRIVATE);
    delay(PUBLISH_DELAY/5);
    Spark.publish(String(DHTNAME_ONE) + " temperature", String(one_temperature)
        + " °C", PRIVATE);
    delay(PUBLISH_DELAY/5);
    Spark.publish(String(DHTNAME_ONE) + " humidity", String(one_humidity)
        + "%", PRIVATE);
    delay(PUBLISH_DELAY/5);
    Spark.publish(String(DHTNAME_TWO) + " temperature", String(two_temperature)
        + " °C", PRIVATE);
    delay(PUBLISH_DELAY/5);
    Spark.publish(String(DHTNAME_TWO) + " humidity", String(two_humidity)
        + "%", PRIVATE);
    delay(PUBLISH_DELAY/5);

#ifdef CLOSE_DELAY
    if (currentState == open && light == 0) {
        currentState = close_wait;
        toggle_at = millis() + CLOSE_DELAY;
        Particle.publish("status", "close_wait");
    } else if (currentState == close_wait && light != 0) {
        currentState = open;
    } else if (currentState == close_wait && light == 0) {
        Particle.publish("toggle_in", String(toggle_at-millis()) + "ms");
        if(millis() >= toggle_at) {
            closeDoor("");
        }
    }
#endif

#ifdef OPEN_DELAY
    if (currentState == closed && light != 0) {
        currentState = open_wait;
        toggle_at = millis() + OPEN_DELAY;
        Particle.publish("status", "open_wait");
    } else if (currentState == open_wait && light == 0) {
        currentState = closed;
    } else if (currentState == open_wait && light != 0) {
        Particle.publish("toggle_in", String(toggle_at-millis()) + "ms");
        if(millis() >= toggle_at) {
            openDoor("");
        }
    }
#endif
}
