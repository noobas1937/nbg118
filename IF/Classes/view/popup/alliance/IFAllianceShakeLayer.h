//
//  IFAllianceShakeLayer.h
//  IF
//
//  Created by wangdianzhen on 15/8/26.
//
//

#ifndef __IF__IFAllianceShakeLayer__
#define __IF__IFAllianceShakeLayer__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "AllianceInfo.h"
#include "CCClipNode.h"

class IFAllianceShakeLayer: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static IFAllianceShakeLayer* create();
    IFAllianceShakeLayer();
    ~IFAllianceShakeLayer();
    void refesh();
protected:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
private:

    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

    void onClickOKBtn(CCObject *pSender, CCControlEvent event);
    void joinSuccess(CCObject* p);
    void onClickCloseBtn(CCObject *pSender, CCControlEvent event);
    void onClickShakeBtn(CCObject *pSender, CCControlEvent event);

    void  nodeAni();
    void initLayerInfo();
    void success(CCObject* p);
   private:
    CCSafeObject<CCLabelIFTTF> m_nameTxt;
    CCSafeObject<CCLabelIFTTF> m_leaderTxt;
    CCSafeObject<CCLabelIFTTF> m_allianceNumTxt;
    CCSafeObject<CCLabelIFTTF> m_language;
    CCSafeObject<CCLabelIFTTF> m_forceTxt;
    CCSafeObject<CCLabelIFTTF> m_msg;
    CCSafeObject<CCLabelIFTTF> m_coinTxt;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCNode> m_bg;
    CCSafeObject<CCNode> m_allianceIcon;
    CCSafeObject<CCNode> m_viewNode;
    CCSafeObject<CCNode> m_msgNode;
    
    CCSafeObject<CCControlButton> m_shakebtn;
    CCSafeObject<CCControlButton> m_okBtn;
    CCSafeObject<CCSprite> m_loadingIcon;

    CCSafeObject<CCNode> m_viewNode2;
    CCSafeObject<AllianceInfo> m_allianceInfo;
};

#endif /* defined(__IF__IFAllianceShakeLayer__) */
