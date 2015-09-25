//
//  CCStarBar.h
//  IF
//
//  Created by fubin on 14-8-26.
//
//

#ifndef IF_CCStarBar_h
#define IF_CCStarBar_h

#include "CommonInclude.h"
#include "CCClipNode.h"


class CCStarBar : public CCNode
{
public:
    CCStarBar(void){};
    ~CCStarBar(void){};
    static CCStarBar* createStarBar(int sumStar);
    bool init(int sumStar);
    virtual void onEnter();
    virtual void onExit();
    void setProgress(int star,int pro,bool islevelUp=false);
    
protected:
    void setInitAnim();
    void setOneStarUpAnim();
    void setOneStarUpAnimFinish();
    void setAllStarUpAnim();
    float m_pro;
    int m_sumStar;
    int m_curStar;
    bool m_blevel;
    bool m_bAddstar;
    vector<CCNode*> m_nodeVec;
    vector<CCSprite*> m_starVec;
};

#endif
