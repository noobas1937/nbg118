//
//  OccupyMailPopUpView.h
//  IF
//
//  Created by 李锐奇 on 14-4-16.
//
//

#ifndef __IF__OccupyMailPopUpView__
#define __IF__OccupyMailPopUpView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "CCMultiColTableView.h"
#include "QuestController.h"
#include "HFHeadImgNode.h"

class OccupyMailPopUpView : public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
, public cocos2d::extension::CCMultiColTableViewDataSource
, public cocos2d::extension::CCMultiColTableViewDelegate
{
public:
    static OccupyMailPopUpView *create(MailInfo *info);
protected:
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

protected:
    // scrollview delegate
    virtual void scrollViewDidScroll(CCScrollView* view){};
    virtual void scrollViewDidZoom(CCScrollView* view){};
    
    virtual void gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell);
	virtual cocos2d::CCSize gridSizeForTable(cocos2d::extension::CCMultiColTableView *table);
    virtual CCTableViewCell* gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table);
    virtual unsigned int numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable);
    
private:
    OccupyMailPopUpView(MailInfo * info) : m_info(info){};
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void onDeleteClick(CCObject *pSender, CCControlEvent event);
    void onBtnPosClick(CCObject *pSender, CCControlEvent event);
    void onAddSaveClick(CCObject *pSender, CCControlEvent event);
    void onReturnClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void onWriteClick(CCObject * pSender, CCControlEvent pCCControlEvent);
    void refresh(CCObject* p);
    void getData();
    void onOKDeleteMail();
    
    CCSafeObject<CCLabelIF> m_titleText;
    CCSafeObject<CCLabelIF> m_mailTitle;
    CCSafeObject<CCLabelIF> m_contentText;
    CCSafeObject<CCLabelIF> m_timeText;
//    CCSafeObject<CCLabelIF> m_occupyText;
    CCSafeObject<CCLabelIF> m_deleteBtnTitle;
    CCSafeObject<CCLabelIF> m_failText;
    CCSafeObject<Label> m_palyName;
//    CCSafeObject<CCLabelIF> m_playLv;
    CCSafeObject<CCLabelIF> m_armsName;
    CCSafeObject<CCLabelIF> m_armsNun;
    CCSafeObject<CCNode> m_playerNode;
    CCSafeObject<CCNode> m_failNode;
    CCSafeObject<CCNode> m_picHeadNode;
    CCSafeObject<CCNode> m_listContainer;
    CCSafeObject<CCNode> m_downNode;
    CCSafeObject<CCNode> m_bgNode;
    CCSafeObject<CCNode> m_armsListNode;
    CCSafeObject<CCNode> m_moveNode;
    CCSafeObject<CCNode> m_totalNode;
    CCSafeObject<CCScale9Sprite> m_kuangBG;
//    CCSafeObject<CCScale9Sprite> m_listBG;
    CCSafeObject<CCScale9Sprite> m_buildBG;
//    CCSafeObject<CCScale9Sprite> m_bg;
    CCSafeObject<CCControlButton> m_deleteBtn;
    CCSafeObject<CCControlButton> m_addSaveBtn;
    CCSafeObject<CCControlButton> m_returnBtn;
    CCSafeObject<CCSprite> m_returnSpr;
    CCSafeObject<CCControlButton> m_writeBtn;
//    CCSafeObject<CCControlButton> m_unSaveBtn;
    //CCSafeObject<CCMultiColTableView> m_tabView;
    CCSafeObject<CCScrollView> m_scrollView;
    CCSafeObject<MailInfo> m_info;
    CCSafeObject<CCScale9Sprite> m_paneBg;
    CCSafeObject<CCNode> m_battlePicNode;
    CCSafeObject<CCNode> m_listNode;
    CCSafeObject<HFHeadImgNode> m_headImgNode;
    CCSafeObject<CCRenderTexture> m_selfModelLayer;
    int m_totalHg;
    
};


class OccupyMailGeneralCell : public CCTableViewCell
,public CCBMemberVariableAssigner

{
public:
    static OccupyMailGeneralCell *create(std::string generalId, int num, int color, int level);
    void setData(std::string generalId, int num, int color, int level);
    
private:
    OccupyMailGeneralCell(std::string generalId, int num, int color, int level) : m_generalId(generalId), m_num(num), m_color(color), m_level(level){};
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual bool init();
    void refreshView();
    std::string m_generalId;
    int m_num;
    int m_color;
    int m_level;
    CCSafeObject<CCLabelIF> m_nameText;
    CCSafeObject<CCLabelIF> m_lvText;
    CCSafeObject<CCLabelIF> m_numText;
    CCSafeObject<CCNode> m_headContainer;
};

class OccupyLossPreviewCell : public CCTableViewCell

{
public:
    static OccupyLossPreviewCell *create(string type, int num, bool b = false);
    void setData(string type, int num, bool b);
    
private:
    OccupyLossPreviewCell(string type, int num, bool b) : m_type(type), m_num(num), m_about(b){};
    virtual bool init();
    void refreshView();
    string m_type;
    int m_num;
    bool m_about;
};
/////
class DetectArmyCell : public CCTableViewCell
,public CCBMemberVariableAssigner
{
public:
    static DetectArmyCell *create(string armId,int num,bool isAbout = false,bool bself = false);
    virtual bool init();
    void setData(string armId,int num,bool isAbout = false,bool bself = false);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
private:
    DetectArmyCell(string armId,int num,bool isAbout = false,bool bself = true) : m_armId(armId), m_num(num),m_isAbout(isAbout),m_self(bself){};
    void refreshView();
    string m_armId;
    int m_num;
    bool m_isAbout;
    bool m_self;
    CCSafeObject<CCLabelIF> m_nameText;
    CCSafeObject<CCLabelIF> m_nunText;
    CCSafeObject<CCNode> m_headPicNode;
    CCSafeObject<CCNode> m_levelNode;
};
#endif /* defined(__IF__OccupyMailPopUpView__) */
