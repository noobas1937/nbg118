//
//  GuidePlotView.h
//  IF
//
//  Created by 李锐奇 on 14-4-9.
//
//

#ifndef __IF__GuidePlotView__
#define __IF__GuidePlotView__

#include "CommonInclude.h"

class GuidePlotView :
public CCNode
, public CCBMemberVariableAssigner
{
public:
    static GuidePlotView *create(std::string did);
    
private:
    GuidePlotView(std::string did) : m_did(did){};
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual bool init();
    void refreshView();
    std::string m_did;
    
    void guideToNext();
    void guideBattleEnd();
    CCSafeObject<CCLabelIF> m_contentText;
    CCSafeObject<CCScale9Sprite> m_bg;
    CCSafeObject<CCLayerColor> m_colorLayer;
};
#endif /* defined(__IF__GuidePlotView__) */
