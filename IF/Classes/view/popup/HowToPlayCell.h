//
//  HowToPlayCell.h
//  COK
//
//  Created by zhaohf on 14-7-24.
//
//

#ifndef IF_HowToPlayCell_h
#define IF_HowToPlayCell_h

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "CCTableViewTouchIFCell.h"
class HowToPlayCell :
public CCBSelectorResolver
, public CCBMemberVariableAssigner
  
, public CCTableViewTouchIFCell
{
public:
    static HowToPlayCell* create(int idx,CCDictionary* _htpGroup);
    HowToPlayCell():m_idx(0){};
    ~HowToPlayCell(){};
    void setData(int idx);
private:
    virtual void onExit();
    virtual void onEnter();
    
    bool init(int idx,CCDictionary* _htpGroup);
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){};
    
    CCSafeObject<CCNode> m_titleNode;
    CCSafeObject<CCNode> m_contextNode;
    CCSafeObject<CCLabelIFTTF> m_titleText;
    CCSafeObject<CCLabelIFTTF> m_contextText;
    CCSafeObject<CCScale9Sprite> m_bg;
    CCSafeObject<CCScale9Sprite> m_line;
    CCSafeObject<CCScale9Sprite> m_image;
    
    int m_idx;
    CCDictionary* m_htpGroup;
};

#endif
