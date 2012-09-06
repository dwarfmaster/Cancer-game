#!/bin/bash

source pot.mk

if [ $# -lt 1 ]
then
	echo Doit avoir au moins 1 argument.
	exit
fi

msginit -l $1 -o $1.po -i $NAME.pot

