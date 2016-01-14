//
//  NBDLCController.hpp
//  IF
//
//  Created by gj on 11/13/15.
//
//

#ifndef NBDLCController_hpp
#define NBDLCController_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>

#define USING_DLC

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

//1. AssetsManagerEx::initManifests 如果下载目录和相应配置存在，先判断包内的配置版本号和下载目录里的配置版本号的大小。包内的配置版本号大于下载目录里的配置版本号时，删除下载目录
//2. AssetsManagerEx::parseVersion()  检查包内的配置和服务器Version配置的版本号
//3. AssetsManagerEx::parseManifest() 检查包内的配置和服务器配置的版本号
//4. 包内的配置版本号小于服务器配置版本号时开始下载

class NBDLCController
: public Ref
{
public:
    static void clean();
    
    // manifest_file_path : 打包 ipa 和 apk 时的配置
    // 引擎默认为以下文件名，游戏实现修改对应命名规则
    // 1. version_filename : version.manifest
    // 2. temp_manifest_filename : project.manifest.temp
    // 3. manifest_filename : project.manifest
    // -----------------------------------------------------
    // 1- local 目录 (manifest/local_xml_main.manifest, local_xml_version.manifest, local_xml_project.manifest.temp, local_xml_project.manifest)
    // 2- Common 目录 (manifest/Common_1_main.manifest, Common_1_version.manifest, Common_1_project.manifest.temp, Common_1_project.manifest)
    // 3- text_*.int (manifest/text_*_main.manifest, text_*_version.manifest, text_*_project.manifest.temp, text_*_project.manifest)
    // LocalController::getLanguageFileNameBasedOnUserSystem()
    static NBDLCController* create(string manifest_file_path,
                                   string version_filename,
                                   string temp_manifest_filename,
                                   string manifest_filename);
    ~NBDLCController();
    
    void start(string manifest_file_path_as_key, std::function<void(string, EventAssetsManagerEx*)> callback);
    
protected:
    NBDLCController();
    void onCompleted();
    
    AssetsManagerEx* m_pAssetsManagerEx;
};

#endif /* NBDLCController_hpp */
