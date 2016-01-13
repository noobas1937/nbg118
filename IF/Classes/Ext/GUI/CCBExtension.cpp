//
//  CCBExtension.cpp
//  ccgoe
//
//  Created by 谢文杰 on 12-11-20.
//
//
#include "CCBExtension.h"
#include "CCModelLayer.h"
#include "CCLabelIF.h"
#include "CCLabelIFBMFont.h"
#include "CCEditBoxLoader.h"
#include "CCAniNode.h"
#include "ImperialScene.h"

USING_NS_CC;
using namespace std;
using namespace cocosbuilder;
NS_CC_EXT_BEGIN

CCNode* CCBLoadFile(const char * pCCBFileName, CCNode* pParent /*= NULL*/, CCObject* pOwner /*= NULL*/, bool batchFlag/*= false*/)
{
    CCLOG("CCBLoadFile : %s",pCCBFileName);
    /* get shared CCNodeLoaderLibrary */
	CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::getInstance();
    if (!ccNodeLoaderLibrary)
        return NULL;
    
    static bool initFlag = false;
    if (!initFlag) {
        ccNodeLoaderLibrary->registerCCNodeLoader("CCModelLayer", CCBCustomLayerLoader<CCModelLayer>::loader());
        ccNodeLoaderLibrary->registerCCNodeLoader("CCModelLayerColor", CCBCustomClassLoader<CCModelLayerColor,LayerColorLoader>::loader());
        ccNodeLoaderLibrary->registerCCNodeLoader("CCLabelIF", CCLabelIFLoader::loader());
        ccNodeLoaderLibrary->registerCCNodeLoader("CCLabelIFBMFont", CCLabelIFBMFontLoader::loader());
        ccNodeLoaderLibrary->registerCCNodeLoader("CCLabelIFTTF", CCLabelIFTTFLoader::loader());
        ccNodeLoaderLibrary->registerCCNodeLoader("CCEditBox", CCEditBoxLoader::loader());
        initFlag = true;
    }
    
	/* Create an CCBReader. */
    cocosbuilder::CCBReader * ccbReader = new cocosbuilder::CCBReader(ccNodeLoaderLibrary);
    if (!ccbReader)
        return NULL;
    
    // because our Resource root path is default root , so we can not just setCCBRootPath to CCBI_PREFIX , but fix it manually
//    ccbReader->setCCBRootPath(CCBI_PREFIX);
  //3.6
    //ccbReader->setBatchFlag(batchFlag);
    
    string prefix = CCBI_PREFIX;
    string __strCCBFileName = prefix + pCCBFileName;
    if(CCCommonUtils::isIosAndroidPad())  // lzy 处理高清ccbi
    {
        prefix = HDCCBI_PREFIX;
        std::string strCCBFileName(pCCBFileName);
        std::string strSuffix(".ccbi");
        // Add ccbi suffix
        if (!cocosbuilder::CCBReader::endsWith(strCCBFileName.c_str(), strSuffix.c_str()))
        {
            strCCBFileName += strSuffix;
        }
        std::string _filePath = CCFileUtils::sharedFileUtils()->getWritablePath()+"dresource/" + prefix + strCCBFileName;
        if(CCFileUtils::sharedFileUtils()->isFileExist(_filePath.c_str()))
        {
            __strCCBFileName = _filePath;
        }
        else
        {
            __strCCBFileName = prefix + strCCBFileName;
            if(!CCFileUtils::sharedFileUtils()->isFileExist(__strCCBFileName.c_str()))
            {
                __strCCBFileName = CCBI_PREFIX + strCCBFileName;
            }
        }
    }
    
	CCNode * node = ccbReader->readNodeGraphFromFile(__strCCBFileName.c_str(), pOwner,CCDirector::sharedDirector()->getWinSize());
    
    auto aniNode = dynamic_cast<CCAniNode*>(pOwner);
    if (aniNode) {
        aniNode->setAnimationManager(ccbReader->getAnimationManager());
    }else {
        auto scene = dynamic_cast<ImperialScene*>(pOwner);
        if (scene) {
            scene->setAnimationManager(ccbReader->getAnimationManager());
        }
    }
    
	if(node != NULL) {
        if (pParent)
            pParent->addChild(node);
    } else {
        CCLOG("Error in Load cocosBuilder data :%s", pCCBFileName);
    }
    ccbReader->release();
    return node;
}

void setButtonTitle(CCControlButton* btn, const char* title, float widthPercent /*=0.9*/, float heightPercent /*=0.75*/,bool isOrigin)
{
    if (title==NULL || strlen(title)==0)
        btn->setTitleForState("", CCControlStateNormal);
    
    CCSize size = btn->getPreferredSize();
    size.width*=widthPercent;
    size.height*=heightPercent;
    if (size.width<=0 || size.height<=0) {
        return;
    }
    btn->setTitleForState(title, CCControlStateNormal);
    Label* label = (Label*)btn->getTitleLabelForState(CCControlStateNormal);
    float scale;
    int fontSize=label->getSystemFontSize();
    if (!isOrigin) {
        Texture2D img;
         bool initWithString(const char *text,  const std::string &fontName, float fontSize, const Size& dimensions = Size(0, 0), TextHAlignment hAlignment = TextHAlignment::CENTER, TextVAlignment vAlignment = TextVAlignment::TOP);
        img.initWithString(title, label->getSystemFontName(),fontSize);
       // img.initWithString(title, "", 0, Device::TextAlign::CENTER, label->getFontName(), fontSize);
        scale = MIN((float)size.width / (float)img.getPixelsWide(),(float)size.height / (float)img.getPixelsHigh());
    } else {
        scale = MIN(widthPercent, heightPercent);
    }
    fontSize *= scale;
    if (fontSize > 26)
        fontSize = 26;
    
    label->setSystemFontSize(int(fontSize));
    btn->setTitleLabelForState(label, CCControlStateNormal);//注掉会偏移
    btn->setTitleForState(title, CCControlStateNormal);
    btn->setTitleForState(title, CCControlStateHighlighted);
    btn->setTitleForState(title, CCControlStateDisabled);
    return;
}

NS_CC_EXT_END