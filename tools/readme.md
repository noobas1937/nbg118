##### icon_gen.py
    i. 根据 iTunesArtwork.png 生成所有 app 的 icon

##### lua
    1. iOS Debug: test_xxtea_lua.sh 加密 IF/Resources/src/*.lua，原始 lua 会备份到 IF/Resources/src_tmp，iOS 版本发布和动态更新时需要注意，不要提交加密后的 lua 代码到 git
    2. iOS Release: DragonClans.xcodeproj/Build Phases/Run Script 在 Release 版本下会产生 1 中提到的效果
    3. git_checkout_test_xxtea_lua.sh: 回滚 1 和 2 加密后的 lua 代码
    4. Android: proj.android/copy_assets.sh 会加密 lua 代码
    5. ccb.command: 导出 CCB/LuaCCB/ccb 里的 ccb 到 IF/Resources/res/ccbi，已经对应的 lua 代码到 IF/Resources/src/ccbi
    6. nbg2tolua.command: 绑定 cocos2d/tools/tolua/nbg.ini 指定的 C++ 到 IF/Classes/Lua/auto 

##### Download Contents module
    notes:
    1. scheduleOnce(schedule_selector(ImperialScene::downloadXML), 3);
    2. xml 路径配置，android 遍历目录实现有问题
    3. 拆分资源的 md5 配置
        i #define VERSION_FILENAME        "version.manifest"
        i #define TEMP_MANIFEST_FILENAME  "project.manifest.temp"
        i #define MANIFEST_FILENAME       "project.manifest"
    4. 下载后的资源读取目录 NBDLCController 
         FileUtils::getInstance()->addSearchPath(p->m_serverAssetsPath, true); // 查找文件时优先搜索从服务器下载下来的文件
    5. lua binding modified: cobj = new cocos2d::extension::AssetsManagerEx(arg0, arg1, VERSION_FILENAME, TEMP_MANIFEST_FILENAME, MANIFEST_FILENAME);
    6. DynamicResourceController
    7. CCLoadSprite::doLoadCommonResourceAsync
    8. init game after update xml files and languages
    9. using DLC_flask.py to start a local DLC server, and all resources are in directory  /static/Resources_download_contents/

    -------------------------------------------------
    CDN:
    1. login:
            ssh root@50.22.64.208
            cd /data/htdocs/dragon_clans_dlc
    2. upload files
            TODO

    -------------------------------------------------
    code snippets:
    1.
    NBDLCController* dlc = NBDLCController::create("manifest/Common_1_main.manifest", "Common_1_version.manifest", "Common_1_project.manifest.temp", "Common_1_project.manifest");
        dlc->start();
    // 1- local/xml dir (manifest/local_xml_main.manifest, local_xml_version.manifest, local_xml_project.manifest.temp, local_xml_project.manifest)
    // 2- Common dir (manifest/Common_%d_main.manifest, Common_%d_version.manifest, Common_%d_project.manifest.temp, Common_%d_project.manifest)
    // 3- 
    // LocalController::getLanguageFileNameBasedOnUserSystem()
    // text_*.int (manifest/text_*_main.manifest, text_*_version.manifest, text_*_project.manifest.temp, text_*_project.manifest)

    2.
    python genDLC.py "http://10.1.37.205:5000/static/Resources_download_contents/Android_debug/" "debug" "Android" "$sh_path/AndroidManifest.xml"
    read -p "Press any key to continue." var

    3.
    LoadingScene::getDownloadContents()
    X removed ImperialScene::downloadXML(float _time)
    X removed UpdateManager

##### Fonts
    1. codes
    // font
    enum NB_FONT
    {
        NB_FONT_Bold_Border,
        NB_FONT_Bold_Regular,
        NB_FONT_Bold,
        NB_FONT_Gradient
    };
    const char * getNBFont(NB_FONT f);

    2. resources
    Arial_Bold_Border.fnt
    Arial_Bold_Border.png
    Arial_Bold_Regular.fnt
    Arial_Bold_Regular.png
    Arial_Bold.fnt
    Arial_Bold.png
    Arial_Gradient.fnt
    Arial_Gradient.png