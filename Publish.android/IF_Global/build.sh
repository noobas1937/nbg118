#!/bin/sh
sourceroot=../
packageroot=../../package
soRootPath=../../package-so
logRoot=$packageroot

mkdir $soRootPath >/dev/null 2>/dev/null
mkdir $logRoot >/dev/null 2>/dev/null
curDate=`date '+%Y%m%d'`
#echo $curDate
curTime=`date '+%H%M-%S'`
#echo $curTime
logfileName=$logRoot/$curDate/$BUILD_NUMBER-$curDate-$curTime
#echo $logfileName
mkdir $logRoot/$curDate >/dev/null 2>/dev/null

apkSrcPath=./bin/NBG_Global-release.apk
soPath=./libs/armeabi/libgame.so
apkTargetDir=$packageroot/$curDate
soTargetDir=$soRootPath/$curDate
mkdir $soTargetDir >/dev/null 2>/dev/null

apkTargetPath=$apkTargetDir/$BUILD_NUMBER-NBG_google-$curDate-$curTime.apk
apkTargetLastPath=$apkTargetDir/NBG_google.apk

if [ ! -n "$ANDROID_HOME" ]; then
echo "Error:Need to specify ANDROID_HOME first"
exit -1
fi

echo "0.change Version Name"
echo "****************Need SVN update*********************"
FILENAME="../versionName.bin"
cat $FILENAME | while read LINE
do
	echo $LINE
	sed -i.bak 's/'$LINE'/g' AndroidManifest.xml
	rm -rf *.bak
done
echo "[Done]"
echo ""

echo "building start with log:$logfileName"
echo "*************************************************"
oldBuildNumber=999
sed -i.bak 's/'$oldBuildNumber'/'$BUILD_NUMBER'/g' AndroidManifest.xml
rm -rf *.bak

cd ../../Android_Resource >/dev/null 2>/dev/null
rm -rf *

cd ../CCB/IF >/dev/null 2>/dev/null
echo "0.pack_all.command..."
sh pack_android.sh
# guo jiang todo
cp  -rf ./Imperial/Imperial_41/*.jpg ../../Android_Resource/Imperial/
echo "[Done]"
echo ""

cd ../../IF/Resources >/dev/null 2>/dev/null
echo "remove ios resources"
rm -rf Icon*.png 
rm -rf Default*.png

#cd - >/dev/null 2>/dev/null
cd ../../proj.android/jni >/dev/null 2>/dev/null
echo "1.Making inclues and source..."
sh mksrc.sh
echo "[Done]"
echo ""

cd .. >/dev/null 2>/dev/null
echo "2.Building for libgame.so..."
#echo $logfileName
sh build_native.sh  #>$logfileName.log 2>$logfileName.err
python build_native.py -b release
if [ -f "$soPath" ]; then
	echo "[Done]"
	echo ""
	rm $logfileName.err
else
	echo "[Failed]:$errorMsg"
	exit -1
fi

cd ../Publish.android/IF_Global/assets

rm -rf Battle/_alpha_Battle_a034_alpha.pkm
rm -rf Battle/_alpha_Battle_a034.pkm
rm -rf Battle/Battle_11.pkm

cd ..

mkdir $soTargetDir/IF_$BUILD_NUMBER >/dev/null 2>/dev/null
copyPath=$soTargetDir/IF_$BUILD_NUMBER/
echo $copyPath
cp -rf ../../proj.android/obj/local/armeabi $copyPath >/dev/null

echo "3.Cleaning for packing APK..."
ant clean #>$logfileName.log 2>$logfileName.err
rm -rf kit-libs
ant crashlytics-update-dependencies
sed -i.bak 's/'"manifestmerger.enabled=true"'/'"manifestmerger.enabled=false"'/g' project.properties
rm -f project.properties.bak
rm -rf kit-libs/com-crashlytics-sdk-android_crashlytics-ndk-labs/libs/arm64-v8a
rm -rf kit-libs/com-crashlytics-sdk-android_crashlytics-ndk-labs/libs/armeabi-v7a
rm -rf kit-libs/com-crashlytics-sdk-android_crashlytics-ndk-labs/libs/mips
rm -rf kit-libs/com-crashlytics-sdk-android_crashlytics-ndk-labs/libs/mips64
rm -rf kit-libs/com-crashlytics-sdk-android_crashlytics-ndk-labs/libs/x86
rm -rf kit-libs/com-crashlytics-sdk-android_crashlytics-ndk-labs/libs/x86_64
errorMsg=`cat $logfileName.err`
if [ ! -n "$errorMsg" ]; then
	echo "[Done]"
	echo ""
else
	echo "[Failed]:$errorMsg"
	exit -1
fi

echo "4.Packing APK..."
ant release #>$logfileName.log 2>$logfileName.err
errorMsg=`cat $logfileName.err`
if [ ! -n "$errorMsg" ]; then
	echo "[Done]"
	echo ""
else
	echo "[Failed]:$errorMsg"
	exit -1
fi
ant crashlytics-symbols
rm -rf AndroidManifest.xml

mkdir $apkTargetDir >/dev/null 2>/dev/null
echo "5.Copying from [$apkSrcPath] to [$apkTargetPath]..."
cp $apkSrcPath $apkTargetPath >/dev/null
cp -f $apkSrcPath $apkTargetLastPath >/dev/null
echo "[Done]"
echo ""

installScriptPath=$apkTargetDir/install.sh
echo "6.Making install script..."

	echo 'echo "1.Waiting for device..."' > $installScriptPath
	echo 'adb wait-for-device' >> $installScriptPath
	echo 'echo "Done"' >> $installScriptPath
	echo 'echo ""' >> $installScriptPath

	echo 'echo "2.Uninstalling old package..."' >> $installScriptPath
	echo 'adb uninstall org.elex.IF' >> $installScriptPath
	echo 'echo "[Done]"' >> $installScriptPath
	echo 'echo ""' >> $installScriptPath

	echo 'echo "3.Installing new package..."' >> $installScriptPath
	echo "adb install IF_debug.apk" >> $installScriptPath
	echo 'echo "[Done]"' >> $installScriptPath
	echo 'echo ""' >> $installScriptPath

	echo 'echo "****************************"' >> $installScriptPath
	echo 'echo "Complete"' >> $installScriptPath

chmod 777 $installScriptPath
echo "[Done]"
echo ""

# uploadScriptPath=$apkTargetDir/upload.sh
# echo "7.apk upload script..."
# uploadSrcPath=./upload.sh
# cp $uploadSrcPath $apkTargetDir >/dev/null

# echo "[Done]"
# echo ""

# echo "8.upload testflight"
# cd $apkTargetDir
# sh $uploadScriptPath
# echo "[Done]"
# echo ""

echo "*************************************************"



errorMsg=`cat $logfileName.err`
if [ ! -n "$errorMsg" ]; then
	echo "Complete"
	echo "You can use $installScriptPath to install the last APK"
else
	echo "Error with msg:"
	echo $errorMsg
	exit -1
fi
