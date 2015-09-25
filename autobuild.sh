	

#!/bin/sh

xcodebuild clean -configuration Distribution            #clean项目

currdate=`date '+%Y-%m-%d'`
distDir="/Users/hongfuzhao/.hudson/jobs/IF_IOS/workspace/package/${currdate}"
ipaTargetLastPath=$distDir/IF_TestFlight.ipa
releaseDir="build/Release-iphoneos" 
version=$BUILD_NUMBER
rm -rdf "$distDir"
mkdir -p "$distDir"
ipafilename=""
sourceid=`date '+%Y-%m-%d_%H-%M-%S'` 
echo "ipafilename=$ipaname"
echo "sourceid=$sourceid"
targetName="IF"             
appName="IF"  
echo "sourceid=$sourceid"
echo "ipafilename=$ipafilename"
rm -rdf "$releaseDir"
mkdir -p "$releaseDir"


ipapath="${distDir}/${targetName}_${version}_at_${sourceid}.ipa"

echo "*** start pak Resources file ***"
#sh ./CCB/IF/pack_all.sh

echo "*** start build app file ***"
xcodebuild -target "$targetName" -configuration Distribution -sdk iphoneos build
appfile="${releaseDir}/${appName}.app"
if [ $sourceid == "appstore" ]
then
cd $releaseDir
zip -r "${targetName}_${ipafilename}_${version}.zip" "${targetName}.app"
mv "${targetName}_${ipafilename}.zip" $distDir 2> /dev/null
cd ../..
else
echo "*** start ipa package ****"
/usr/bin/xcrun -sdk iphoneos PackageApplication -v "$appfile" -o "$ipapath"

echo "*** start delete Resources file ***"
#rm -rf IF/Resources/*

cp -f $ipapath $ipaTargetLastPath >/dev/null

#uploadScriptPath=$distDir/upload.sh
echo "7.apk upload script..."
#uploadSrcPath=./upload.sh
#cp $uploadSrcPath $distDir >/dev/null

echo "[Done]"
echo ""

echo "8.upload testflight"
#cd $distDir
#sh $uploadScriptPath
sh ./upload_ios.sh
echo "[Done]"
echo ""

echo "*************************************************"

fi
