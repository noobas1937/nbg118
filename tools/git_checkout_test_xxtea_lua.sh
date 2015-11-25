#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
SRCROOT="${DIR}/.."
luaSrcDir="${SRCROOT}/IF/Resources/src"
luaSrcDir_tmp="${SRCROOT}/IF/Resources/src_tmp"

git checkout -- $luaSrcDir
rm -rf $luaSrcDir_tmp