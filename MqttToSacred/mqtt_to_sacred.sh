#!/usr/bin/env bash

if [ "x$1" == "x" ];
then
    echo "Usage: store.sh <system-id>"
    echo "Example: store.sh system-001"
    exit 1
fi
SYSTEMID=$1

source ../my-credentials.sh
python3 mqtt_to_sacred.py $SYSTEMID


