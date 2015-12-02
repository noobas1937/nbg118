BASE_DIR_FOR_SCRIPT_SELF=$(cd "$(dirname "$0")"; pwd)

# TODO
# cd ${BASE_DIR_FOR_SCRIPT_SELF}/nbg2lua
# python nbg_ini_exporter.py




cd "${BASE_DIR_FOR_SCRIPT_SELF}/../cocos2d/tools/tolua"
python genbindings_nbg.py
