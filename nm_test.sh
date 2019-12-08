#!/bin/bash

count=0
crash=0
error=0
crash=0

for entry in "/bin/"*
do
	count=$((count + 1))
	err=$(./ft_nm $1 $entry > file1)
	$(nm $1 $entry > file2 2>&-)
	diff=$(diff file1 file2)
	if [ -z "$diff" ]
	then
		success=$((success + 1))
		printf "%-40s\033[32m%40s\033[0m\n" $entry "OK"
	else
		error=$((error + 1))
		printf "%-40s\033[31m%40s\033[0m\n" $entry "NO"
	fi
	rm file1 file2
done
