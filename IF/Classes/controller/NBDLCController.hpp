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
    static NBDLCController* create(string manifest_file_path);
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
