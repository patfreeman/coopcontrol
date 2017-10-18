#!/bin/bash

URL="https://api.particle.io/v1/devices/${PARTICLE_DEVICE_ID}/"
DATA="access_token=${PARTICLE_ACCESS_TOKEN}"

function http () {
	url=$1
	ret=`curl -sd "${DATA}" ${URL}${url}${ARGS} | jq '.return_value'`
	if [ "$ret" = "0" ]; then
		echo "OPEN"
	else
		echo "CLOSED"
	fi
}

if [ "$1" = "OPEN" -o "$1" = "CLOSED" ]; then
	http $1
else
	http state
fi
