#!/bin/bash

# export PATH="/home/ken/App/ns-3.29/source/ns-3.29:$PATH"
export PATH="/home/hiroshima-lenovo/App/ns3/source/ns-3.29:$PATH"

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
mac_type=("SSMT_CI" "SRB" "DCF" "ACW")

for mac in ${mac_type[@]}
do
  echo "---------------------------"
  echo "[RECT]($mac) proc... {$i}"
  for i in `seq 3`
  do
    waf --run "SSMT --EnableOutputFile=true --MacType=$mac --SimTime=$sim_time" &
  done
  wait
  echo "[RECT]($mac) done!"
done

# for mt in ${mac_type[@]}
# do
#   for i in `seq 1`
#   do
#     echo "---------------------------"
#     echo "[$i]$mt proc..."
#     for n in `seq 1 10`
#     do
#       for s in 10 100 1000 10000
#       do
#         waf --run "SSMT --EnableOutputFile=true --Seed=$s --Square=$s --MacType=$mt --SimTime=$sim_time --NumOfBss=$n" &
#       done
#       wait
#     done
#     echo "[$i]$mt done!"
#   done
# done
