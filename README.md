# CoopControl
Controls my chicken coop. Automated door through OpenHAB and the particle cloud.

### Prerequisites
* RelayShield Particle Library - https://github.com/spark/relayshield
* OpenHAB2 - http://www.openhab.org/

### OpenHAB2 Setup
1. Make sure the exec binding is enabled.
2. Edit the files from the [openhab](openhab) subdirectory.
 * [coop_door.sh](openhab/coop_door.sh) needs the appropriate PARTICLEi\_ variables set
 * [coop.things](openhab/coop.things) needs the appriopriate location of the coop_door.sh script
3. Add the files from the [openhab](openhab) subdirectory to the appropriate configuration directories.
 * [coop_door.sh](openhab/coop_door.sh) into your openhab conf/scripts directory
 * [coop.things](openhab/coop.things) into your openhab conf/things directory
 * [coop.items](openhab/coop.items) into your openhab conf/items directory
 * [coop.rules](openhab/coop.rules) into your openhab conf/rules directory
