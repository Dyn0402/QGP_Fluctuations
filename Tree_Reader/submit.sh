#! /bin/bash

for f in *.xml
do
	echo "Submitting $f"
	echo
	eval "star-submit $f"
	echo
	echo
done

echo "donzo"
