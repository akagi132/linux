#!/bin/sh

while [ true ]
do
    map_goal=`ps -A | grep map_goal`
    if [ -n "$map_goal" ]; then
        echo 'I found!'
        break
    fi
    echo 'watching...'
    sleep 1s
done

