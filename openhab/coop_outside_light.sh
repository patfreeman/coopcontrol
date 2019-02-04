#!/bin/bash

URL="https://api.particle.io/v1/devices/${PARTICLE_DEVICE_ID}/"
url="light"
DATA="access_token=${PARTICLE_ACCESS_TOKEN}"

ret=`curl -s ${URL}${url}?${DATA} | jq '.result'`
if [ "$ret" != "null" ]; then
	echo "$ret"
else
	echo null
fi
