function loadLuaResource(path)
	if (nil ~=path) then
		CCSpriteFrameCache:sharedSpriteFrameCache():addSpriteFramesWithFile(path)
    end
end
function releaseLuaResource(path)
	if(nil ~= path) then
		local plistPath = path .. ".plist"
		print ("release plist: " .. plistPath)
		CCSpriteFrameCache:sharedSpriteFrameCache():removeSpriteFramesFromFile(plistPath)
		local texturePath = path .. ".pvr.ccz"
		local texture  = CCTextureCache:sharedTextureCache():textureForKey(texturePath)
        if(nil ~= texture) then
        	print ("release texture" .. texturePath)
            CCTextureCache:sharedTextureCache():removeTexture(texture)
        end
	end
end
function loadCommonResource(index,isLoad)
	LuaController:doResourceByCommonIndex(index,isLoad)
end