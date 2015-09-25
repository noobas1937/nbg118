//
//  UserUpgradeView.h
//  IF
//
//  Created by fubin on 14-4-8.
//
//

#ifndef IF_UserUpgradeView_h
#define IF_UserUpgradeView_h

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCMultiColTableView.h"


class UserUpgradeView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public cocos2d::extension::CCMultiColTableViewDataSource
,public cocos2d::extension::CCMultiColTableViewDelegate
{
public:
    static UserUpgradeView* create();
    UserUpgradeView();
    virtual ~UserUpgradeView();
    
protected:
    virtual void scrollViewDidScroll(CCScrollView* view){};
    virtual void scrollViewDidZoom(CCScrollView* view){};
    virtual void gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell);
	virtual cocos2d::CCSize gridSizeForTable(cocos2d::extension::CCMultiColTableView *table);
    virtual CCTableViewCell* gridAtIndex(cocos2d::extension::CCMultiColTableView *table, unsigned int idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table);
    virtual unsigned int numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable);
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
        
    void onSkillBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onOKBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onRewardBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onShareBtnClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void playLvAnim();
    void playLvParticle();
    void playhatParticle();
    void playGiftAnim(float _time);
    void playGiftEndAnim();
    void playGetRewardAnim();
    void shareSuccess(CCObject* param);
    void delayClose(float _time);
    
    CCSafeObject<CCMultiColTableView> m_tabView;
    
    //CCSafeObject<CCControlButton> m_okBtn;
//    CCSafeObject<CCControlButton> m_skillBtn;
    CCSafeObject<CCLabelIF> m_titleLabel;
//    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCLabelIF> m_lvLabel;
    CCSafeObject<CCLabelIF> m_powLabel;
    CCSafeObject<CCLabelBMFont> m_powNumLabel;
    CCSafeObject<CCLabelIF> m_skillLabel;
    CCSafeObject<CCLabelBMFont> m_skillNumLabel;
    CCSafeObject<CCNode>  m_rewardlist;
    CCSafeObject<CCScale9Sprite> m_buildBG;
    CCSafeObject<CCScale9Sprite> m_kuangBG;
    CCSafeObject<CCNode>  m_LvAnimNode;
    CCSafeObject<CCNode>  m_hatAnimNode;
    CCSafeObject<CCNode>  m_likeNode;
    CCSafeObject<CCArray> m_cellArr;
    CCSafeObject<CCLabelIFTTF> m_fbTxt;
    CCSafeObject<CCControlButton> m_fbShareBtn;
    CCSafeObject<WaitInterface> m_waitInterFace;
    
    vector<std::string> vector_item;
    vector<std::string> vector_number;
    int m_falg;
    int m_endfalg;
};

#endif
