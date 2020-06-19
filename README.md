# CoopControl
Controls my chicken coop. Automated door through OpenHAB and the particle cloud.

### Features
 * Automatically open and close with the sun.
 * Temperature and humidity sensors for ingestion into openhab.

### Prerequisites
* RelayShield Particle Library - https://github.com/spark/relayshield
* Home Assistant https://www.home-assistant.io/
* OpenHAB2 - http://www.openhab.org/

### Hardware Setup
1. [Particle](https://store.particle.io/) Photon with Relay Shield
2. A [linear actuator](https://www.amazon.com/gp/product/B0722R3H9Z) or similar
3. TODO add wiring diagram

### Particle Web IDE install
1. Create new app
2. Include RelayShield library
3. Copy paste [coopcontrol.ino](coopcontrol.ino) into ID
4. Add separate files with contents of [DHT.cpp](DHT.cpp) and [DHT.h](DHT.h)

### Home Assistant Setup
See [home-assistant](home-assistant)

### OpenHAB2 Setup
See [openhab](openhab)
