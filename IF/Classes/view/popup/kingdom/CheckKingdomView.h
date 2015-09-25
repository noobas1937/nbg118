//
//  CheckKingdomView.h
//  IF
//
//  Created by ganxiaohua on 14-12-9.
//
//

#ifndef __IF__CheckKingdomView__
#define __IF__CheckKingdomView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"
#include "CCMultiColTableView.h"
#include "KingdomServantCell.h"
#include "KingdomOfficalsCell.h"
#include "OfficeInfo.h"
#include "KingdomOfficersCommand.h"

class CheckKingdomView : public PopupBaseView
, public CCBSelectorResolver
 
, public CCBMemberVariableAssigner
{
public:
    static CheckKingdomView *create(int serverId=0);
    CheckKingdomView(int serverId):m_serverId(serverId){};
    
protected:
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void onChangeClick(CCObject *pSender, CCControlEvent event);
    
    void refreshView();
    int getGeneralResIndex(string pic);
    void asyDelayLoad(CCObject* p);
    void getOfficersData(CCObject* data);
    
    CCSafeObject<CCNode> m_viewBg;
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCNode> m_officalsNode;
    CCSafeObject<CCNode> m_servantNode;
    CCSafeObject<CCNode> m_topNode;
    CCSafeObject<CCLabelIFTTF> m_kingTxt;
    CCSafeObject<CCLabelIFTTF> m_allianceTxt;
    CCSafeObject<CCLabelIFTTF> m_playerTxt;
    CCSafeObject<CCLabelIF> m_officialsTxt1;
    CCSafeObject<CCLabelIF> m_officialsTxt2;
    CCSafeObject<CCNode> m_scNode;
    CCSafeObject<CCNode> m_kingNode;
    CCSafeObject<CCNode> m_giftArea;
    CCSafeObject<CCNode> m_giftNode;
    CCSafeObject<CCNode> m_oNode;
    CCSafeObject<CCNode> m_sNode;
    
    CCSafeObject<CCScrollView> m_scrollView;
    CCSafeObject<CCArray> m_data;
    CCSafeObject<CCArray> m_officalsData;
    CCSafeObject<CCArray> m_servantData;
    CCPoint m_startPos;
    int m_serverId;
    string m_kingIcon;
};

#endif /* defined(__IF__CheckKingdomView__) */
