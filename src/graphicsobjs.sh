#/bin/bash

objs=
files=`cat graphics/objs`

for file in $files
do
	objs+="$1/$file "
done
echo $objs


