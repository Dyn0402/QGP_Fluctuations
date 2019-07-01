#! /bin/bash

for f in *.xml
do
	echo "Submitting $f"
	eval "star-submit $f"
done

echo "donzo"
