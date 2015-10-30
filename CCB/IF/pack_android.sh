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


			if [[ "$fullFileName" == "_alpha_nb_ocean" || "$fullFileName" == "_alpha_decoration" || "$fullFileName" == "_alpha_throne" || "$fullFileName" == "_alpha_World_1" || "$fullFileName" == "_alpha_World_2" || "$fullFileName" == "_alpha_World_4" || "$fullFileName" == "_alpha_World_5" ]]; then
				TexturePacker --texture-format png --png-opt-level 0 --reduce-border-artifacts --opt RGBA8888 --size-constraints POT --sheet "$fullFileName.png" --data "$fileName.plist" $tpsFile
            #elif [[ "$fullFileName" == "_alpha_World_3" || "$fullFileName" == "_alpha_Imperial_30" || "$fullFileName" == "_alpha_Loading_1" || "$fullFileName" == "_alpha_Common_511" || "$fullFileName" == "_alpha_Loading_3" ]]; then
            elif [[ "$fullFileName" == "_alpha_World_3"  || "$fullFileName" == "_alpha_Loading_1" || "$fullFileName" == "_alpha_Common_511" || "$fullFileName" == "_alpha_Loading_3" ]]; then
                TexturePacker --format libgdx --texture-format png --png-opt-level 0 --opt RGBA8888 --size-constraints POT --sheet "$fullFileName.png" --data "$fileName.atlas" $tpsFile
            	#TexturePacker --format libgdx --texture-format png --png-opt-level 0 --opt ALPHA    --size-constraints POT --sheet "$alphaFullFileName.png" --data "drop.plist" "$fullFileName.png"
				#TexturePacker --format libgdx --texture-format pkm with etc1 --png-opt-level 0  --opt ETC1 --size-constraints POT --sheet "$fullFileName.pkm" --data "drop.plist" "$fullFileName.png"
			elif [[ "$fullFileName" == "_alpha_Imperial_30" ]]; then
                TexturePacker --format libgdx --texture-format png --png-opt-level 0 --opt RGBA8888 --size-constraints POT --premultiply-alpha --sheet "$fullFileName.png" --data "$fileName.atlas" $tpsFile
			elif [[ "$fullFileName" == "_alpha_Common_502" ]]; then
				TexturePacker --texture-format png --png-opt-level 0 --opt RGBA8888  --scale 0.5 --algorithm Basic --size-constraints POT --sheet "$fullFileName.png" --data "$fileName.plist" $tpsFile
			elif [[ "$fullFileName" == "_alpha_Common_500" || "$fullFileName" == "_alpha_Common_4" || "$fullFileName" == "_alpha_Common_5"  || "$fullFileName" == "_alpha_Common_6" || "$fullFileName" == "_alpha_Common_7"  || "$fullFileName" == "_alpha_Common_8" || "$fullFileName" == "_alpha_Common_11" || "$fullFileName" == "_alpha_Common_100" || "$fullFileName" == "_alpha_Common_101" || "$fullFileName" == "_alpha_Common_102" || "$fullFileName" == "_alpha_Common_104" || "$fullFileName" == "_alpha_Common_105" || "$fullFileName" == "_alpha_Common_200" || "$fullFileName" == "_alpha_Common_201" || "$fullFileName" == "_alpha_Common_202" || "$fullFileName" == "_alpha_Common_203"  || "$fullFileName" == "_alpha_Common_204" || "$fullFileName" == "_alpha_Common_205" || "$fullFileName" == "_alpha_Common_206" || "$fullFileName" == "_alpha_Common_207" || "$fullFileName" == "_alpha_Common_208" || "$fullFileName" == "_alpha_Common_209" || "$fullFileName" == "_alpha_Common_305" || "$fullFileName" == "_alpha_Common_308" || "$fullFileName" == "_alpha_Common_310" || "$fullFileName" == "_alpha_Common_501" || "$fullFileName" == "_alpha_Common_503" || "$fullFileName" == "_alpha_Common_504" || "$fullFileName" == "_alpha_Common_505" || "$fullFileName" == "_alpha_Common_506" || "$fullFileName" == "_alpha_Common_508" || "$fullFileName" == "_alpha_Common_509" ]]; then
				TexturePacker --texture-format png --png-opt-level 0 --opt RGBA8888 --size-constraints POT --sheet "$fullFileName.png" --data "$fileName.plist" $tpsFile
			else
				TexturePacker --texture-format png --png-opt-level 0 --opt RGBA8888 --size-constraints POT --sheet "$fullFileName.png" --data "$fileName.plist" $tpsFile 		
			fi

            if [[ "$fullFileName" == "_alpha_World_3" || "$fullFileName" == "_alpha_Imperial_30" || "$fullFileName" == "_alpha_Loading_1" || "$fullFileName" == "_alpha_Common_511" || "$fullFileName" == "_alpha_Loading_3" ]]; then
                # fix atlas file from .png to .pkm
                sed -i.bak 's/'$fullFileName'.png/'$fullFileName'.pkm/g' $fileName.atlas
                rm -rf *.bak
            else
                # fix plist file from .png to .pkm
                sed -i.bak 's/'$fullFileName'.png/'$fullFileName'.pkm/g' $fileName.plist
                rm -rf *.bak
            fi

			# remember PWD for the stupid etcpack
			filePWD=$PWD
			cd "../bin"
			if [[ "$fullFileName" == "$fileName" ]]; then
				# NO alpha channel
				./etcpack "$filePWD/$fullFileName.png" "$filePWD" -c etc1
			else
				# HAVE alpha channel
				#./etcpack "$filePWD/$fullFileName.png" "$filePWD" -c etc1 -as
				./etcpack "$filePWD/$fullFileName.png" "$filePWD" -c etc1
				TexturePacker --texture-format png --png-opt-level 0 --opt ALPHA  --size-constraints AnySize --disable-rotation --border-padding 0  --no-trim --sheet "$filePWD/tempalpha.png" --data "$filePWD/drop.plist" "$filePWD/$fullFileName.png"
				./etcpack "$filePWD/tempalpha.png" "$filePWD" -c etc1
				mv "$filePWD/tempalpha.pkm"  $filePWD'/'$fullFileName'_alpha.pkm'
				rm -f "$filePWD/drop.plist"
				rm -f "$filePWD/tempalpha.png"
			fi

			cd $filePWD
			desDir="$filePWD/../../../Android_Resource/$fileType"
			if [[ ! -d $desDir ]]; then
				echo "Creating dir : $desDir"
				mkdir $desDir
			fi

            if [[ "$fullFileName" == "_alpha_World_3" || "$fullFileName" == "_alpha_Imperial_30" || "$fullFileName" == "_alpha_Loading_1" || "$fullFileName" == "_alpha_Common_511" || "$fullFileName" == "_alpha_Loading_3" ]]; then
                if [[ "$fullFileName" == "$fileName" ]]; then
                    # NO alpha channel
                    cp "$fullFileName.pkm" "$fileName.atlas" $desDir
                else
                    # HAVE alpha channel
                    cp "$fullFileName.pkm" $fullFileName'_alpha.pkm' "$fileName.atlas" $desDir
                    rm -rf $fullFileName'_alpha.pkm'
                fi
                rm -rf "$fullFileName.pkm" "$fullFileName.png" "$fileName.atlas"
            else
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
	)
done

