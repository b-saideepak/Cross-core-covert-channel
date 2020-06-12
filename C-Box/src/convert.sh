#!/bin/bash
if [ $# -ne 1 ]
then
	echo "Usage: bash convert.sh file"
	exit
fi

cat $1 | awk -F, '{
if($2>$3 && $2>$4 && $2>$5) {max = 0}
else if($3>$4 && $3>$5) {max = 1}
else if($4>$5) {max = 2}
else {max = 3}
print $1/64","max
}'

