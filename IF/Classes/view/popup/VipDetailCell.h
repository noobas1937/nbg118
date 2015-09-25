//
//  VipDetailCell.h
//  IF
//
//  Created by chenliang on 14-3-18.
//
//

#ifndef __IF__VipDetailCell__
#define __IF__VipDetailCell__


#include "CommonInclude.h"


class VipDetailCell :
public CCTableViewCell
 
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static const float CELL_WIDTH;
    static const float CELL_HEIGHT;
    
    static VipDetailCell* create(int index);
    VipDetailCell(int index):m_index(index){};
    
    ~VipDetailCell()
    {
        
    }
    
    void setData(int index);
    float getHG();
private:
    bool init();
    virtual void onEnter();
    virtual void onExit();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
   

    CCSafeObject<CCLabelIF> m_leftText;
    CCSafeObject<CCLabelIF> m_rightText;
    CCSafeObject<CCNode> m_leftPicNode;
    CCSafeObject<CCNode> m_rightPicNode;
    CCSafeObject<CCScale9Sprite> m_bg;
    CCSafeObject<CCNode> m_listNode;
    
    
    //CCScrollView* m_infoScrollView;
    int m_index;
    
};

class VipEffectCell :public CCNode
 
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:

    static VipEffectCell* create(string type1,int value1,string type2,int value2);
    VipEffectCell(string type1,int value1,string type2,int value2):m_type1(type1),m_value1(value1),m_type2(type2),m_value2(value2){};
    
    ~VipEffectCell()
    {
        
    }
    
    void setData(string type1,int value1,string type2,int value2);
    float getHG();
private:
    bool init();
    virtual void onEnter();
    virtual void onExit();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    CCSafeObject<CCLabelIF> m_leftValueText;
    CCSafeObject<CCLabelIF> m_rightValueText;
    CCSafeObject<CCLabelIF> m_leftText;
    CCSafeObject<CCLabelIF> m_rightText;
    CCSafeObject<CCNode> m_leftPicNode;
    CCSafeObject<CCNode> m_rightPicNode;
    CCSafeObject<CCNode> m_showNode1;
    CCSafeObject<CCNode> m_showNode2;
   // CCSafeObject<CCScale9Sprite> m_bg;
    CCSafeObject<CCSprite> m_rightAddFlag;
    
    //CCScrollView* m_infoScrollView;
    string m_type1;
    int m_value1;
    int m_value2;
    string m_type2;
    
};
#endif /* defined(__IF__VipDetailCell__) */
