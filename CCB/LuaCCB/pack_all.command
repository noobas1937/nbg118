cd `dirname $0`
find . -name "*.tps" -exec TexturePacker {} \;

cd ../../tools
python remove_texturepacker_smartupdate.py