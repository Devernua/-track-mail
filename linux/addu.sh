#!/bin/bash

if [[ $# -lt 1 ]]
then
	echo "[-f|--file] \"input.file\""
	exit
fi

while [[ $# -gt 1 ]]
do
key="$1"

case $key in
	-f|--file)
	INPUTFILE="$2"
	shift
	;;
	*)
	echo "[-f|--file] \"input.file\""
	exit
	;;
esac
shift
done

#FORMAT: user group /home/dir hash

cat $INPUTFILE | while read line; do
	commands=( $line )
	if [[ ${#commands[@]} -ne 4 ]]
	then
		echo "ERROR: format input file lines must be 'user group dir hash'\n"
	else
		sudo groupadd ${commands[1]}
		sudo useradd -m -d ${commands[2]} -g ${commands[1]} -p ${commands[3]} ${commands[0]}
	fi
done
