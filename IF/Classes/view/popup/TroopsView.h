//
//  TroopsView.h
//  IF
//
//  Created by fubin on 14-8-12.
//
//

#ifndef IF_TroopsView_h
#define IF_TroopsView_h

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"
#include "MainCityMoreView.h"

class TroopMoreInfoCell;
class TroopTmpInfoCell;

class TroopsView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
  
{
public:
    static TroopsView* create(int type);
    TroopsView():m_type(0),m_tip1Pos(CCPointZero),m_tip2Pos(CCPointZero),m_tipStep(0){};
    virtual ~TroopsView(){};
    void updateInfo();
    void changeGuideTip(CCObject *ccObj = NULL);
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    
    virtual bool init(int type);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCNode> m_BGNode;
    CCSafeObject<CCScrollView> m_scrollView;
    
    void showGuideTip(int index);
    int m_tipStep;
    CCPoint m_tip1Pos;
    CCPoint m_tip2Pos;
    
    int m_type;
};

class TroopMoreInfoCell :
public CCNode
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static TroopMoreInfoCell* create(int restype, string msg);
    TroopMoreInfoCell(){};
private:
    bool init(int restype, string msg);
    virtual void onEnter();
    virtual void onExit();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    void asyDelayLoad(CCObject* p);
    void onFormation1Click(CCObject *pSender, CCControlEvent event);
    void onFormation2Click(CCObject *pSender, CCControlEvent event);
    CCSafeObject<CCLabelIF> m_troopText1;
    CCSafeObject<CCLabelIF> m_troopText2;
    CCSafeObject<CCLabelIF> m_troopText3;
    CCSafeObject<CCLabelIF> m_troopText4;
    CCSafeObject<CCLabelIF> m_troopText5;
    CCSafeObject<CCLabelIF> m_troopNumText1;
    CCSafeObject<CCLabelIF> m_troopNumText2;
    CCSafeObject<CCLabelIF> m_troopNumText3;
    CCSafeObject<CCLabelIF> m_troopNumText4;
    CCSafeObject<CCLabelIF> m_troopNumText5;
    CCSafeObject<CCNode> m_headPicNode;
    CCSafeObject<CCNode> m_moveNode;
    CCSafeObject<CCControlButton> m_formationBtn1;
    CCSafeObject<CCControlButton> m_formationBtn2;
};
class TroopTotalTmpInfoCell :
public CCNode
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static TroopTotalTmpInfoCell* create(string msg1, string msg2, int type, string msg3="");
    TroopTotalTmpInfoCell(){};
private:
    bool init(string msg1, string msg2, int type, string msg3="");
    virtual void onEnter();
    virtual void onExit();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    void onHelpBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    
    CCSafeObject<CCLabelIF> m_troopTitleText;
    CCSafeObject<CCScale9Sprite> m_bg1;
    CCSafeObject<CCScale9Sprite> m_bg2;
    CCSafeObject<CCScale9Sprite> m_bg3;
    CCSafeObject<CCScale9Sprite> m_bg4;
    CCSafeObject<CCScale9Sprite> m_titleBG;
    CCSafeObject<CCNode> m_titleNode;
    CCSafeObject<CCNode> m_listNode;
    CCSafeObject<CCControlButton> m_btnTip;
};
class TroopTmpInfoCell :
public CCNode
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
{
public:
    static TroopTmpInfoCell* create(string msg1, string msg2, string msg3="",string path1="",string path2="",string path3="",string path4="", int type=0, string msg4="",string path5="");
    TroopTmpInfoCell(){};
private:
    bool init(string msg1, string msg2, string msg3="",string path1="",string path2="",string path3="",string path4="",int type=0,string msg4="",string path5="");
    virtual void onEnter();
    virtual void onExit();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void initfortData(string msg1, string msg2, string msg3="",string path1="",string path2="",string path3="",string path4="",string msg4 = "");
    void initArmyData(string msg1, string msg2, string msg3="",string path1="",string path2="",string path3="",string path4="",string msg4 = "",string path5="");
    
    CCSafeObject<CCLabelIF> m_msg1Label;
    CCSafeObject<CCLabelIF> m_msg2Label;
    CCSafeObject<CCLabelIF> m_msg3Label;
//    CCSafeObject<CCScale9Sprite> m_bgFrame;
    CCSafeObject<CCNode> m_levelSprNode;
    CCSafeObject<CCNode> m_picHead;
    CCSafeObject<CCNode> m_picNode1;
    CCSafeObject<CCNode> m_picNode2;
    CCSafeObject<CCNode> m_fortNode;
    CCSafeObject<CCNode> m_armyNode;
    CCSafeObject<CCNode> m_foodPicNode;
    CCSafeObject<CCNode> m_freeNode;
    CCSafeObject<CCNode> m_marchNode;
    CCSafeObject<CCLabelIF> m_nameTTF;
    CCSafeObject<CCLabelIF> m_footNumTTF;
    CCSafeObject<CCLabelIF> m_freeNumTTF;
    CCSafeObject<CCLabelIF> m_marchNumTTF;
};

#endif
