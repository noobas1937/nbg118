//
//  WaitInterface.h
//  IF
//
//  Created by zhaohf on 13-9-04.
//
//

#ifndef __WaitInterface_
#define __WaitInterface_

#include "CCBExtension.h"
#include "CCModelLayer.h"

#define GLOBAL_BG 5673

USING_NS_CC;
USING_NS_CC_EXT;

class WaitInterface :
	public cocos2d::CCLayer
{
public:
	WaitInterface(CCPoint point,CCSize size,int touchMaskOpacity);
	~WaitInterface(void);
    
    /// @param touchMaskOpacity 屏蔽点击的遮罩透明度，-1表示不使用遮罩.
    static WaitInterface* show(CCNode * parent,CCPoint point,CCSize size,int touchMaskOpacity = 64,float delayTime=0.0f);
    void remove();

    
private:
    /// 延迟显示.
	void delayAni(float delay);
    void doShowAni(float t);
    void createTouchMask(int opacity);
    void createLoadingAni();

private:
    cocos2d::extension::CCSafeObject<cocos2d::extension::CCModelLayerColor > fullBtn;
    cocos2d::extension::CCSafeObject<cocos2d::CCSprite > sprAni;
    
    int m_touchMaskOpacity;
    CCSize m_size;
    CCPoint m_point;
};

#endif