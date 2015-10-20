//
//  HFHeadImgNode.cpp
//  HappyFarm
//
//  Created by yuanyuan on 14-5-22.
//
//

#include "HFHeadImgNode.h"
#include "HFUrlImageCache.h"
#include "UploadImageController.h"

using namespace std;

#define CREATE_SPRITE_FRAME CCLoadSprite::loadResource

bool HFHeadImgNode::init()
{
	
	if ( !CCNode::init() )
    {
        return false;
    }
    m_scale = 0.0f;
	return true;
}

HFHeadImgNode::HFHeadImgNode()
{
    m_headImgUrl = "";
    m_defaultImgUrl = "no_iconFlag.png";
    m_myPortrait = NULL;
    m_scale = 0.0f;
    m_retry = 0;
}

HFHeadImgNode::~HFHeadImgNode()
{
    
}

void HFHeadImgNode::initHeadImg(CCSprite* myPortrait, float scale){
    m_scale = scale;
    myPortrait->setScale(scale);
    myPortrait->setDisplayFrame(CREATE_SPRITE_FRAME(m_defaultImgUrl.c_str()));
    
    m_myPortrait = myPortrait;
    
    // 自己的头像
    if(m_headImgUrl.length() > 0)
    {
        HFUrlImageCache::shared()->cancelImageLoad(m_headImgUrl, this);
        m_headImgUrl.clear();
    }
    
    showHeadImg();
}

void HFHeadImgNode::initHeadImgUrl(CCSprite* myPortrait, string url, float scale){
    if (UploadImageController::shared()->getUploadImageFlag() == 0)
    {
        url = "";
    }
//    m_defaultImgUrl = url;
//    m_scale = scale;
//    myPortrait->setScale(scale);
//    myPortrait->setDisplayFrame(CREATE_SPRITE_FRAME(m_defaultImgUrl.c_str()));
    
    if (url == "") {
        return;
    }
    
    m_myPortrait = myPortrait;
    
    // 自己的头像
    if(m_headImgUrl.length() > 0)
    {
        HFUrlImageCache::shared()->cancelImageLoad(m_headImgUrl, this);
        m_headImgUrl.clear();
    }
    
    m_headImgUrl = url;
    HFUrlImageCache::shared()->loadImageAsync(m_headImgUrl, CCCallFuncO::create(this, callfuncO_selector(HFHeadImgNode::onMyHeadImgLoaded), NULL));
}

void HFHeadImgNode::initHeadImgUrl(cocos2d::CCSprite *myPortrait, PlayerInfo *player, float scale)
{
    m_defaultImgUrl = player->getPic();
    if (player->isUseCustomPic())
    {
        initHeadImgUrl(myPortrait, player->getCustomPicUrl(), myPortrait->getScale());
    }
    else
    {
        myPortrait->setDisplayFrame(CREATE_SPRITE_FRAME(player->getPic().c_str()));
    }
}

void HFHeadImgNode::initHeadImgUrl2(cocos2d::CCNode *parentNode, string url, float scale, float size, bool clearNode, CCPoint p)
{
    if (UploadImageController::shared()->getUploadImageFlag() == 0)
    {
        url = "";
    }
    
    if (url == "") {
        return;
    }
    
    m_scale = scale;
    m_size = size;
    m_clearNode = clearNode;
    m_myParentNode = parentNode;
    m_imgPoint = p;
    m_retry = 0;
    // 自己的头像
    if(m_headImgUrl.length() > 0)
    {
        HFUrlImageCache::shared()->cancelImageLoad(m_headImgUrl, this);
        m_headImgUrl.clear();
    }
    
    m_headImgUrl = url;
    HFUrlImageCache::shared()->loadImageAsync(m_headImgUrl, CCCallFuncO::create(this, callfuncO_selector(HFHeadImgNode::onMyheadImgBack), NULL));
}

// 回家显示自己的头像
void HFHeadImgNode::showHeadImg()
{
    if(true) // 上传过头像
    {
        m_headImgUrl = "";
        
        HFUrlImageCache::shared()->loadImageAsync(m_headImgUrl, CCCallFuncO::create(this, callfuncO_selector(HFHeadImgNode::onMyHeadImgLoaded), NULL));
    }
    else if(false) // 连接了facebook
    {
        
    }
    else // 默认
    {
        m_myPortrait->setDisplayFrame(CREATE_SPRITE_FRAME("no_iconFlag.png"));
    }
}

void HFHeadImgNode::onMyHeadImgLoaded(CCObject* object)
{
    // 本地上传的头像
    CCString* image = dynamic_cast<CCString*>(object);
    if (image == NULL) {
        return;
    }
    
    CCSpriteFrame* pFrame = getSpriteFrame(image->getCString());
    if (pFrame == NULL)
    {
        int removeState = remove(image->getCString());
        if (removeState==0 && m_retry<6)//重试5次
        {
            m_retry++;
            HFUrlImageCache::shared()->loadImageAsync(m_headImgUrl, CCCallFuncO::create(this, callfuncO_selector(HFHeadImgNode::onMyheadImgBack), NULL));
        }
        return;
    }
    m_retry = 0;
    CCSize ccsz = pFrame->getOriginalSizeInPixels();
    m_myPortrait->setDisplayFrame(pFrame);
    m_myPortrait->setScale(m_scale * (90.0f/ccsz.width));
    m_headImgUrl.clear();
}

void HFHeadImgNode::onMyheadImgBack(cocos2d::CCObject *pObj)
{
    CCString* image = dynamic_cast<CCString*>(pObj);
    if (image == NULL) {
        return;
    }
    
    CCSpriteFrame* pFrame = getSpriteFrame(image->getCString());
    if (pFrame == NULL)
    {
        int removeState = remove(image->getCString());
        if (removeState==0 && m_retry<6)//重试5次
        {
            m_retry++;
            HFUrlImageCache::shared()->loadImageAsync(m_headImgUrl, CCCallFuncO::create(this, callfuncO_selector(HFHeadImgNode::onMyheadImgBack), NULL));
        }
        return;
    }
    m_retry = 0;
    CCSize ccsz = pFrame->getOriginalSizeInPixels();
    CCSprite* head = CCSprite::createWithSpriteFrame(pFrame);
    if (m_clearNode)
    {
        m_myParentNode->removeAllChildren();
    }
    m_myParentNode->addChild(head);
    if (m_size > 1E-6)
    {
        CCCommonUtils::setSpriteMaxSize(head, m_size, true);
    }
    else
    {
        head->setScale(m_scale);
    }
    if (!m_imgPoint.equals(CCPointZero))
    {
        head->setPosition(m_imgPoint);
    }
    
    m_headImgUrl.clear();
}

void HFHeadImgNode::onFBImageLoaded(CCObject* object)
{
    //FaceBook头像
    CCString* image = dynamic_cast<CCString*>(object);
    if (image == NULL) {
        return;
    }
    CCSpriteFrame* spFrame = getSpriteFrame(image->getCString());
    if (spFrame == NULL)
    {
        return;
    }
    CCSize ccsz = spFrame->getOriginalSizeInPixels();
    m_myPortrait->setDisplayFrame(spFrame);
    m_myPortrait->setScale(1.7f * (50.0f/ccsz.width)*m_scale);
    m_headImgUrl.clear();
}

void HFHeadImgNode::initHeadImgBg(CCSprite* myPortraitBg)
{
//    if (GET_SESSION()->m_GameVip->isVip())
//    {
//        myPortraitBg->setDisplayFrame(CREATE_SPRITE_FRAME("gamefriendVipbg.png"));
//    }else
//    {
//        myPortraitBg->setDisplayFrame(CREATE_SPRITE_FRAME("ordinaryFd.png"));
//    }
}
void HFHeadImgNode::initHeadImg9Bg(CCScale9Sprite* myPortraitBg)
{
//    if (GET_SESSION()->m_GameVip->isVip())
//    {
//        myPortraitBg->setSpriteFrame(CREATE_SPRITE_FRAME("gamefriendVipbg.png"));
//    }else
//    {
//        myPortraitBg->setSpriteFrame(CREATE_SPRITE_FRAME("ordinaryFd.png"));
//    }
//    myPortraitBg->setPreferredSize(ccp(140.0f,147.0f));
//    myPortraitBg->setInsetLeft(47.0f);
//    myPortraitBg->setInsetTop(49.0f);
//    myPortraitBg->setInsetRight(47.0f);
//    myPortraitBg->setInsetBottom(50.0f);
}

CCSpriteFrame* HFHeadImgNode::getSpriteFrame(const char *frameName)
{
    CCAssert(frameName != NULL, "frame name cant be NULL");
    CCSpriteFrame* pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frameName);
    if(NULL != pFrame )
    {
        // 已经加载到缓存中
        return pFrame;
    }
    // 缓存中都不存在，则从单独的文件中加载
    CCSprite* sprite = CCSprite::create(frameName);
    if(NULL != sprite)
    {
        pFrame = CCSpriteFrame::createWithTexture(sprite->getTexture(), sprite->getTextureRect());
        if(NULL != pFrame)
        {
            return pFrame;
        }
    }
    
    return NULL;
}
