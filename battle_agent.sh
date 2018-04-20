#!/bin/sh

curDir=`pwd`
runDir="$(cd "$(dirname "$0")" && pwd)"
cd ${runDir}
python battle_agent.py 2>>data/battle.log
cd ${curDir}