APPNAME="if"

# options

buildexternalsfromsource=

usage(){
cat << EOF
usage: $0 [options]

Build C/C++ code for $APPNAME using Android NDK

OPTIONS:
-s	Build externals from source
-h	this help
EOF
}

while getopts "sh" OPTION; do
case "$OPTION" in
s)
buildexternalsfromsource=1
;;
h)
usage
exit 0
;;
esac
done

# paths

if [ -z "${NDK_ROOT+aaa}" ];then
echo "please define NDK_ROOT"
exit 1
fi

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

cd $DIR
python android_update_project.py

# ... use paths relative to current directory
# COCOS2DX_ROOT="$DIR/../IF/libs"
APP_ROOT="$DIR/.."
APP_ANDROID_ROOT="$DIR"
APP_COCOS_RES_ROOT="$DIR/assets_tmp"

echo "NDK_ROOT = $NDK_ROOT"
# echo "COCOS2DX_ROOT = $COCOS2DX_ROOT"
echo "APP_ROOT = $APP_ROOT"
echo "APP_ANDROID_ROOT = $APP_ANDROID_ROOT"
echo "APP_COCOS3_RESOURCE_ROOT=$APP_COCOS_RES_ROOT"

# make sure assets is exist
# if [ -d "$APP_ANDROID_ROOT"/assets ]; then
#     rm -rf "$APP_ANDROID_ROOT"/assets
# fi

#modefy by liudi fit 3.6 resource use
if [ -d "$APP_COCOS_RES_ROOT" ]; then
    rm -rf "$APP_COCOS_RES_ROOT"
fi


# mkdir "$APP_ANDROID_ROOT"/assets

mkdir "$APP_COCOS_RES_ROOT"

# copy resources
for file in "$APP_ROOT"/IF/Resources/*
do
echo $file
if [ -d "$file" ]; then
    cp -rf "$file" "$APP_COCOS_RES_ROOT"
fi

if [ -f "$file" ]; then
    cp "$file" "$APP_COCOS_RES_ROOT"
fi
done

# copy android-only files
for extraFile in "$APP_ROOT"/Android_Resource/*
do
    rm -rf "$APP_COCOS_RES_ROOT"/"${extraFile##*/}"
    cp -rf "$extraFile" "$APP_COCOS_RES_ROOT"
done

# copy android-only sound files
for extraFile in "$APP_ROOT"/Android_Sounds/*
do
    rm -rf "$APP_COCOS_RES_ROOT"/sounds/"${extraFile##*/}"
    cp -rf "$extraFile" "$APP_COCOS_RES_ROOT"/sounds
done

# fix world map tmx from .pvr.ccz to .pkm
# sed -i.bak 's/World_1.pvr.ccz/_alpha_World_1.pkm/g' "$APP_COCOS_RES_ROOT"/WorldMap1.tmx
# rm -rf "$APP_COCOS_RES_ROOT"/assets/*.bak
# sed -i.bak 's/World_1.pvr.ccz/_alpha_World_1.pkm/g' "$APP_COCOS_RES_ROOT"/WorldMap.tmx
# rm -rf "$APP_COCOS_RES_ROOT"/*.bak

# sed -i.bak 's/World_4.pvr.ccz/_alpha_World_4.pkm/g' "$APP_COCOS_RES_ROOT"/WorldMap.tmx
# rm -rf "$APP_COCOS_RES_ROOT"/*.bak

rm -rf "$APP_COCOS_RES_ROOT"/WorldMap.tmx
rm -rf "$APP_COCOS_RES_ROOT"/WorldMap1.tmx

sed -i.bak 's/nb_ocean.pvr.ccz/_alpha_nb_ocean.pkm/g' "$APP_COCOS_RES_ROOT"/nb_world_link_area.tmx
rm -rf "$APP_COCOS_RES_ROOT"/*.bak
sed -i.bak 's/decoration.pvr.ccz/_alpha_decoration.pkm/g' "$APP_COCOS_RES_ROOT"/nb_world_link_area.tmx
rm -rf "$APP_COCOS_RES_ROOT"/*.bak
sed -i.bak 's/throne.pvr.ccz/_alpha_throne.pkm/g' "$APP_COCOS_RES_ROOT"/nb_world_link_area.tmx
rm -rf "$APP_COCOS_RES_ROOT"/*.bak

sed -i.bak 's/nb_ocean.pvr.ccz/_alpha_nb_ocean.pkm/g' "$APP_COCOS_RES_ROOT"/nb_world.tmx
rm -rf "$APP_COCOS_RES_ROOT"/*.bak
sed -i.bak 's/decoration.pvr.ccz/_alpha_decoration.pkm/g' "$APP_COCOS_RES_ROOT"/nb_world.tmx
rm -rf "$APP_COCOS_RES_ROOT"/*.bak
sed -i.bak 's/throne.pvr.ccz/_alpha_throne.pkm/g' "$APP_COCOS_RES_ROOT"/nb_world.tmx
rm -rf "$APP_COCOS_RES_ROOT"/*.bak

rm -rf "$APP_COCOS_RES_ROOT"/nb_world/*

# copy icons (if they exist)
file="$APP_COCOS_RES_ROOT"/Icon-72.png
if [ -f "$file" ]; then
	cp "$file" "$APP_ANDROID_ROOT"/res/drawable-hdpi/icon.png
fi
file="$APP_COCOS_RES_ROOT"/Icon-48.png
if [ -f "$file" ]; then
	cp "$file" "$APP_ANDROID_ROOT"/res/drawable-mdpi/icon.png
fi
file="$APP_COCOS_RES_ROOT"/Icon-32.png
if [ -f "$file" ]; then
	cp "$file" "$APP_ANDROID_ROOT"/res/drawable-ldpi/icon.png
fi

echo "copy ccbi files for lua"
cp -r "${APP_ROOT}/IF/Resources/res/ccbi" "${APP_COCOS_RES_ROOT}/res/ccbi"

echo "xxtea lua codes"
luaSrcDir="${APP_COCOS_RES_ROOT}/src"
luaSrcDir_tmp="${APP_COCOS_RES_ROOT}/src_tmp"
mv ${luaSrcDir} ${luaSrcDir_tmp}
cd "${APP_ROOT}/tools"
./pack_files/pack_files.sh -i ${luaSrcDir_tmp} -o ${luaSrcDir} -ek n_77wDDolqz -es i900_gFymA
rm -rf ${luaSrcDir_tmp}
cd $DIR

# if [[ "$buildexternalsfromsource" ]]; then
#     echo "Building external dependencies from source"
#     "$NDK_ROOT"/ndk-build -j2 -C "$APP_ANDROID_ROOT" $* \
#         "NDK_MODULE_PATH=${COCOS2DX_ROOT}:${COCOS2DX_ROOT}/cocos2dx/platform/third_party/android/source"
# else
#     echo "Using prebuilt externals"
#     "$NDK_ROOT"/ndk-build -j2 -C "$APP_ANDROID_ROOT" $* \
#         "NDK_MODULE_PATH=${COCOS2DX_ROOT}:${COCOS2DX_ROOT}/cocos2dx/platform/third_party/android/prebuilt"
# fi
