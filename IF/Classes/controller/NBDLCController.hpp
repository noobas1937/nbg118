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
    // 1- local 目录 (local_main.manifest, local_version.manifest, local_project.manifest.temp, local_project.manifest)
    static NBDLCController* create(string manifest_file_path,
                                   string version_filename,
                                   string temp_manifest_filename,
                                   string manifest_filename);
    ~NBDLCController();
    
    void start();
    
protected:
    NBDLCController();
    void onCompleted();
    
    AssetsManagerEx* m_pAssetsManagerEx;
    string m_dlcPath;
    string m_serverAssetsPath;
};

#endif /* NBDLCController_hpp */
