#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
SRCROOT="${DIR}/.."
luaSrcDir="${SRCROOT}/IF/Resources/src"
luaSrcDir_tmp="${SRCROOT}/IF/Resources/src_tmp"
mv ${luaSrcDir} ${luaSrcDir_tmp}
cd ${DIR}
./pack_files/pack_files.sh -i ${luaSrcDir_tmp} -o ${luaSrcDir} -ek n_77wDDolqz -es i900_gFymA