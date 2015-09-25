//
//  CCEditBoxLoader.h
//  IF
//
//  Created by chenliang on 14-3-24.
//
//

#ifndef __IF__CCEditBoxLoader__
#define __IF__CCEditBoxLoader__

#include "CommonInclude.h"

NS_CC_EXT_BEGIN

/* Forward declaration. */
class CCBReader;

class CCEditBoxLoader : public ControlButtonLoader {
public:
    CCEditBoxLoader() {};
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CCEditBoxLoader, loader);
    
protected:
    //CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(CCEditBox);
    virtual CCEditBox * createCCNode(cocos2d::CCNode * pParent, cocos2d::extension::CCBReader * pCCBReader);
};

NS_CC_EXT_END


#endif /* defined(__IF__CCEditBoxLoader__) */
