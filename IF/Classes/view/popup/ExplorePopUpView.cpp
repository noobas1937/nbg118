//
//  ExplorePopUpView.cpp
//  IF
//
//  Created by 李锐奇 on 14-7-10.
//
//

#include "ExplorePopUpView.h"
#include "MailController.h"
#include "PopupViewController.h"
#include "RewardController.h"
#include "WorldController.h"
#include "YesNoDialog.h"
#include "MailSaveCommand.h"
#include "MailPopUpView.h"
#include "DetectMailPopUpView.h"

ExplorePopUpView *ExplorePopUpView::create(MailInfo *info){
    ExplorePopUpView *ret = new ExplorePopUpView(info);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void ExplorePopUpView::onEnter(){
    PopupBaseView::onEnter();
    setTitleName(m_info->fromName.c_str());
}

void ExplorePopUpView::onExit(){
    PopupBaseView::onExit();
}

bool ExplorePopUpView::init(){
    if(!PopupBaseView::init()){
        return false;
    }
    auto bg = CCBLoadFile("ExploreMailView", this, this);
    this->setContentSize(bg->getContentSize());
//    CCLoadSprite::doResourceByCommonIndex(6, true);
//    setCleanFunction([](){
//        CCLoadSprite::doResourceByCommonIndex(6, false);
//        
//    });
    int preHeight = this->m_buildBG->getContentSize().height;
    changeBGHeight(m_buildBG);
    int dh = m_buildBG->getContentSize().height - preHeight;
    this->m_infoContainer->setContentSize(CCSize(m_infoContainer->getContentSize().width, m_infoContainer->getContentSize().height + dh));
    m_bg->setContentSize(CCSize(m_bg->getContentSize().width, m_bg->getContentSize().height + dh));
    m_downNode->setPositionY(m_downNode->getPositionY() - dh);
    m_listNode = CCNode::create();
    this->m_infoContainer->setPositionY(this->m_infoContainer->getPositionY() - dh);
    m_scrollView = CCScrollView::create(m_infoContainer->getContentSize());
    m_scrollView->setDirection(kCCScrollViewDirectionVertical);
    m_scrollView->setAnchorPoint(ccp(0, 0));
    m_scrollView->setTouchPriority(Touch_Popup);
    m_infoContainer->addChild(m_scrollView);

    m_titleText->setString(_lang("108675"));
    m_totalText->setString(_lang("108675"));
    this->m_timeText->setString(CCCommonUtils::timeStampToDate(m_info->createTime).c_str());
    this->m_totalNode->removeChild(this->m_firstNode);
    this->m_totalNode->removeChild(this->m_secondNode);
    this->m_totalNode->removeChild(this->m_upNode);
    m_scrollView->addChild(m_listNode);
    
    m_listNode->addChild(m_upNode);
    this->m_upNode->setPositionX(0);
    this->m_upNode->setPositionY(0);
   // m_lostLabel->setString(_lang("105563"));
   // m_lostTitle->setString(_lang("105544"));
   // m_getTitle->setString(_lang("105562"));
    m_contentText->setString(_lang("108655"));
//    m_deleteBtnTitle->setString(_lang("108523").c_str());
    m_totalH = 0;
    m_totalH-=428;
    m_upNode->setPositionY(m_totalH);
    auto battlePic = CCLoadSprite::createSprite("Mail_yijitanxian.png");
    this->m_battlePicNode->addChild(battlePic);
    if(!m_info->isReadContent){
        getData();
    }else{
        refreshView();
        this->m_listNode->setPositionY(-m_totalH);
        m_scrollView->setContentSize(CCSize(m_infoContainer->getContentSize().width, -m_totalH));
        m_scrollView->setContentOffset(ccp(0, m_infoContainer->getContentSize().height - (-m_totalH)));
        
    }

    return true;
}

SEL_CCControlHandler ExplorePopUpView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onDeleteClick", ExplorePopUpView::onDeleteClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAddSaveClick", ExplorePopUpView::onAddSaveClick);
    return NULL;
}

bool ExplorePopUpView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_titleText", CCLabelIF*, this->m_titleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_contentText", CCLabelIF*, this->m_contentText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_timeText", CCLabelIF*, this->m_timeText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_totalText", CCLabelIF*, this->m_totalText);

//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_deleteBtnTitle", CCLabelIF*, this->m_deleteBtnTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_downNode", CCNode*, this->m_downNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_totalNode", CCNode*, this->m_totalNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_firstNode", CCNode*, this->m_firstNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_secondNode", CCNode*, this->m_secondNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_upNode", CCNode*, this->m_upNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_battlePicNode", CCNode*, this->m_battlePicNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_infoContainer", CCNode*, this->m_infoContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_buildBG", CCScale9Sprite*, this->m_buildBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_bg", CCScale9Sprite*, this->m_bg);
  //  CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_getBG", CCScale9Sprite*, this->m_getBG);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_saveBtnTitle", CCLabelIF*, this->m_saveBtnTitle);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_savespr", CCSprite*, this->m_savespr);
//    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this,"m_unsavespr", CCSprite*, this->m_unsavespr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_addSaveBtn", CCControlButton*, this->m_addSaveBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_unSaveBtn", CCControlButton*, this->m_unSaveBtn);


    return false;
}

void ExplorePopUpView::getData(){
    MailController::getInstance()->readMailFromServer(m_info->uid, CC_ITOA(m_info->type));
}

void ExplorePopUpView::refreshView(){
    if(m_info->save==0){
        //CCCommonUtils::setButtonTitle(m_addSaveBtn, "save");
//        m_saveBtnTitle->setString(_lang("105574").c_str());
        m_addSaveBtn->setVisible(false);
        m_unSaveBtn->setVisible(true);
    }else{
        m_addSaveBtn->setVisible(true);
        m_unSaveBtn->setVisible(false);
    }
    std::string timeStr = CCCommonUtils::timeLeftToCountDown(m_info->mazeTime);
    std::string ptStr = _lang("108655");
    auto pt = WorldController::getPointByIndex(m_info->mazeIndex);
    ptStr = ptStr + CC_ITOA(pt.x) + ", " + CC_ITOA(pt.y);
    m_contentText->setString(_lang_2("105561", timeStr.c_str(), ptStr.c_str()));
    addGet();
    addLost();
    m_totalH-=80;
//    CCArray *arr = m_scrollView->getContainer()->getChildren();108675
//    int index = 0;
//    while(index < arr->count()){
//        CCNode *node = dynamic_cast<CCNode*>(arr->objectAtIndex(index));
//        node->setPositionY(node->getPositionY() + m_totalH);
//        index++;
//    }
//    m_scrollView->setContentSize(CCSize(m_infoContainer->getContentSize().width, m_totalH));
//    m_scrollView->setContentOffset(ccp(0, m_infoContainer->getContentSize().height - m_totalH));
}

void ExplorePopUpView::onDeleteClick(CCObject *pSender, CCControlEvent event){
    if(m_info->save!=0){
        CCCommonUtils::flyHint("","",_lang("105575"));
        return;
    }
    YesNoDialog::showYesDialog(_lang("105570").c_str(),false,CCCallFunc::create(this, callfunc_selector(ExplorePopUpView::onOKDeleteMail)),true);
}
void ExplorePopUpView::onOKDeleteMail(){
    MailController::getInstance()->removeMail(m_info->uid, CC_ITOA(m_info->type));
    PopupViewController::getInstance()->goBackPopupView();

}
void ExplorePopUpView::onAddSaveClick(cocos2d::CCObject *pSender, CCControlEvent event){
    if(m_info->save ==1){
        MailCancelSaveCommand *cmd = new MailCancelSaveCommand(m_info->uid, m_info->type);
        cmd->sendAndRelease();
        m_info->save = 0;
        CCCommonUtils::flyHint("quest_icon_1.png", "", _lang("105576"));
    }else{
        if(MailController::getInstance()->isMailFull(SAVEMAIL)){
            CCCommonUtils::flyText("full");
            return;
        }
        MailSaveCommand *cmd = new MailSaveCommand(m_info->uid, m_info->type);
        cmd->sendAndRelease();
        m_info->save = 1;
        CCCommonUtils::flyHint("quest_icon_1.png", "", _lang("105575"));
    }
    if(m_info->save==0){
        m_addSaveBtn->setVisible(false);
        m_unSaveBtn->setVisible(true);
    }else{
        m_addSaveBtn->setVisible(true);
        m_unSaveBtn->setVisible(false);
    }
}
CCSafeObject<CCNode> m_infoContainer;
CCSafeObject<CCScrollView> m_scrollView;

void ExplorePopUpView::addGet(){

    if(m_info->mazeReward && m_info->mazeReward->count() > 0){
//        int preH = m_totalH;
//        auto addCell = [](CCNode *parent, int idx, int type, int value, int &totalH){
//            int startX = (idx % 2 == 0 ? 0 : CCDirector::sharedDirector()->getWinSize().width / 2);
//            CCSprite *sprite = CCLoadSprite::createSprite("icon_kuang.png");
//            std::string rewardPic = RewardController::getInstance()->getPicByType(type, value);
//            CCSprite *pic = CCLoadSprite::createSprite(rewardPic.c_str());
//            CCLabelIF *label = NULL;
//            if(type == R_GOODS){
//                label = CCLabelIF::create("1");
//            }
//            else
//            {
//                label = CCLabelIF::create(CC_ITOA(value));
//            }
//            label->setColor(ccc3(77, 37, 0));
//            parent->addChild(sprite);
//            parent->addChild(pic);
//            parent->addChild(label);
//            sprite->setAnchorPoint(ccp(0, 1));
//            pic->setAnchorPoint(ccp(0, 1));
//            label->setAnchorPoint(ccp(0, 0.5));
//            if(idx % 2 == 0){
//                totalH += sprite->getContentSize().height + 5;
//            }
//            sprite->setPosition(ccp(startX + 50, -totalH + sprite->getContentSize().height));
//            pic->setPosition(sprite->getPosition() + ccp((sprite->getContentSize().width - pic->getContentSize().width) / 2, -(sprite->getContentSize().height - pic->getContentSize().height) / 2));
//            label->setPosition(sprite->getPosition() + ccp(sprite->getContentSize().width + 20, -sprite->getContentSize().height / 2));
//        };
//        
//        m_totalH += 60;
        int rowNum = m_info->mazeReward->count();
        DetectBgCell* bgCell = DetectBgCell::create(_lang("105562").c_str());
        if(rowNum>2){
            int bgHeight = (rowNum-2)*40+bgCell->getBgCellHeight();
            bgCell->setBgHeight(bgHeight);
        }
        m_totalH-=40;
        m_totalH -= bgCell->getBgCellHeight();
        bgCell->setPosition(0, m_totalH);
        this->m_listNode->addChild(bgCell);
        CCObject *obj;
        int posX = 60;
        int i = 0;
        CCARRAY_FOREACH(m_info->mazeReward, obj){
            CCDictionary *reward = dynamic_cast<CCDictionary*>(obj);
            int value = atoi(reward->valueForKey("value")->getCString());
            int type = atoi(reward->valueForKey("type")->getCString());
            auto cell = ExploreRewardPreviewCell::create(type, value);
            cell->setAnchorPoint(ccp(0, 0));

            cell->setPosition(ccp(posX,  10+i*40));
            bgCell->m_bg->addChild(cell);
            i++;
            if(rowNum==1){
                cell->setPosition(ccp(posX,  10+20));
            }
        }
        if(rowNum>2){
            // bgCell->m_bg->setContentSize(CCSize(bgCell->m_bg->getContentSize().width, bgCell->m_bg->getContentSize().height+(rowNum-2)*50));
           // bgCell->m_bg->setPositionY(bgCell->m_bg->getPositionY()-(rowNum-2)*40);
           // bgCell->m_kuangbg->setContentSize(CCSize(bgCell->m_kuangbg->getContentSize().width, bgCell->m_kuangbg->getContentSize().height+(rowNum-2)*40));
           // bgCell->m_kuangbg->setPositionY(bgCell->m_kuangbg->getPositionY()-(rowNum-2)*40);
        }

    }
}

void ExplorePopUpView::addLost(){
//    this->m_lostNode->retain();
//    this->m_lostNode->removeFromParent();
    if(m_info->mazeSodiler && m_info->mazeSodiler->count() > 0){
//        int preH = m_totalH;
//        auto addCell = [](CCNode *parent, std::string id, int value, int &totalH){
//            CCScale9Sprite *sprite = CCLoadSprite::createScale9Sprite("UI_frame_qianse.png");
//            sprite->setPreferredSize(CCSize(567, 44));
//            CCLabelIF *nameLabel = CCLabelIF::create(CCCommonUtils::getNameById(id).c_str());
//            CCLabelIF *numLabel = CCLabelIF::create(CC_ITOA(value));
//            nameLabel->setColor(ccc3(77, 37, 0));
//            numLabel->setColor(ccc3(77, 37, 0));
//            nameLabel->setFontSize(18);
//            numLabel->setFontSize(18);
//            parent->addChild(sprite);
//            parent->addChild(nameLabel);
//            parent->addChild(numLabel);
//            sprite->setAnchorPoint(ccp(0, 1));
//            nameLabel->setAnchorPoint(ccp(0, 0.5));
//            numLabel->setAnchorPoint(ccp(1, 0.5));
//            
//            totalH += sprite->getContentSize().height;
//            sprite->setPosition(ccp(38, -totalH + sprite->getContentSize().height));
//            nameLabel->setPosition(sprite->getPosition() + ccp(13, -sprite->getContentSize().height / 2));
//            numLabel->setPosition(sprite->getPosition() + ccp(sprite->getContentSize().width - 15, -sprite->getContentSize().height / 2));
//        };
        
        int rowNum = m_info->mazeSodiler->count();
        DetectBgCell* bgCell = DetectBgCell::create(_lang("105544").c_str());
        if(rowNum>2){
            int bgHeight = (rowNum-2)*50+bgCell->getBgCellHeight();
            bgCell->setBgHeight(bgHeight);
        }
        m_totalH-=40;
        m_totalH -= bgCell->getBgCellHeight();
        bgCell->setPosition(0, m_totalH);
        this->m_listNode->addChild(bgCell);
        CCDictElement *element = NULL;

        CCDictionary *dict = m_info->mazeSodiler;
        int posX = 60;
        int i=0;
        CCDICT_FOREACH(dict, element){
            std::string armyId = element->getStrKey();
            int num = dict->valueForKey(armyId)->intValue();

            auto cell = ExploreLossPreviewCell::create(armyId, num);
            cell->setAnchorPoint(ccp(0, 0));
            
            cell->setPosition(ccp(posX,  30+i*50));
            bgCell->m_bg->addChild(cell);
            i++;
//                        auto cell1 = ExploreLossPreviewCell::create(armyId, num);
//                        cell1->setAnchorPoint(ccp(0, 0));
//            //            //int posX = 20;
//                        cell1->setPosition(ccp(posX,  30+i*50));
//                        bgCell->m_bg->addChild(cell1);
//                        i++;
            //            i++;
            if(rowNum==1){
                cell->setPosition(ccp(posX,  30+25));
            }
        }
        if(rowNum>2){
           // bgCell->m_bg->setContentSize(CCSize(bgCell->m_bg->getContentSize().width, bgCell->m_bg->getContentSize().height+(rowNum-2)*50));
            //bgCell->m_bg->setPositionY(bgCell->m_bg->getPositionY()-(rowNum-2)*50);
           // bgCell->m_kuangbg->setContentSize(CCSize(bgCell->m_kuangbg->getContentSize().width, bgCell->m_kuangbg->getContentSize().height+(rowNum-2)*50));
           // bgCell->m_kuangbg->setPositionY(bgCell->m_kuangbg->getPositionY()-(rowNum-2)*50);
        }
    }

}

ExploreRewardPreviewCell *ExploreRewardPreviewCell::create(int type, int num){
    ExploreRewardPreviewCell *ret = new ExploreRewardPreviewCell(type, num);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void ExploreRewardPreviewCell::setData(int type, int num){
    m_type = type;
    m_num = num;
    refreshView();
}

bool ExploreRewardPreviewCell::init(){
    refreshView();
    
    return true;
}

void ExploreRewardPreviewCell::refreshView(){
    this->removeAllChildren();
    
    auto icon = CCLoadSprite::createSprite(RewardController::getInstance()->getPicByType(m_type, m_num).c_str());
    this->addChild(icon);
    icon->setAnchorPoint(ccp(0.5, 0.5));
    icon->setPosition(ccp(0, 30));
    CCCommonUtils::setSpriteMaxSize(icon, 50);
    
    std::string namestr = RewardController::getInstance()->getNameByType(m_type);
    auto label = CCLabelIF::create();
    label->setFontSize(22);
    label->setColor(ccc3(130, 99, 56));
    label->setString(namestr.c_str());
    label->setAnchorPoint(ccp(0, 0.5));
    //int width =bg->getContentSize().width/2;
    label->setPosition(40, icon->getPositionY());
    this->addChild(label);
    
    std::string numstr = "";
    numstr.append(CC_CMDITOA(m_num));
    auto label1 = CCLabelIF::create();
    label1->setFontSize(22);
    label1->setColor(ccc3(127, 35, 29));
    label1->setString(numstr.c_str());
    label1->setAnchorPoint(ccp(1.0, 0.5));
    label1->setPosition(435, icon->getPositionY());
    this->addChild(label1);
}

//loss cell
ExploreLossPreviewCell *ExploreLossPreviewCell::create(string type, int num){
    ExploreLossPreviewCell *ret = new ExploreLossPreviewCell(type, num);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void ExploreLossPreviewCell::setData(int type, int num){
    m_type = type;
    m_num = num;
    refreshView();
}

bool ExploreLossPreviewCell::init(){
    refreshView();
    
    return true;
}

void ExploreLossPreviewCell::refreshView(){
    this->removeAllChildren();
    
    std::string namestr = CCCommonUtils::getNameById(m_type);
    auto label = CCLabelIF::create();
    label->setFontSize(22);
    label->setColor(ccc3(130, 99, 56));
    label->setString(namestr.c_str());
    label->setAnchorPoint(ccp(0, 0.5));
    //int width =bg->getContentSize().width/2;
    label->setPosition(20, 0);
    this->addChild(label);
    
    std::string numstr = "";
    numstr.append(CC_CMDITOA(m_num));
    auto label1 = CCLabelIF::create();
    label1->setFontSize(22);
    label1->setColor(ccc3(127, 35, 29));
    label1->setString(numstr.c_str());
    label1->setAnchorPoint(ccp(1.0, 0.5));
    label1->setPosition(435, 0);
    this->addChild(label1);
}
