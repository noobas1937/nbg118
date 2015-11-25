##### icon_gen.py
i. 根据 iTunesArtwork.png 生成所有 app 的 icon

##### DLC
i. 可供下载资源的生成

##### lua
1. iOS Debug: test_xxtea_lua.sh 加密 IF/Resources/src/*.lua，原始 lua 会备份到 IF/Resources/src_tmp，iOS 版本发布和动态更新时需要注意，不要提交加密后的 lua 代码到 git
2. iOS Release: DragonClans.xcodeproj/Build Phases/Run Script 在 Release 版本下会产生 1 中提到的效果
3. git_checkout_test_xxtea_lua.sh: 回滚 1 和 2 加密后的 lua 代码
4. Android: proj.android/copy_assets.sh 会加密 lua 代码
5. ccb.command: 导出 CCB/LuaCCB/ccb 里的 ccb 到 IF/Resources/res/ccbi，已经对应的 lua 代码到 IF/Resources/src/ccbi
6. nbg2tolua.command: 绑定 cocos2d/tools/tolua/nbg.ini 指定的 C++ 到 IF/Classes/Lua/auto 