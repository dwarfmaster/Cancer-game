#/bin/bash

objs=
files=`cat games/objs`

for file in $files
do
	objs+="$1/$file "
done
echo $objs


