#! /bin/bash

cd "Debug"

for CENT in {0..15}
do
	eval "Angular_Ratios $CENT"
done
