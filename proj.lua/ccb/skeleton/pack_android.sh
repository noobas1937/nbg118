cd `dirname $0`
excludeList='bin/':'Resources/'
for dir in **/
do
	(
		if [[ $excludeList =~ $dir ]]; then
			echo "This dir do not need pack : $dir"
			break
		fi
		fileType=${dir%%/}
		echo "The file type is $fileType"
		cd $dir
		for tpsFile in ` find . -name "*.tps" `
		do
			fullFileName=${tpsFile##./}
			fullFileName=${fullFileName%%.tps}
			fileName=${fullFileName##_alpha_}
			#if [[ "$fullFileName" == "_alpha_World_1" ||  "$fullFileName" == "_alpha_World_2" || "$fullFileName" == "_alpha_World_4" ]]; then
			#	TexturePacker --texture-format png --png-opt-level 0 --reduce-border-artifacts --opt RGBA8888 --size-constraints POT --sheet "$fullFileName.png" --data "$fileName.plist" $tpsFile
            #elif [[ "$fullFileName" == "_alpha_World_3" || "$fullFileName" == "_alpha_World_5" || "$fullFileName" == "_alpha_Imperial_30" || "$fullFileName" == "_alpha_Loading_4"  || "$fullFileName" == "_alpha_Loading_5" ]]; then
                TexturePacker --format libgdx --texture-format png --png-opt-level 0 --opt RGBA8888 --size-constraints POT --sheet "$fullFileName.png" --data "$fileName.atlas" $tpsFile
			#else
			#	TexturePacker --texture-format png --png-opt-level 0 --opt RGBA8888 --size-constraints POT --sheet "$fullFileName.png" --data "$fileName.plist" $tpsFile 			
			#fi

            #if [[ "$fullFileName" == "_alpha_World_3" || "$fullFileName" == "_alpha_World_5" || "$fullFileName" == "_alpha_Imperial_30" || "$fullFileName" == "_alpha_Loading_4"  || "$fullFileName" == "_alpha_Loading_5"  ]]; then
                # fix atlas file from .png to .pkm
                sed -i.bak 's/'$fullFileName'.png/'$fullFileName'.pkm/g' $fileName.atlas
                rm -rf *.bak
            #else
                # fix plist file from .png to .pkm
            #    sed -i.bak 's/'$fullFileName'.png/'$fullFileName'.pkm/g' $fileName.plist
            #    rm -rf *.bak
            #fi

			# remember PWD for the stupid etcpack
			filePWD=$PWD
			cd "../bin"
			#if [[ "$fullFileName" == "$fileName" ]]; then
				# NO alpha channel
			#	./etcpack "$filePWD/$fullFileName.png" "$filePWD" -c etc1
			#else
				# HAVE alpha channel
				./etcpack "$filePWD/$fullFileName.png" "$filePWD" -c etc1 -as				
			#fi

			cd $filePWD
			#desDir="$filePWD/../Android_Resource/$fileType"
			desDir="$filePWD/../../../lua/skeleton/android"
			if [[ ! -d $desDir ]]; then
				#echo "Creating dir : $desDir"
				#mkdir $desDir
				echo "error dir not exit: $desDir"
			fi

            #if [[ "$fullFileName" == "_alpha_World_3" || "$fullFileName" == "_alpha_World_5" || "$fullFileName" == "_alpha_Imperial_30" || "$fullFileName" == "_alpha_Loading_4"  || "$fullFileName" == "_alpha_Loading_5"  ]]; then
                #if [[ "$fullFileName" == "$fileName" ]]; then
                    # NO alpha channel
                #    cp "$fullFileName.pkm" "$fileName.atlas" $desDir
                #else
                    # HAVE alpha channel
                    cp "$fullFileName.pkm" $fullFileName'_alpha.pkm' "$fileName.atlas" $desDir
                    rm -rf $fullFileName'_alpha.pkm'
                #fi
                rm -rf "$fullFileName.pkm" "$fullFileName.png" "$fileName.atlas"
            #else
            #    if [[ "$fullFileName" == "$fileName" ]]; then
                    # NO alpha channel
            #        cp "$fullFileName.pkm" "$fileName.plist" $desDir
            #    else
                    # HAVE alpha channel
            #        cp "$fullFileName.pkm" $fullFileName'_alpha.pkm' "$fileName.plist" $desDir
            #        rm -rf $fullFileName'_alpha.pkm'				
            #    fi
            #    rm -rf "$fullFileName.pkm" "$fullFileName.png" "$fileName.plist"
            #fi

			echo "File $fileName finished!"
		done
	)
done

