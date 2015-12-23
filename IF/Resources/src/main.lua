
cc.FileUtils:getInstance():addSearchPath("res/")
cc.FileUtils:getInstance():addSearchPath("src/")

require 'cocos.init'
local director = cc.Director:getInstance()
local textureCache = director:getTextureCache()
local spriteFrameCache = cc.SpriteFrameCache:getInstance()
local test = require('ccbi.test')

local function main()
    print('Lua main')
    -- 手动加载 test.ccbi 中用到的图片
    spriteFrameCache:addSpriteFrames('test.plist')

    -- local node = test.create('ccbi/test.ccbi')

    -- 手动释放 test.ccbi 中用到的图片
    spriteFrameCache:removeSpriteFramesFromFile('test.plist')
    textureCache:removeUnusedTextures()
end

local status, msg = xpcall(main, __G__TRACKBACK__)
if not status then
    error(msg)
end
