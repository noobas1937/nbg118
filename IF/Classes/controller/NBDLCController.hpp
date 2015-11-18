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

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class NBDLCController
: public Ref
{
public:
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
    string m_dlcPath;
    string m_serverAssetsPath;
};

#endif /* NBDLCController_hpp */
