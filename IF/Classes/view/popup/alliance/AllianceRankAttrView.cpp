//
//  AllianceRankAttrView.cpp
//  IF
//
//  Created by ganxiaohua on 14-8-6.
//
//

#include "AllianceRankAttrView.h"
#include "AllianceManager.h"

float hd_cell_height = 64.2;
float hd_cell_width = 1400;
float dw = (hd_cell_width - 661 - 8) / 5.0;

AllianceRankAttrView *AllianceRankAttrView::create(){
    AllianceRankAttrView *ret = new AllianceRankAttrView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void AllianceRankAttrView::onEnter(){
    PopupBaseView::onEnter();
    this->setTitleName(_lang("115904"));
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, false);
}

void AllianceRankAttrView::onExit(){
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ALERT_ALLIANCE_VIEW_CLOSE);
    PopupBaseView::onExit();
}

bool AllianceRankAttrView::init(){
    if (CCCommonUtils::isIosAndroidPad())
    {
        if (!PopupBaseView::init()) {
            return false;
        }
        setIsHDPanel(true);
        auto ccb = CCBLoadFile("AllianceRankAttrView", this, this);
        this->setContentSize(ccb->getContentSize());
        changeBGMaxHeight(m_viewBg);
        float addh = CCDirector::sharedDirector()->getWinSize().height - 2048;
        m_scrollNode->setContentSize(CCSize(m_scrollNode->getContentSize().width, m_scrollNode->getContentSize().height));
        m_scrollNode->setPositionY(m_scrollNode->getPositionY() - addh);
        m_scrollView = CCScrollView::create(m_scrollNode->getContentSize());
        m_scrollView->setDirection(kCCScrollViewDirectionVertical);
        m_scrollView->setTouchPriority(Touch_Popup);
        m_scrollNode->addChild(m_scrollView);
        
        float curY = 0;
        auto node = CCNode::create();
        for (int i = -1; i < 23; ++i) {
            auto cell = AllRankAttrCellHd::create(i);
            cell->setPosition(ccp(0, curY));
            node->addChild(cell);
            curY -= hd_cell_height;
        }
        auto spr = CCLoadSprite::createScale9Sprite("line_gray03.png");
        spr->setPreferredSize(CCSize(abs(curY), 8));
        spr->setRotation(90);
        spr->setAnchorPoint(ccp(0, 0));
        spr->setPosition(ccp(0, -8));
        node->addChild(spr);
        for (int i = 2; i <= 7; ++i) {
            spr = CCLoadSprite::createScale9Sprite("line_gray03.png");
            spr->setPreferredSize(CCSize(abs(curY), 8));
            spr->setRotation(90);
            spr->setAnchorPoint(ccp(0, 0));
            spr->setPosition(ccp(661 + (i - 2) * dw, -8));
            node->addChild(spr);
        }
        curY -= 8;
        spr = CCLoadSprite::createScale9Sprite("line_gray03.png");
        spr->setPreferredSize(CCSize(hd_cell_width, 8));
        spr->setAnchorPoint(CCPointZero);
        spr->setPosition(0, curY);
        node->addChild(spr);
        
        auto label = CCLabelIF::create();
        label->setAnchorPoint(ccp(0, 1));
        label->setFontSize(45);
        label->setColor({96, 49, 0});
        string tip = _lang("115927");
        tip += "\n";
        tip += _lang("115928");
        label->setString(tip.c_str());
        curY -= 30;
        label->setPosition(ccp(8, curY));
        label->setDimensions(CCSize(hd_cell_width - 16, 0));
        label->setVerticalAlignment(kCCVerticalTextAlignmentTop);
        label->setHorizontalAlignment(kCCTextAlignmentLeft);
        node->addChild(label);
        curY -= label->getContentSize().height * label->getOriginScaleY();
        curY -= 30;
        
        m_scrollView->setContentSize(CCSize(hd_cell_width, abs(curY)));
        node->setPosition(0, abs(curY));
        m_scrollView->addChild(node);
        m_scrollView->setContentOffset(ccp(0, m_scrollNode->getContentSize().height - m_scrollView->getContentSize().height));
        return true;
    } else {
        bool ret = false;
        if(PopupBaseView::init()){
            auto node = CCBLoadFile("AllianceRankAttrView", this, this);
            this->setContentSize(node->getContentSize());
            int prevH = m_viewBg->getContentSize().height;
            changeBGHeight(m_viewBg);
            int changeH = m_viewBg->getContentSize().height;
            int add = changeH - prevH;
            m_scrollBg->setContentSize(CCSize(m_scrollBg->getContentSize().width,MIN(m_scrollBg->getContentSize().height + add, 875)));
            
            m_scrollNode->setContentSize(CCSize(m_scrollNode->getContentSize().width,m_scrollNode->getContentSize().height+add));
            //m_scrollNode->setPositionY(m_scrollNode->getPositionY() - add);
            
            m_scrollView = CCScrollView::create(m_scrollNode->getContentSize());
            m_scrollView->setDirection(kCCScrollViewDirectionVertical);
            m_scrollView->setTouchPriority(Touch_Popup);
            m_scrollNode->addChild(m_scrollView,10000);
            m_rankNode->removeFromParent();
            m_rankNode->setPosition(ccp(312, 413+add+200));
            m_scrollView->addChild(m_rankNode);
            
            m_scrollView->setContentSize(CCSize(m_scrollNode->getContentSize().width,735+300));
            m_scrollView->setContentOffset(ccp(0, m_scrollNode->getContentSize().height - 735 - 200 -add));
            m_scrollView->setTouchEnabled(m_scrollNode->getContentSize().height < 920);
            string tip = _lang("115927");
            tip += "\n";
            tip += _lang("115928");
            m_tip->setString(tip.c_str());
            
            m_txt0->setString(_lang("115910"));
            m_txt1->setString(_lang("115909"));
            m_txt2->setString(_lang("115908"));
            m_txt3->setString(_lang("115907"));
            m_txt4->setString(_lang("115913"));
            m_txt5->setString(_lang("115916"));
            m_txt6->setString(_lang("115915"));
            m_txt7->setString(_lang("115905"));
            m_txt8->setString(_lang("115906"));
            m_txt9->setString(_lang("115912"));
            m_txt10->setString(_lang("115911"));
            m_txt11->setString(_lang("115921"));
            m_txt12->setString(_lang("115920"));
            m_txt13->setString(_lang("115919"));
            m_txt14->setString(_lang("133061"));
            m_txt15->setString(_lang("115931"));
            m_txt16->setString(_lang("115918"));
            m_txt17->setString(_lang("115917"));
            m_txt18->setString(_lang("115914"));
            m_txt19->setString(_lang("115904"));
            m_txt20->setString(_lang("115903"));
            m_txt21->setString(_lang("115902"));
            m_txt22->setString(_lang("115901"));
            m_txt23->setString(_lang("115900"));
            
            for (int i=0; i<24; i++) {
                if(AllianceManager::getInstance()->v_rank5[i]==1){
                    CCSprite* spr = CCLoadSprite::createSprite("icon_complete.png");
                    spr->setPosition(ccp(323, 695.9 - i*34.9));
                    spr->setScale(0.7);
                    m_listContainer->addChild(spr);
                }
                
                if(AllianceManager::getInstance()->v_rank4[i]==1){
                    CCSprite* spr = CCLoadSprite::createSprite("icon_complete.png");
                    spr->setPosition(ccp(323 + 70 , 695.9 - i*34.9));
                    spr->setScale(0.7);
                    m_listContainer->addChild(spr);
                }
                if(AllianceManager::getInstance()->v_rank3[i]==1){
                    CCSprite* spr = CCLoadSprite::createSprite("icon_complete.png");
                    spr->setPosition(ccp(323 + 68*2, 695.9 - i*34.9));
                    spr->setScale(0.7);
                    m_listContainer->addChild(spr);
                }
                if(AllianceManager::getInstance()->v_rank2[i]==1){
                    CCSprite* spr = CCLoadSprite::createSprite("icon_complete.png");
                    spr->setPosition(ccp(323+ 68*3, 695.9 - i*34.9));
                    spr->setScale(0.7);
                    m_listContainer->addChild(spr);
                }
                if(AllianceManager::getInstance()->v_rank1[i]==1){
                    CCSprite* spr = CCLoadSprite::createSprite("icon_complete.png");
                    spr->setPosition(ccp(323+ 68*4, 695.9 - i*34.9));
                    spr->setScale(0.7);
                    m_listContainer->addChild(spr);
                }
            }
            ret = true;
        }
        return ret;
    }
}

bool AllianceRankAttrView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listContainer", CCNode*, this->m_listContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_viewBg", CCNode*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleTxt", CCLabelIF*, this->m_titleTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rankTxt5", CCLabelIF*, this->m_rankTxt5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rankTxt4", CCLabelIF*, this->m_rankTxt4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rankTxt3", CCLabelIF*, this->m_rankTxt3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rankTxt2", CCLabelIF*, this->m_rankTxt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rankTxt1", CCLabelIF*, this->m_rankTxt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_tip", CCLabelIF*, this->m_tip);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt0", CCLabelIF*, this->m_txt0);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt1", CCLabelIF*, this->m_txt1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt2", CCLabelIF*, this->m_txt2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt3", CCLabelIF*, this->m_txt3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt4", CCLabelIF*, this->m_txt4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt5", CCLabelIF*, this->m_txt5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt6", CCLabelIF*, this->m_txt6);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt7", CCLabelIF*, this->m_txt7);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt8", CCLabelIF*, this->m_txt8);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt9", CCLabelIF*, this->m_txt9);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt10", CCLabelIF*, this->m_txt10);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt11", CCLabelIF*, this->m_txt11);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt12", CCLabelIF*, this->m_txt12);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt13", CCLabelIF*, this->m_txt13);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt14", CCLabelIF*, this->m_txt14);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt15", CCLabelIF*, this->m_txt15);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt16", CCLabelIF*, this->m_txt16);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt17", CCLabelIF*, this->m_txt17);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt18", CCLabelIF*, this->m_txt18);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt19", CCLabelIF*, this->m_txt19);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt20", CCLabelIF*, this->m_txt20);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt21", CCLabelIF*, this->m_txt21);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt22", CCLabelIF*, this->m_txt22);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_txt23", CCLabelIF*, this->m_txt23);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rankNode", CCNode*, this->m_rankNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_scrollNode", CCNode*, this->m_scrollNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_scrollBg", CCNode*, this->m_scrollBg);
    return false;
}

bool AllianceRankAttrView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    
    return true;
}

void AllianceRankAttrView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){

}

SEL_CCControlHandler AllianceRankAttrView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    //CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickClose", AllianceRankAttrView::onClickClose);
    return NULL;
}

AllRankAttrCellHd* AllRankAttrCellHd::create(int idx)
{
    auto ret = new AllRankAttrCellHd(idx);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool AllRankAttrCellHd::init()
{
    if (!CCNode::init()) {
        return false;
    }
    auto spr1 = CCLoadSprite::createScale9Sprite("line_gray03.png");
    spr1->setAnchorPoint(ccp(0, 1));
    spr1->setPreferredSize(CCSize(hd_cell_width, 8));
    this->addChild(spr1);
    
    m_txt = CCLabelIF::create();
    m_txt->setFontSize(37);
    m_txt->setColor({96, 49, 0});
    m_txt->setAnchorPoint(ccp(0, 0.5));
    m_txt->setPosition(8, 0 - 8 - (hd_cell_height - 8) / 2.0);
    this->addChild(m_txt);
    switch (m_idx) {
        case -1:
            m_txt->setString("");
            break;
        case 0:
            m_txt->setString(_lang("115910"));
            break;
        case 1:
            m_txt->setString(_lang("115909"));
            break;
        case 2:
            m_txt->setString(_lang("115908"));
            break;
        case 3:
            m_txt->setString(_lang("115907"));
            break;
        case 4:
            m_txt->setString(_lang("115913"));
            break;
        case 5:
            m_txt->setString(_lang("115916"));
            break;
        case 6:
            m_txt->setString(_lang("115915"));
            break;
        case 7:
            m_txt->setString(_lang("115905"));
            break;
        case 8:
            m_txt->setString(_lang("115906"));
            break;
        case 9:
            m_txt->setString(_lang("115912"));
            break;
        case 10:
            m_txt->setString(_lang("115911"));
            break;
        case 11:
            m_txt->setString(_lang("115921"));
            break;
        case 12:
            m_txt->setString(_lang("115920"));
            break;
        case 13:
            m_txt->setString(_lang("115919"));
            break;
        case 14:
            m_txt->setString(_lang("133061"));
            break;
        case 15:
            m_txt->setString(_lang("115931"));
            break;
        case 16:
            m_txt->setString(_lang("115918"));
            break;
        case 17:
            m_txt->setString(_lang("115917"));
            break;
        case 18:
            m_txt->setString(_lang("115914"));
            break;
        case 19:
            m_txt->setString(_lang("115904"));
            break;
        case 20:
            m_txt->setString(_lang("115903"));
            break;
        case 21:
            m_txt->setString(_lang("115902"));
            break;
        case 22:
            m_txt->setString(_lang("115901"));
            break;
        case 23:
            m_txt->setString(_lang("115900"));
            break;
        default:
            break;
    }
    
    if (m_idx == -1)
    {
        for (int i = 5; i > 0; --i) {
            string name = "Alliance_R";
            name += CC_ITOA(i);
            name += ".png";
            auto spr = CCLoadSprite::createSprite(name.c_str());
            spr->setAnchorPoint(ccp(0.5, 0.5));
            spr->setScale(1.8);
            spr->setPositionX(661 + 8 + (5 - i) * dw + (dw - 8) / 2.0);
            spr->setPositionY(0 - 8 - (hd_cell_height - 8) / 2);
            this->addChild(spr);
        }
    } else {
        if(AllianceManager::getInstance()->v_rank5[m_idx]==1){
            CCSprite* spr = CCLoadSprite::createSprite("icon_complete.png");
            spr->setAnchorPoint(ccp(0.5, 0.5));
            spr->setScale(1.4);
            spr->setPositionX(661 + 8 + (dw - 8) / 2.0);
            spr->setPositionY(0 - 8 - (hd_cell_height - 8) / 2.0);
            this->addChild(spr);
        }
        if(AllianceManager::getInstance()->v_rank4[m_idx]==1){
            CCSprite* spr = CCLoadSprite::createSprite("icon_complete.png");
            spr->setAnchorPoint(ccp(0.5, 0.5));
            spr->setScale(1.4);
            spr->setPositionX(661 + dw + 8 + (dw - 8) / 2.0);
            spr->setPositionY(0 - 8 - (hd_cell_height - 8) / 2.0);
            this->addChild(spr);
        }
        if(AllianceManager::getInstance()->v_rank3[m_idx]==1){
            CCSprite* spr = CCLoadSprite::createSprite("icon_complete.png");
            spr->setAnchorPoint(ccp(0.5, 0.5));
            spr->setScale(1.4);
            spr->setPositionX(661 + dw * 2 + 8 + (dw - 8) / 2.0);
            spr->setPositionY(0 - 8 - (hd_cell_height - 8) / 2.0);
            this->addChild(spr);
        }
        if(AllianceManager::getInstance()->v_rank2[m_idx]==1){
            CCSprite* spr = CCLoadSprite::createSprite("icon_complete.png");
            spr->setAnchorPoint(ccp(0.5, 0.5));
            spr->setScale(1.4);
            spr->setPositionX(661 + dw * 3 + 8 + (dw - 8) / 2.0);
            spr->setPositionY(0 - 8 - (hd_cell_height - 8) / 2.0);
            this->addChild(spr);
        }
        if(AllianceManager::getInstance()->v_rank1[m_idx]==1){
            CCSprite* spr = CCLoadSprite::createSprite("icon_complete.png");
            spr->setAnchorPoint(ccp(0.5, 0.5));
            spr->setScale(1.4);
            spr->setPositionX(661 + dw * 4 + 8 + (dw - 8) / 2.0);
            spr->setPositionY(0 - 8 - (hd_cell_height - 8) / 2.0);
            this->addChild(spr);
        }
    }
    
    
    return true;
}