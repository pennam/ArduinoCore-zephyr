#!/bin/bash

set -e

if [ -z "$1" ]; then
	echo "Usage: $0 VERSION [OUTPUT_FILE]"
	exit 1
fi

if [ ! -f platform.txt ]; then
	echo "Launch this script from the root core folder."
	exit 2
fi

PACKAGE=zephyr
VERSION=$1
OUTPUT_FILE=${2:-distrib/${PACKAGE}-${VERSION}.tar.bz2}

# create a temporary platform.txt file with the correct version
TEMP_PLATFORM=$(mktemp -p . | sed 's/\.\///')
sed -e "s/^version=.*/version=${VERSION}/" platform.txt > ${TEMP_PLATFORM}

declutter_file() {
	[ -f "$1" ] || return 0
	cat "$1" | sed -e 's/\s*#.*//' | grep -v '^\s*$'
}

# create the list of files and directories to include
TEMP_INC=$(mktemp -p . | sed 's/\.\///')
echo ${TEMP_PLATFORM} >> ${TEMP_INC}
declutter_file extra/package_core.inc >> ${TEMP_INC}
extra/get_board_details.sh | jq -cr '.[]' | while read -r item; do
	variant=$(jq -cr '.variant' <<< "$item")
	echo "::info::`${variant}`"
	echo "variants/${variant}/" >> ${TEMP_INC}
	ls firmwares/zephyr-${variant}.* >> ${TEMP_INC}
done

# create the list of files and directories to exclude
TEMP_EXC=$(mktemp -p . | sed 's/\.\///')
declutter_file extra/package.exc >> ${TEMP_EXC}

mkdir -p $(dirname ${OUTPUT_FILE})
tar -cjhf ${OUTPUT_FILE} -X ${TEMP_EXC} -T ${TEMP_INC} \
	--transform "s,${TEMP_PLATFORM},platform.txt," \
	--transform "s,^,ArduinoCore-zephyr/,"
rm -f ${TEMP_INC} ${TEMP_EXC} ${TEMP_PLATFORM}
