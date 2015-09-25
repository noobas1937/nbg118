//
//  CCEditBoxLoader.cpp
//  IF
//
//  Created by chenliang on 14-3-24.
//
//

#include "CCEditBoxLoader.h"


#define PROPERTY_COLOR "color"
#define PROPERTY_OPACITY "opacity"
#define PROPERTY_BLENDFUNC "blendFunc"
#define PROPERTY_FONTNAME "fontName"
#define PROPERTY_FONTSIZE "fontSize"
#define PROPERTY_HORIZONTALALIGNMENT "horizontalAlignment"
#define PROPERTY_VERTICALALIGNMENT "verticalAlignment"
#define PROPERTY_STRING "string"
#define PROPERTY_DIMENSIONS "dimensions"

NS_CC_EXT_BEGIN

CCEditBox*  CCEditBoxLoader::createCCNode(cocos2d::CCNode * pParent, cocos2d::extension::CCBReader * pCCBReader) {
    //CCEditBox* node = CCEditBox::create(CCSize(400, 30), CCScale9Sprite::create("ui_input_bg.9.png"));
    CCEditBox* node = CCEditBox::create(CCSize(400, 30), CCScale9Sprite::create());
    node->setFontColor(ccBLACK);
    
    return node;
}

NS_CC_EXT_END