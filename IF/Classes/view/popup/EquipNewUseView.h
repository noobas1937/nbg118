//
//  EquipNewUseView.h
//  IF
//
//  Created by fubin on 15/3/5.
//
//

#ifndef __IF__EquipNewUseView__
#define __IF__EquipNewUseView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"
#include "CCClipNode.h"

#define MSG_CLICK_VIEW "msg.click.cell.view"
#define MSG_POST_NET_PUT "msg.post.net.put"
#define MSG_PLAY_EQUIP_PUT "msg.play.equip.put"
#include "CCTableViewTouchIFCell.h"
class EquipNewUseView: public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
//, public CCTouchDelegate
, public CCTableViewDataSource
, public DefaultTableViewDelegate
{
public:
    static EquipNewUseView* create(int site);
    int m_weapType;
protected:
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell);
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init(int site);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onTakeOffClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onCloseView(CCObject * obj);
    void onDelayWearfunc(float dt);
    void onOpenUpdate(CCObject * obj);
    
    void onShowWateInterface(CCObject * obj);
    void onPlayDropTable();
    void onPlayParticle();
    void onDropEnd();
    void onPlayPutOnEquip(CCObject * obj);
    void onPostPutOn();
    void onDelayTfunc(float dt);
    void refresh();
    
    //穿装备战斗力特效
    void onGetShowPowerMsg(CCObject*);
    void showPowerEffect(CCObject*);
    
    CCSafeObject<CCNode> m_bgNode;
    CCSafeObject<CCNode> m_middleNode;
    CCSafeObject<CCNode> m_middle1Node;
    CCSafeObject<CCNode> m_middle2Node;
    
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCNode> m_equipNode;
    CCSafeObject<CCNode> m_iconNode;
    CCSafeObject<CCNode> m_particle;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCControlButton> m_takeOffBtn;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_lvLabel;
    CCSafeObject<CCLabelIF> m_msgLabel;
    
    CCSafeObject<CCNode> m_msgNode;
    CCSafeObject<CCLabelIF> m_noEquipLabel;
    CCSafeObject<CCLabelIF> m_prar1Label;
    CCSafeObject<CCLabelIF> m_prar2Label;
    CCSafeObject<CCLabelIF> m_prar3Label;
    CCSafeObject<CCLabelIF> m_prar4Label;
    CCSafeObject<CCLabelIF> m_prar5Label;
    CCSafeObject<CCLabelIF> m_num1Label;
    CCSafeObject<CCLabelIF> m_num2Label;
    CCSafeObject<CCLabelIF> m_num3Label;
    CCSafeObject<CCLabelIF> m_num4Label;
    CCSafeObject<CCLabelIF> m_num5Label;
    
    CCSafeObject<CCNode> m_aniNode;
    CCSafeObject<CCNode> m_playNode;
    
    CCSafeObject<CCNode> m_shadeNode;
    WaitInterface* m_waitInterface;
    CCClipNode* m_clipNode;
    
    CCSafeObject<CCTableView> m_tabView;
    
    void generateData();
    vector<string> m_data;
    map<string, int> m_openMap;
    
    int m_count;
    CCPoint m_startPoint;
    CCSize m_tableViewSize;
    string m_willPutOnUuid;
};

//...cell class
class EquipNewUseCell :
public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCTableViewTouchIFCell
 
{
public:
    static EquipNewUseCell* create(string itemId, CCNode* touchNode, int openState);
    EquipNewUseCell():m_uuid(""){};
    void setData(string itemId, int openState);
private:
    bool init(string itemId, CCNode* touchNode, int openState);
    virtual void onEnter();
    virtual void onExit();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onPutOnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onGoToClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onHavestEquip();
    
    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<CCNode> m_particle;
    CCNode* m_parentNode;
    
    CCSafeObject<CCControlButton> m_putOnBtn;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_lvLabel;
    CCSafeObject<CCLabelIF> m_msgLabel;
    CCSafeObject<CCLabelIF> m_cannotEquipLabel;
    
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCNode> m_mainNode;
    CCSafeObject<CCNode> m_msgNode;
    CCSafeObject<CCNode> m_bottomNode;
    
    CCSafeObject<CCNode> m_equipNode;
    CCSafeObject<CCNode> m_gotoNode;
    CCSafeObject<CCLabelIF> m_gotoLabel;
    
    CCSafeObject<CCLabelIF> m_prar1Label;
    CCSafeObject<CCLabelIF> m_prar2Label;
    CCSafeObject<CCLabelIF> m_prar3Label;
    CCSafeObject<CCLabelIF> m_prar4Label;
    CCSafeObject<CCLabelIF> m_prar5Label;
    CCSafeObject<CCLabelIF> m_num1Label;
    CCSafeObject<CCLabelIF> m_num2Label;
    CCSafeObject<CCLabelIF> m_num3Label;
    CCSafeObject<CCLabelIF> m_num4Label;
    CCSafeObject<CCLabelIF> m_num5Label;
    CCSafeObject<CCLabelIF> m_desLabel;
    
    CCSafeObject<CCControlButton> m_gotoBtn;
    
    string m_uuid;
    CCPoint m_startPoint;
    int m_qid;
};

class EquipConfrimView :
public CCBSelectorResolver
,public CCBMemberVariableAssigner
 
,public PopupBaseView
{
public:
    static EquipConfrimView* create(string itemId);
    EquipConfrimView():m_itemId(""){};
    void setData(string itemId);
    string m_itemId;
    
private:
    bool init(string itemId);
    virtual void onEnter();
    virtual void onExit();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onOKClick(CCObject * pSender, Control::EventType pCCControlEvent);
    
    CCSafeObject<CCControlButton> m_okBtn;
    CCSafeObject<CCNode> m_mainNode;
    CCSafeObject<CCScale9Sprite> m_bg;
    CCSafeObject<CCNode> m_touchNode;
    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_lvLabel;
    CCSafeObject<CCLabelIF> m_msgLabel;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCNode> m_picNode;
};

//...msg cell class
class EquipMsgCell :
public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCNode
{
public:
    static EquipMsgCell* create(string msg1, string msg2, bool isAdd=true);
    EquipMsgCell(){};
private:
    bool init(string msg1, string msg2, bool isAdd);
    virtual void onEnter();
    virtual void onExit();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    CCSafeObject<CCLabelIF> m_msg1Label;
    CCSafeObject<CCLabelIF> m_msg2Label;
};

#endif /* defined(__IF__EquipNewUseView__) */
