#/bin/bash

objs=
files=`cat core/objs`

for file in $files
do
	objs+="$1/$file "
done
echo $objs


