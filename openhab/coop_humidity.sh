#!/bin/bash

URL="https://api.particle.io/v1/devices/${PARTICLE_DEVICE_ID}/"
url="inside_hum"
DATA="access_token=${PARTICLE_ACCESS_TOKEN}"

ret=`curl -s ${URL}${url}?${DATA} | jq '.result'`
if [ "$ret" != "null" ]; then
	printf "%.0f" $ret
else
	echo null
fi
