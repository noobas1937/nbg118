

cc.FileUtils:getInstance():addSearchPath("src/")

require "cocos.init"

local function main()
    print('Lua main')
end

local status, msg = xpcall(main, __G__TRACKBACK__)
if not status then
    error(msg)
end
