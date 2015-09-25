//
//  BaseTileInfo.cpp
//  IF
//
//  Created by 邹 程 on 14-2-26.
//
//

#include "BaseTileInfo.h"
#include "AddFavorite.h"
#include "WorldMapView.h"

static const float TileButtonHeight = 74.0;
static const float TileBigButtonWidth = 500.0;
static const float TileSmallButtonWidth = 244.0;
static const float TileButtonGap = 10.0;
static const float TileOriginButtonNodeHeight = 390.0;
static const ccColor3B TileButtonFontColor = ccWHITE;

bool BaseTileInfo::init(bool selfInit) {
    PopupBaseView::init();
    setIsHDPanel(true);
    this->setModelLayerOpacity(0);
    this->setModelLayerTouchCallback([&](cocos2d::CCTouch *pTouch){
//        auto bgSize = m_tileBg->getContentSize();
//        auto bgPos = m_tileBg->getPosition();
//        auto point = m_subNode->convertToNodeSpace(pTouch->getStartLocationInView());
//        // detect touch inside panel
//        if (point.x > bgSize.width || point.x < 0 || point.y < -(bgPos.y + 30) || point.y > bgSize.height - bgPos.y) {
//            this->closeThis();
//        }
        if (! isTouchInside(m_tileBg, pTouch) )
        {
            this->closeThis();
        }
    });
    
    if (selfInit) {
        auto ccb = CCBLoadFile("TileInfo",this,this);
        this->setContentSize(ccb->getContentSize());
    }

    return true;
}


bool BaseTileInfo::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_subNode", CCNode*, this->m_subNode)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tileBg", CCScale9Sprite*, this->m_tileBg)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_title", CCLabelIF*, this->m_title)
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnCancel", CCControlButton*, this->m_btnCancel)
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buttonNode", CCNode*, this->m_buttonNode)
    return false;
}

SEL_CCControlHandler BaseTileInfo::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "keyPressedBtnCancel", BaseTileInfo::keyPressedBtnCancel)
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "keyPressedBtnFavorite", BaseTileInfo::keyPressedBtnFavorite)
    return NULL;
}

//void BaseTileInfo::keyPressedBtnCancel(CCObject * pSender, Control::EventType pCCControlEvent) {
//    closeThis();
//}
void BaseTileInfo::keyPressedBtnFavorite(CCObject * pSender, Control::EventType pCCControlEvent) {
    auto world = WorldMapView::instance();
    if (world) {
        world->addPopupView(this->getFavoriteView());
    }
    closeThis();
}

BaseTileInfo* BaseTileInfo::getFavoriteView() {
    return AddFavorite::create(m_cityInfo.cityIndex);
}

//void BaseTileInfo::setFavoButtonVisible(bool isVisible) {
////    if (m_btnFavorite->isVisible() == isVisible) {
////        return;
////    }
////    
////    m_btnFavorite->setVisible(isVisible);
//}

void BaseTileInfo::setButtonCount(unsigned int count,bool needLargeButton) {
    m_buttonNode->removeAllChildren();
    m_buttonStack.clear();
    unsigned int rowCounter = 0;
    for (int i = 1; i <= count; ++i) {
        auto button = CCControlButton::create(CCLoadSprite::createScale9Sprite("btn_green3.png"));
        button->setBackgroundSpriteForState(CCLoadSprite::createScale9Sprite("btn_green3.png"),CCControlStateHighlighted);
        button->setBackgroundSpriteForState(CCLoadSprite::createScale9Sprite("Btn_grey.png"),CCControlStateDisabled);
        button->setTitleColorForState(TileButtonFontColor, CCControlStateNormal);
        button->setTitleColorForState(TileButtonFontColor, CCControlStateHighlighted);
        button->setTitleColorForState(TileButtonFontColor, CCControlStateDisabled);
        button->setTag(i);
        button->setPreferredSize(CCSize(TileSmallButtonWidth,TileButtonHeight));
        
        // set position by count
        // odd  : every row except last one has two button , last one is last button in large size
        //        example :   5
        //                  1   2
        //                  3   4
        //
        // even : every row has two button
        //        exmaple : 1   2
        //                  3   4
        
        int row = 0;
        int col = 0;
        
        if (count % 2 && i == count) {
            if (needLargeButton) {
                button->setPreferredSize(CCSize(TileBigButtonWidth,TileButtonHeight));
            }
        } else {
            int temp = count % 2 ? i + 1 : i - 1;
            row = temp / 2;
            col = temp % 2 ? 1 : -1;
        }
        
        rowCounter = row == 0 ? rowCounter : row;
        row = -row;
        
        auto position = ccp(col * (TileBigButtonWidth - TileSmallButtonWidth) / 2,row * (TileButtonHeight + TileButtonGap) + TileOriginButtonNodeHeight - getBgHeight() + 15);
        
        button->setPosition(position);

        m_buttonNode->addChild(button);
        m_buttonStack.push_back(button);
        m_buttonState.push_back(ButtonNone);
    }
    setBgHeight(getBgHeight() + (TileButtonHeight + TileButtonGap) * rowCounter);
}

unsigned int BaseTileInfo::getButtonCount() {
    return m_buttonStack.size();
}

void BaseTileInfo::setButtonName(unsigned int order,const string& name) {
    if (order == 0 || order > m_buttonStack.size()) {
        return;
    }
    CCCommonUtils::setButtonTitle(m_buttonStack[order - 1],name.c_str());
}

void BaseTileInfo::setButtonCallback(unsigned int order,SEL_CCControlHandler handler) {
    if (order == 0 || order > m_buttonStack.size()) {
        return;
    }
    m_buttonStack[order - 1]->addTargetWithActionForControlEvents(this, handler, CCControlEventTouchUpInside);
}

CCControlButton* BaseTileInfo::getButton(unsigned int order) {
    if (order == 0 || order > m_buttonStack.size()) {
        return NULL;
    }
    return m_buttonStack[order - 1];
}

TileButtonState BaseTileInfo::getButtonState(unsigned int order) {
    if (order == 0 || order > m_buttonState.size()) {
        return ButtonNone;
    }
    return m_buttonState[order - 1];
}

TileButtonState BaseTileInfo::getButtonState(CCControlButton* button) {
    if (button) {
        auto it = std::find(m_buttonStack.begin(), m_buttonStack.end(), button);
        if (it != m_buttonStack.end()) {
            return getButtonState(it - m_buttonStack.begin() + 1);
        }
    }
    return ButtonNone;
}

void BaseTileInfo::setButtonState(unsigned int order, TileButtonState state) {
    if (order == 0 || order > m_buttonState.size()) {
        return;
    }
    m_buttonState[order - 1] = state;
}

void BaseTileInfo::setButtonState(CCControlButton *button, TileButtonState state) {
    if (button) {
        auto it = std::find(m_buttonStack.begin(), m_buttonStack.end(), button);
        if (it != m_buttonStack.end()) {
            setButtonState(it - m_buttonStack.begin() + 1,state);
        }
    }
}

void BaseTileInfo::closeThis() {
    this->removeFromParent();
}
