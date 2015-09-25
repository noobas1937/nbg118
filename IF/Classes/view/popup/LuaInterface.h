//
//  LuaInterface.h
//  IF
//
//  Created by wzhy on 14-6-5.
//
//

#ifndef __IF__LuaInterface__
#define __IF__LuaInterface__

#include "CommonInclude.h"
#include "SceneController.h"
#include "UIComponent.h"
#include "PopupBaseView.h"

class LuaInterface: public CCObject
{
public:
    ~LuaInterface();
    LuaInterface();
    static LuaInterface* getInstance();
    
    void luaGoldBuyPanel();
};

#endif /* defined(__IF__LuaInterface__) */



