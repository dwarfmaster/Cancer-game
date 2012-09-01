#/bin/bash

objs=
files=`cat /tmp/objs`

for file in $files
do
	objs+=$1/$file
done
echo $objs


