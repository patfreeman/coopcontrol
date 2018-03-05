# CoopControl
Controls my chicken coop. Automated door through OpenHAB and the particle cloud.

### Features
 * Automatically open and close with the sun.
 * Temperature and humidity sensors for ingestion into openhab.

### Prerequisites
* RelayShield Particle Library - https://github.com/spark/relayshield
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

### OpenHAB2 Setup
1. Make sure the exec binding is enabled.
2. Edit the files from the [openhab](openhab) subdirectory.
 * coop\_\*.sh need the appropriate PARTICLE\_ variables set
 * [coop.things](openhab/coop.things) needs the appriopriate location of the coop_door.sh script
3. Add the files from the [openhab](openhab) subdirectory to the appropriate configuration directories.
 * coop\_\*.sh into your openhab conf/scripts directory
 * [coop.things](openhab/coop.things) into your openhab conf/things directory
 * [coop.items](openhab/coop.items) into your openhab conf/items directory
 * [coop.rules](openhab/coop.rules) into your openhab conf/rules directory
