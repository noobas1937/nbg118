//
//  ManagerKingdomView.h
//  IF
//
//  Created by ganxiaohua on 14-12-10.
//
//

#ifndef __IF__ManagerKingdomView__
#define __IF__ManagerKingdomView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"
#include "CCMultiColTableView.h"
#include "KingdomServantCell.h"
#include "KingdomOfficalsCell.h"
#include "OfficeInfo.h"
#include "KingdomOfficersCommand.h"
#include "KingdomCheckNameCommand.h"
#define REMOVE_APPOINT_OFFICER "remove_appoint_officer"
#define KINGDOM_NAME_CHANGE "kingdom_name_change"
#define KINGDOM_BANNER_CHANGE "kingdom_banner_change"
#define KINGDOM_PALACE_CHANGE "kingdom_palace_change"

class ManagerKingdomView : public PopupBaseView
, public CCBSelectorResolver
 
, public CCBMemberVariableAssigner
{
public:
    static ManagerKingdomView *create();
    ManagerKingdomView();
    ~ManagerKingdomView();
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
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    void enableTouchSwallow(Node* tmpCCB);
    void onChangeNameClick(CCObject *pSender, CCControlEvent event);
    void onModifyResClick(CCObject *pSender, CCControlEvent event);
    void onResClick1(CCObject *pSender, CCControlEvent event);
    void onResClick2(CCObject *pSender, CCControlEvent event);
    void onResClick3(CCObject *pSender, CCControlEvent event);
    void onResClick4(CCObject *pSender, CCControlEvent event);
    
    void refreshView(CCObject* p);
    int getGeneralResIndex(string pic);
    void asyDelayLoad(CCObject* data);
    void getOfficersData(CCObject* data);
    void checkResTime(float f_time);
    void modifyOutputResSuccess(CCObject* data);
    void refreshRes(int index);
    OfficeInfo* getKing();
    void refreshKingdomName(CCObject* data);
    void refreshKingdomBanner(CCObject* data);
    void refreshKingdomPalace(CCObject* data);
    void refreshBanner();
    void refreshPalace();
    
    CCSafeObject<CCNode> m_viewBg;
    CCSafeObject<CCNode> m_resNode;
    CCSafeObject<CCNode> m_officalsNode;
    CCSafeObject<CCNode> m_servantNode;
    CCSafeObject<CCNode> m_topNode;
    CCSafeObject<CCLabelIFTTF> m_kingTxt;
    CCSafeObject<CCLabelIFTTF> m_allianceTxt;
    CCSafeObject<CCLabelIFTTF> m_playerTxt;
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCNode> m_flagNode;
    CCSafeObject<CCNode> m_kingNode;
    CCSafeObject<CCNode> m_flagArea;
    CCSafeObject<CCNode> m_giftArea;
    CCSafeObject<CCLabelIF> m_officialsTxt1;
    CCSafeObject<CCLabelIF> m_officialsTxt2;
    CCSafeObject<CCNode> m_scNode;
    CCSafeObject<CCLabelIF> m_resTxt;
    CCSafeObject<CCControlButton> m_nameBtn;
    CCSafeObject<CCControlButton> m_modifyBtn;
    CCSafeObject<CCControlButton> m_woodBtn;
    CCSafeObject<CCControlButton> m_foodBtn;
    CCSafeObject<CCControlButton> m_ironBtn;
    CCSafeObject<CCControlButton> m_stoneBtn;
    CCSafeObject<CCControlButton> m_changeBtn;
    CCSafeObject<CCEditBox> m_inputName;
    CCSafeObject<CCNode> m_oNode;
    CCSafeObject<CCNode> m_sNode;
    CCSafeObject<CCNode> m_animNode;
    CCSafeObject<CCNode> m_bannerAni;
    CCSafeObject<CCNode> m_palaceArea;
    CCSafeObject<CCNode> m_palaceNode;
    CCSafeObject<CCNode> m_palaceAni;
    CCSafeObject<CCNode> m_countryFlagNode;
    
    CCSafeObject<CCScrollView> m_scrollView;
    CCSafeObject<CCArray> m_data;
    CCPoint m_startPos;
    string m_chenckName;
    string m_kingIcon;
    string m_banner;
    string m_palace;
    bool m_chBanner;
    bool m_chThronePicFlag;
    double m_chTime;
    int m_resIndex;
    
};
#endif /* defined(__IF__ManagerKingdomView__) */
