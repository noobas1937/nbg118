//
//  ArcScrollView.h
//  IF
//
//  Created by ganxiaohua on 14-5-12.
//
//

#ifndef __IF__ArcScrollView__
#define __IF__ArcScrollView__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "CCLabelIF.h"
#include "ScrollTableView.h"

#define  MSG_ARC_CELL_CLICK    "msg_arc_cell_click"
#define  MSG_TOUCH_ARC_VIEW    "msg_touch_arc_view"

class ArcInfo : public cocos2d::CCObject {
public:
    ~ArcInfo(){};
    ArcInfo(int index,string title,string icon,bool isLock=false,int iconSize=70,int srcIndex=-1,string itemId=""):m_index(index),m_title(title),m_icon(icon),isLock(isLock),maxIconSize(iconSize),m_srcIndex(srcIndex),m_id(itemId){};
    int m_index; //传出用的索引
    string m_title;//按钮的标题
    string m_icon;//按钮的图标
    bool isLock;//按钮的是否锁住
    int maxIconSize;//按钮的上的图片的最大尺寸
    int m_srcIndex;
    string m_id;
};

/** ArcScrollView* create
 * @array 数据源
 * @level 级数，一级就是直接显示，没有弹开的动画
 * @pos 打开时的位置
 */
class ArcScrollView: public Layer
 
,CCScrollViewDelegate
{
public:
    static ArcScrollView* create(CCArray* array,int level=1,int pos=0);
    ArcScrollView(CCArray* array,int level,int pos):m_data(array),m_level(level),m_pos(pos){};
    ~ArcScrollView();
    void updateInfo();
    int m_currentIndex;
    bool m_isInTouch;
    bool m_touchEnd;
    void setCallback(CCObject *target,SEL_CallFunc func);
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view){};
    void setShowPos(int pos);
    void moveOneCell(bool up=false);
    CCNode* getShowAreaByPos(int pos);
    bool getAniState();
protected:
    void getMsgFun(CCObject* param);
    void touchViewFun(CCObject* param);
    void callback();
    void asyLoad(CCObject* p);
private:
    virtual void onEnter();
    virtual void onExit();    
    virtual bool init();
    CCSafeObject<CCScale9Sprite> m_viewBg;
    CCSafeObject<CCScale9Sprite> m_clickArea;
    CCSafeObject<CCScale9Sprite> m_touchArea;
    CCSafeObject<CCScrollView> csView;
    CCSafeObject<ScrollTableView> m_scrollView;
    CCSafeObject<CCSprite> m_circle;
    bool m_isMoving;
    CCPoint m_touchPoint;
    CCObject *m_callbackTarget;
    SEL_CallFunc m_callbackFunc;
    int m_num;
    int m_cellW;
    int m_level;
    int m_pos;
    CCSafeObject<CCArray> m_data;
    CCSafeObject<CCArray> m_cells;
};

class ArcScrollCell :
public Layer
 
,public CCBMemberVariableAssigner
{
public:
    static ArcScrollCell* create(ArcInfo* info,CCNode* node,CCPoint worldPoint,int level=1);
    ArcScrollCell(ArcInfo* info,CCNode* node,CCPoint worldPoint,int level):m_info(info),m_clickNode(node),m_worldPoint(worldPoint),m_level(level){_ignoreAnchorPointForPosition=false;};
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
   virtual void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &parentTransform, uint32_t parentFlags);
    void setData(ArcInfo* info,CCNode* node,CCPoint worldPoint);
    void asyLoad();
    
    ArcInfo* m_info;
    CCSafeObject<CCSprite> m_button;
    CCPoint getBottomPoint();
    CCPoint getUpPoint();
    void playAni(float delayTime=0.0);
    bool m_haveAni;
    void playFinish();
    bool checkValide();
    int  getWorldGapY();
    bool m_isDraging;
    int m_level;
    CCPoint currentWorldPoint;
    CCSafeObject<CCNode> m_showArea;
private:
    bool init();
    virtual void onEnter();
    virtual void onExit();
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    CCSafeObject<CCLabelIF> m_buttonTxt;
    CCSafeObject<CCNode> m_buttonNode;
    CCSafeObject<CCNode> m_txtNode;
    CCNode* m_clickNode;
    CCPoint m_worldPoint;
    CCSafeObject<CCSprite> m_icon;
    CCSafeObject<CCSprite> m_lockIcon;
    CCSafeObject<CCSprite> m_LvSpr;
    CCSafeObject<CCNode> m_LvNode;
    
    CCSafeObject<CCNode> m_lockNode;
    CCSafeObject<CCNode> m_lockLvNode;
    CCSafeObject<CCLabelIF> m_buttonLockTxt;
    
    float m_maxIconScale;
};
#endif /* defined(__IF__ArcScrollView__) */
