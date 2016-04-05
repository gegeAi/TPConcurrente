#!/bin/sh

key=1760000

while test $key -le 2032000
	do
	ipcrm -s $key
	key=$(($key+1))
done

key2=3276810

while test $key2 -le 3407887
	do
	ipcrm -m $key2
	key2=$(($key2+1))
done

key3=720000

while test $key3 -le 820000
	do
	ipcrm -q $key3
	key3=$(($key3+1))
done
