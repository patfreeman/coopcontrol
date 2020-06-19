#!/bin/bash

S=`date +%s`
A=`echo $1 | tr 'A-Z' 'a-z'`
curl -X POST "http://grafana:8086/write?db=home_assistant&precision=s" --data-binary 'events title="Door '${A^}'",tags="'${A}'" '${S}
