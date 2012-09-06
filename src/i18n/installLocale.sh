#!/bin/bash

source pot.mk

if [ $# -lt 2 ]
then
	echo usage : $0 locale \(like fr_Fr\) name \(like fr\)
	exit
fi

file=$1.po

if [ ! -e $file ]
then
	echo Le fichier $file n\'existe pas.
	exit
fi

if [ -d $file ]
then
	echo Le chemin indiqué est un dossier.
	exit
fi

msgfmt -c -v -o $NAME.mo $file

instdir=$DIR/$2/LC_MESSAGES
if [ ! -e $instdir ]
then
	mkdir -p $instdir
fi

cp $NAME.mo $instdir

