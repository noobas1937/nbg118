//
//  MailCell.h
//  IF
//
//  Created by 李锐奇 on 13-10-11.
//
//

#ifndef IF_MailCell_h
#define IF_MailCell_h

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "MailInfo.h"
#include "CheckBox.h"
#include "CCMultiColTableView.h"
#include "HFHeadImgNode.h"
#include "CCTableViewTouchIFCell.h"
class MailCell :
public CCTableViewTouchIFCell,
//public CCTargetedTouchDelegate,
public CCBSelectorResolver,
public CCBMemberVariableAssigner
{
public:
    static MailCell* create(MailInfo *info,CCTableView *tableView);
    void showReadPopUp();
    void setData(MailInfo *mailInfo,CCTableView *tableView);
    void setDeleteStatusFlag(bool isNeedAnim = false);
    void onDeleteStatusFlag(CCObject* p);
    void showLoadingMoreAnim();
private:
    MailCell(MailInfo *info,CCTableView *tableView) : m_mailInfo(info),m_tableView(tableView){};
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void showDelState(CCObject* obj);    void refreshStatus();
    void refreshRewardIcon(CCObject* obj);
    void refreshView();
    void addRewardIcon();
    void calculateKillandLoss();
    virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    void onPlayRemove();
    void onDeleteMail();
    void onDeleteMailCommand();
    void addPicFlag();
    
    void deleteMailByOp();
    
    CCSafeObject<Label> m_nameText;
    CCSafeObject<CCLabelIF> m_timeText;
    CCSafeObject<CCNode> m_rewardNode1;
    CCSafeObject<CCNode> m_ccbNode;
    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<CCNode> m_mailRwardIcon;
    CCSafeObject<CCSprite> m_removeBG;
    CCSafeObject<CCLabelIF> m_unreadNumText;
    CCSafeObject<CCSprite> m_unreadNumBG;
    CCSafeObject<CCNode> m_moveNode;
    CCSafeObject<CCLabelIF> m_deleteText;
    CCSafeObject<CCScale9Sprite> m_deleteBG;
    CCSafeObject<CCLabelIF> m_tipLoadingText;
    CCSafeObject<CCSprite> m_deletSpr;
    CCSafeObject<CCLayerColor> m_modelLayer;
    CCSafeObject<CCSprite> m_sprSel;
    CCSafeObject<CCNode> m_editNode;
    CCSafeObject<HFHeadImgNode> m_headImgNode;
    ccColor3B m_readColor;
    CCPoint m_touchPoint;
    CCTableView* m_tableView;
    
    int m_kill;
    int m_loss;
    int m_tabType;
    bool m_ismoveX;
    bool m_predelete;
    bool m_isAtk;
    CCSafeObject<MailInfo> m_mailInfo;
    
    int m_rightPosX;
    void resizeString(std::string &str,int len);
};

#endif
