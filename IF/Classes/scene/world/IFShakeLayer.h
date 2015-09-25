//
//  IFShakeLayer.h
//  IF
//
//  Created by wangdianzhen on 15/8/18.
//
//

#ifndef __IF__IFShakeLayer__
#define __IF__IFShakeLayer__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"
class IFShakeLayer:public PopupBaseView,
public CCBSelectorResolver,
public CCBMemberVariableAssigner
{
public:
    IFShakeLayer(int tileIndex,int monsterId);
    ~IFShakeLayer(){};
    static IFShakeLayer* create(int tileIndex,int monsterId);
public:
    void refreshInfo(int index,int monsterId);
private:
    bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode);
    void onGotoPoint(CCObject* pSender,CCControlEvent event);
    void onClose(CCObject* pSender,CCControlEvent event);
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void initLayerInfo(bool needAnimation = false);
private:
    CCSafeObject<CCLabelIF> m_titleLb;
    CCSafeObject<CCLabelIF> m_subTitleLb;
    CCSafeObject<CCLabelIF> m_infoLb;
    CCSafeObject<CCLabelIF> m_disLb;
    CCSafeObject<CCLabelIF> m_pointLb;
    CCSafeObject<CCSprite> m_shakeSp;
    CCSafeObject<CCSprite> m_toushSp;
    
    CCSafeObject<CCControlButton> m_gotoBtn;
    CCSafeObject<CCControlButton> m_closeBtn;
    
    CCSafeObject<CCNode> m_picNode;
    
    CCSafeObject<CCNode> m_rightNode;
    CCSafeObject<CCNode> m_touchNode;
    
private:
    int m_tileIndex;
    int m_monsterId;
    CCPoint m_tilePoint;
    
    
};

#endif /* defined(__IF__IFShakeLayer__) */
