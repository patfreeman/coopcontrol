I've moved my home automation to Home Assistant, so the last version this worked with was ~2.4.

### OpenHAB2 Setup
1. Make sure the exec binding is enabled.
2. Edit the files from this directory
 * coop\_\*.sh need the appropriate `URL` and `DATA` variables set for your particle device.
 * [coop.things](coop.things) needs the appriopriate location of the coop_door.sh script
3. Add the files from this directory to the appropriate configuration directories.
 * coop\_\*.sh into your openhab conf/scripts directory
 * [coop.things](coop.things) into your openhab conf/things directory
 * [coop.items](coop.items) into your openhab conf/items directory
 * [coop.rules](coop.rules) into your openhab conf/rules directory
