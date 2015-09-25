//
//  BaseTileInfo.h
//  IF
//
//  Created by 邹 程 on 14-2-26.
//
//

#ifndef __IF__BaseTileInfo__
#define __IF__BaseTileInfo__

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "WorldController.h"
#include "PopupViewController.h"
#include "UseToolView.h"
#include "ToolController.h"

class BaseTileInfo : public PopupBaseView
,public CCBMemberVariableAssigner
,public CCBSelectorResolver
{
public:
    
    BaseTileInfo(WorldCityInfo& info):
    m_cityInfo(info)
    {}
    
    static BaseTileInfo* create(WorldCityInfo& info) {
        auto ret = new BaseTileInfo(info);
        if (ret->init(true)) {
            ret->autorelease();
        } else {
            CC_SAFE_DELETE(ret);
        }
        return ret;
    }
    
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName){return NULL;}
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
    
    bool init(bool selfInit = false);
    
    virtual BaseTileInfo* getFavoriteView();
    
//    void keyPressedBtnCancel(CCObject * pSender, Control::EventType pCCControlEvent);
    void keyPressedBtnFavorite(CCObject * pSender, Control::EventType pCCControlEvent);
    
    //void setFavoButtonVisible(bool isVisible);
    
    void inline setBgHeight(float height) {
        if (m_tileBg && m_tileBg->getContentSize().height != height) {
            auto size = CCSize(m_tileBg->getContentSize().width,height);
            m_tileBg->setPreferredSize(size);
        }
    }
    float inline getBgHeight() {
        if (m_tileBg) {
            return m_tileBg->getContentSize().height;
        }
        return 0.0;
    }

    void setButtonCount(unsigned int count,bool isNeedLargeButton = true);
    unsigned int getButtonCount();

    void setButtonName(unsigned int order,const string& name);
    void setButtonCallback(unsigned int order,SEL_CCControlHandler handler);
    void setButtonState(unsigned int order,TileButtonState state);
    void setButtonState(CCControlButton* button,TileButtonState state);

    CCControlButton* getButton(unsigned int order);
    TileButtonState getButtonState(unsigned int order);
    TileButtonState getButtonState(CCControlButton* button);
    
    virtual void closeThis();
    
protected:
    WorldCityInfo& m_cityInfo;
    CCSafeObject<CCNode> m_subNode;
    CCSafeObject<CCScale9Sprite> m_tileBg;
    CCSafeObject<CCLabelIF> m_title;
    
private:
    CCSafeObject<CCNode> m_buttonNode;

//    CCSafeObject<CCControlButton> m_btnCancel;
//    CCSafeObject<CCControlButton> m_btnFavorite;

//    CCSafeObject<CCSprite> m_leftTitle;
//    CCSafeObject<CCSprite> m_rightTitle;

    std::vector<CCControlButton*> m_buttonStack;
    std::vector<TileButtonState> m_buttonState;
    
};

#endif /* defined(__IF__BaseTileInfo__) */