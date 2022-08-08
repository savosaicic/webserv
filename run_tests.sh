#!/bin/bash

PORT=8080

for test in tests/*
do
	printf "%-21s " $(basename -s .py ${test})
	if (python2.7 ${test} ${PORT})
	then
		printf "[${GREEN}%s${NC}]\n" "✅"
	else
		printf "[${RED}%s${NC}]\n" "❌"
	fi
done
