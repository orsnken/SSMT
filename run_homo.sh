#!/bin/bash

export PATH="/home/ken/App/ns-3.29/source/ns-3.29:$PATH"

waf

if [ $? -ne 0 ]; then
  echo "Compile error."
  exit 1
fi

echo "Compile done!"
echo "---------------------------"
echo "Begin simulation ///"

pwd

sim_time=10
mac_type=("SSMT_CI" "SRB" "DCF" "CW_ONLY" "ACW")

for mt in ${mac_type[@]}
do
  for i in `seq 5`
  do
    echo "---------------------------"
    echo "[$i]$mt proc..."
    for d in 1 5 10 15 20 25 30 35 40 45 50 60 70 80 90 100
    # for d in 1 10 20 30 40 50 60 70 80 90 100 110 120 130 140 150 160 170 180 190 200
    do
      waf --run "SSMT --EnableOutputFile=true --DistanceWlans=$d --MacType=$mt --SimTime=$sim_time" &
    done
    wait
    echo "[$i]$mt done!"
  done
done
