//
//  MinimapView.h
//  IF
//
//  Created by 邹 程 on 14-5-5.
//
//

#ifndef __IF__MinimapView__
#define __IF__MinimapView__

#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "HFViewport.h"
#include "WorldController.h"

//static const int MINI_MAP_X_COUNT = 9;
//static const int MINI_MAP_Y_COUNT = 9;
//static const int MINI_MAP_X_ADDITIONAL = 2;
//static const int MINI_MAP_Y_ADDITIONAL = 4;
//static const int MINI_MAP_X_TOTAL = MINI_MAP_X_COUNT + MINI_MAP_X_ADDITIONAL;
//static const int MINI_MAP_Y_TOTAL = MINI_MAP_Y_COUNT + MINI_MAP_Y_ADDITIONAL;
//static const int START_X = -MINI_MAP_X_ADDITIONAL / 2;
//static const int START_Y = -MINI_MAP_Y_ADDITIONAL / 2;
//static const int

static CCPoint viewCenterPoint;

#define FIRST_IN_MINI_MAP_GUIDE "3000100"
#define MINIMAP_MIN 1.3
#define MINIMAP_MAX 1.3
#define MINIMAP_HD_SCALE 1.5
#define MINIMAP_SCALE 1

class MinimapInfo : public CCObject{
public:
    CREATE_FUNC(MinimapInfo);
    virtual bool init(){return true;};
    int selfPt;
    int chiefPt;
    std::map<int, int> m_members;//second==1为原有 second==2为联盟哨塔
};

class MinimapView : public PopupBaseView
, ITouchDelegate
//, public CCTouchDelegate
{
public:
    static MinimapView *create(int serverId);
    MinimapView(int serverId);
    ~MinimapView();
    void addToMiniMap(CCNode *node, int layerIndex);
    void closeSelf();
    static CCPoint getPointByWorldPoint(CCPoint &pt);
    static CCPoint getWorldPointByPoint(const CCPoint &pt);
    static CCPoint getRealPosByPoint(CCPoint &pt);
    static CCPoint getPosByIndex(int index);
    virtual void update(float time);    
private:
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

    void clickResourceBtn(cocos2d::CCObject *pTarget, CCControlEvent touchEvent);
    
    // ITouchDelegate
    virtual bool onBeginScroll(const cocos2d::CCPoint& worldLocation);
    virtual void onStepScroll(const cocos2d::CCPoint& worldLocation,const cocos2d::CCPoint& delta);
    virtual void onEndScroll(const cocos2d::CCPoint& worldLocation);
    virtual void onSingleTouchBegin(const cocos2d::CCPoint& worldLocation);
    virtual void onSingleTouchEnd(const cocos2d::CCPoint& worldLocation);
    virtual void onStepZoom(const cocos2d::CCPoint& point1, const cocos2d::CCPoint& point2);
    virtual void onBeginZoom(const CCPoint& point1,const CCPoint& point2);
    virtual void onEndZoom(float targetZoom);
    virtual void autoMoveEnd(){};
    void getData();
    void initMap();
    void addMapTile(CCPoint midPt, int tag);
    void addEdge(int x, int y);
    void addExtraMap(int index);
    void removeExtraMap();
    void getCurrentServerData();
    std::string getGoToServerData(int index);
    void moveBack();
    void scaleBack();
    void openResourceWithAnimation();
    void openResourceAnimationBack();
    void showResource(bool flag);
    void showResourceCallBack();
    void showFirstOpenGuide(float _time);
    void hideResource();
    void showSelfCity();
    void onMemberDataBack(CCObject *obj);
    void setAllianceMemeberVisible(bool b);
    void addWorldObject(int index);
    void clickServerBtn();
    void doClickServerBtn(float _time);
    void doClickBackBtn();
    void doClickGoBackMyServerBtn();
    void doClickGoToServerBtn(int index);
    void refreshMinimapView();
    void doClickShowMember();
    void addTitle();
    void addRelic();
    void showAllianceMember();
    void addCity(int index, int type);//0 self 1 chief 2 member
    void addLegend();
    void moveMapWithAnimation();
    
    // 表示正在加载图标的运行与停止
    void loadingIconRun();
    void loadingIconStopRun();
    
    //显示联盟标记
    void onShowMarkLine(CCObject* obj);
    void onGoToMarkPt(const cocos2d::CCPoint& worldLocation);

    //按钮动画
    void showSettingNode();
    void resetSettingNode(CCObject* ccObj = NULL);
    void showLegendEffect();
    void endShowLegendEffect();
    CCSafeObject<HFViewport> m_viewPort;
    CCSafeObject<CCNode> m_container;
    CCSafeObject<CCNode> m_bg;
    CCSafeObject<CCSpriteBatchNode> m_edgeBatchNode;
    CCSafeObject<CCSpriteBatchNode> m_mapBatchNode;
    CCSafeObject<CCNode> m_arrowBatchNode;
    CCSafeObject<CCSpriteBatchNode> m_cityBatchNode;
    CCSafeObject<CCNode> m_resourceIntroPointer;
    CCSafeObject<CCNode> m_resourceIntroText;
    CCSafeObject<CCSpriteBatchNode> m_resBeltBatchNode;
    CCSafeObject<CCNode> m_nameContainer;
    CCSafeObject<CCNode> m_phoneIcon;
    CCSafeObject<CCNode> m_markNode;
    CCSafeObject<CCNode> m_markTouchNode;
    
    CCSafeObject<CCSprite> m_serverListBtn;
    CCSafeObject<CCSprite> m_returnBtn;
    CCSafeObject<CCSprite> m_goBackMyServerBtn;
    CCSafeObject<CCSprite> m_goToServerBtn1;
    CCSafeObject<CCSprite> m_goToServerBtn2;
    CCSafeObject<CCSprite> m_goToServerBtn3;
    CCSafeObject<CCSprite> m_goToServerBtn4;
    CCSafeObject<CCSprite> m_lServerBg;
    CCSafeObject<CCSprite> m_uServerBg;
    CCSafeObject<CCSprite> m_rServerBg;
    CCSafeObject<CCSprite> m_dServerBg;
    CCSafeObject<CCLabelIF> m_lSerTitle;
    CCSafeObject<CCLabelIF> m_uSerTitle;
    CCSafeObject<CCLabelIF> m_rSerTitle;
    CCSafeObject<CCLabelIF> m_dSerTitle;
    
    CCSafeObject<CCSprite> m_loadingIcon;
    CCSafeObject<CCLabelIF> m_title;

    CCSafeObject<MinimapInfo> m_mapInfo;
    CCSafeObject<CCLabelIF> m_timeText;
    
    CCSafeObject<CCNode> m_settingNode;
    CCSafeObject<CCSprite> m_settingBtn;
    CCSafeObject<CCSprite> m_allAlianceBtn;
    CCSafeObject<CCSprite> m_alianceHideIcon;
    CCSafeObject<CCLayerColor> m_alianceTouchLayer;
    CCSafeObject<CCSprite> m_resourceBtn;
    CCSafeObject<CCLayerColor> m_resourceTouchLayer;
    CCSafeObject<CCSprite> m_hideIcon;
    bool m_InAction;
    
    CCPoint m_curPt;
    
    bool singleTouchClose;
    int m_mapW;
    int m_mapH;
    bool isAllShow;
    bool m_bCanClickServerBtn;
    int m_currentServerId;
    int m_currentServerX;
    int m_currentServerY;
    int m_lServerId;
    int m_rServerId;
    int m_uServerId;
    int m_dServerId;
    int m_selectIndex;
};
#endif /* defined(__IF__MinimapView__) */
