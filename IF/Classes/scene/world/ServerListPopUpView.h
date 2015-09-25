//
//  ServerListPopUpView.h
//  IF
//
//  Created by 李锐奇 on 14-10-13.
//
//

#ifndef __IF__ServerListPopUpView__
#define __IF__ServerListPopUpView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "HFViewport.h"
#include "WorldController.h"

#define CITY_NUM_PER_UNIT 9
#define NUM_PER_UNIT 4
#define BASE_NUM 4
#define ADD_NUM 2
#define CELL_W 500
#define CELL_H 500
#define SHOW_DISTANCE 300

static std::vector<CCPoint> _index_to_point = {
    ccp(95, 11) * 2,
    ccp(243, 37) * 2,
    ccp(440, 82) * 2,
    
    ccp(38, 223) * 2,
    ccp(295, 188) * 2,
    ccp(433, 179) * 2,
    
    ccp(101, 347) * 2,
    ccp(269, 397) * 2,
    ccp(459, 334) * 2
};

class ServerListPopUpView : public PopupBaseView
, public CCBMemberVariableAssigner
, public ITouchDelegate
//, public CCTouchDelegate

{
public:
    static ServerListPopUpView *create();
    virtual void update(float delta);
private:
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    // ITouchDelegate
    virtual bool onBeginScroll(const cocos2d::CCPoint& worldLocation);
    virtual void onStepScroll(const cocos2d::CCPoint& worldLocation,const cocos2d::CCPoint& delta);
    virtual void onEndScroll(const cocos2d::CCPoint& worldLocation);
    virtual void onSingleTouchBegin(const cocos2d::CCPoint& worldLocation);
    virtual void onSingleTouchEnd(const cocos2d::CCPoint& worldLocation);
    virtual void onStepZoom(const cocos2d::CCPoint& point1, const cocos2d::CCPoint& point2);
    virtual void onBeginZoom(const CCPoint& point1,const CCPoint& point2);
    virtual void onEndZoom(float targetZoom);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void gotoWorld();
    void refreshServerList(int drawNum = 9);
    void showBG();
    void parseServerInfo(CCArray *arr);
    static int getIndexByPt(CCPoint &pt, int xNum);
    CCPoint getPointByIndex(int index);
    void addServerCell(ServerListInfo &info);
    void addLines();
    void doWhenDataBack(CCObject* obj);
    static CCPoint getRealPt(CCPoint pt);
//    void sortBanner();
    
    CCSafeObject<CCSpriteBatchNode> m_bgBatch;
    CCSafeObject<CCSpriteBatchNode> m_lineBatch;
    CCSafeObject<CCSpriteBatchNode> m_iconBatch;
    CCSafeObject<CCNode> m_flagBatch;
    CCSafeObject<CCSpriteBatchNode> m_flagPoleBatch;
//    CCSafeObject<CCSpriteBatchNode> m_allianceFlagBatch;

    CCSafeObject<CCNode> m_serverNameNode;
    CCSafeObject<CCNode> m_pointerNode;
    CCSafeObject<HFViewport> m_viewPort;
    CCSafeObject<CCNode> m_container;
    CCSafeObject<CCSprite> m_returnBtn;
    CCSafeObject<CCLabelIF> m_title;
    CCSafeObject<CCSprite> m_bannerStaticsBtn;

    void addTitle();
    int totalH;
    int totalW;
    int xNum;
    int yNum;
    std::map<int, int> m_lines;
    CCSafeObject<CCArray> m_serverToDraw;
    int m_jumpId;
    CCPoint centerPt;
    int addCityNum;
};

class ChangeServerCover : public CCNode{
public:
    static ChangeServerCover *create(int serverId);

private:
    bool init(int serverId);
    void onEnter();
    void onExit();
    void onTimeOut(float _time);
};

#endif /* defined(__IF__ServerListPopUpView__) */
