//
//  LuaInterface.cpp
//  IF
//
//  Created by wzhy on 14-6-5.
//
//

#include "LuaInterface.h"


static LuaInterface *_instance = NULL;
LuaInterface* LuaInterface::getInstance() {
    if (!_instance) {
        _instance = new LuaInterface();
    }
    return _instance;
}

LuaInterface::LuaInterface(){
}

LuaInterface::~LuaInterface(){
}

void LuaInterface::luaGoldBuyPanel(){
    UIComponent::getInstance()->showGoldBuyPanel();
}