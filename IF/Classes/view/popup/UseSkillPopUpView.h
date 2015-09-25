//
//  UseSkillPopUpView.h
//  IF
//
//  Created by ganxiaohua on 14-11-3.
//
//

#ifndef __IF__UseSkillPopUpView__
#define __IF__UseSkillPopUpView__
#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"
#include "CCMultiColTableView.h"
#include "CCTableViewTouchIFCell.h"
class UseSkillPopUpView : public PopupBaseView
, public CCBSelectorResolver
 
, public CCBMemberVariableAssigner
, public cocos2d::extension::CCMultiColTableViewDataSource
, public cocos2d::extension::CCMultiColTableViewDelegate
{
public:
    static UseSkillPopUpView *create();
    
protected:
    // scrollview delegate
    virtual void scrollViewDidScroll(CCScrollView* view);
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
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void onChangeClick(CCObject *pSender, CCControlEvent event);
    void onUseClick(CCObject *pSender, CCControlEvent event);
    void onPicClick(CCObject *data);
    void successCallBack(CCObject *data);
    void failCallBack(CCObject *data);
    void updateCDTime(float t);
    void refreshView();
    void sendUseSkillCommand();
    std::map<int, std::vector<int> > m_statusGroup;
    CCSafeObject<CCArray> m_data;
    CCSafeObject<CCNode> m_listContainer;
    CCSafeObject<CCNode> m_viewBg;
    CCSafeObject<CCNode> m_moveNode;
    CCSafeObject<CCLabelIF> m_skillTitle;
    CCSafeObject<CCLabelIF> m_descTxt;
    CCSafeObject<CCLabelIF> m_tipsTxt;
    CCSafeObject<CCLabelIF> m_skillName;
    CCSafeObject<CCNode> m_head;
    CCSafeObject<CCNode> m_useSkillNode;
    CCSafeObject<CCControlButton> m_useBtn;
    CCSafeObject<CCLabelIF> m_useTxt;
    CCSafeObject<CCNode> m_useNode;
    CCSafeObject<CCNode> m_clickArea;
    CCSafeObject<CCSprite> m_skillItemBg;
    CCSafeObject<CCNode> m_useBgNode;
    
    CCSafeObject<CCSprite> m_sprR;
    CCSafeObject<CCSprite> m_sprL;
    
    CCSafeObject<CCMultiColTableView> m_tabView;
    std::string m_skillId;
};

class UseSkillItemCell : public CCTableViewTouchIFCell
,public CCBSelectorResolver
 
,public CCBMemberVariableAssigner

{
public:
    static UseSkillItemCell *create(std::string pic);
    void setData(std::string pic);
    CCSafeObject<CCControlButton> m_btnReward;
private:
    UseSkillItemCell(std::string skillId):m_skillId(skillId){};
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    
    void changePic();
    void changeSuccess();
    void clickHead(CCObject* data);
    void updateTime(float t);
    
    CCSafeObject<CCLabelIF> m_timeTxt;
    CCSafeObject<CCNode> m_cdNode;
    CCSafeObject<CCNode> m_pNode;
    CCSafeObject<CCNode> m_head;
    CCSafeObject<CCNode> m_ccNode;
    CCSafeObject<CCNode> m_headNode;
    CCSafeObject<CCSprite> m_clickNode;
    bool m_isDraging;
    int m_type;
    std::string m_skillId;
    bool m_touchMove;
    float touchX;
};

#endif /* defined(__IF__UseSkillPopUpView__) */
