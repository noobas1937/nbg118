
cc.FileUtils:getInstance():addSearchPath("res/")
cc.FileUtils:getInstance():addSearchPath("src/")

require 'cocos.init'
local test = require('ccbi.test')

local function main()
    print('Lua main')
    cc.SpriteFrameCache:getInstance():addSpriteFrames('test.plist')
    test.create('ccbi/test.ccbi')
end

local status, msg = xpcall(main, __G__TRACKBACK__)
if not status then
    error(msg)
end
