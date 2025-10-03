#!/bin/bash

if [ $# -ne 2 ] ; then
	echo "Usage: $0 <core_tag> <artifact_file>"
  exit 1
fi

PACKAGE=zephyr
CORE_TAG=$1
ARTIFACT_FILE=$2

if [ ! -f "$ARTIFACT_FILE" ] ; then
	echo "Artifact file '$ARTIFACT_FILE' not found" 1>&2
	exit 1
fi

JSON_TEMPLATE="extra/zephyr-core-template.json"
cat $JSON_TEMPLATE | sed \
	-e "s/__CORE_TAG__/$CORE_TAG/" \
	-e "s/__ARTIFACT_FILE__/$(basename $ARTIFACT_FILE)/" \
	-e "s/__ARTIFACT_HASH__/$(sha256sum $ARTIFACT_FILE | awk '{print $1}')/" \
	-e "s/__ARTIFACT_SIZE__/$(stat -c %s $ARTIFACT_FILE)/" | jq .
