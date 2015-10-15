//
//  PopupBaseView.h
//  IF
//
//  Created by 邹 程 on 13-8-29.
//
//

#ifndef __IF__PopupBaseView__
#define __IF__PopupBaseView__

#include "CommonInclude.h"
#include "CCModelLayer.h"
#include "CCAniNode.h"

class PopupViewController;

#define TOP_HEIGHT 75
#define TOP_HEIGHT_HD 154
#define DOWN_HEIGHT 105
#define DOWN_HEIGHT_HD 210
#define DEF_HEIGHT 852
#define DEF_HEIGHT_HD 2048

class PopupBaseView : public CCAniNode
{
public:
    virtual ~PopupBaseView();
    static PopupBaseView * create();
    friend class PopupViewController;
    bool init();
    void setModelLayerOpacity(int opacity);
    void setModelLayerDisplay(bool isDisplay);
    void setModelLayerColor(ccColor3B color);
    void setModelLayerPosition(CCPoint position);
    void setModelLayerContentSize(CCSize size);
    void setModelLayerTouchPriority(int _priority);
    void removeModelLayer();
    void popUpCloseAnim();
    void closeSelf();
    virtual void addToLayer(int x = -1, int y = -1);
    void removeFromLayer(bool returnFlag = true,bool isNewMailListPopup = false);
    void setUseAnimation(bool b);
    void setOpenAnimation(bool b);
    void setReturnCloseAnim();
    void setReturnPlayAnim();
    bool isAniComplete();
    void setTitleName(string _name);
    void changeBGHeight(CCNode* control);
    void changeBGMaxHeight(CCNode* control);
    float getBGHeight();
    float getExtendHeight();
    virtual void doWhenClose();
    void  PlayCloseAnim();
    void whenPalyFinishAnim();
    void playOpenFinish();
    void playReturnFinish();
    CC_SYNTHESIZE(bool, m_inFlag, InFlag);
    
    void setModelLayerTouchCallback(std::function<void(cocos2d::CCTouch *pTouch)> function);
    
    bool getIsHDPanel() {return m_isHDPanel;}
    void setHDPanelFlag(bool flag);
protected:
    bool m_useAnimation;
    bool m_bcloseAnim;
    bool m_bopenAnim;
    bool m_isHDPanel;
    CCSafeObject<CCModelLayerColor> _modelLayer;
    virtual void onRemoveAnimationPlayBack(bool returnFlag = true);
    void onRemoveAniCallBack();
    void setIsHDPanel(bool flag);
private:
    void setAniFlag();
//    int _id;
//    int m_closeId;
    CC_SYNTHESIZE(int, _id, ViewID);
    CC_SYNTHESIZE(int, m_closeId, CloseId);
    CC_SYNTHESIZE(bool, m_isSystemMail, IsSystemMail);
    bool m_isAniComplete;
    int m_x;
    int m_y;
    int m_opacity;
};

#endif /* defined(__IF__PopupBaseView__) */
