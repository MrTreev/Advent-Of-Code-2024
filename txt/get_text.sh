#!/bin/sh
set -eu

cookie="${1}";
txtpath="$(dirname "${0}";)";

for dayno in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25;
do
	wget \
		--no-cookies \
		--header \
		"Cookie: session=${cookie}" \
		"https://adventofcode.com/2024/day/${dayno}/input";
	mv "input" "${txtpath}/day${dayno}.txt";
done

unset dayno;
unset txtpath;
unset cookie;
