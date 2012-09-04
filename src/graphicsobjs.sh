#/bin/bash

objs=
files=`cat /tmp/gobjs`

for file in $files
do
	objs+="$1/$file "
done
echo $objs


