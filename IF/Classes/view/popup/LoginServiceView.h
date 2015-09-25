//
//  LoginServiceView.h
//  IF
//
//  Created by wangzhenlei on 13-9-25.
//
//

#ifndef __IF__LoginServiceView__
#define __IF__LoginServiceView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCEditLabel.h"
#include "CCLabelIF.h"
#include "CCMultiColTableView.h"

class LoginServiceView : public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
,public cocos2d::extension::CCMultiColTableViewDataSource
,public cocos2d::extension::CCMultiColTableViewDelegate
{
public:
    static LoginServiceView* create(int type);
    LoginServiceView(int type){
        m_type = type;
        m_offset= 0;
    };
    ~LoginServiceView(){};
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
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    void onNewAccountBtnClick(CCObject *pSender, CCControlEvent event);

    void onGetServerList(CCHttpClient* client, CCHttpResponse* response);
    void showTableView();
    void refreshView();

    CCSafeObject<CCScale9Sprite> m_bg;
    CCSafeObject<CCLabelIF> m_title;
    CCSafeObject<CCLabelIF> m_text1;
    CCSafeObject<CCLabelIF> m_text2;
    
    CCSafeObject<CCArray> m_arrServerList;
    CCSafeObject<CCControlButton> m_newAccountBtn;
    
    CCSafeObject<CCNode>m_listNode;
    CCSafeObject<CCNode>m_recentNode;
    CCSafeObject<CCMultiColTableView> m_tableView;
    int m_type;
    int m_selectedIndex;
    float m_offset;
};

#endif /* defined(__IF__LoginServiceView__) */
