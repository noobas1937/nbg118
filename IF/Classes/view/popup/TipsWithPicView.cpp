//
//  TipsWithPicView.cpp
//  IF
//
//  Created by 〜TIAN~ on 15/4/2.
//
//

#include "TipsWithPicView.h"


const float numPerRow = 1.0;
TipsWithPicView* TipsWithPicView::create(std::string type){
    TipsWithPicView* ret = new TipsWithPicView(type);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

CCNode* TipsWithPicView::addPic(std::string resourceIcon, std::string icon1, std::string icon2){
    auto node = CCNode::create();
    auto bg = CCLoadSprite::createSprite("intro_bg.png");
    bg->setScale(2.0);
    bg->setAnchorPoint(ccp(0, 0));
    node->addChild(bg);
    
    auto sprite = CCLoadSprite::createSprite(resourceIcon.c_str());
    node->addChild(sprite);
    sprite->setPosition(ccp(150, 65));
    
    if(icon1 != ""){
        auto btn = CCLoadSprite::createSprite("bnt_02.png");
        auto icon = CCLoadSprite::createSprite(icon1.c_str());
        auto arrow = CCLoadSprite::createSprite("UI_world_Arrow.png");
        node->addChild(btn);
        node->addChild(icon);
        node->addChild(arrow);
        btn->setPosition(ccp(283, 157));
        icon->setPosition(btn->getPosition());
        arrow->setFlipX(true);
        arrow->setPosition(btn->getPosition() - ccp(btn->getContentSize().width / 2 - 15, 0));
    }
    
    if(icon2 != ""){
        auto icon2Sprite = CCLoadSprite::createSprite(icon2.c_str());
        node->addChild(icon2Sprite);
        icon2Sprite->setPosition(ccp(150, 155));
    }
    return node;
}

bool TipsWithPicView::init(){
    if(PopupBaseView::init()){
        setIsHDPanel(true);
        CCLoadSprite::doResourceByCommonIndex(505, true);
        setCleanFunction([](){
            CCLoadSprite::doResourceByCommonIndex(505, false);
        });
        
        auto tmpCCB = CCBLoadFile("TipsWithPicView",this,this,true);
        this->setContentSize(CCDirector::sharedDirector()->getWinSize());
        
        if (m_typestr.compare("Food")==0) {
            m_infoLabel1->setString(_lang_1("108764", _lang("102013").c_str()));
            m_infoLabel2->setString(_lang_1("108766", _lang("102013").c_str()));
            m_infoLabel3->setString(_lang("108769"));
            m_infoLabel4->setString(_lang_1("108767", _lang("102013").c_str()));
            m_infoLabel5->setString(_lang("108770"));
            
//            CCSprite* pic1;
//            string mpic1 = "guide_info_farm_0.png";
//            pic1 = CCLoadSprite::createSprite(mpic1.c_str());
//            pic1->setAnchorPoint(ccp(0,0));
//            m_spriteNode1->addChild(pic1);
            m_spriteNode1->addChild(addPic("0020.png", "tile_pop_icon1.png", ""));
            
//            CCSprite* pic2;
//            string mpic2 = "guide_info_farm_1.png";
//            pic2 = CCLoadSprite::createSprite(mpic2.c_str());
//            pic2->setAnchorPoint(ccp(0,0));
//            m_spriteNode2->addChild(pic2);
            m_spriteNode2->addChild(addPic("0020.png", "tile_pop_icon4.png", "world_occupy_other.png"));
            
            int height = 0;
            int h1 = 0;
            int h2 = 0;
            int h;
            int dy = 20;
            CCPoint p1;
            
            height += 270;
            p1 = m_infoLabel3->getPosition();
            h = m_infoLabel3->getContentSize().height * m_infoLabel3->getOriginScaleY();
            height += h;
            height += dy;
            m_infoNode2->setPositionY(p1.y - height);
            h1 = height;
            
            height += 270;
            h = m_infoLabel5->getContentSize().height * m_infoLabel5->getOriginScaleY();
            height += h;
            height += dy;
            h2 = height - h1;
            
            m_infoNode3->setVisible(false);
            
            m_scrollView = CCScrollView::create(m_infoList->getContentSize());
            m_scrollView->setContentSize(CCSize(m_infoList->getContentSize().width, height));
            m_scrollView->setDirection(kCCScrollViewDirectionVertical);
            m_scrollView->setAnchorPoint(ccp(0, 0));
            m_infoList->removeAllChildren();
            m_infoList->addChild(m_scrollView);
            m_scrollView->setPosition(0, 0);
            
            m_scrollView->addChild(m_infoNode1);
            m_scrollView->addChild(m_infoNode2);
            m_infoNode2->setPositionY(h2);
            m_infoNode1->setPositionY(height);
            m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - m_scrollView->getContentSize().height));
        }
        else if (m_typestr.compare("Wood")==0){
            m_infoLabel1->setString(_lang_1("108764", _lang("102011").c_str()));
            m_infoLabel2->setString(_lang_1("108766", _lang("102011").c_str()));
            m_infoLabel3->setString(_lang("108771"));
            m_infoLabel4->setString(_lang_1("108767", _lang("102011").c_str()));
            m_infoLabel5->setString(_lang("108772"));
            
//            CCSprite* pic1;
//            string mpic1 = "guide_info_wood_0.png";
//            pic1 = CCLoadSprite::createSprite(mpic1.c_str());
//            pic1->setAnchorPoint(ccp(0,0));
//            m_spriteNode1->addChild(pic1);
            m_spriteNode1->addChild(addPic("0017.png", "tile_pop_icon1.png", ""));
            
//            CCSprite* pic2;
//            string mpic2 = "guide_info_wood_1.png";
//            pic2 = CCLoadSprite::createSprite(mpic2.c_str());
//            pic2->setAnchorPoint(ccp(0,0));
//            m_spriteNode2->addChild(pic2);
            m_spriteNode2->addChild(addPic("0017.png", "tile_pop_icon4.png", "world_occupy_other.png"));
            
            int height = 0;
            int h1 = 0;
            int h2 = 0;
            int h;
            int dy = 20;
            CCPoint p1;
            
            height += 270;
            p1 = m_infoLabel3->getPosition();
            h = m_infoLabel3->getContentSize().height * m_infoLabel3->getOriginScaleY();
            height += h;
            height += dy;
            m_infoNode2->setPositionY(p1.y - height);
            h1 = height;
            
            height += 270;
            h = m_infoLabel5->getContentSize().height * m_infoLabel5->getOriginScaleY();
            height += h;
            height += dy;
            h2 = height - h1;
            
            m_infoNode3->setVisible(false);
            
            m_scrollView = CCScrollView::create(m_infoList->getContentSize());
            m_scrollView->setContentSize(CCSize(m_infoList->getContentSize().width, height));
            m_scrollView->setDirection(kCCScrollViewDirectionVertical);
            m_scrollView->setAnchorPoint(ccp(0, 0));
            m_infoList->removeAllChildren();
            m_infoList->addChild(m_scrollView);
            m_scrollView->setPosition(0, 0);
            
            m_scrollView->addChild(m_infoNode1);
            m_scrollView->addChild(m_infoNode2);
            m_infoNode2->setPositionY(h2);
            m_infoNode1->setPositionY(height);
            m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - m_scrollView->getContentSize().height));
        }
        else if (m_typestr.compare("Iron")==0){
            m_infoLabel1->setString(_lang_1("108764", _lang("102012").c_str()));
            m_infoLabel2->setString(_lang_1("108766", _lang("102012").c_str()));
            m_infoLabel3->setString(_lang("108773"));
            m_infoLabel4->setString(_lang_1("108767", _lang("102012").c_str()));
            m_infoLabel5->setString(_lang("108774"));
            
//            CCSprite* pic1;
//            string mpic1 = "guide_info_iron_0.png";
//            pic1 = CCLoadSprite::createSprite(mpic1.c_str());
//            pic1->setAnchorPoint(ccp(0,0));
//            m_spriteNode1->addChild(pic1);
            m_spriteNode1->addChild(addPic("0019.png", "tile_pop_icon1.png", ""));
            
//            CCSprite* pic2;
//            string mpic2 = "guide_info_iron_1.png";
//            pic2 = CCLoadSprite::createSprite(mpic2.c_str());
//            pic2->setAnchorPoint(ccp(0,0));
//            m_spriteNode2->addChild(pic2);
            m_spriteNode2->addChild(addPic("0019.png", "tile_pop_icon4.png", "world_occupy_other.png"));
            
            int height = 0;
            int h1 = 0;
            int h2 = 0;
            int h;
            int dy = 20;
            CCPoint p1;
            
            height += 270;
            p1 = m_infoLabel3->getPosition();
            h = m_infoLabel3->getContentSize().height * m_infoLabel3->getOriginScaleY();
            height += h;
            height += dy;
            m_infoNode2->setPositionY(p1.y - height);
            h1 = height;
            
            height += 270;
            h = m_infoLabel5->getContentSize().height * m_infoLabel5->getOriginScaleY();
            height += h;
            height += dy;
            h2 = height - h1;
            
            m_infoNode3->setVisible(false);
            
            m_scrollView = CCScrollView::create(m_infoList->getContentSize());
            m_scrollView->setContentSize(CCSize(m_infoList->getContentSize().width, height));
            m_scrollView->setDirection(kCCScrollViewDirectionVertical);
            m_scrollView->setAnchorPoint(ccp(0, 0));
            m_infoList->removeAllChildren();
            m_infoList->addChild(m_scrollView);
            m_scrollView->setPosition(0, 0);
            
            m_scrollView->addChild(m_infoNode1);
            m_scrollView->addChild(m_infoNode2);
            m_infoNode2->setPositionY(h2);
            m_infoNode1->setPositionY(height);
            m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - m_scrollView->getContentSize().height));
        }
        else if (m_typestr.compare("Stone")==0){
            m_infoLabel1->setString(_lang_1("108764", _lang("102010").c_str()));
            m_infoLabel2->setString(_lang_1("108766", _lang("102010").c_str()));
            m_infoLabel3->setString(_lang("108775"));
            m_infoLabel4->setString(_lang_1("108767", _lang("102010").c_str()));
            m_infoLabel5->setString(_lang("108776"));
            
//            CCSprite* pic1;
//            string mpic1 = "guide_info_stone_0.png";
//            pic1 = CCLoadSprite::createSprite(mpic1.c_str());
//            pic1->setAnchorPoint(ccp(0,0));
//            m_spriteNode1->addChild(pic1);
            m_spriteNode1->addChild(addPic("0018.png", "tile_pop_icon1.png", ""));
            
//            CCSprite* pic2;
//            string mpic2 = "guide_info_stone_1.png";
//            pic2 = CCLoadSprite::createSprite(mpic2.c_str());
//            pic2->setAnchorPoint(ccp(0,0));
//            m_spriteNode2->addChild(pic2);
            m_spriteNode2->addChild(addPic("0018.png", "tile_pop_icon4.png", "world_occupy_other.png"));
            int height = 0;
            int h1 = 0;
            int h2 = 0;
            int h;
            int dy = 20;
            CCPoint p1;
            
            height += 270;
            p1 = m_infoLabel3->getPosition();
            h = m_infoLabel3->getContentSize().height * m_infoLabel3->getOriginScaleY();
            height += h;
            height += dy;
            m_infoNode2->setPositionY(p1.y - height);
            h1 = height;
            
            height += 270;
            h = m_infoLabel5->getContentSize().height * m_infoLabel5->getOriginScaleY();
            height += h;
            height += dy;
            h2 = height - h1;
            
            m_infoNode3->setVisible(false);
            
            m_scrollView = CCScrollView::create(m_infoList->getContentSize());
            m_scrollView->setContentSize(CCSize(m_infoList->getContentSize().width, height));
            m_scrollView->setDirection(kCCScrollViewDirectionVertical);
            m_scrollView->setAnchorPoint(ccp(0, 0));
            m_infoList->removeAllChildren();
            m_infoList->addChild(m_scrollView);
            m_scrollView->setPosition(0, 0);
            
            m_scrollView->addChild(m_infoNode1);
            m_scrollView->addChild(m_infoNode2);
            m_infoNode2->setPositionY(h2);
            m_infoNode1->setPositionY(height);
            m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - m_scrollView->getContentSize().height));
        }
        else if (m_typestr.compare("Gold")==0){
            m_infoLabel1->setString(_lang_1("108764", _lang("108581").c_str()));
            m_infoLabel2->setString(_lang("108768"));
            m_infoLabel3->setString(_lang("108777"));
            m_infoLabel4->setString(_lang_1("108766", _lang("108581").c_str()));
            m_infoLabel5->setString(_lang("108778"));
            m_infoLabel6->setString(_lang_1("108767", _lang("108581").c_str()));
            m_infoLabel7->setString(_lang("108779"));
            
            CCSprite* pic1;
            string mpic1 = "guide_info_gold_2.png";
            pic1 = CCLoadSprite::createSprite(mpic1.c_str());
            pic1->setAnchorPoint(ccp(0,0));
            m_spriteNode1->addChild(pic1);
            
//            CCSprite* pic2;
//            string mpic2 = "guide_info_gold_0.png";
//            pic2 = CCLoadSprite::createSprite(mpic2.c_str());
//            pic2->setAnchorPoint(ccp(0,0));
//            m_spriteNode2->addChild(pic2);
            m_spriteNode2->addChild(addPic("0021.png", "tile_pop_icon1.png", ""));
//            CCSprite* pic3;
//            string mpic3 = "guide_info_gold_1.png";
//            pic3 = CCLoadSprite::createSprite(mpic3.c_str());
//            pic3->setAnchorPoint(ccp(0,0));
//            m_spriteNode3->addChild(pic3);
            m_spriteNode3->addChild(addPic("0021.png", "tile_pop_icon4.png", "world_occupy_other.png"));
            
            int height = 0;
            int h1 = 0;
            int h2 = 0;
            int h3 = 0;
            int h;
            int dy = 20;
            CCPoint p1;
            
            height += 270;
            p1 = m_infoNode1->getPosition();
            h = m_infoLabel3->getContentSize().height * m_infoLabel3->getOriginScaleY();
            height += h;
            height += dy;
            h1 = height;
            m_infoNode2->setPositionY(p1.y - h1);

            
            height += 270;
            p1 = m_infoNode2->getPosition();
            h = m_infoLabel5->getContentSize().height * m_infoLabel5->getOriginScaleY();
            height += h;
            height += dy;
            h2 = height - h1;
            m_infoNode3->setPositionY(p1.y - h2);
            
            height += 270;
            h = m_infoLabel7->getContentSize().height * m_infoLabel7->getOriginScaleY();
            height += h;
            height += dy;
            h3 = height - h2 - h1;
            
            m_scrollView = CCScrollView::create(m_infoList->getContentSize());
            m_scrollView->setContentSize(CCSize(m_infoList->getContentSize().width, height));
            m_scrollView->setDirection(kCCScrollViewDirectionVertical);
            m_scrollView->setAnchorPoint(ccp(0, 0));
            m_infoList->removeAllChildren();
            m_infoList->addChild(m_scrollView);
            m_scrollView->setPosition(0, 0);
            
            m_scrollView->addChild(m_infoNode1);
            m_scrollView->addChild(m_infoNode2);
            m_scrollView->addChild(m_infoNode3);
            m_infoNode3->setPositionY(h3);
            m_infoNode2->setPositionY(h2+h3);
            m_infoNode1->setPositionY(height);
            m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - m_scrollView->getContentSize().height));
        }
        else if (m_typestr.compare("Monster")==0){
            m_infoLabel1->setString(_lang("108765"));
            m_infoLabel2->setString(_lang("108765"));
            m_infoLabel3->setString(_lang("108780"));

            CCSprite* pic1;
            string mpic1 = "guide_info_mazz.png";
            pic1 = CCLoadSprite::createSprite(mpic1.c_str());
            pic1->setAnchorPoint(ccp(0,0));
            m_spriteNode1->addChild(pic1);
            
            m_infoNode2->setVisible(false);
            m_infoNode3->setVisible(false);
            
            m_scrollView = CCScrollView::create(m_infoList->getContentSize());
            m_scrollView->setContentSize(m_infoList->getContentSize());
            m_scrollView->setDirection(kCCScrollViewDirectionVertical);
            m_scrollView->setAnchorPoint(ccp(0, 0));
            m_infoList->removeAllChildren();
            m_infoList->addChild(m_scrollView);
            m_scrollView->setPosition(0, 0);
            
            m_scrollView->addChild(m_infoNode1);
            m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - m_scrollView->getContentSize().height));
        }
        else if (m_typestr.compare("TerritoryFood")==0) {
            m_infoLabel1->setString(_lang("115387"));
            m_infoLabel2->setString(_lang("115387"));
            m_infoLabel3->setString(_lang("115391"));
            
            CCSprite* pic1;
            string mpic1 = "guide_info_mazz.png";
            pic1 = CCLoadSprite::createSprite(mpic1.c_str());
            pic1->setAnchorPoint(ccp(0,0));
            m_spriteNode1->addChild(pic1);
            
            m_infoNode1->setVisible(false);
            m_infoNode2->setVisible(false);
            m_infoNode3->setVisible(false);
            
            m_scrollView = CCScrollView::create(m_infoList->getContentSize());
            m_scrollView->setContentSize(m_infoList->getContentSize());
            m_scrollView->setDirection(kCCScrollViewDirectionVertical);
            m_scrollView->setAnchorPoint(ccp(0, 0));
            m_infoList->removeAllChildren();
            m_infoList->addChild(m_scrollView);
            m_scrollView->setPosition(0, 0);
            
            m_scrollView->addChild(m_infoNode1);
            m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - m_scrollView->getContentSize().height));
        }
        else if (m_typestr.compare("TerritoryWood")==0) {
            m_infoLabel1->setString(_lang("115388"));
            m_infoLabel2->setString(_lang("115388"));
            m_infoLabel3->setString(_lang("115391"));
            
            CCSprite* pic1;
            string mpic1 = "guide_info_mazz.png";
            pic1 = CCLoadSprite::createSprite(mpic1.c_str());
            pic1->setAnchorPoint(ccp(0,0));
            m_spriteNode1->addChild(pic1);
            
            m_infoNode1->setVisible(false);
            m_infoNode2->setVisible(false);
            m_infoNode3->setVisible(false);
            
            m_scrollView = CCScrollView::create(m_infoList->getContentSize());
            m_scrollView->setContentSize(m_infoList->getContentSize());
            m_scrollView->setDirection(kCCScrollViewDirectionVertical);
            m_scrollView->setAnchorPoint(ccp(0, 0));
            m_infoList->removeAllChildren();
            m_infoList->addChild(m_scrollView);
            m_scrollView->setPosition(0, 0);
            
            m_scrollView->addChild(m_infoNode1);
            m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - m_scrollView->getContentSize().height));
        }
        else if (m_typestr.compare("TerritoryIron")==0) {
            m_infoLabel1->setString(_lang("115389"));
            m_infoLabel2->setString(_lang("115389"));
            m_infoLabel3->setString(_lang("115391"));
            
            CCSprite* pic1;
            string mpic1 = "guide_info_mazz.png";
            pic1 = CCLoadSprite::createSprite(mpic1.c_str());
            pic1->setAnchorPoint(ccp(0,0));
            m_spriteNode1->addChild(pic1);
            
            m_infoNode1->setVisible(false);
            m_infoNode2->setVisible(false);
            m_infoNode3->setVisible(false);
            
            m_scrollView = CCScrollView::create(m_infoList->getContentSize());
            m_scrollView->setContentSize(m_infoList->getContentSize());
            m_scrollView->setDirection(kCCScrollViewDirectionVertical);
            m_scrollView->setAnchorPoint(ccp(0, 0));
            m_infoList->removeAllChildren();
            m_infoList->addChild(m_scrollView);
            m_scrollView->setPosition(0, 0);
            
            m_scrollView->addChild(m_infoNode1);
            m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - m_scrollView->getContentSize().height));
        }
        else if (m_typestr.compare("TerritoryStone")==0) {
            m_infoLabel1->setString(_lang("115390"));
            m_infoLabel2->setString(_lang("115390"));
            m_infoLabel3->setString(_lang("115391"));
            
            CCSprite* pic1;
            string mpic1 = "guide_info_mazz.png";
            pic1 = CCLoadSprite::createSprite(mpic1.c_str());
            pic1->setAnchorPoint(ccp(0,0));
            m_spriteNode1->addChild(pic1);
            
            m_infoNode1->setVisible(false);
            m_infoNode2->setVisible(false);
            m_infoNode3->setVisible(false);
            
            m_scrollView = CCScrollView::create(m_infoList->getContentSize());
            m_scrollView->setContentSize(m_infoList->getContentSize());
            m_scrollView->setDirection(kCCScrollViewDirectionVertical);
            m_scrollView->setAnchorPoint(ccp(0, 0));
            m_infoList->removeAllChildren();
            m_infoList->addChild(m_scrollView);
            m_scrollView->setPosition(0, 0);
            
            m_scrollView->addChild(m_infoNode1);
            m_scrollView->setContentOffset(ccp(0, m_infoList->getContentSize().height - m_scrollView->getContentSize().height));
        }
        return true;
    }
    return false;
}

void TipsWithPicView::onEnter(){
    PopupBaseView::onEnter();
}

void TipsWithPicView::onExit(){
    PopupBaseView::onExit();
}

bool TipsWithPicView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel1", CCLabelIF*, m_infoLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel2", CCLabelIF*, m_infoLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel3", CCLabelIF*, m_infoLabel3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel4", CCLabelIF*, m_infoLabel4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel5", CCLabelIF*, m_infoLabel5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel6", CCLabelIF*, m_infoLabel6);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoLabel7", CCLabelIF*, m_infoLabel7);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_spriteNode1", CCNode*, this->m_spriteNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_spriteNode2", CCNode*, this->m_spriteNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_spriteNode3", CCNode*, this->m_spriteNode3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoNode1", CCNode*, this->m_infoNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoNode2", CCNode*, this->m_infoNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoNode3", CCNode*, this->m_infoNode3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_closeBtn", CCControlButton*, this->m_closeBtn);
    return false;
}

SEL_CCControlHandler TipsWithPicView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCloseBtnClick", TipsWithPicView::onCloseClick);
    return NULL;
}

void TipsWithPicView::onCloseClick(cocos2d::CCObject *pSender, CCControlEvent event){
    closeSelf();
}

bool TipsWithPicView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (isTouchInside(m_infoList,pTouch)){
        return true;
    }
    return false;
}
void TipsWithPicView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if(!isTouchInside(m_infoList, pTouch)){
    }
}