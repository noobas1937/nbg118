cd ../../Proj.androidcommon/jni
sh mksrc.command

cd ..
sh ./build_native.sh

cd ../Publish.android/AOE_Global
ant release && ant installr