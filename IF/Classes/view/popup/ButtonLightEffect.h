//
//  ButtonLightEffect.h
//  IF
//
//  Created by Émilie.Jiang on 15/2/10.
//
//

#ifndef __IF__ButtonLightEffect__
#define __IF__ButtonLightEffect__

#include "CommonInclude.h"
#include "CCClipNode.h"

enum ligthEffectType
{
    NONE,
    LOTTERY,
};

class ButtonLightEffect : public CCNode
{
public:
    static ButtonLightEffect *create(CCSize &size, ccColor3B color = ccWHITE,bool autoPlay = true,float delayTime = 2.0,int type = NONE);
    void playEffect();
    void stopEffect();
    void setBlendFunc(ccBlendFunc blendFunc);
private:
    virtual bool init(CCSize &size, ccColor3B color, bool autoPlay,float delayTime,int type);
    CC_PROPERTY(ccColor3B, m_effColor, EffColor);
    CC_SYNTHESIZE(CCSize, m_effBound, EffBound);
    CCSafeObject<CCClipNode> m_clipNode;
    CCSafeObject<CCSprite> m_effSpr;
    void loopEffect(CCObject *ccObj = NULL);
    
    float m_delayTime;
    float m_playTime;
};

#endif /* defined(__IF__ButtonLightEffect__) */
