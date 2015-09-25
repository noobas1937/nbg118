//
//  FlyRewardPic.h
//  IF
//
//  Created by lifangkai on 14-8-12.
//
//

#ifndef __IF__FlyRewardPic__
#define __IF__FlyRewardPic__

#include "CommonInclude.h"
#include "CCLabelIF.h"
#include "ParticleController.h"

class FlyRewardPic: public CCNode
{
public:
    FlyRewardPic(string type,CCPoint spt,string picPath,float delaytime):m_type(type),m_startPoint(spt),m_path(picPath),m_delayTime(delaytime){};
    ~FlyRewardPic(){};
    bool init(string type,CCPoint spt,string picPath,float delaytime);
    static FlyRewardPic* create(string type,CCPoint spt,string picPath,float delaytime);
    static void addFlyRewardAnim(string type,CCPoint spt,string picPath,float delaytime=0.3);
    
    virtual void onEnter();
    virtual void onExit();
    void onFlyRewardDelete();
    
protected:
    CCSpriteBatchNode* m_batchNode;
    CCSafeObject<CCSprite> m_Spr;
    string m_type;
    CCPoint m_startPoint;
    string m_path;
    CCModelLayerColor* m_modalLayer;
    CCPoint m_endpoint;
private:
    void playScaleAnim();
    void playFlyAnim();
    float m_delayTime;
};

#endif /* defined(__IF__FlyRewardPic__) */
