#!/bin/bash

set -e

if [ -z "$1" ]; then
	echo "Usage: $0 VERSION [OUTPUT_FILE]"
	exit 1
fi

if [ ! -f platform.txt ]; then
	echo "Launch this script from the root core folder as ./extra/package_core.sh VERSION"
	exit 2
fi

PACKAGE=ArduinoCore-zephyr
VERSION=$1
OUTPUT_FILE=${2:-distrib/${PACKAGE}-${VERSION}.tar.bz2}

# create a temporary platform.txt file with the correct version
TEMP_PLATFORM=$(mktemp -p . | sed 's/\.\///')
sed -e "s/^version=.*/version=${VERSION}/" platform.txt > ${TEMP_PLATFORM}

TEMP_LIST=$(mktemp)
echo ${TEMP_PLATFORM} > ${TEMP_LIST}

# import a basic list of files and directories
cat extra/package_core.inc | sed -e 's/\s*#.*//' | grep -v '^\s*$' >> ${TEMP_LIST}

# add the board-specific files
extra/get_board_details.sh | jq -cr '.[]' | while read -r item; do
	variant=$(jq -cr '.variant' <<< "$item")
	echo "variants/${variant}/" >> ${TEMP_LIST}
	ls firmwares/zephyr-${variant}.* >> ${TEMP_LIST}
done
cat ${TEMP_LIST}
mkdir -p $(dirname ${OUTPUT_FILE})
tar -cjhf ${OUTPUT_FILE} -X extra/package_core.exc -T ${TEMP_LIST} --transform "s,${TEMP_PLATFORM},platform.txt," --transform "s,^,${PACKAGE}/,"
rm -f ${TEMP_LIST} ${TEMP_PLATFORM}
