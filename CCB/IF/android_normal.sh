if [ $# != 2 ]; then
	echo "parameter error. format [ sh android_normal n1,n2,n3 appversion ]."
	exit 1
fi

cd `dirname $0`
excludeList='bin/':'Resources/'
cd Common

for tpsFile in ` find . -name "*.tps" `
do
	fullFileName=${tpsFile##./}
	fullFileName=${fullFileName%%.tps}
	fileName=${fullFileName##_alpha_}

	if [[ "$fullFileName" == "_alpha_Common_507" || "$fullFileName" == "_alpha_Common_502" || "$fullFileName" == "_alpha_Common_500" || "$fullFileName" == "_alpha_Common_4" || "$fullFileName" == "_alpha_Common_5"  || "$fullFileName" == "_alpha_Common_6" || "$fullFileName" == "_alpha_Common_7"  || "$fullFileName" == "_alpha_Common_8" || "$fullFileName" == "_alpha_Common_11" || "$fullFileName" == "_alpha_Common_100" || "$fullFileName" == "_alpha_Common_101" || "$fullFileName" == "_alpha_Common_102" || "$fullFileName" == "_alpha_Common_104" || "$fullFileName" == "_alpha_Common_105" || "$fullFileName" == "_alpha_Common_200" || "$fullFileName" == "_alpha_Common_201" || "$fullFileName" == "_alpha_Common_202" || "$fullFileName" == "_alpha_Common_203"  || "$fullFileName" == "_alpha_Common_204" || "$fullFileName" == "_alpha_Common_205" || "$fullFileName" == "_alpha_Common_206" || "$fullFileName" == "_alpha_Common_207" || "$fullFileName" == "_alpha_Common_208" || "$fullFileName" == "_alpha_Common_209" || "$fullFileName" == "_alpha_Common_305" || "$fullFileName" == "_alpha_Common_308" || "$fullFileName" == "_alpha_Common_310" || "$fullFileName" == "_alpha_Common_501" || "$fullFileName" == "_alpha_Common_503" || "$fullFileName" == "_alpha_Common_504" || "$fullFileName" == "_alpha_Common_505" || "$fullFileName" == "_alpha_Common_506" || "$fullFileName" == "_alpha_Common_508" || "$fullFileName" == "_alpha_Common_509" ]]; then
		TexturePacker --texture-format png --png-opt-level 0 --opt RGBA8888 --size-constraints POT --sheet "$fullFileName.png" --data "$fileName.plist" $tpsFile 			
		# fix plist file from .png to .pkm
        sed -i.bak 's/'$fullFileName'.png/'$fullFileName'.pkm/g' $fileName.plist
        rm -rf *.bak


        # remember PWD for the stupid etcpack
		filePWD=$PWD
		cd "../bin"
		if [[ "$fullFileName" == "$fileName" ]]; then
			# NO alpha channel
			./etcpack "$filePWD/$fullFileName.png" "$filePWD" -c etc1
		else
			# HAVE alpha channel
			./etcpack "$filePWD/$fullFileName.png" "$filePWD" -c etc1 -as				
		fi

		cd $filePWD
		desDir="$filePWD/../../../Android_Resource/Common"
		if [[ ! -d $desDir ]]; then
			echo "Creating dir : $desDir"
			mkdir $desDir
		fi

		if [[ "$fullFileName" == "$fileName" ]]; then
            # NO alpha channel
            cp "$fullFileName.pkm" "$fileName.plist" $desDir
        else
            # HAVE alpha channel
            cp "$fullFileName.pkm" $fullFileName'_alpha.pkm' "$fileName.plist" $desDir
            rm -rf $fullFileName'_alpha.pkm'				
        fi
        rm -rf "$fullFileName.pkm" "$fullFileName.png" "$fileName.plist"

	fi
	echo "File $fileName finished!"
done

cd $filePWD
cd ..
sh ios_normal.sh

cd $filePWD
cd ..
python android_half.py 1.0.1 $1 $2