#!/bin/bash

pid=`ps aux | grep python | grep -v grep | grep train.py | awk '{print $2;}'`
echo "killing ${pid}"
ps aux | grep python | grep -v grep | grep train.py | awk '{print $2;}' | xargs kill -9


nohup python train.py 1>>data/train.log 2>&1 &
