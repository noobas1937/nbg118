//
//  TranslateOptimizeCommand.cpp
//  IF
//
//  Created by xupengzhan on 15-10-12.
//
//

#include "TranslateOptimizeCommand.h"
#include "ChatServiceCocos2dx.h"
#include "ChatController.h"

bool TranslateOptimizeCommand::handleRecieve(cocos2d::CCDictionary *dict){
    
    CCLOGFUNC("");
    if (dict->valueForKey("cmd")->compare(TRANSLATE_IMPROVE) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    if (!params) {
        return false;
    }
    
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        return false;
    }
    
    if (params->objectForKey("url") && (m_method == METHOD_ADD_TRANSLATE || m_method == METHOD_VOTE_TRANSLATE)) {
        string url = params->valueForKey("url")->getCString();
        
        CCLOGFUNCF("url:%s",url.c_str());
        if(url!="")
        {
            ChatController::getInstance()->translateOptimizationUrl = url;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            ChatServiceCocos2dx::showTranslationOptimizationFrom2dx(url.c_str());
#endif
        }
    }
    
    return true;
}
