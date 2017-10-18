# CoopControl
Controls my chicken coop. Automated door through OpenHAB and the particle cloud.

### Prerequisites
RelayShield Particle Library - https://github.com/spark/relayshield
OpenHAB2 - http://www.openhab.org/

### OpenHAB2 Setup
1. Make sure the exec binding is enabled.
2. Add an exec thing:
  * `Thing exec:command:coop_door [command="/srv/openhab2/conf/scripts/coop_door.sh %2$s", interval=0, timeout=5, autorun=true]`
3. Add two items. One to control the door, one tied to the exec Thing.
  * `String Coop_Door        "Chicken Coop Door [%s]"        (Outdoor)       {channel="exec:command:coop_door:input"}`
  * `Switch Coop_Door_Switch "Chicken Coop Door"        (Outdoor)       ["Switchable"]`
4. Add the control item to your sitemap
  * `Switch item=Coop_Door_Switch icon="garagedoor" label="Coop Door" mappings=[ON="Open", OFF="Closed"]`
