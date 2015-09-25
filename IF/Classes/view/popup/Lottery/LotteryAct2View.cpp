//
//  LotteryAct2View.cpp
//  IF
//
//  Created by chenfubi on 15/1/23.
//
//

#include "LotteryAct2View.h"
#include "LotteryController.h"
#include "LotteryActCommand.h"
#include "PopupViewController.h"
#include "TipsView.h"
#include "LotteryActView.h"
#include "ParticleController.h"
#include "YesNoDialog.h"
#include "LotteryAct2ShowView.h"
#include "SoundController.h"
#include "CCMathUtils.h"

#define REWARD_NUM 9
#define LOTTERYACT2CELL_SHOW "LOTTERYACT2CELL_SHOW"
#define LOTTERYACT2_GUIDE "LOTTERYACT2_GUIDE"
#define LOTTERYACT2_PLAYREWARD "LOTTERYACT2_PLAYREWARD"
#define LOTTERYACT2_PLAYPARTICLE "LOTTERYACT2_PLAYPARTICLE"


LotteryAct2View* LotteryAct2View::create()
{
    LotteryAct2View* ret = new LotteryAct2View();
    if (ret && ret->init())
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool LotteryAct2View::init()
{
	bool bRet = false;
	do
	{
        if (!PopupBaseView::init())
        {
            break;
        }
        setIsHDPanel(true);
        CCLoadSprite::doResourceByCommonIndex(11, true);
        setCleanFunction([](){
            //CCLoadSprite::doResourceByCommonIndex(11, false);
        });
        
        auto ccb = CCBLoadFile("LotteryAct2View", this, this);
        this->setContentSize(ccb->getContentSize());
        
        int extH = getExtendHeight();
        float oldH = m_bg0->getContentSize().height;
        float newH = extH + oldH;
        if (newH > oldH)
        {
            m_bg0->setScaleY(newH/oldH);
            m_bg1->setScaleY(newH/oldH);
            m_superNode->setPositionY(m_superNode->getPositionY() - extH*3/4);
        }
        m_midNode->setPositionY(m_midNode->getPositionY() - extH/2.0);
        
        CCCommonUtils::setButtonTitle(m_quitBtn, _lang("111108").c_str());
        setDesNode(NULL);
//        m_askBtn->setPositionX(m_infoTTF1->getPositionX() + m_infoTTF1->getContentSize().width + 10);
        
        m_quitBtn->setTouchPriority(Touch_Default);
        m_playBtn->setTouchPriority(Touch_Default);
        m_askBtn->setTouchPriority(Touch_Default);
        m_guideNode0->setVisible(false);
        m_guideNode2->setVisible(false);
        m_guideSp0->setVisible(false);
        m_guideSp1->setVisible(false);
        
        m_firstOpen = !LotteryController::shared()->lotteryInfo.hasResetReward;
        
        if (m_firstOpen)
        {
            initAniNode();
            CCCommonUtils::setButtonTitle(m_playBtn, _lang("111119").c_str());
            m_listNode->setVisible(false);
            m_aniListNode->setVisible(true);
            this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(LotteryAct2View::openAniCom));
            this->getAnimationManager()->runAnimationsForSequenceNamed("FadeIn");
        }
        else
        {
            initListNode();
            CCCommonUtils::setButtonTitle(m_playBtn, _lang("111131").c_str());
            m_playBtn->setVisible(false);
            m_listNode->setVisible(true);
            m_aniListNode->setVisible(false);
        }
		bRet = true;
	}while(0);
	return bRet;
}

void LotteryAct2View::initDiamondTTF()
{
    int cost = LotteryController::shared()->getLotteryCost2();
    cost = m_firstOpen?0:cost;
    long total = GlobalData::shared()->resourceInfo.lDiamond;
//    CCString* c = CCString::createWithFormat("%d/", cost);
//    m_infoTTF->setString(c->getCString());
    m_infoTTF->setString("");
    m_infoTTF1->setString(CC_CMDITOA(total));
//    if (total < cost)
//    {
//        m_infoTTF1->setColor(ccRED);
//    }
//    else
//    {
//        ccColor3B color = {205, 203, 200};
//        m_infoTTF1->setColor(color);
//    }
}

void LotteryAct2View::openAniCom()
{
    int guideIndex = CCUserDefault::sharedUserDefault()->getIntegerForKey(LOTTERYACT2_GUIDE, 0);
    if (guideIndex == 0)
    {
        m_guideNode0->setVisible(true);
        m_guideSp0->setVisible(false);
        m_guideSp1->setVisible(false);
        m_guideNode2->setVisible(false);
        string guideStr = _lang("111127");
        m_guideTTF0->setString(guideStr);
        
        int extH = m_guideTTF0->getContentSize().height - 140;
        extH = extH>0?extH:0;
        m_guideBg->setPreferredSize(CCSizeMake(390.0, 185+extH));
        if (CCCommonUtils::isIosAndroidPad())
        {
            m_guideBg->setPreferredSize(CCSizeMake(780, 260+extH));
        }
        
        auto orgSize = CCDirector::sharedDirector()->getVisibleSize();
        CCLayerColor *color = CCLayerColor::create(ccc4(0, 0, 0, 100),orgSize.width,orgSize.height);
        m_guideNode0->addChild(color);
        color->setZOrder(-10);
        color->setPosition(ccp(-orgSize.width/2, -orgSize.height/2 - 30));
        this->getAnimationManager()->setAnimationCompletedCallback(this, NULL);
        this->getAnimationManager()->runAnimationsForSequenceNamed("Default");
        CCUserDefault::sharedUserDefault()->setIntegerForKey(LOTTERYACT2_GUIDE, 0);
        CCUserDefault::sharedUserDefault()->flush();
    }
    else
    {
        this->getAnimationManager()->setAnimationCompletedCallback(this, NULL);
        this->getAnimationManager()->runAnimationsForSequenceNamed("Default");
    }

    
//    if (m_firstOpen)
//    {
//        this->scheduleOnce(schedule_selector(LotteryAct2View::playAnimation), 1.0f);
//    }
}

void LotteryAct2View::guideAniCom()
{
    this->getAnimationManager()->setAnimationCompletedCallback(this, NULL);
    this->getAnimationManager()->runAnimationsForSequenceNamed("Default");
}

void LotteryAct2View::initAniNode(int type/*==0*/)
{
    CCNode* nodeB = NULL;
    CCNode* nodeC = NULL;
    CCNode* nodeC2 = NULL;
    CCSprite* bg = NULL;
    CCSprite* icon = NULL;
    CCSprite* iconBg = NULL;
    CCSprite* sIcon = NULL;
    CCSprite* sIconBg = NULL;
    CCLabelIF* numTTF = NULL;
    string itemId = "";
    string itemCnt = "1";
    int colorInt = 0;
    string color = "";
    vector<string> reward;
    std::string posArrayStr = CCUserDefault::sharedUserDefault()->getStringForKey(LOTTERYACT2_PREVIEW_REWARD);
    vector<string> posArray;
    bool localData =false;
    string savePosArrayStr = "";
    if(!posArrayStr.empty()){
        CCCommonUtils::splitString(posArrayStr, ",", posArray);
        localData = true;
    }else{
        int iTmp = 0;
        while(iTmp<REWARD_NUM){
            posArray.push_back(CC_ITOA(iTmp));
            ++iTmp;
        }
        localData=false;
    }
    
    for (int iIndex1 = 0; iIndex1 < REWARD_NUM; iIndex1++)
    {
        int iChild  = 0;
        if(localData==false){
            int iIndex = CCMathUtils::getRandomInt(0, (posArray.size()-1)*100+99);
            iIndex = floor(iIndex/100);
            std::string indexStr = posArray.at(iIndex);
            iChild = atoi(indexStr.c_str());
            auto iter = posArray.begin();
            iter += iIndex;
            posArray.erase(iter);
            savePosArrayStr = savePosArrayStr.empty()?indexStr:savePosArrayStr+","+indexStr;
        }else{
            iChild =atoi(posArray.at(iIndex1).c_str());
        }
        reward.clear();
        CCLOG("random pos [%d], index [%d]",iChild,iIndex1);
        nodeB = m_aniListNode->getChildByTag(iChild);
        if (nodeB)
        {
            bg = (CCSprite*)nodeB->getChildByTag(3);
            nodeC = nodeB->getChildByTag(2);
            nodeC2 = nodeB->getChildByTag(1);
            
            icon = (CCSprite*)nodeC2->getChildByTag(1);
            numTTF = (CCLabelIF*)nodeC2->getChildByTag(2);
            iconBg = (CCSprite*)nodeC2->getChildByTag(3);
            nodeC2->removeChildByTag(4);
            sIconBg = (CCSprite*)nodeC2->getChildByTag(5);
            
            sIcon = (CCSprite*)nodeC->getChildByTag(0);
            LotteryController::shared()->getRewardInfo(iIndex1, reward);
            itemId = reward.at(0);
            itemCnt = reward.at(1);
            itemCnt = (itemCnt == "" || itemCnt == "0")?"1":itemCnt;
            itemId = type==1?"":itemId;
            nodeC2->setVisible(itemId.length());
            if (itemId.length() > 0)
            {
                icon->removeAllChildren();
                bg->setDisplayFrame(CCLoadSprite::loadResource("Lottery_Card1.png"));
                icon->setDisplayFrame(CCLoadSprite::loadResource(getIcon(itemId).c_str()));
                colorInt = atoi(getColor(itemId).c_str());
                color = CCCommonUtils::getToolBgByColor(colorInt);
                iconBg->setDisplayFrame(CCLoadSprite::loadResource(color.c_str()));
                numTTF->setString(itemCnt);
                icon->setVisible(true);
                sIconBg->setVisible(true);
                numTTF->setVisible(true);
                int itemIdInt = atoi(itemId.c_str());
                if (200<=itemIdInt && itemIdInt<=205)
                {
                    CCLabelIFBMFont* sp = LotteryController::shared()->getLabelBMfont(itemIdInt);
                    sp->setScale(2);
                    icon->setVisible(false);
                    sIconBg->setVisible(false);
                    numTTF->setVisible(false);
                    nodeC2->addChild(sp, 2, 4);
                    
                    string tmpStart = "UIGlow_UD_";
                    string tmpStart2 = "UIGlow_LR_";
                    int count = 3;
                    float offsetP = 87.0f;
                    float offsetS = 1.5f;
                    for (int i=1; i<=count; i++)
                    {
                        if (i == 2)
                        {
                            continue;
                        }
                        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
                        nodeC2->addChild(particle);
                        particle->setPosition(ccp(0, offsetP));
                        particle->setScaleX(offsetS);
                        
                        auto particle2 = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart2.c_str(),i)->getCString());
                        nodeC2->addChild(particle2);
                        particle2->setPosition(ccp(offsetP, 0));
                        particle2->setScaleY(offsetS);
                    }
                    
                    for (int i=1; i<=count; i++)
                    {
                        if (i == 2)
                        {
                            continue;
                        }
                        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
                        nodeC2->addChild(particle);
                        particle->setPosition(ccp(0, -offsetP));
                        particle->setScaleX(offsetS);
                        
                        auto particle2 = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart2.c_str(),i)->getCString());
                        nodeC2->addChild(particle2);
                        particle2->setPosition(ccp(-offsetP, 0));
                        particle2->setScaleY(offsetS);
                    }
                }
                int index = atoi(getColor(itemId).c_str()) + 1;
                sIcon->setDisplayFrame(CCLoadSprite::loadResource(CCString::createWithFormat("Lottery_rating%d.png",index)->getCString()));
                LotteryController::addCountInfo(icon, itemId);
            }
            else
            {
                bg->setDisplayFrame(CCLoadSprite::loadResource("Lottery_Card2.png"));
            }
        }
    }
    if(!savePosArrayStr.empty()){
        CCUserDefault::sharedUserDefault()->setStringForKey(LOTTERYACT2_PREVIEW_REWARD, savePosArrayStr);
    }
}

string LotteryAct2View::getIcon(string itemId)
{
    int id = atoi(itemId.c_str());
    return LotteryController::shared()->getLotteryIcon(id);
}

string LotteryAct2View::getColor(string itemId)
{
    int id = atoi(itemId.c_str());
    if (id >= 200 && id <= 205)
    {
        return "5";
    }
    else if (id != R_GOODS && id < 100)
    {
        return "4";
    }
    else{
        return CCCommonUtils::getPropById(itemId, "color");
    }
}

void LotteryAct2View::initListNode()
{
    vector<string> reward;
    for (int i = 0; i < REWARD_NUM; i++)
    {
        reward.clear();
        LotteryController::shared()->getReward2Map(i, reward);
        LotteryAct2Cell* cell;
        cell = LotteryAct2Cell::create(reward.at(0), i, reward.at(1));
        if (CCCommonUtils::isIosAndroidPad())
        {
            cell->setPosition(ccp((-207.0f+207*(i%3))*2.1 , (157.0f-217*(i/3))*2.1));
            cell->setScale(2.1f);
        }
        else
            cell->setPosition(ccp(-207.0f+207*(i%3) , 157.0f-217*(i/3)));
        cell->setTag(i);
        m_listNode->addChild(cell);
    }
}

void LotteryAct2View::playAnimation(float time)
{
    LotteryController::shared()->lotteryInfo.hasResetReward = true;
    m_listNode->setVisible(false);
    m_aniListNode->setVisible(true);
    initAniNode(1);
    this->scheduleOnce(schedule_selector(LotteryAct2View::removeParticle), 0.667f);
    this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(LotteryAct2View::animationComplete));
    this->getAnimationManager()->runAnimationsForSequenceNamed("Shuffle");

}

void LotteryAct2View::removeParticle(float time)
{
    string tmpStart = "ScienceOut_";
    int count = 9;
    for (int i=1; i<count; i++)
    {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
        m_particleNode->addChild(particle);
    }
}

void LotteryAct2View::animationComplete()
{
    m_particleNode->removeAllChildren();
    initListNode();
    this->getAnimationManager()->runAnimationsForSequenceNamed("1");
    m_listNode->setVisible(true);
    m_aniListNode->setVisible(false);
}

void LotteryAct2View::notifyCallBack(CCObject* pObj)
{
    CCString* p = dynamic_cast<CCString*>(pObj);
    if (p == NULL)
    {
        return;
    }
    if (p->compare("0") == 0)
    {
        initAniNode();
        m_listNode->setVisible(false);
        m_aniListNode->setVisible(true);
        this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(LotteryAct2View::closeAniCom));
        this->getAnimationManager()->runAnimationsForSequenceNamed("FadeOut");
    }
    if (p->compare("1") == 0)
    {
        initDiamondTTF();
    }
}

void LotteryAct2View::closeAniCom()
{
    PopupViewController::getInstance()->removePopupView(this);
    PopupViewController::getInstance()->addPopupInView(LotteryActView::create(true));
}

void LotteryAct2View::onQuitSuperModeClick(cocos2d::CCObject *pSender, CCControlEvent event)
{
//    int guideIndex = CCUserDefault::sharedUserDefault()->getIntegerForKey(LOTTERYACT2_GUIDE, -1);
//    if ((0<=guideIndex && guideIndex < 2) || guideIndex == 3)
//    {
//        return;
//    }
    CCCallFunc* fun = CCCallFunc::create(this, callfunc_selector(LotteryAct2View::yesDialogBack));
    YesNoDialog::show(_lang("111109").c_str(), fun, 0, false);
}

void LotteryAct2View::yesDialogBack()
{
    LotteryController::shared()->lotteryInfo.superMode = 0;
    LotteryCanCelCmd* cmd = new LotteryCanCelCmd();
    cmd->sendAndRelease();
    LotteryController::shared()->lotteryInfo.type = 1;
    LotteryController::shared()->lotteryInfo.reward2Map.clear();
    LotteryController::shared()->lotteryInfo.rewardSort.clear();
    CCUserDefault::sharedUserDefault()->setStringForKey(LotteryAct2ShowView_PREVIEW_REWARD, "");
    CCUserDefault::sharedUserDefault()->setStringForKey(LotteryAct2ShowView_PREVIEW_REWARD0, "");
    CCUserDefault::sharedUserDefault()->setStringForKey(LOTTERY_REWARD_INFO, "");
    CCUserDefault::sharedUserDefault()->flush();
    notifyCallBack(ccs("0"));
}

void LotteryAct2View::onAskClick(cocos2d::CCObject *pSender, CCControlEvent event)
{
//    int guideIndex = CCUserDefault::sharedUserDefault()->getIntegerForKey(LOTTERYACT2_GUIDE, -1);
//    if ((0<=guideIndex && guideIndex < 2) || guideIndex == 3)
//    {
//        return;
//    }
    string info = _lang("111120");
    info.append("\n\n").append(_lang("111114"));
    info.append("\n\n");
    info.append(_lang("111115"));
    PopupViewController::getInstance()->addPopupView(TipsView::create(info, kCCTextAlignmentLeft));
}

void LotteryAct2View::onPlayClick(cocos2d::CCObject *pSender, CCControlEvent event)
{
    if (m_firstOpen)
    {
//        int guideIndex = CCUserDefault::sharedUserDefault()->getIntegerForKey(LOTTERYACT2_GUIDE, -1);
//        if ((0<=guideIndex && guideIndex < 2) || guideIndex == 3)
//        {
//            return;
//        }
        m_firstOpen = false;
        CCUserDefault::sharedUserDefault()->setStringForKey(LOTTERYACT2_PREVIEW_REWARD, "");
        CCUserDefault::sharedUserDefault()->flush();
        SoundController::sharedSound()->playEffects(Music_Sfx_UI_shuffle);
        playAnimation(0);
        CCCommonUtils::setButtonTitle(m_playBtn, _lang("111131").c_str());
        m_playBtn->setVisible(false);
    }
    else{
        onQuitSuperModeClick(NULL,Control::EventType::TOUCH_DOWN);
    }
}
void LotteryAct2View::onViewClick(cocos2d::CCObject *pSender, CCControlEvent event){
//    if (m_firstOpen){
//        
//    }else{
        PopupViewController::getInstance()->addPopupView(LotteryAct2ShowView::create());
//    }
}
void LotteryAct2View::setDesNode(cocos2d::CCObject *pObj, bool desAniInfo/* = false*/)
{
    CCInteger* i = dynamic_cast<CCInteger*>(pObj);
    if (i == NULL){
        m_desNode->setVisible(false);
        return;
    }
    int position = i->getValue();
    m_desNode->setVisible(true);
    vector<string> infoVec;
    if(desAniInfo){
        std::string indexStr = CCUserDefault::sharedUserDefault()->getStringForKey(LOTTERYACT2_PREVIEW_REWARD);
        vector<std::string> infoVec1;
        CCCommonUtils::splitString(indexStr, ",", infoVec1);
        auto iter = find(infoVec1.begin(), infoVec1.end(), CC_ITOA(position));
        if(iter!=infoVec1.end()){
            int iPos = iter - infoVec1.begin();
            LotteryController::shared()->getItemDes(iPos, infoVec, desAniInfo);
        }
    }else{
        LotteryController::shared()->getItemDes(position, infoVec, desAniInfo);
    }
    if(infoVec.size()<2){
        m_desNode->setVisible(false);
        return;
    }
    m_desName->setString(infoVec.at(0));
    m_desLabel->setString(infoVec.at(1));
    float extH = m_desLabel->getContentSize().height - 80;
    extH = extH>0?extH:0;
    m_desBg->setPreferredSize(CCSizeMake(295, 149+extH));
    CCNode* cell = NULL;
    if (desAniInfo){
        cell = m_aniListNode->getChildByTag(position);
    }else{
        cell = m_listNode->getChildByTag(position);
    }
    if(!cell){
        m_desNode->setVisible(false);
        return;
    }
    CCPoint pos = cell->getPosition();
    if (position%3 == 2){
        pos = ccpAdd(pos, ccp(cell->getContentSize().width/2 - 295, 0));
    }else{
        pos = ccpAdd(pos, ccp(295/2, 0));
    }
    m_desNode->setPosition(pos);
    m_desNode->setZOrder(m_listNode->getChildrenCount());
}

void LotteryAct2View::displayDesNode(cocos2d::CCObject *pObj)
{
    setDesNode(pObj);
}

void LotteryAct2View::guideBack(cocos2d::CCObject *pObj)
{
    int dataId = dynamic_cast<CCString*>(pObj)->intValue();
    m_guideNode0->setVisible(false);
    m_guideSp0->setVisible(false);
    m_guideSp1->setVisible(false);
    m_guideNode2->setVisible(true);
    m_guideTTF->setString(_lang("111115"));
    m_insideGuide->setPositionX(0);
    m_guideArrow->setPositionX(-140+dataId%3*140);
    
    if (0<=dataId && dataId<=2)
    {
        m_insideGuide->setPositionY(-30);
        m_guideArrow->setPositionY(80);
    }
    else if (3<=dataId && dataId<=5)
    {
        m_insideGuide->setPositionY(-260);
        m_guideArrow->setPositionY(-150);
    }
    else if (6<=dataId && dataId<=8)
    {
        m_insideGuide->setPositionY(-40);
        m_guideArrow->setPositionY(-150);
    }
    switch (dataId)
    {
        case 0:
        case 1:
        case 3:
        case 4:
            m_guideArrow->setFlipY(true);
            m_guideArrow->setFlipX(false);
            break;
        case 8:
            m_guideArrow->setFlipX(true);
            m_guideArrow->setFlipY(false);
            break;
        case 6:
        case 7:
            m_guideArrow->setFlipX(true);
            m_guideArrow->setFlipY(true);
            break;
        default:
            m_guideArrow->setFlipX(false);
            m_guideArrow->setFlipY(false);
            break;
    }
    int guide = CCUserDefault::sharedUserDefault()->getIntegerForKey(LOTTERYACT2_GUIDE, 0);
    if (guide != 4)
    {
        CCUserDefault::sharedUserDefault()->setIntegerForKey(LOTTERYACT2_GUIDE, 3);
        CCUserDefault::sharedUserDefault()->flush();
    }
    else
    {
        CCUserDefault::sharedUserDefault()->setIntegerForKey(LOTTERYACT2_GUIDE, 5);
        CCUserDefault::sharedUserDefault()->flush();
    }
}

void LotteryAct2View::doListAniAct(CCObject* pObj)
{
    CCDictionary* dict = (CCDictionary*)pObj;
    int dataId = dict->valueForKey("dataId")->intValue();
    string itemId = dict->valueForKey("itemId")->getCString();
    int itemCnt = dict->valueForKey("itemCnt")->intValue();
    
    m_listAniNode->removeAllChildren();
    float baseX = 0.0f;
    float baseY = 0.0f;
    float randomNumX = 0.0f;
    float randomNumY = 0.0f;
    CCPoint beginPos = m_aniListNode->getChildByTag(dataId)->getPosition();
    CCPoint endPos = CCPointZero;
    float rate = 1800.0f;
    float rate2 = 2200.0f;
    float duration = 0.0f;
    float duration2 = 0.0f;
    CCPoint targetP = m_superNode->convertToWorldSpace(m_viewBtn->getPosition());
    targetP = m_listAniNode->convertToNodeSpace(targetP);
    
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_listAniNode->setScale(1.6);
    }
    
    int index = 3;
    for (int i = 0; i < itemCnt; i++)
    {
        index = itemCnt>3?3:(itemCnt-1);
        randomNumX = CCRANDOM_MINUS1_1()*index;
        randomNumY = CCRANDOM_MINUS1_1()*index;
        endPos = ccp(baseX+randomNumX*90, baseY+randomNumY*90);
        if (itemCnt == 1)
        {
            endPos = CCPointZero;
        }
        CCSprite* sp = CCLoadSprite::createSprite(CCCommonUtils::getIcon(itemId).c_str());
        sp->setPosition(beginPos);
        m_listAniNode->addChild(sp);
        duration = abs(ccpDistance(beginPos, endPos)/rate);
        duration2 = abs(ccpDistance(targetP, endPos)/rate2);
        CCMoveTo* move0 = CCMoveTo::create(duration, endPos);
        CCDelayTime* delay = CCDelayTime::create(0.8f);
        CCMoveTo* move1 = CCMoveTo::create(duration2, targetP);
        CCScaleTo* scale = CCScaleTo::create(duration2, 0.5);
        CCSpawn* spawn = CCSpawn::createWithTwoActions(move1, scale);
        CCCallFuncND* callBack = CCCallFuncND::create(this, callfuncND_selector(LotteryAct2View::playComplete), NULL);
        sp->runAction(CCSequence::create(move0, delay, spawn, callBack, NULL));
    }
    if(LotteryController::shared()->lotteryInfo.reward2Map.size()<9){
        m_playBtn->setVisible(false);
    }else{
        m_playBtn->setVisible(true);
    }
}

void LotteryAct2View::playComplete(CCNode* pNode, void *pObj)
{
    pNode->removeFromParent();
}

void LotteryAct2View::playParticle(cocos2d::CCObject *pObj)
{
    int dataId = dynamic_cast<CCString*>(pObj)->intValue();
    CCPoint p = m_aniListNode->convertToWorldSpace(m_aniListNode->getChildByTag(dataId)->getPosition());
    p = m_midNode->convertToNodeSpace(p);
    m_particleNode2->setPosition(p);
    
    string tmpStart = "Synthesis_";
    int count = 8;
    for (int i=1; i<=count; i++) {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
        particle->setScale(3.5);
        m_particleNode2->addChild(particle);
    }
    auto particle2 = ParticleController::createParticle("LevelUp_1");
    m_particleNode2->addChild(particle2);
    auto particle3 = ParticleController::createParticle("LevelUp_7");
    m_particleNode2->addChild(particle3);
    this->scheduleOnce(schedule_selector(LotteryAct2View::removeParticle2), 0.8);
}

void LotteryAct2View::removeParticle2(float dt)
{
    m_particleNode2->removeAllChildren();
}

void LotteryAct2View::onEnter()
{
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(LotteryAct2View::notifyCallBack), LOTTERYACT2VIEW_NOTIFY, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(LotteryAct2View::displayDesNode), LOTTERYACT2CELL_SHOW, NULL);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
    setTitleName(_lang("111107"));
    initDiamondTTF();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(LotteryAct2View::guideBack), LOTTERYACT2_GUIDE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(LotteryAct2View::doListAniAct), LOTTERYACT2_PLAYREWARD, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(LotteryAct2View::playParticle), LOTTERYACT2_PLAYPARTICLE, NULL);
    
}

void LotteryAct2View::onExit()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, LOTTERYACT2VIEW_NOTIFY);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, LOTTERYACT2CELL_SHOW);
    setTouchEnabled(false);
    CCLoadSprite::doResourceByCommonIndex(11, false);
    if (LotteryController::shared()->lotteryInfo.reward2Map.size() >= 9)
    {
        LotteryController::shared()->lotteryInfo.reward2Map.clear();
        CCUserDefault::sharedUserDefault()->setStringForKey(LotteryAct2ShowView_PREVIEW_REWARD, "");
        CCUserDefault::sharedUserDefault()->setStringForKey(LotteryAct2ShowView_PREVIEW_REWARD0, "");
        CCUserDefault::sharedUserDefault()->setStringForKey(LOTTERY_REWARD_INFO, "");
        CCUserDefault::sharedUserDefault()->flush();
    }
    
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, LOTTERYACT2_GUIDE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, LOTTERYACT2_PLAYREWARD);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, LOTTERYACT2_PLAYPARTICLE);
    this->unscheduleAllSelectors();
    CCNode::onExit();
}

int LotteryAct2View::hasTouchNode(cocos2d::CCTouch *pTouch)
{
    CCNode* node = NULL;
    for (int i = 0; i < m_aniListNode->getChildrenCount(); i++)
    {
        node = m_aniListNode->getChildByTag(i);
        if (isTouchInside(node->getChildByTag(0), pTouch))
        {
            return i;
        }
    }
    return -1;
}

bool LotteryAct2View::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    int guideIndex = CCUserDefault::sharedUserDefault()->getIntegerForKey(LOTTERYACT2_GUIDE, 0);
    if (guideIndex == 3 || guideIndex == 0 || guideIndex == 5)
    {
        m_quitBtn->setEnabled(false);
        m_askBtn->setEnabled(false);
        m_playBtn->setEnabled(false);
        return true;
    }
    m_aniIndex = hasTouchNode(pTouch);
    if (m_firstOpen && m_aniIndex>-1)
    {
        setDesNode(CCInteger::create(m_aniIndex), true);
        return true;
    }
    if (guideIndex == 1)
    {
        m_quitBtn->setEnabled(false);
        m_askBtn->setEnabled(false);
        return true;
    }
    return false;
}

void LotteryAct2View::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    int guideIndex = CCUserDefault::sharedUserDefault()->getIntegerForKey(LOTTERYACT2_GUIDE, 0);
    if (guideIndex == 0)
    {
        m_guideNode0->setVisible(false);
        m_guideSp1->setVisible(true);
        m_guideSp0->setVisible(false);
        this->getAnimationManager()->setAnimationCompletedCallback(this, NULL);
        this->getAnimationManager()->runAnimationsForSequenceNamed("Guide1");
        CCUserDefault::sharedUserDefault()->setIntegerForKey(LOTTERYACT2_GUIDE, 1);
        CCUserDefault::sharedUserDefault()->flush();
    }
    else if (guideIndex == 1)
    {
        m_guideNode0->setVisible(false);
        m_guideNode2->setVisible(false);
        m_guideSp1->setVisible(false);
        m_guideSp0->setVisible(false);
        CCUserDefault::sharedUserDefault()->setIntegerForKey(LOTTERYACT2_GUIDE, 2);
        CCUserDefault::sharedUserDefault()->flush();
        this->getAnimationManager()->setAnimationCompletedCallback(this, NULL);
        this->getAnimationManager()->runAnimationsForSequenceNamed("Default");
    }
    else if (guideIndex == 3)
    {
        m_guideNode0->setVisible(false);
        m_guideNode2->setVisible(false);
        m_guideSp1->setVisible(false);
        m_guideSp0->setVisible(false);
        CCUserDefault::sharedUserDefault()->setIntegerForKey(LOTTERYACT2_GUIDE, 4);
        CCUserDefault::sharedUserDefault()->flush();
    }
    else if (guideIndex == 5)
    {
        m_guideNode0->setVisible(false);
        m_guideNode2->setVisible(false);
        m_guideSp1->setVisible(false);
        m_guideSp0->setVisible(false);
        CCUserDefault::sharedUserDefault()->setIntegerForKey(LOTTERYACT2_GUIDE, 6);
        CCUserDefault::sharedUserDefault()->flush();
    }
    m_quitBtn->setEnabled(true);
    m_askBtn->setEnabled(true);
    m_playBtn->setEnabled(true);
    
    m_aniIndex = -1;
    m_desNode->setVisible(false);
}

bool LotteryAct2View::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listNode", CCNode*, m_listNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_aniList", CCNode*, m_aniListNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_superNode", CCNode*, m_superNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_particleNode", CCNode*, m_particleNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_midNode", CCNode*, m_midNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desNode", CCNode*, m_desNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desName", CCLabelIF*, m_desName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelIF*, m_desLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoTTF", CCLabelIF*, m_infoTTF);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoTTF1", CCLabelIF*, m_infoTTF1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_askBtn", CCControlButton*, m_askBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_quitBtn", CCControlButton*, m_quitBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_playBtn", CCControlButton*, m_playBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_viewBtn", CCControlButton*, m_viewBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg0", CCSprite*, m_bg0);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg1", CCSprite*, m_bg1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desBg", CCScale9Sprite*, m_desBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_guideNode0", CCNode*, m_guideNode0);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_guideTTF0", CCLabelIF*, m_guideTTF0);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_guideNode2", CCNode*, m_guideNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_insideGuide", CCNode*, m_insideGuide);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_guideTTF", CCLabelIF*, m_guideTTF);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_guideArrow", CCSprite*, m_guideArrow);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_guideSp0", CCSprite*, m_guideSp0);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_guideSp1", CCSprite*, m_guideSp1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_guideBg", CCScale9Sprite*, m_guideBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_listAniNode", CCNode*, m_listAniNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_particleNode2", CCNode*, m_particleNode2);

    return false;
}

SEL_CCControlHandler LotteryAct2View::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onQuitSuperModeClick", LotteryAct2View::onQuitSuperModeClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAskClick", LotteryAct2View::onAskClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onPlayClick", LotteryAct2View::onPlayClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onViewClick", LotteryAct2View::onViewClick);
    return NULL;
}

//LotteryAct2Cell class
#include "RewardController.h"
#include "GCMRewardController.h"
#include "UseToolView.h"
#include "ToolController.h"
#include "FlyHint.h"

static bool hasSendCmd = false;
LotteryAct2Cell* LotteryAct2Cell::create(string rewardId, int dataId, string rewardCnt)
{
    LotteryAct2Cell* ret = new LotteryAct2Cell();
    if (ret && ret->init(rewardId, dataId, rewardCnt))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool LotteryAct2Cell::init(string rewardId, int dataId, string rewardCnt)
{
    auto ccb = CCBLoadFile("LotteryAct2Cell", this, this);
    this->setContentSize(ccb->getContentSize());
    this->setAnchorPoint(ccp(0.0, 0.0));
    m_dataId = dataId;
    setData(rewardId, rewardCnt);
    setLightEffect();
    return true;
}

void LotteryAct2Cell::setData(string rewardId, string rewardCnt)
{
    m_rewardId = rewardId;
    m_rewardCnt = (rewardCnt=="" || rewardCnt == "0")?"1":rewardCnt;
    m_itemNode->setVisible(rewardId.length()>0);
    string bgFrameName = rewardId.length()>0?"Lottery_Card1.png":"Lottery_Card2.png";
    m_lightBg->setVisible(false);
    this->getAnimationManager()->setAnimationCompletedCallback(this, NULL);
    this->getAnimationManager()->runAnimationsForSequenceNamed("0");
    m_particleNode2->removeAllChildren();
    if (rewardId.length() > 0)
    {
        string icon = getIcon();
        CCLOG("itemId:%s,icon:%s",m_rewardId.c_str(), icon.c_str());
        m_icon->removeAllChildren();
        m_icon->setDisplayFrame(CCLoadSprite::loadResource(icon.c_str()));
        m_numLabel->setString(m_rewardCnt);
        int colorInt = atoi(getColor().c_str());
        string color = CCCommonUtils::getToolBgByColor(colorInt);
        m_iconBg->setDisplayFrame(CCLoadSprite::loadResource(color.c_str()));
        string ratingName = CCString::createWithFormat("Lottery_rating%d.png", (colorInt+1))->getCString();
        m_lotteryRatingSp->setDisplayFrame(CCLoadSprite::loadResource(ratingName.c_str()));
        int itemId = atoi(m_rewardId.c_str());
        m_itemNode->removeChildByTag(2);
        m_numBg->setVisible(true);
        m_numLabel->setVisible(true);
        if (200<=itemId && itemId <= 205)
        {
            CCLabelIFBMFont* sp = LotteryController::shared()->getLabelBMfont(itemId);
            sp->setScale(2);
            sp->setAnchorPoint(ccp(0.5, 0.5));
            m_itemNode->addChild(sp, 2, 2);
            CCSize nodeSize = m_itemNode->getContentSize();
            sp->setPosition(ccp(nodeSize.width*0.5, nodeSize.height*0.5));
            m_numBg->setVisible(false);
            m_numLabel->setVisible(false);
            
            setMultiEffect(NULL);
        }
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification("LOTTERYACT2CELL_EFFECT");
        
        LotteryController::addCountInfo(m_itemNode, m_rewardId);
    }
    
    m_bg->setDisplayFrame(CCLoadSprite::loadResource(bgFrameName.c_str()));
}

void LotteryAct2Cell::setMultiEffect(CCObject* pObj)
{
    m_particleNode2->removeAllChildren();
    if (!LotteryController::shared()->isMultiReward(m_rewardId))
    {
        CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, "LOTTERYACT2CELL_EFFECT");
        return;
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(LotteryAct2Cell::setMultiEffect), "LOTTERYACT2CELL_EFFECT", NULL);
    string tmpStart = "UIGlow_UD_";
    string tmpStart2 = "UIGlow_LR_";
    int count = 3;
    float offsetP = 87.0f;
    float offsetS = 1.5f;
    for (int i=1; i<=count; i++)
    {
        if (i == 2)
        {
            continue;
        }
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
        m_particleNode2->addChild(particle);
        particle->setPosition(ccp(0, offsetP));
        particle->setScaleX(offsetS);
        
        auto particle2 = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart2.c_str(),i)->getCString());
        m_particleNode2->addChild(particle2);
        particle2->setPosition(ccp(offsetP, 0));
        particle2->setScaleY(offsetS);
    }
    
    for (int i=1; i<=count; i++)
    {
        if (i == 2)
        {
            continue;
        }
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
        m_particleNode2->addChild(particle);
        particle->setPosition(ccp(0, -offsetP));
        particle->setScaleX(offsetS);
        
        auto particle2 = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart2.c_str(),i)->getCString());
        m_particleNode2->addChild(particle2);
        particle2->setPosition(ccp(-offsetP, 0));
        particle2->setScaleY(offsetS);
    }
}

void LotteryAct2Cell::setLightEffect()
{
    if (m_dataId == 0)
    {
        m_lightBg->setVisible(m_rewardId.length()==0);
    }
    switch (m_dataId)
    {
        case 0:
            onGameTick(0);
            break;
        case 1:
        case 3:
            this->scheduleOnce(schedule_selector(LotteryAct2Cell::onGameTick), 0.5f);
            break;
        case 2:
        case 4:
        case 6:
            this->scheduleOnce(schedule_selector(LotteryAct2Cell::onGameTick), 1.0);
            break;
        case 5:
        case 7:
            this->scheduleOnce(schedule_selector(LotteryAct2Cell::onGameTick), 1.5f);
            break;
        case 8:
            this->scheduleOnce(schedule_selector(LotteryAct2Cell::onGameTick), 2.0f);
        default:
            break;
    }
    
}

void LotteryAct2Cell::onGameTick(float dt)
{
    m_lightBg->setVisible(m_rewardId.length()==0);
    this->getAnimationManager()->setAnimationCompletedCallback(this, NULL);
    this->getAnimationManager()->runAnimationsForSequenceNamed("2");
}

string LotteryAct2Cell::getIcon()
{
    int itemId = atoi(m_rewardId.c_str());
    if (itemId >= 200 && itemId <= 205)
    {
        return m_rewardId;
    }
    return LotteryController::shared()->getLotteryIcon(itemId);
}

string LotteryAct2Cell::getColor()
{
    int itemId = atoi(m_rewardId.c_str());
    if (itemId >= 200 && itemId <= 205)
    {
        return "5";
    }
    else if (itemId != R_GOODS && itemId < 100)
    {
        return "4";
    }
    else{
        return CCCommonUtils::getPropById(m_rewardId, "color");
    }
}

void LotteryAct2Cell::onEnter()
{
    CCNode::onEnter();
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(LotteryAct2Cell::cmdCallBack), LOTTERYACTVIEWCMD2, NULL);
    int itemId = atoi(m_rewardId.c_str());
    if (itemId >= 200 && itemId <= 205)
    {
        CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(LotteryAct2Cell::setMultiEffect), "LOTTERYACT2CELL_EFFECT", NULL);
    }
}

void LotteryAct2Cell::onExit()
{
    hasSendCmd = false;
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, LOTTERYACTVIEWCMD2);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, "LOTTERYACT2CELL_EFFECT");
    this->unschedule(schedule_selector(LotteryAct2Cell::onGameTick));
    CCNode::onExit();
}

bool LotteryAct2Cell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    int guideIndex = CCUserDefault::sharedUserDefault()->getIntegerForKey(LOTTERYACT2_GUIDE, 0);
    if (guideIndex == 3)
    {
        return false;
    }
    if (isTouchInside(m_touchNode, pTouch))
    {
        setDesNode();
        return true;
    }
    return false;
}

void LotteryAct2Cell::setDesNode()
{
    if (m_rewardId.length() == 0)
    {
        return;
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(LOTTERYACT2CELL_SHOW, CCInteger::create(m_dataId));
}

void LotteryAct2Cell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if ((ccpDistance(pTouch->getStartLocation(), pTouch->getLocation())<10) && (m_rewardId.length()==0))
    {
        onTouchClick();
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(LOTTERYACT2CELL_SHOW, NULL);
}

void LotteryAct2Cell::onTouchClick()
{
    if (hasSendCmd)
    {
        return;
    }
    int need = LotteryController::shared()->getLotteryCost2();
    int cost = need - GlobalData::shared()->resourceInfo.lDiamond;
    if (cost > 0)
    {
        int money = ceil(cost*LotteryController::shared()->getRate2());
        CCCallFuncO* fun = CCCallFuncO::create(this, callfuncO_selector(LotteryAct2Cell::yesDialogBack), CCInteger::create(money));
        YesNoDialog::lotteryShow(_lang_1("111112", CC_ITOA(need)), fun, 0, true, NULL, NULL, _lang("102153").c_str(), "item410.png");
        return;
    }
    if (need > 0)
    {
        CCCallFunc* fun = CCCallFunc::create(this, callfunc_selector(LotteryAct2Cell::yesDialogBack2));
        YesNoDialog::lotteryShow(_lang("111111").c_str(), fun, 0, false, NULL, NULL, CC_ITOA(need), "item410.png");
        return;
    }
    if (need == 0)
    {
        CCCallFunc* fun = CCCallFunc::create(this, callfunc_selector(LotteryAct2Cell::yesDialogBack2));
        YesNoDialog::lotteryShow(_lang("111111").c_str(), fun, 0, true, NULL, NULL, _lang("111124").c_str(), "item410.png");
        return;
    }
    sendCmd(0);
}

void LotteryAct2Cell::yesDialogBack2()
{
    sendCmd(0);
}

void LotteryAct2Cell::yesDialogBack(CCObject* pObj)
{
    CCDictionary* dict = CCDictionary::create();
    PopupViewController::getInstance()->addPopupInView(UseToolView::create(USE_TOOL_LOTTERY2,dict,"111102"));
    
}

void LotteryAct2Cell::sendCmd(int useGold)
{
    hasSendCmd = true;
    m_rewardId = "1";
    m_waitInterface = GameController::getInstance()->showWaitInterface(this);
    LotteryActCommand* cmd = new LotteryActCommand(m_dataId, useGold, 2);
//    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(LotteryAct2Cell::cmdCallBack), NULL));
    cmd->sendAndRelease();
}

void LotteryAct2Cell::cmdCallBack(CCObject *pObj)
{
    if (m_rewardId != "1")
    {
        return;
    }
    hasSendCmd = false;
    if (m_waitInterface)
    {
        m_waitInterface->removeFromParent();
        m_waitInterface = nullptr;
    }
    m_rewardId = "";
    
    NetResult* result = dynamic_cast<NetResult*>(pObj);
    if (result == NULL)
    {
        return;
    }
    CCDictionary* dic = _dict(result->getData());
    const CCString *pStr = dic->valueForKey("errorCode");
    if (pStr->compare("")!=0)
    {
        return;
    }
    m_rewardDic = _dict(dic->objectForKey("rewards"));
    int rewardId = m_rewardDic->valueForKey("rewardId")->intValue();
    if (rewardId == 0)
    {
        return;
    }
    m_itemNode->setVisible(true);
    SoundController::sharedSound()->playEffects(Music_Sfx_UI_drafting);
    doRewardAni();
    
    string tmpStart = "Reversal_";
    int count = 5;
    for (int i=1; i<count; i++)
    {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
        m_particleNode->addChild(particle);
    }
    
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(LOTTERYACT2_PLAYPARTICLE, CCString::createWithFormat("%d", m_dataId));
    
    this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(LotteryAct2Cell::aniComplete));
    this->getAnimationManager()->runAnimationsForSequenceNamed("1");
    
    LotteryController::addCountInfo(m_itemNode, m_rewardId);
}

void LotteryAct2Cell::aniComplete()
{
    m_particleNode->removeAllChildren();
    this->getAnimationManager()->setAnimationCompletedCallback(this, NULL);
    this->getAnimationManager()->runAnimationsForSequenceNamed("0");
}

void LotteryAct2Cell::doRewardAni()
{
    int rewardId = m_rewardDic->valueForKey("rewardId")->intValue();
    int multiple = m_rewardDic->valueForKey("multiple")->intValue();
    multiple = multiple<=0?1:multiple;
    string itemId = "";
    int itemCnt = 1;
    if (rewardId >= 200 && rewardId <= 205)
    {
        itemId = m_rewardDic->valueForKey("rewardId")->getCString();
        itemCnt = 1;
    }
    else
    {
        int type = m_rewardDic->valueForKey("type")->intValue();
        if (type == R_EQUIP) {
            CCArray* array = dynamic_cast<CCArray*>(m_rewardDic->objectForKey("value"));
            if(array && array->count()>0){
                CCDictionary *ccDict = _dict(array->objectAtIndex(0));
                itemId = ccDict->valueForKey("itemId")->getCString();
                itemCnt = array->count();
            }
        }else{
            if (type != R_GOODS)
            {
                itemId = CC_ITOA(type);
                itemCnt = m_rewardDic->valueForKey("value")->intValue();
            }
            else
            {
                CCDictionary* value = _dict(m_rewardDic->objectForKey("value"));
                itemId = value->valueForKey("itemId")->getCString();
                itemCnt = value->valueForKey("rewardAdd")->intValue();
            }
        }
        
//        CCArray* arr = CCArray::create();
//        arr->addObject(m_rewardDic);
//        GCMRewardController::getInstance()->retReward2(arr, false);
        CCDictionary* dict = CCDictionary::create();
        dict->setObject(CCString::createWithFormat("%d",m_dataId), "dataId");
        dict->setObject(ccs(itemId), "itemId");
        dict->setObject(CCString::createWithFormat("%d", itemCnt), "itemCnt");
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(LOTTERYACT2_PLAYREWARD, dict);
    }
    string cntStr = CC_ITOA(itemCnt/multiple);
    vector<string> strVec;
    strVec.push_back(itemId);
    strVec.push_back(cntStr);
    setData(itemId, cntStr);
//    LotteryController::shared()->setReward2Map(m_dataId, strVec);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(LOTTERYACT2VIEW_NOTIFY, ccs("1"));
    
    int id = atoi(itemId.c_str());
    auto hint = FlyHint::create(getIcon(), "", _lang_1("104913", LotteryController::shared()->getLotteryName(id, itemCnt).c_str()),3,0,true);
    if (hint != NULL) {
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        hint->setPosition(ccp(size.width / 2, size.height*0.8));
    }
    
    int guideIndex = CCUserDefault::sharedUserDefault()->getIntegerForKey(LOTTERYACT2_GUIDE, 0);
    if (200<=id && id<= 205 && guideIndex > 0 && guideIndex!=6)
    {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(LOTTERYACT2_GUIDE, CCString::createWithFormat("%d", m_dataId));
    }
}

SEL_CCControlHandler LotteryAct2Cell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName)
{
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onRewardClick", ActivityListCell::onRewardClick);
    return NULL;
}

bool LotteryAct2Cell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemNode", CCNode*, m_itemNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_particleNode", CCNode*, m_particleNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCSprite*, m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_icon", CCSprite*, m_icon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconBg", CCSprite*, m_iconBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lotteryRatingSp", CCSprite*, m_lotteryRatingSp);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel", CCLabelIF*, m_numLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numBg", CCSprite*, m_numBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_particleNode2", CCNode*, m_particleNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lightBg", CCSprite*, m_lightBg);
    return false;
}

