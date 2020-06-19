#!/bin/bash
url="inside_temp"

# this
URL="https://api.particle.io/v1/devices/345634563456345634563456/"
DATA="access_token=da39a3ee5e6b4b0d3255bfef95601890afd80709"
ret=`curl -s ${URL}${url}?${DATA} | jq '.result'`
# or this
#URL="http://192.168.86.51/"
#ret=`curl -s ${URL}${url}`
if [ "$ret" != "null" ]; then
	printf "%.1f" $ret
else
	echo null
fi
