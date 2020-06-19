#!/bin/bash

URL="https://api.particle.io/v1/devices/345634563456345634563456/"
DATA="access_token=da39a3ee5e6b4b0d3255bfef95601890afd80709"

function http () {
	url=$1
	ret=`curl -sd "${DATA}" ${URL}${url}${ARGS} | jq '.return_value'`
	case ${ret} in
		0) echo "OPEN" ;;
		1) echo "CLOSED" ;;
		2) echo "OPEN_WAIT" ;;
		3) echo "CLOSE_WAIT" ;;
		*) echo "UNKNOWN" ;;
	esac
}

if [ "$1" = "OPEN" ]; then
	http open
	/config/scripts/coop_annotation.sh $1
elif [ "$1" = "CLOSED" ]; then
	http close
	/config/scripts/coop_annotation.sh $1
else
	http state
fi
