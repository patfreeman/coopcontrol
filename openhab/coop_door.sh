#!/bin/bash

URL="https://api.particle.io/v1/devices/${PARTICLE_DEVICE_ID}/"
DATA="access_token=${PARTICLE_ACCESS_TOKEN}"

function http () {
	url=$1
	ret=`curl -sd "${DATA}" ${URL}${url}${ARGS} | jq '.return_value'`
	case ${ret} in
		0) echo "OPEN" ;;
		1) echo "CLOSED" ;;
		2) echo "OPEN_WAIT" ;;
		3) echo "CLOSE_WAIT" ;;
		4) echo "UNKNOWN" ;;
	esac
}

if [ "$1" = "OPEN" -o "$1" = "CLOSED" ]; then
	http $1
else
	http state
fi
