//
//  HFHeadImgNode.h
//  HappyFarm
//
//  Created by yuanyuan on 14-5-24.
//
//

#ifndef __HappyFarm__HFHeadImgNode__
#define __HappyFarm__HFHeadImgNode__

#include "CommonInclude.h"

class HFHeadImgNode : public CCNode
{
public:
	virtual bool init();
	CREATE_FUNC(HFHeadImgNode);
	HFHeadImgNode();
	virtual ~HFHeadImgNode();
    void initHeadImg(CCSprite* myPortrait, float scale = 1.0f);
    void initHeadImgUrl(CCSprite* myPortrait, string url, float scale = 1.0f);
    void initHeadImgUrl(CCSprite* myPortrait, PlayerInfo* player, float scale = 1.0f);
    void initHeadImgUrl2(CCNode* parentNode, string url, float scale = 1.0f, float size = 0.0f, bool clearNode = false, CCPoint p = CCPointZero);
    void setImgScale(float scale);
    
    void initHeadImgBg(CCSprite* myPortraitBg);
    void initHeadImg9Bg(CCScale9Sprite* myPortraitBg);
//    void initFriendCellHeadImgBg(CCSprite* myPortraitBg,int isvip);
private:
    void showHeadImg();
    void onMyHeadImgLoaded(CCObject* object);
    void onMyheadImgBack(CCObject* pObj);
    void onFBImageLoaded(CCObject* object);
    CCSpriteFrame* getSpriteFrame(const char* frameName);
    
    string m_headImgUrl;
    CCSafeObject<CCSprite> m_myPortrait;
    CCSafeObject<CCNode> m_myParentNode;
    
    float m_scale;
    float m_size;
    bool m_clearNode;
    CCPoint m_imgPoint;
    string m_defaultImgUrl;
    int m_retry;
};

#endif /* defined(__HappyFarm__HFHeadImgNode__) */
