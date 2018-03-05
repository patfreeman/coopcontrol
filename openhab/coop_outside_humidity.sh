#!/bin/bash

URL="https://api.particle.io/v1/devices/${PARTICLE_DEVICE_ID}/"
url="outside_hum"
DATA="access_token=${PARTICLE_ACCESS_TOKEN}"

ret=`curl -s ${URL}${url}?${DATA} | jq '.result'`
if [ -n "$ret" ]; then
	echo "$ret%"
	exit $ret
fi
