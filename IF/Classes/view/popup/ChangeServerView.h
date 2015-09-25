//
//  ChangeServerView02.h
//  IF
//
//  Created by liweiyu on 14-9-18.
//
//

#ifndef __IF__ChangeServerView__
#define __IF__ChangeServerView__

#include "CommonInclude.h"
#include "PopupBaseView.h"

class ServerGroup : public CCObject
{
public:
    ServerGroup(CCDictionary* dict);
    ServerGroup():
    name (""),
    zone (""),
    ip (""),
    port(SERVER_PORT),
    serverId(-1)
    {
    }
    
    string name;
    string zone;
    string ip;
    int port;
    int serverId;
};
//=====================
class ChangeServeView: public PopupBaseView
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
  
, public CCTableViewDataSource
, public CCTableViewDelegate
{
public:
    static ChangeServeView* create();
private:
    ChangeServeView(){};
    ~ChangeServeView(){};
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void setData();
    void createBtns(int idx, CCDictionary* dic);
    void showTableView();
    void onGetServerList(CCHttpClient* client, CCHttpResponse* response);
    
    void onChangeClick(CCObject *pSender, CCControlEvent event);

    void setCurServer();
    
    string info_name;
    string info_ip;
    string info_zone;
    int info_port;
    int info_serverId;
    string info_gameUid;
    vector<ServerGroup*> m_serverGroupVec;
    vector<CCControlButton*> m_buttonNodes;
    int m_celeted;
    CCSafeObject<CCTableView> m_tableView;
    
    CCSafeObject<CCLabelIF> m_titleIp;
    CCSafeObject<CCLabelIF> m_titlePort;
    CCSafeObject<CCLabelIF> m_titleZone;
    CCSafeObject<CCLabelIF> m_titleGameUid;
    CCSafeObject<CCEditBox> m_ip;
    CCSafeObject<CCEditBox> m_port;
    CCSafeObject<CCEditBox> m_zone;
    CCSafeObject<CCEditBox> m_gameUid;
    CCSafeObject<CCControlButton> m_btnChange;
    CCSafeObject<CCLabelIF> m_curServer;
    CCSafeObject<CCLabelIF> m_curIp;
    CCSafeObject<CCLabelIF> m_curPort;
    CCSafeObject<CCLabelIF> m_curZone;
    CCSafeObject<CCLabelIF> m_curGameUid;
    
    CCSafeObject<CCControlButton> m_btn1;
    CCSafeObject<CCControlButton> m_btn2;
    CCSafeObject<CCControlButton> m_btn3;
    CCSafeObject<CCControlButton> m_btn4;
    CCSafeObject<CCControlButton> m_btn5;
    
    CCSafeObject<CCScale9Sprite> m_BG;
    CCSafeObject<CCNode> m_container;
    CCSafeObject<CCNode> m_serverList;
    CCPoint m_touchPoint;
protected:
    // scrollview delegate
    virtual void scrollViewDidScroll(CCScrollView* view){};
    virtual void scrollViewDidZoom(CCScrollView* view){};
    
    virtual void tableCellTouched(  cocos2d::extension::TableView* table, CCTableViewCell* cell);
	virtual cocos2d::CCSize cellSizeForTable(  cocos2d::extension::TableView *table);
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(  cocos2d::extension::TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(  cocos2d::extension::TableView *table);
//    virtual unsigned int numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable);
};

#endif /* defined(__IF__ChangeServerView__) */
