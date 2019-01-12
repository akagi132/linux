#!/bin/bash

echo 'nav_map start'

timeout -sKILL 3 sh check_launch.sh

STATUS=$?

echo $STATUS
if [ "$STATUS" -ne 0 ]; then
    echo 'map_goal not launched...'
    touch failed.txt
else
    echo 'map_goal launched.'
fi

