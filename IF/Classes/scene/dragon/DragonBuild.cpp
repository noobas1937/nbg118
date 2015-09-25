//
//  DragonBuild.cpp
//  IF
//
//  Created by 付彬 on 15/6/5.
//
//

#include "DragonBuild.h"
#include "SceneController.h"
#include "SoundController.h"
#include "PopupViewController.h"
#include "ScienceListView.h"
#include "WatchtowerView.h"
#include "MainCityView.h"
#include "BuildingHospitalPopUpView.h"
#include "AllianceMarketView.h"
#include "BattleView.h"
#include "QueueController.h"
#include "ToolController.h"
#include "WorldController.h"

#include "FunBuildView.h"
#include "MakingSoldierView.h"
#include "ProductionSoldiersView.h"
#include "ItemStatusView.h"
#include "YesNoDialog.h"
#include "AllianceManager.h"
#include "ArmyController.h"
#include "GuideController.h"
#include "HarvestSkillAni.h"
#include "AllianceIntroTip.h"
#include "EquipmentController.h"
#include "ToolCreateView.h"
#include "DragonBuildingController.h"
#include "DragonScene.h"
#include "DragonBuildListView.h"

using namespace cocos2d;

const float nameWidth = 456;
const float nameHeight = 164;

DragonBuild* DragonBuild::create(int itemId, CCLabelBatchNode* nameLayer)
{
    DragonBuild *pRet = new DragonBuild();
    if (pRet && pRet->initDragonBuild(itemId,nameLayer))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

DragonBuild* DragonBuild::createTmpBuild(int itemId, int x, int y, CCSpriteBatchNode* batchNode, int zOrder, CCSpriteBatchNode* blentbatch)
{
    DragonBuild *pRet = new DragonBuild();
    if (pRet && pRet->initTmpBuild(itemId,x,y,batchNode,zOrder,blentbatch))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool DragonBuild::initTmpBuild(int itemId, int x, int y, CCSpriteBatchNode* batchNode, int zOrder, CCSpriteBatchNode* blentbatch)
{
    if(m_sprArray) {
        m_sprArray->removeAllObjects();
        m_personArray->removeAllObjects();
        m_blentSprArray->removeAllObjects();
    }
    else {
        m_sprArray = CCArray::create();
        m_personArray = CCArray::create();
        m_blentSprArray = CCArray::create();
    }
    string pic = CCCommonUtils::getPropById(CC_ITOA(itemId), "pic");
    string ccbName = pic + "_" + CC_ITOA(GlobalData::shared()->contryResType) + "_1";
    m_mainNode = CCNode::create();
    this->addChild(m_mainNode);
    CCBLoadFile(ccbName.c_str(),m_mainNode,this);
    
    parentX = x;
    parentY = y;
    m_batchNode = batchNode;
    m_blentBatchNode = blentbatch;
    
    int tmpOrd = 100;
    if(m_mainNode) {
        auto array = m_mainNode->getChildren();
        if (array.size()>0) {
            CCNode* tmpNode = dynamic_cast<CCNode*>(array.at(0));//ccb 中的layer层
            if (tmpNode) {
                auto arr1 = tmpNode->getChildren();
                if (arr1.size()>0) {
                    CCNode* allSprNode = dynamic_cast<CCNode*>(arr1.at(0));//ccb 中的layer的 node
                    if (allSprNode) {
                        auto& arrSpr = allSprNode->getChildren();
                        if (true) {
                            for (int i=0; i<arrSpr.size(); i++) {
                                CCSprite* tmpSpr = dynamic_cast<CCSprite*>(arrSpr.at(i));
                                if (tmpSpr) {
                                    tmpSpr->removeFromParent();
                                    tmpSpr->getTexture()->setAntiAliasTexParameters();
                                    
                                    if (m_blentBatchNode) {
                                        auto batchBlend = m_blentBatchNode->getBlendFunc();
                                        auto particleBlend = tmpSpr->getBlendFunc();
                                        if (batchBlend.src == particleBlend.src && batchBlend.dst == particleBlend.dst) {
                                            tmpSpr->setPosition(ccp(tmpSpr->getPositionX()+parentX, tmpSpr->getPositionY()+parentY));
                                            m_blentBatchNode->addChild(tmpSpr);
                                            m_blentSprArray->addObject(tmpSpr);
                                        }
                                        else {
                                            m_batchNode->addChild(tmpSpr);
                                            m_sprArray->addObject(tmpSpr);
                                        }
                                    }
                                    else {
                                        m_batchNode->addChild(tmpSpr);
                                        m_sprArray->addObject(tmpSpr);
                                    }
                                    tmpSpr->setZOrder(zOrder*1000+tmpOrd);
                                    tmpOrd++;
                                    i--;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if(m_arrSpr) {
        this->m_arrSpr->setVisible(false);
    }
    if(m_lvBG) {
        this->m_arrSpr->setVisible(false);
    }
    return true;
}

bool DragonBuild::initDragonBuild(int itemId, CCLabelBatchNode* nameLayer)
{
    m_gainSpr = NULL;
    m_buildingKey = itemId;
    m_nameLayer = nameLayer;
    
    m_signNode = CCNode::create();   //这两行代码前移，避免return 后  m_signNode和m_upEffectNode没有构建
    m_upEffectNode = CCNode::create();
    m_forgeEffectNode = CCNode::create();
    
    if(m_sprArray) {
        m_sprArray->removeAllObjects();
        m_personArray->removeAllObjects();
        m_blentSprArray->removeAllObjects();
    }
    else {
        m_sprArray = CCArray::create();
        m_personArray = CCArray::create();
        m_blentSprArray = CCArray::create();
    }
    
    if(itemId > 1000)
    {
        m_info = &((*DragonBuildingController::getInstance()->curDragonBuildsInfo)[m_buildingKey]);
        
        m_mainNode = CCNode::create();
        this->addChild(m_mainNode);
        
        int lv = 1;
        for (int i=0; i<m_info->picVec.size(); i++) {
            int blv = atoi(m_info->picVec[i].c_str());
            if (m_info->level<=blv) {
                lv = i+1;
                break;
            }
            else {
                if (i == m_info->picVec.size()-1) {
                    lv = 5;
                }
            }
        }
        lv = lv>5?5:lv;
        
        string ccbName = m_info->pic + "_" + CC_ITOA(GlobalData::shared()->contryResType) + "_" + CC_ITOA(lv);
        CCBLoadFile(ccbName.c_str(),m_mainNode,this);
        
        auto pt = m_touchNode->getPosition();
        mainWidth = (m_touchNode->getContentSize().width*m_touchNode->getScaleX()/2+pt.x)*2;
        mainHeight = (m_touchNode->getContentSize().height*m_touchNode->getScaleY()/2+pt.y)*2;
        
        if (m_batchNode && m_tile) {
            m_tile->removeFromParent();
            m_tile = NULL;
        }
        
        m_lvLabel = CCLabelBatch::create(CC_ITOA(m_info->level), m_nameLayer);
        m_lvLabel->setScale(0.6);
        m_lvLabel->setAnchorPoint(ccp(0.5,0.5));
        m_lvLabel->setColor(ccc3(255, 255, 0));
        m_lvLabel->setSkewY(22.5);
        
        if (m_info->open>0  && m_info->open>DragonBuildingController::getInstance()->getMainCityLv()) {
            m_lockIcon = CCLoadSprite::createSprite("build_lock.png");
            m_lockIcon->setAnchorPoint(ccp(0.5, 0.5));
            int lockX = mainWidth/2;
            int lockY = mainHeight/2;
            m_lockIcon->setPosition(ccp(lockX, lockY));
            
            m_lvLabel->setVisible(false);
            m_arrSpr->setVisible(false);
            m_lvBG->setVisible(false);
        }
        onCheckOutPut();
        
        m_effectSpr = CCSprite::create();
        m_buildState = NULL;
        
        m_moveFrame = CCLoadSprite::createSprite("Tile_frame.png");
        m_moveFrame->setPosition(ccp(93, 56));
        m_moveFrame->setVisible(false);
        if ((m_info->pos>=6 && m_info->pos<=16) || (m_info->pos>=52 && m_info->pos<=53)) {
            m_moveFrame->setScale(1.3);
            m_moveFrame->setPosition(ccp(131, 67));
        }
    }
    else
    {
        string tileName = "build_tile.png";
        if(itemId>16) {
            tileName = "res_tile.png";
            if (itemId>=47 && itemId<=51) {
                tileName = "res_tile_1.png";
            }
            else if (itemId>=42 && itemId<=46) {
                tileName = "res_tile_2.png";
            }
            else if (itemId>=37 && itemId<=41) {
                tileName = "res_tile_4.png";
            }
            else if (itemId>=27 && itemId<=31) {
                tileName = "res_tile_4.png";
            }
            else if (itemId>=52 && itemId<=54) {
                tileName = "build_tile.png";
            }
        }
        
        m_tile = CCLoadSprite::createSprite(tileName.c_str());
        m_tile->getTexture()->setAntiAliasTexParameters();
        this->setContentSize(m_tile->getContentSize());
        m_tile->setAnchorPoint(ccp(0,0));
        mainWidth = m_tile->getContentSize().width;
        mainHeight = m_tile->getContentSize().height;
        
        auto& tileInfo = DragonBuildingController::getInstance()->m_tileMap[itemId];
//        tileInfo.state = 2;
        if (tileInfo.state == FUN_BUILD_LOCK || tileInfo.xmlOpen==1) {
            m_tile->setVisible(false);
        }
        
        m_moveFrame = CCLoadSprite::createSprite("Tile_frame.png");
        m_moveFrame->setPosition(ccp(93, 56));
        m_moveFrame->setVisible(false);
        
        return true;
    }
    this->setContentSize(CCSizeMake(mainWidth, mainHeight));
    this->setAnchorPoint(ccp(0,0));
    
    
    if(this->getAnimationManager()->getSequenceId("Default Timeline") >=0) {
        this->getAnimationManager()->runAnimationsForSequenceNamed("Default Timeline");
    }
    
    return true;
}

void DragonBuild::onShowBuildState(CCObject* params)
{
    int itemId = dynamic_cast<CCInteger*>(params)->getValue();
    if (itemId == m_buildingKey) {
        addDragonBuildState();
    }
}

void DragonBuild::onRemoveBuildState(CCObject* params)
{
    int itemId = dynamic_cast<CCInteger*>(params)->getValue();
    if (itemId == m_buildingKey) {
        removeDragonBuildState();
        onHideParticle();
        onShowParticleByType(2);
    }
}

void DragonBuild::unLockTile(CCObject* params)
{
    if(params) {
        int itemId = dynamic_cast<CCInteger*>(params)->getValue();
        if (itemId == m_buildingKey) {
            m_tile->setOpacity(0);
            m_tile->setVisible(true);
            auto delate = CCDelayTime::create(1.6);
            auto fadeIn = CCFadeIn::create(1.0);
            m_tile->runAction(CCSequence::create(delate,fadeIn,NULL));
        }
    }
}

void DragonBuild::onOpenLock()
{
    if (m_lockIcon) {
        if (m_info->open<=DragonBuildingController::getInstance()->getMainCityLv()) {
            playLockJump();
            isUnLock = true;
        }
    }
}

void DragonBuild::playLockJump()
{
    CCActionInterval* move = CCMoveBy::create(0.5, ccp(0, 10));
    CCActionInterval * easeElasticIn= CCEaseElasticIn::create(move);
    CCActionInterval* move1 = CCMoveBy::create(0.5, ccp(0, -10));
    CCActionInterval * easeElasticIn1= CCEaseElasticIn::create(move1);
    CCSequence* sequence = CCSequence::create(easeElasticIn,easeElasticIn1,NULL);
    m_lockIcon->runAction(CCRepeatForever::create(sequence));
}

void DragonBuild::playOpenLock()
{
    isUnLock = false;
    m_lockIcon->stopAllActions();
    m_lockIcon->setVisible(false);
    m_lockIcon->removeFromParent();
    m_lockIcon = NULL;
    m_lvLabel->setVisible(true);
    m_arrSpr->setVisible(true);
    m_lvBG->setVisible(true);
}

void DragonBuild::setTileBatch(int x, int y, CCSpriteBatchNode* batchNode, int zOrder)
{
    parentX = x;
    parentY = y;
    m_batchNode = batchNode;
    m_zOrder = zOrder;
    
    m_tile->setPosition(ccp(0, 0));
    m_tile->setAnchorPoint(ccp(0, 0));
    m_batchNode->addChild(m_tile);
    m_sprArray->addObject(m_tile);
    
    m_tile->setZOrder(zOrder*1000);

    m_batchNode->addChild(m_moveFrame);
    m_sprArray->addObject(m_moveFrame);
    m_moveFrame->setZOrder(zOrder*1000+1);
}

void DragonBuild::addTips(){
    
}
void DragonBuild::removeTips(){
    if(m_popLayer && m_tipNode){
        m_popLayer->removeChild(m_tipNode);
        m_tipNode=NULL;
    }
}
void DragonBuild::setNamePos(int x, int y, CCLayer* sginLayer, CCLayer* popLayer, CCSpriteBatchNode* batchNode, int zOrder, CCSpriteBatchNode* blentbatch)
{
    parentX = x;
    parentY = y;
    
    m_signLayer = sginLayer;
    m_batchNode = batchNode;
    m_blentBatchNode = blentbatch;
    m_popLayer = popLayer;
    
    if (m_lvLabel && m_lvNode) {
        m_lvLabel->setPosition((m_lvNode->getPosition())+ccp(x, y));
    }
    
    if (m_lockIcon) {
        m_lockIcon->setPosition(m_lockIcon->getPosition()+ccp(x, y));
//        m_popBatchNode->addChild(m_lockIcon);
    }
    
    m_signNode->setPosition(ccp(x,y));
    m_upEffectNode->setPosition(ccp(x+mainWidth/2,y+mainHeight/2));
    m_forgeEffectNode->setPosition(ccp(x+mainWidth/2,y+mainHeight/2));
    sginLayer->addChild(m_signNode);
    sginLayer->addChild(m_upEffectNode);
    sginLayer->addChild(m_forgeEffectNode);
    
    int tmpOffx = parentX;
    int tmpOffy = parentY;
    
    m_zOrder = zOrder;
    int tmpOrd = 1;
    
    m_batchNode->addChild(m_moveFrame);
    m_sprArray->addObject(m_moveFrame);
    m_moveFrame->setZOrder(zOrder*1000+tmpOrd);
    tmpOrd ++;
    
    if(m_mainNode) {
        auto array = m_mainNode->getChildren();
        if (array.size()>0) {
            CCNode* tmpNode = dynamic_cast<CCNode*>(array.at(0));//ccb 中的layer层
            if (tmpNode) {
                auto arr1 = tmpNode->getChildren();
                if (arr1.size()>0) {
                    CCNode* allSprNode = dynamic_cast<CCNode*>(arr1.at(0));//ccb 中的layer的 node
                    if (allSprNode) {
                        auto& arrSpr = allSprNode->getChildren();
                        if (true) {
                            int j = 0;
                            for (int i=0; i<arrSpr.size(); i++) {
                                j++;
                                CCSprite* tmpSpr = dynamic_cast<CCSprite*>(arrSpr.at(i));
                                if (tmpSpr) {
                                    tmpSpr->removeFromParent();
                                    tmpSpr->getTexture()->setAntiAliasTexParameters();
                                    
                                    if (m_blentBatchNode) {
                                        auto batchBlend = m_blentBatchNode->getBlendFunc();
                                        auto particleBlend = tmpSpr->getBlendFunc();
                                        if (batchBlend.src == particleBlend.src && batchBlend.dst == particleBlend.dst) {
                                            tmpSpr->setPosition(ccp(tmpSpr->getPositionX()+tmpOffx, tmpSpr->getPositionY()+tmpOffy));
                                            m_blentBatchNode->addChild(tmpSpr, zOrder * 100 + tmpOrd);
                                            m_blentSprArray->addObject(tmpSpr);
                                        }
                                        else {
                                            m_batchNode->addChild(tmpSpr);
                                            m_sprArray->addObject(tmpSpr);
                                        }
                                    }
                                    else {
                                        m_batchNode->addChild(tmpSpr);
                                        m_sprArray->addObject(tmpSpr);
                                    }
                                    tmpSpr->setZOrder(zOrder*1000+tmpOrd);
                                    tmpOrd++;
                                    i--;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else {
    }
    arrY = m_arrSpr->getPositionY();
    drowEffectSpr(zOrder, tmpOrd);
    drowPersonSpr(zOrder, tmpOrd);
    
    if (isCanRecState()) {
        addDragonBuildState();
        if(m_buildState) {
            m_buildState->onShowRecState();
        }
        onShowParticleByType(1);
    }
    if (m_info->state == FUN_BUILD_CREATE) {
        showUping();
    }
}

void DragonBuild::onBuildDelete()
{
    if(!GlobalData::shared()->isInitFlag){
        return;
    }
    
    if (m_signLayer) {
        hideUping();
        m_signLayer->removeChild(m_signNode);
        m_signLayer->removeChild(m_upEffectNode);
        m_signLayer->removeChild(m_forgeEffectNode);
        
        onHideParticle();
        onHideSpeParticle();
        onHideForgeFirePar();
    }
    
    if (m_nameLayer) {
        m_nameLayer->removeChild(m_lvLabel, true);
    }
    
    removeDragonBuildState();
    removeTips();
    for (int i=0; i<m_sprArray->count(); i++) {
        CCSprite* spr = dynamic_cast<CCSprite*>(m_sprArray->objectAtIndex(i));
        if (spr)
        {
            spr->removeFromParentAndCleanup(true);
        }
    }
    m_sprArray->removeAllObjects();
    
    for (int i=0; i<m_personArray->count(); i++) {
        Person* person = dynamic_cast<Person*>(m_personArray->objectAtIndex(i));
        person->onPersonDelete();
    }
    m_personArray->removeAllObjects();
    if (m_blentBatchNode) {
        for (int i=0; i<m_blentSprArray->count(); i++) {
            CCSprite* spr = dynamic_cast<CCSprite*>(m_blentSprArray->objectAtIndex(i));
            m_blentBatchNode->removeChild(spr, true);
        }
        m_blentSprArray->removeAllObjects();
    }
    
    hideTmpBuild();
}

void DragonBuild::onStartFlyOutPut()
{
    this->schedule(schedule_selector(DragonBuild::onFlyOutPut), 30);
}

void DragonBuild::onFlyOutPut(float _time)
{
    if (true) {
        int ot = DragonBuildingController::getInstance()->getOutPut(m_info->itemId);
        string temp = "+";
        temp = temp+CC_ITOA(ot);
        CCCommonUtils::flyUiResText(temp.c_str(), this->m_signNode,ccp(70, 60) ,ccGREEN, floating_type_ui1, 23);
    }
}

void DragonBuild::quickProduceTroop(CCObject* param)
{
    std::string paramString = dynamic_cast<CCString*>(param)->getCString();
    std::vector<string> paramVect;
    CCCommonUtils::splitString(paramString, ";", paramVect);
    int itemId = atoi(paramVect[0].c_str());
    bool bAutoRec = atoi(paramVect[1].c_str()) == 1? true:false;
    if (itemId == m_buildingKey && isCanRecState()) {
        addDragonBuildState();
        if(m_buildState) {
            m_buildState->onShowRecState();
        }
        onShowParticleByType(1);
        if(bAutoRec){
            if(m_buildState){
                m_buildState->autoClickRecBtn();
            }
        }
    }
}

void DragonBuild::buildFunctionEffect(CCObject* param)
{
    if(!param){
        return;
    }
    int itemId = dynamic_cast<CCInteger*>(param)->getValue();
    if (itemId == m_buildingKey) {
        this->checkIncreasedStatus();
    }
}

void DragonBuild::addDragonBuildState()
{
    if (m_popLayer && m_buildState==NULL) {
        removeTips();
        m_buildState = DragonBuildState::create(m_buildingKey);
        m_buildState->setPosition(ccp(parentX+mainWidth/2, parentY));
        m_popLayer->addChild(m_buildState,m_zOrder);
    }
}

void DragonBuild::removeDragonBuildState()
{
    if (m_popLayer && m_buildState) {
        m_popLayer->removeChild(m_buildState);
        m_buildState = NULL;
    }
}

bool DragonBuild::isCanRecState()
{
    int tqid = QID_MAX;
    int buildType = m_buildingKey/1000;
    int qType = CCCommonUtils::getQueueTypeByBuildType(buildType);
    if (qType>0) {
        tqid = QueueController::getInstance()->getCanRecQidByType(qType);
    }
    if (tqid != QID_MAX) {
        return true;
    }
    return false;
}

void DragonBuild::onCheckOutPut()
{
    if(!GlobalData::shared()->isInitFlag){
        return;
    }
    m_output = atoi(m_info->para[1].c_str());
}

void DragonBuild::onResetDirc(float _time)
{
    m_info->is_Dirc = false;
}
void DragonBuild::showUping()
{
    m_signNode->removeAllChildren();
    if ( m_info->is_Dirc ) {
        onShowParticleByType(1);
        this->scheduleOnce(schedule_selector(DragonBuild::onResetDirc), 60);
        return;
    }
    
    int tmppX = 0;
    int tmppY = 0;
    m_buildCCB = BuildCCB::create(0);
    int tmpOffx = 0;
    int tmpOffy = 0;
    m_buildCCB->setNamePos(tmpOffx+mainWidth/2+tmppX, tmpOffy+mainHeight/2+tmppY, m_signLayer, m_batchNode, m_zOrder);
    m_signNode->addChild(m_buildCCB);
    
    auto sprAni = CCSprite::create();
    
    if (CCCommonUtils::isIosAndroidPad())
    {
        std::string _path = CCFileUtils::sharedFileUtils()->getWritablePath() + "dresource/" + IMPERIAL_PATH_0_HD;
        if (CCFileUtils::sharedFileUtils()->isFileExist(_path))
        {
            CCCommonUtils::makeEffectSpr(sprAni, "inner_risk_%d_hd.png", 12);
        }
        else
        {
            CCCommonUtils::makeEffectSpr(sprAni, "inner_risk_%d.png", 18);
            sprAni->setScale(2.0);
        }
           
    }
    else
    {
        CCCommonUtils::makeEffectSpr(sprAni, "inner_risk_%d.png", 18);
        sprAni->setScale(2.0);
    }
    sprAni->setAnchorPoint(ccp(0.5, 0.5));
    
    int tmpY = 108;//_cbg->getContentSize().height;
    sprAni->setPosition(ccp(mainWidth/2, tmpY-40));
    
    auto garyCloud1 = CCSprite::create();
    CCCommonUtils::makeEffectSpr(garyCloud1, "gary_cloud_%d.png", 13);
    garyCloud1->setAnchorPoint(ccp(1, 0));
    garyCloud1->setPosition(ccp(mainWidth/2+30, -40));
    auto garyCloud2 = CCSprite::create();
    CCCommonUtils::makeEffectSpr(garyCloud2, "gary_cloud_%d.png", 13);
    garyCloud2->setFlipX(true);
    garyCloud2->setPosition(ccp(mainWidth/2-10, -40));
    
    m_signNode->addChild(sprAni);
    m_signNode->addChild(garyCloud1);
    m_signNode->addChild(garyCloud2);
    
    
    if(this->getAnimationManager()->getSequenceId("Update") >=0) {
        this->getAnimationManager()->runAnimationsForSequenceNamed("Update");
    }
    
    if ( !m_info->is_Dirc ) {
        addDragonBuildState();
    }
}

void DragonBuild::hideUping()
{
    if(m_buildCCB) {
        m_buildCCB->onBuildDelete();
        m_buildCCB->removeFromParent();
        m_buildCCB = NULL;
    }
    m_signNode->removeAllChildren();
}

void DragonBuild::changeBg()
{
    if(m_buildCCB) {
        m_buildCCB->onBuildDelete();
        m_buildCCB->removeFromParent();
        m_buildCCB = NULL;
    }
    m_signNode->removeAllChildren();
}

void DragonBuild::setTime(int times, bool updateLv)
{
    if(m_info && (m_info->state == FUN_BUILD_UPING || m_info->state == FUN_BUILD_DESTROY) && isShowUping==false)
    {
        isShowUping = true;
        showUping();
    }
    else if (isShowUping==true && times<=0)
    {
        isShowUping = false;
        hideUping();
        removeDragonBuildState();
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                               , CCString::createWithFormat("BU_%d_cd", m_buildingKey/1000));
    }
    
    if (times<=0 || updateLv) {
        onCheckOutPut();
        m_lvLabel->setString(CC_ITOA(m_info->level));
    }
    
    if(times > 0 && isShowUping) {
    }
}

void DragonBuild::onUpdateUpIcon(float dt)
{
    if(m_info==NULL) {
        return;
    }
    
    if (m_info->level >= GlobalData::shared()->MaxBuildLv) {
        m_lvLabel->setString("MAX");
        m_arrSpr->setVisible(false);
    }
    else{
        if (m_info->state == FUN_BUILD_NORMAL && m_info->isUnLock()) {
            bool canUp = true;
            int tmpTime = m_info->time_need/(1+CCCommonUtils::getEffectValueByNum(68)*1.0/100);
            int qid = QueueController::getInstance()->canMakeItemByType(TYPE_DRAGON_BUILD, tmpTime);
            if(QID_MAX == qid){
                canUp = false;
            }
            if (canUp) {
                if (GlobalData::shared()->resourceInfo.lMoney<m_info->silver_need
                    || GlobalData::shared()->resourceInfo.lWood<m_info->wood_need
                    || GlobalData::shared()->resourceInfo.lFood<m_info->food_need
                    || GlobalData::shared()->resourceInfo.lStone<m_info->stone_need
                    || GlobalData::shared()->resourceInfo.lIron<m_info->iron_need) {
                    canUp = false;
                }
            }
            if (canUp && m_info->tool_need!="") {
                std::vector<string> toolItems;
                CCCommonUtils::splitString(m_info->tool_need, "|", toolItems);
                for (int i=0; i<toolItems.size(); i++) {
                    string curTool = toolItems[i];
                    std::vector<string> item;
                    CCCommonUtils::splitString(curTool, ";", item);
                    int id = atoi(item[0].c_str());
                    int num = atoi(item[1].c_str());
                    auto& info = ToolController::getInstance()->getToolInfoById(id);
                    if(num > info.getCNT()){
                        canUp = false;
                    }
                }
            }
            m_arrSpr->setVisible(canUp);
        }
        else {
            m_arrSpr->setVisible(false);
        }
    }
}

void DragonBuild::onEnter() {
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DragonBuild::onRemoveBuildState), MSG_REMOVE_STATE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DragonBuild::onShowBuildState), MSG_SHOW_TIME_BAR, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DragonBuild::unLockTile), MSG_UNLOCK_TILE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DragonBuild::quickProduceTroop), MSG_QUICK_TROOPS, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DragonBuild::buildFunctionEffect), MSG_BUILD_FUNCTION_EFFECT, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DragonBuild::showHarvestAni), MSG_SHOW_SKILL_HARVEST, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DragonBuild::playFortSkillEffect), MSG_SHOW_FORT_SKILL_EFFECT, NULL);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DragonBuild::onMoveBuild), MSG_MOVE_BUILD, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DragonBuild::onCancelLastMoveBuild), MSG_CANCEL_LAST_MOVE_BUILD, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DragonBuild::onCancelMoveBuild), MSG_CANCEL_BUILD, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DragonBuild::playBuildShadow), MSG_PLAY_BUILD_SHADOW, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DragonBuild::stopBuildShadow), MSG_STOP_BUILD_SHADOW, NULL);
    
}

void DragonBuild::onExit() {
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_REMOVE_STATE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SHOW_TIME_BAR);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_UNLOCK_TILE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_QUICK_TROOPS);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_BUILD_FUNCTION_EFFECT);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SHOW_SKILL_HARVEST);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SHOW_FORT_SKILL_EFFECT);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_MOVE_BUILD);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_CANCEL_LAST_MOVE_BUILD);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_CANCEL_BUILD);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_PLAY_BUILD_SHADOW);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_STOP_BUILD_SHADOW);
    
    CCNode::onExit();
}

bool DragonBuild::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    bool ret = false;
    if (m_buildingKey<1000) {
        if (m_tile->isVisible()) {
            ret = isTouchInside(m_tile, pTouch);
        }
    }
    else {
        if ( m_info && m_info->is_Dirc ) {
            return ret;
        }
        ret = isTouchInside(m_touchNode, pTouch);
    }
    
    return ret;
}

CCPoint DragonBuild::IsNeedMove(float x, float y, float scale)
{
    int retX = 0;
    int retY = 0;
    float bottomY = 150;
    float topY = 150;
    auto size = CCDirector::sharedDirector()->getWinSize();
    float minX = nameWidth*scale/2;
    float maxX = size.width-minX;
    float minY = nameHeight*scale+bottomY;
    float maxY = size.height-topY;
    
    if (x<minX) {
        retX = minX;
    }
    if (x>maxX) {
        retX = maxX;
    }
    if (x>maxX && x<minX) {
        retX = size.width/2;
    }
    
    if (y<minY) {
        retY = minY;
    }
    if (y>maxY) {
        retY = maxY;
    }
    if (y>maxY && y<minY) {
        retY = size.height/2;
    }
    
    return ccp(retX, retY);
}

void DragonBuild::setGary()
{
    if(m_spr) {
        m_spr->setColor(ccGRAY);
    }
    else if (m_tile) {
        m_tile->setColor(ccGRAY);
    }
    
    if (m_buildingKey == FUN_BUILD_WALL_ID) {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SET_GARY);
    }
    
    return;
    for (int i=0; i<m_sprArray->count(); i++) {
        CCSprite* spr = dynamic_cast<CCSprite*>(m_sprArray->objectAtIndex(i));
        if (spr && spr->isVisible()) {
            spr->setColor(ccGRAY);
        }
    }
}

void DragonBuild::setWhite()
{
    if(m_spr) {
        m_spr->setColor(ccWHITE);
    }
    else if (m_tile) {
        m_tile->setColor(ccWHITE);
    }
    
    if (m_buildingKey == FUN_BUILD_WALL_ID) {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SET_WHITE);
    }
    
    return;
    for (int i=0; i<m_sprArray->count(); i++) {
        CCSprite* spr = dynamic_cast<CCSprite*>(m_sprArray->objectAtIndex(i));
        if (spr && spr->isVisible()) {
            spr->setColor(ccWHITE);
        }
    }
}

void DragonBuild::playBuildShadow(CCObject* obj){
    if (obj) {
        int key = (dynamic_cast<CCString*>(obj))->intValue();
        if (key == m_buildingKey) {
            CCTintTo* tinto1 = CCTintTo::create(0.5, 107, 205, 26);
            CCTintTo* tinto2 = CCTintTo::create(0.5, 255, 255, 255);
            CCSequence* seq = CCSequence::create(tinto1,tinto2,NULL);
            if(m_spr) {
                m_spr->runAction(CCRepeatForever::create(seq));
            }
        }
    }
}
void DragonBuild::stopBuildShadow(CCObject* obj){
    if (obj) {
        int key = (dynamic_cast<CCString*>(obj))->intValue();
        if (key == m_buildingKey) {
            stopShadow();
        }
    }
}

void DragonBuild::playShadow()
{
    CCTintTo* tinto1 = CCTintTo::create(0.5, 166, 166, 166);
    CCTintTo* tinto2 = CCTintTo::create(0.5, 255, 255, 255);
    
    CCSequence* seq = CCSequence::create(tinto1,tinto2,NULL);
    if(m_spr) {
        m_spr->runAction(CCRepeatForever::create(seq));
    }
    
    if (m_buildingKey == FUN_BUILD_WALL_ID) {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_PLAY_SHADOW);
    }
}

void DragonBuild::stopShadow()
{
    if(m_spr) {
        m_spr->stopAllActions();
        m_spr->setColor(ccWHITE);
    }
    
    if (m_buildingKey == FUN_BUILD_WALL_ID) {
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_STOP_SHADOW);
    }
}

void DragonBuild::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (!isCanClick) {
        return;
    }
    m_clickType = 0;
    
    if (DragonBuildingController::getInstance()->OpenMoveBuildStat) {
        int pos = getCurPos();
        if (pos>0) {
            int cnt = DragonBuildingController::getInstance()->WillMovePos.size();
            if (cnt == 0) {
                if (m_buildingKey>1000) {
                    DragonBuildingController::getInstance()->SetMoveBuildPos(pos);
                }
            }else if (cnt == 1){
                int firstPos = DragonBuildingController::getInstance()->WillMovePos[0];
                if (firstPos>16 && firstPos<52 && pos>16 && pos<52 && firstPos!=pos) {
                    DragonBuildingController::getInstance()->SetMoveBuildPos(pos);
                }else if ( ((firstPos<=16 && firstPos>=6) || (firstPos>=52 && firstPos<=53)) && firstPos!=pos) {
                    if ((pos<=16 && pos>=6) || (pos>=52 && pos<=53)) {
                        DragonBuildingController::getInstance()->SetMoveBuildPos(pos);
                    }
                }
            }
        }
    }else {
        isCanClick=false;
        setGary();
        scheduleOnce(schedule_selector(DragonBuild::onClickThis), 0.1f);
    }
}

bool DragonBuild::isTouchInBtns(CCTouch *pTouch, CCEvent *pEvent)
{
    return false;
}

void DragonBuild::onSetClickType()
{
    m_clickType = 1;
}

void DragonBuild::onClickThis(float _time)
{
    setWhite();
    auto layer = dynamic_cast<DragonScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
    if (layer) {
        layer->hideFlyArrow(0);
    }
    
    if(m_buildingKey<1000)
    {
        auto& tileInfo = DragonBuildingController::getInstance()->m_tileMap[m_buildingKey];
        if (tileInfo.state == FUN_BUILD_ULOCK) {
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MAINSCENCE_SAVEPOS);
            if(layer) {
                layer->setUnMoveScence(false);
                layer->onMoveToBuildAndOpen(m_buildingKey, TYPE_POS_UP);
                SoundController::sharedSound()->playEffects(Music_Sfx_city_space);
                CCLOG("zao sha");
//                PopupViewController::getInstance()->addPopupInView(BuildListView::create(m_buildingKey));
                PopupViewController::getInstance()->addPopupInView(DragonBuildListView::create(m_buildingKey));
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                                       , CCString::createWithFormat("MC_%d", m_buildingKey));
            }
        }
        
    }
    else
    {
        if(m_lockIcon && m_lockIcon->isVisible() && !isUnLock)
        {
            string tmpInfo = _lang_2("102184", _lang(m_info->name).c_str(), CC_ITOA(m_info->open));
            CCCommonUtils::flyHint("", "", tmpInfo);
            scheduleOnce(schedule_selector(DragonBuild::onCanClick), 0.2f);
            return;
        }
        if (isUnLock) {
            playOpenLock();
            scheduleOnce(schedule_selector(DragonBuild::onCanClick), 0.2f);
            return;
        }
        
        if (true) {
            if (m_buildState && m_buildState->isCanClick()) {
                if(m_clickType==0 && layer) {
                    layer->onResetLastBuildId();
                    m_buildState->ClickState();
                }
            }
            else {
                
                if(layer) {
                    auto pt = layer->onGetSceenPt(parentX+mainWidth/2, parentY);
                    auto movePt = IsNeedMove(pt.x, pt.y, layer->onGetTouchLayerScale());
                    if(movePt.x>0 || movePt.y>0) {
                        int ex = pt.x;
                        int ey = pt.y;
                        if (movePt.x>0) {
                            ex = movePt.x;
                        }
                        if (movePt.y>0) {
                            ey = movePt.y;
                        }
                        layer->onMoveToPosDir(parentX+mainWidth/2, parentY, ex, ey);
                    }
                }
                
                int dh = 0;
                if (m_buildingKey == DBUILDING_HATCHER_ItemId) {
                    dh = 100;
                }
                if (!m_info->is_Dirc && layer) {
                    layer->onShowBtnsView(parentX+mainWidth/2, parentY+dh, m_buildingKey);
                    playShadow();
                }
            }
            if (m_clickType==0 && layer) {
                layer->setUnMoveScence(false);
            }
            m_clickType = 0;
        }
        
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                               , CCString::createWithFormat("MC_%d", m_buildingKey/1000));
    }
    scheduleOnce(schedule_selector(DragonBuild::onCanClick), 0.2f);
}

void DragonBuild::onCanClick(float _time)
{
    isCanClick = true;
}

void DragonBuild::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
}

void DragonBuild::onPlayUpEnd(bool isOnly)
{
    this->unschedule(schedule_selector(DragonBuild::onResetDirc));
    m_upEffectNode->stopAllActions();
    m_upEffectNode->removeAllChildren();
    if (true) {
        string tmpStart = "CollectionOut_";
        int count = 9;
//        if (m_info->type==FUN_BUILD_WOOD || m_info->type==FUN_BUILD_FOOD || m_info->type==FUN_BUILD_IRON || m_info->type==FUN_BUILD_STONE || m_info->type==FUN_BUILD_BARRACK || m_info->type==FUN_BUILD_HOSPITAL ) {
//            tmpStart = "CrownDown_";
//            count = 8;
//        }
        if (1) {
            
            auto tmp_ptArray = CCPointArray::create(20);
            tmp_ptArray->addControlPoint(ccp(-140, 130));
            tmp_ptArray->addControlPoint(ccp(0, 300));
            tmp_ptArray->addControlPoint(ccp(100, 150));
            
            for (int j=0; j<tmp_ptArray->count(); j++) {
                for (int i=1; i<5; i++) {
                    auto particle = ParticleController::createParticle(CCString::createWithFormat("Fireworks_%d",i)->getCString());
                    particle->setPosition(tmp_ptArray->getControlPointAtIndex(j));
                    m_upEffectNode->addChild(particle);
                }
            }
        }
        
        for (int i=1; i<count; i++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
            particle->setPosition(ccp(0, 0));
            m_upEffectNode->addChild(particle);
        }
        
        if(!isOnly) {
            auto& queue = QueueController::getInstance()->m_buildQueueInfo;
            auto it = queue.begin();
            for (; it != queue.end(); ++it)
            {
                if (it->second.type != TYPE_DRAGON_BUILD) {
                    continue;
                }
                if (it->second.key == "") {
                    continue;
                }
                if (atoi(it->second.key.c_str()) != m_info->itemId) {
                    continue;
                }
                if ((int)(it->second.finishTime) > GlobalData::shared()->getWorldTime()) {
                    continue;
                }
                break;
            }
            if (it != queue.end()) {
                scheduleOnce(schedule_selector(DragonBuild::onStopUpEffect), 0.6f);
                scheduleOnce(schedule_selector(DragonBuild::playCompleteSound), 0.6f);
                it->second.startTime = 0;
                it->second.finishTime = 0;
                it->second.endTime = 0;
                it->second.key = "";
            } else {
                scheduleOnce(schedule_selector(DragonBuild::onStopUpEffect), 2.6f);
                scheduleOnce(schedule_selector(DragonBuild::playCompleteSound), 2.6f);
                string para1 = _lang(m_info->name);
                string para2 = string(" Lv") + CC_ITOA(m_info->level) + string(" ");
                CCCommonUtils::flyHint("", "", _lang_2("137480", para1.c_str(), para2.c_str()), 2, 0, true);
                SoundController::sharedSound()->playEffects(Music_Sfx_UI_buildfinish);
            }
        } else {
            scheduleOnce(schedule_selector(DragonBuild::playCompleteSound), 0.6f);
        }
        
    }
    
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::createWithFormat("MC_%d_2", m_info->itemId));
}

void DragonBuild::playCompleteSound(float _time)
{
    SoundController::sharedSound()->playEffects(Music_Sfx_city_complete);
}

void DragonBuild::onStopUpEffect(float _time)
{
    if(!GlobalData::shared()->isInitFlag){
        return;
    }
    m_upEffectNode->removeAllChildren();
    long addPower = m_info->mapPower[m_info->level];
    UIComponent::getInstance()->showFlyCBView(m_info->oldExp, addPower);
    scheduleOnce(schedule_selector(DragonBuild::onPlayBall), 2.1f);
}

void DragonBuild::onPlayBall(float _time)
{
    auto layer = dynamic_cast<DragonScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
    if(layer) {
        layer->flyBall(m_info->itemId, 0, false);
    }
}

void DragonBuild::canShowState()
{
    auto now = WorldController::getInstance()->getTime();
    if (m_parVec.size()==0 && now < GlobalData::shared()->cityFireStamp) {
        onShowParticle();
    }
    else if (m_parVec.size()>0 && now >= GlobalData::shared()->cityFireStamp) {
        onHideParticle();
    }
//    if (m_info->type == FUN_BUILD_MAIN) {
//        
//    }
//    else if (m_info->type == FUN_BUILD_SCIENE) {
//        if (!isEffectRunning && QueueController::getInstance()->getQueueNumByType(TYPE_SCIENCE)>0) {
//            addDragonBuildState();
//            this->getAnimationManager()->runAnimationsForSequenceNamed("FadeIn");
//            isEffectRunning = true;
//        }
//        else if (isEffectRunning && QueueController::getInstance()->getQueueNumByType(TYPE_SCIENCE)<=0 ) {
//            removeDragonBuildState();
//            this->getAnimationManager()->runAnimationsForSequenceNamed("FadeOut");
//            isEffectRunning = false;
//        }
//    }
//    else if (m_info->type == FUN_BUILD_FORGE) {
//        if (!isEffectRunning && QueueController::getInstance()->getQueueNumByType(TYPE_FORGE)>0) {
//            addDragonBuildState();
//            this->getAnimationManager()->runAnimationsForSequenceNamed("FadeIn");
//            drowEffectSpr(NULL,Control::EventType::TOUCH_DOWN);
//            isEffectRunning = true;
//        }
//        else if (isEffectRunning && QueueController::getInstance()->getQueueNumByType(TYPE_FORGE)<=0 ) {
//            isEffectRunning = false;
//            
//            int qid = QueueController::getInstance()->getCanRecQidByType(TYPE_FORGE);
//            if (qid != QID_MAX) {
//                if(m_buildState) {
//                    m_buildState->onShowRecState();
//                }
//                this->getAnimationManager()->runAnimationsForSequenceNamed("FadeOut");
//                onShowParticleByType(1);
//            }else {
//                removeDragonBuildState();
//                onHideParticle();
//                this->getAnimationManager()->runAnimationsForSequenceNamed("FadeOut");
//            }
//        }
//    }
//    else if (m_info->type == FUN_BUILD_WORKSHOP) {
//        if (!isEffectRunning && QueueController::getInstance()->getQueueNumByType(TYPE_MATE)>0) {
//            addDragonBuildState();
//            this->getAnimationManager()->runAnimationsForSequenceNamed("FadeIn");
//            onShowParticleByType(7);
//            
//            if (CCFileUtils::sharedFileUtils()->isFileExist("Spine/Imperial/lianjingongfang.json") &&
//                CCFileUtils::sharedFileUtils()->isFileExist("Imperial/Imperial_30.atlas"))
//            {
//                auto animationObj = new IFSkeletonAnimation("Spine/Imperial/lianjingongfang.json","Imperial/Imperial_30.atlas");
//                if (animationObj) {
//                    animationObj->setVisibleStop(false);
//                    animationObj->setPosition(ccp(100, -20));
//                    m_signNode->addChild(animationObj);
//                    spTrackEntry* entry = animationObj->setAnimation(0, "gongzuo", true);
//                    animationObj->timeScale = entry->endTime/4.0f;
//                }
//            }
//            
//            int qid = QueueController::getInstance()->getMinTimeQidByType(TYPE_MATE);
//            if(qid != QID_MAX) {
//                auto& qInfo = GlobalData::shared()->allQueuesInfo[qid];
//                int sid = QueueController::getInstance()->getItemId(qInfo.key);
//                auto icon = CCLoadSprite::createSprite( CCCommonUtils::getIcon(CC_ITOA(sid)).c_str() );
//                icon->setPosition(ccp(100, 70));
//                m_signNode->addChild(icon);
//                CCActionInterval * moveBy1 = CCMoveBy::create(0.5,ccp(0, 20));
//                CCActionInterval * moveBy2 = CCMoveBy::create(0.5,ccp(0, -20));
//                CCActionInterval * repeat = CCRepeatForever::create( CCSequence::create(moveBy1, moveBy2, NULL) );
//                icon->runAction(repeat);
//            }
//            m_signNode->setZOrder(999999);
//            isEffectRunning = true;
//        }
//        else if (isEffectRunning && QueueController::getInstance()->getQueueNumByType(TYPE_MATE)<=0 ) {
//            isEffectRunning = false;
//            m_signNode->removeAllChildren();
//            onHideSpeParticle();
//            int qid = QueueController::getInstance()->getCanRecQidByType(TYPE_MATE);
//            if (qid != QID_MAX) {
//                if(m_buildState) {
//                    m_buildState->onShowRecState();
//                }
//                this->getAnimationManager()->runAnimationsForSequenceNamed("FadeOut");
//                onShowParticleByType(1);
//            }else {
//                removeDragonBuildState();
//                onHideSpeParticle();
//                this->getAnimationManager()->runAnimationsForSequenceNamed("FadeOut");
//            }
//        }
//    }
//    else if (m_info->type == FUN_BUILD_HOSPITAL) {
//        if (!isEffectRunning && QueueController::getInstance()->getQueueNumByType(TYPE_HOSPITAL)>0) {
//            addDragonBuildState();
//            isEffectRunning = true;
//        }
//        else if (isEffectRunning && QueueController::getInstance()->getQueueNumByType(TYPE_HOSPITAL)<=0 ) {
//            removeDragonBuildState();
//            isEffectRunning = false;
//        }
//        
//        if (m_buildAnimatSt!="Help" && ArmyController::getInstance()->getTotalDead()>0) {
//            this->getAnimationManager()->runAnimationsForSequenceNamed("Help");
//            m_buildAnimatSt="Help";
//            onShowParticleByType(4);
//        }
//        else if (m_buildAnimatSt!="Default" && ArmyController::getInstance()->getTotalDead()<=0) {
//            this->getAnimationManager()->runAnimationsForSequenceNamed("Default");
//            m_buildAnimatSt="Default";
//            onHideParticle();
//        }
//    }
//    else if (m_info->type == FUN_BUILD_BARRACK1) {
//        if (!isEffectRunning && QueueController::getInstance()->getQueueNumByType(TYPE_FORCE)>0) {
//            isEffectRunning = true;
//            addDragonBuildState();
//            this->getAnimationManager()->runAnimationsForSequenceNamed("FadeIn");
//        }
//        else if (isEffectRunning && QueueController::getInstance()->getQueueNumByType(TYPE_FORCE)<=0 ) {
//            isEffectRunning = false;
//            int qid = QueueController::getInstance()->getCanRecQidByType(TYPE_FORCE);
//            if (qid != QID_MAX) {
//                if(m_buildState) {
//                    m_buildState->onShowRecState();
//                }
//                this->getAnimationManager()->runAnimationsForSequenceNamed("FadeOut");
//                onShowParticleByType(1);
//            }else {
//                removeDragonBuildState();
//                this->getAnimationManager()->runAnimationsForSequenceNamed("TrainingFadeOut");
//            }
//        }
//    }
//    else if (m_info->type == FUN_BUILD_BARRACK2) {
//        if (!isEffectRunning && QueueController::getInstance()->getQueueNumByType(TYPE_RIDE_SOLDIER)>0) {
//            isEffectRunning = true;
//            addDragonBuildState();
//            this->getAnimationManager()->runAnimationsForSequenceNamed("FadeIn");
//        }
//        else if (isEffectRunning && QueueController::getInstance()->getQueueNumByType(TYPE_RIDE_SOLDIER)<=0 ) {
//            isEffectRunning = false;
//            int qid = QueueController::getInstance()->getCanRecQidByType(TYPE_RIDE_SOLDIER);
//            if (qid != QID_MAX) {
//                if(m_buildState) {
//                    m_buildState->onShowRecState();
//                }
//                this->getAnimationManager()->runAnimationsForSequenceNamed("FadeOut");
//                onShowParticleByType(1);
//            }else {
//                removeDragonBuildState();
//                this->getAnimationManager()->runAnimationsForSequenceNamed("TrainingFadeOut");
//            }
//        }
//    }
//    else if (m_info->type == FUN_BUILD_BARRACK3) {
//        if (!isEffectRunning && QueueController::getInstance()->getQueueNumByType(TYPE_BOW_SOLDIER)>0) {
//            isEffectRunning = true;
//            addDragonBuildState();
//            this->getAnimationManager()->runAnimationsForSequenceNamed("FadeIn");
//        }
//        else if (isEffectRunning && QueueController::getInstance()->getQueueNumByType(TYPE_BOW_SOLDIER)<=0 ) {
//            isEffectRunning = false;
//            int qid = QueueController::getInstance()->getCanRecQidByType(TYPE_BOW_SOLDIER);
//            if (qid != QID_MAX) {
//                if(m_buildState) {
//                    m_buildState->onShowRecState();
//                }
//                this->getAnimationManager()->runAnimationsForSequenceNamed("FadeOut");
//                onShowParticleByType(1);
//            }else {
//                removeDragonBuildState();
//                this->getAnimationManager()->runAnimationsForSequenceNamed("TrainingFadeOut");
//            }
//        }
//    }
//    else if (m_info->type == FUN_BUILD_BARRACK4) {
//        if (!isEffectRunning && QueueController::getInstance()->getQueueNumByType(TYPE_CAR_SOLDIER)>0) {
//            isEffectRunning = true;
//            addDragonBuildState();
//            this->getAnimationManager()->runAnimationsForSequenceNamed("FadeIn");
//        }
//        else if (isEffectRunning && QueueController::getInstance()->getQueueNumByType(TYPE_CAR_SOLDIER)<=0 ) {
//            isEffectRunning = false;
//            int qid = QueueController::getInstance()->getCanRecQidByType(TYPE_CAR_SOLDIER);
//            if (qid != QID_MAX) {
//                if(m_buildState) {
//                    m_buildState->onShowRecState();
//                }
//                this->getAnimationManager()->runAnimationsForSequenceNamed("FadeOut");
//                onShowParticleByType(1);
//            }else {
//                removeDragonBuildState();
//                this->getAnimationManager()->runAnimationsForSequenceNamed("TrainingFadeOut");
//            }
//        }
//    }
//    else if (m_info->type == FUN_BUILD_FORT) {
//        if (!isEffectRunning && QueueController::getInstance()->getQueueNumByType(TYPE_FORT)>0) {
//            isEffectRunning = true;
//            addDragonBuildState();
//            this->getAnimationManager()->runAnimationsForSequenceNamed("FadeIn");
//        }
//        else if (isEffectRunning && QueueController::getInstance()->getQueueNumByType(TYPE_FORT)<=0 ) {
//            isEffectRunning = false;
//            int qid = QueueController::getInstance()->getCanRecQidByType(TYPE_FORT);
//            if (qid != QID_MAX) {
//                if(m_buildState) {
//                    m_buildState->onShowRecState();
//                }
//                this->getAnimationManager()->runAnimationsForSequenceNamed("FadeOut");
//                onShowParticleByType(1);
//            }else {
//                removeDragonBuildState();
//                this->getAnimationManager()->runAnimationsForSequenceNamed("TrainingFadeOut");
//            }
//        }
//    }
//    else if (m_info->type == FUN_BUILD_STABLE) {
//        if (!isEffectRunning && GlobalData::shared()->playerInfo.allianceInfo.helpcount>0) {//有帮助需求
//            isEffectRunning = true;
//            addDragonBuildState();
//            m_buildState->onCheckAllianceHelp();
//        }
//        else if (isEffectRunning && GlobalData::shared()->playerInfo.allianceInfo.helpcount<=0 ) {//没有有帮助需求
//            if(m_buildState) {
//                m_buildState->onHideBtn(4);
//            }
//            if (m_info->state == FUN_BUILD_NORMAL) {
//                removeDragonBuildState();
//            }
//            isEffectRunning = false;
//        }
//    }
//    else if (m_info->type == FUN_BUILD_WOOD || m_info->type == FUN_BUILD_FOOD || m_info->type == FUN_BUILD_IRON || m_info->type == FUN_BUILD_STONE) {
//        if (!isEffectRunning && DragonBuildingController::getInstance()->canShowOutPut(m_info->itemId)) {
//            addDragonBuildState();
//            isEffectRunning = true;
//            if(m_info->type == FUN_BUILD_FOOD || m_info->type == FUN_BUILD_WOOD) {
//                this->getAnimationManager()->runAnimationsForSequenceNamed("GrowthProcess");
//            }
//        }
//        else if (isEffectRunning && !DragonBuildingController::getInstance()->canShowOutPut(m_info->itemId) && m_buildState->CanDel ) {
//            removeDragonBuildState();
//            isEffectRunning = false;
//            if(m_info->type == FUN_BUILD_FOOD || m_info->type == FUN_BUILD_WOOD) {
//                this->getAnimationManager()->runAnimationsForSequenceNamed("Havest");
//            }
//        }
//    }
//    else if (m_info->type == FUN_BUILD_SACRIFICE) {
//        
//        if (!isEffectRunning && GlobalData::shared()->sacrificeInfo.canFreePray()==true) {
//            addTips();
//            isEffectRunning = true;
//            onShowParticleByType(6);
//        }
//        else if (isEffectRunning && GlobalData::shared()->sacrificeInfo.canFreePray()==false) {
//            removeTips();
//            isEffectRunning = false;
//            onHideSpeParticle();
//        }
//    }
}

void DragonBuild::retTouch(CCTouch *pTouch, CCEvent *pEvent)
{
    if(pTouch==NULL)
        return;
    bool ret = false;
    if (m_buildingKey<1000) {
        ret = isTouchInside(m_tile, pTouch);
    }
    else {
        ret = isTouchInside(m_touchNode, pTouch);
    }
    
    if (ret) {
        this->onTouchEnded(pTouch, pEvent);
    }
}

void DragonBuild::onPlayUpIcon()
{
    if (!m_arrSpr->isVisible()) {
        return;
    }
    m_arrSpr->setPositionY(arrY);
    CCMoveBy* moveUp= CCMoveBy::create(0.1f, ccp(0,10));
    CCMoveBy* moveDown= CCMoveBy::create(0.1f, ccp(0,-10));
    CCMoveBy* moveUp2= CCMoveBy::create(0.1f, ccp(0,5));
    CCMoveBy* moveDown2= CCMoveBy::create(0.1f, ccp(0,-5));
    m_arrSpr->runAction(CCRepeatForever::create(CCSequence::create(CCDelayTime::create(1),moveUp,moveDown,moveUp2,moveDown2,NULL)));
}
void DragonBuild::stopUpIcon()
{
    m_arrSpr->stopAllActions();
    m_arrSpr->setPositionY(arrY);
}

void DragonBuild::drowEffectSpr(int zOrder, int tmpOrd)
{
    int x = parentX;
    int y = parentY;
    onShowParticle();
    
}

void DragonBuild::drowPersonSpr(int zOrder, int tmpOrd)
{
    
}

void DragonBuild::onMoveBuild(CCObject* param)
{
    if (m_buildingKey<1000) {
        if (!m_tile->isVisible()) {
            return;
        }
    }
    
    int pos = getCurPos();
    if (pos < 0) {
        return;
    }
    
    CCString* posInfo = dynamic_cast<CCString*>(param);
    int paramPos = posInfo->intValue();
    int cnt = DragonBuildingController::getInstance()->WillMovePos.size();
    if (cnt == 0) {
        //所有建筑都播放，地格不播放，只能点建筑
        if (m_buildingKey>1000) {
            float scale1 = 1.5;
            float scale2 = 1.3;
            if (pos>16 && pos<52) {
                scale1 = 1.15;
                scale2 = 1.0;
            }
            CCActionInterval * scaleto1 = CCScaleTo::create(0.5, scale1);
            CCActionInterval * scaleto2 = CCScaleTo::create(0.5, scale2);
            CCActionInterval * repeatForever =CCRepeatForever::create(CCSequence::create(scaleto1, scaleto2, NULL));
            m_moveFrame->runAction(repeatForever);
            
            m_moveFrame->setColor(ccGREEN);
            m_moveFrame->setVisible(true);
        }
    }else if (cnt == 1) {
        //paramPos 变颜色, 并取消掉 不可交换的建筑效果; 播放可以移动到地格
        if (pos == paramPos) {
            m_moveFrame->setColor(ccRED);
        }else {
            if (paramPos>16 && paramPos<52 && (pos<=16 || pos>=52)) {
                onCancelMoveBuild(NULL);
            }else if (pos>16 && pos<52 && (paramPos<=16 || paramPos>=52)) {
                onCancelMoveBuild(NULL);
            }else {
                if (m_buildingKey<1000) {
                    float scale1 = 1.5;
                    float scale2 = 1.3;
                    if (pos>16 && pos<52) {
                        scale1 = 1.15;
                        scale2 = 1.0;
                    }
                    CCActionInterval * scaleto1 = CCScaleTo::create(0.5, scale1);
                    CCActionInterval * scaleto2 = CCScaleTo::create(0.5, scale2);
                    CCActionInterval * repeatForever =CCRepeatForever::create(CCSequence::create(scaleto1, scaleto2, NULL));
                    m_moveFrame->runAction(repeatForever);
                    
                    m_moveFrame->setColor(ccGREEN);
                    m_moveFrame->setVisible(true);
                }
            }
        }
        
    }else if (cnt == 2) {
        //paramPos 变颜色
        if (pos == paramPos) {
            m_moveFrame->setColor(ccRED);
        }
    }
}

void DragonBuild::onCancelLastMoveBuild(CCObject* param)
{
    if (m_buildingKey<1000) {
        if (!m_tile->isVisible()) {
            return;
        }
    }
    
    int pos = getCurPos();
    if (pos < 0) {
        return;
    }
    
    CCString* posInfo = dynamic_cast<CCString*>(param);
    int paramPos = posInfo->intValue();
    if (pos == paramPos) {
        m_moveFrame->setColor(ccGREEN);
    }
}

void DragonBuild::onCancelMoveBuild(CCObject* param)
{
    if (m_buildingKey<1000) {
        if (!m_tile->isVisible()) {
            return;
        }
    }
    
    int pos = getCurPos();
    if (pos < 0) {
        return;
    }
    
    m_moveFrame->stopAllActions();
    m_moveFrame->setVisible(false);
}

int DragonBuild::getCurPos()
{
    if (m_buildingKey>1000) {
        if (m_info) {
            if (m_info->pos>16 && m_info->pos<52) {//外城建筑
                return m_info->pos;
            }else if (m_info->pos<6) {
                return -1;
            }else {
                return m_info->pos;
            }
        }else {
            return -1;
        }
    }else {
        if (m_buildingKey>=6 && m_buildingKey<=53) {
            return m_buildingKey;
        }
        else {
            return -1;
        }
    }
}

#pragma mark -
#pragma mark CocosBuilder Part
SEL_MenuHandler DragonBuild::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler DragonBuild::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

bool DragonBuild::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, this->m_touchNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lvNode", CCNode*, this->m_lvNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_arrNode", CCNode*, this->m_arrNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_spr", CCSprite*, this->m_spr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_arrSpr", CCSprite*, this->m_arrSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lvBG", CCSprite*, this->m_lvBG);
    return false;
}

void DragonBuild::addSpeParticleToBatch(cocos2d::CCParticleSystemQuad *particle)
{
    auto batchCount = m_speVec.size();
    while (batchCount--) {
        auto &batch = m_speVec[batchCount];
        if (batch) {
            auto batchBlend = batch->getBlendFunc();
            auto particleBlend = particle->getBlendFunc();
            if (batchBlend.src == particleBlend.src && batchBlend.dst == particleBlend.dst) {
                batch->addChild(particle);
                return;
            }
        }
    }
    auto newBatch = ParticleController::createParticleBatch();
    m_signLayer->addChild(newBatch);
    newBatch->addChild(particle);
    m_speVec.push_back(newBatch);
}
void DragonBuild::onHideSpeParticle()
{
    auto batchCount = m_speVec.size();
    while (batchCount--) {
        m_signLayer->removeChild(m_speVec[batchCount]);
    }
    m_speVec.clear();
}

void DragonBuild::onHideForgeFirePar()
{
    if (m_upEffectNode) {
        auto batchCount = m_forgeFireParVec.size();
        while (batchCount--) {
            m_upEffectNode->removeChild(m_forgeFireParVec[batchCount]);
        }
        m_forgeFireParVec.clear();
    }
}

void DragonBuild::addForgeFireParToBatch(cocos2d::CCParticleSystemQuad *particle)
{
    auto batchCount = m_forgeFireParVec.size();
    while (batchCount--) {
        auto &batch = m_forgeFireParVec[batchCount];
        if (batch) {
            auto batchBlend = batch->getBlendFunc();
            auto particleBlend = particle->getBlendFunc();
            if (batchBlend.src == particleBlend.src && batchBlend.dst == particleBlend.dst) {
                batch->addChild(particle);
                return;
            }
        }
    }
    auto newBatch = ParticleController::createParticleBatch();
    m_upEffectNode->addChild(newBatch);
    newBatch->addChild(particle);
    particle->setAutoRemoveOnFinish(true);
    particle->setDuration(0.5f);
    m_forgeFireParVec.push_back(newBatch);
}

void DragonBuild::addParticleToBatch(cocos2d::CCParticleSystemQuad *particle)
{
    auto batchCount = m_parVec.size();
    while (batchCount--) {
        auto &batch = m_parVec[batchCount];
        if (batch) {
            auto batchBlend = batch->getBlendFunc();
            auto particleBlend = particle->getBlendFunc();
            if (batchBlend.src == particleBlend.src && batchBlend.dst == particleBlend.dst) {
                batch->addChild(particle);
                return;
            }
        }
    }
    auto newBatch = ParticleController::createParticleBatch();
    m_signLayer->addChild(newBatch);
    newBatch->addChild(particle);
    m_parVec.push_back(newBatch);
}

void DragonBuild::onShowParticle()
{
//    if (m_info->type == FUN_BUILD_MAIN) {
//        auto tmp_ptArray = CCPointArray::create(20);
//        tmp_ptArray->addControlPoint(ccp(parentX+270, parentY+210));
//        tmp_ptArray->addControlPoint(ccp(parentX+20, parentY+180));
//        tmp_ptArray->addControlPoint(ccp(parentX+180, parentY+200));
//        tmp_ptArray->addControlPoint(ccp(parentX+120, parentY+80));
//        tmp_ptArray->addControlPoint(ccp(parentX+360, parentY+200));
//        tmp_ptArray->addControlPoint(ccp(parentX+220, parentY+300));
//        string tmpStart = "CityFire_";
//        int count = 5;
//        for (int j=0; j<tmp_ptArray->count(); j++) {
//            for (int i=1; i<count; i++) {
//                auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
//                particle->setPosition(tmp_ptArray->getControlPointAtIndex(j));
//                addParticleToBatch(particle);
//            }
//        }
//        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_SHOW_CITY_FIRE);
//    }
//    else if (m_info->type == FUN_BUILD_SCIENE) {
//        string tmpStart = "Research_";
//        int count = 3;
//        for (int i=1; i<count; i++) {
//            auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
//            particle->setPosition(ccp(parentX+145, parentY+210));
//            addParticleToBatch(particle);
//        }
//    }
//    else if (m_info->type == FUN_BUILD_TEMPLE) {
//        string tmpStart = "Temple_";
//        int count = 9;
//        for (int i=1; i<count; i++) {
//            auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
//            particle->setPosition(ccp(parentX+100, parentY+130));
//            addParticleToBatch(particle);
//        }
//        
//        auto particle1 = ParticleController::createParticle("TempleStar");
//        particle1->setPosition(ccp(parentX+50, parentY+130));
//        addParticleToBatch(particle1);
//        auto particle2 = ParticleController::createParticle("TempleStar");
//        particle2->setPosition(ccp(parentX+150, parentY+130));
//        addParticleToBatch(particle2);
//    }
//    else if (m_info->type == FUN_BUILD_FORGE) {
//        auto tmp_ptArray = CCPointArray::create(20);
//        tmp_ptArray->addControlPoint(ccp(parentX+215, parentY+135));
//        tmp_ptArray->addControlPoint(ccp(parentX+60, parentY+170));
//        tmp_ptArray->addControlPoint(ccp(parentX+115, parentY+190));
//        int count = 2;
//        for (int j=0; j<tmp_ptArray->count(); j++) {
//            for (int i=1; i<=count; i++) {
//                auto particle = ParticleController::createParticle(CCString::createWithFormat("Smoke_%d",i)->getCString());
//                particle->setPosition(tmp_ptArray->getControlPointAtIndex(j));
//                addParticleToBatch(particle);
//            }
//        }
//        
//        // 每隔6秒喷一次的火焰
//        showForgeFire(0.0f);
//        schedule(schedule_selector(DragonBuild::showForgeFire),6.0f);
//        
//    }
//    else if (m_info->type == FUN_BUILD_WATCH_TOWER) {
//    }
//    else if (m_info->type == FUN_BUILD_WORKSHOP) {
//        int count = 3;
//        for (int i=1; i<=count; i++) {
//            auto particle = ParticleController::createParticle(CCString::createWithFormat("ArmySpray_%d",i)->getCString());
//            particle->setPosition(ccp(parentX+190, parentY+35));
//            addParticleToBatch(particle);
//        }
//    }
}

void DragonBuild::showForgeFire(float dt)
{
    onHideForgeFirePar();
    for(int i=1; i<= 4; i++)
    {
        auto particle = ParticleController::createParticle(CCString::createWithFormat("SmithyFire_%d",i)->getCString());
        particle->setPosition(ccp(0,-20));
        addForgeFireParToBatch(particle);
    }
}

void DragonBuild::onHideParticle()
{
    auto batchCount = m_parVec.size();
    while (batchCount--) {
        m_signLayer->removeChild(m_parVec[batchCount]);
    }
    m_parVec.clear();
}

void DragonBuild::onShowParticleByType(int type)
{
    if (type == 1) {
        string tmpStart = "Collection_Loop_";
        int count = 4;
        for (int i=1; i<=count; i++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
            particle->setPosition(ccp(parentX+mainWidth/2, parentY+100));
            addParticleToBatch(particle);
        }
    }
    else if (type == 4) {
        string tmpStart = "Treatment_";
        int count = 2;
        for (int i=1; i<=count; i++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
            particle->setPosition(ccp(parentX+mainWidth/2, parentY+100));
            addParticleToBatch(particle);
        }
    }
    else if (type == 5) {
        string tmpStart = "UiFireBarbecue_";
        int count = 3;
        for (int i=1; i<=count; i++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
            particle->setPosition(ccp(parentX+mainWidth/2, parentY+60));
            addParticleToBatch(particle);
        }
    }
    else if (type == 2) {
        onPlayParticle();
    }
    else if (type == 6) {
        auto particleL1 = ParticleController::createParticle(CCString::createWithFormat("Wish_Fountain_1")->getCString());
        particleL1->setPosition(ccp(parentX+135, parentY+70));
        addSpeParticleToBatch(particleL1);
        auto particleL2 = ParticleController::createParticle(CCString::createWithFormat("Wish_Fountain_2")->getCString());
        particleL2->setPosition(ccp(parentX+135, parentY+75));
        addSpeParticleToBatch(particleL2);
        auto particleL3 = ParticleController::createParticle(CCString::createWithFormat("Wish_Fountain_S")->getCString());
        particleL3->setPosition(ccp(parentX+135, parentY+95));
        addSpeParticleToBatch(particleL3);
        auto particleL4 = ParticleController::createParticle(CCString::createWithFormat("Wish_Fountain_S")->getCString());
        particleL4->setPosition(ccp(parentX+80, parentY+65));
        addSpeParticleToBatch(particleL4);
        auto particleL5 = ParticleController::createParticle(CCString::createWithFormat("Wish_Fountain_S")->getCString());
        particleL5->setPosition(ccp(parentX+190, parentY+65));
        addSpeParticleToBatch(particleL5);
    }
    else if (type == 7) {
        int count = 6;
        for (int i=1; i<=count; i++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("ManualWorkshop_%d",i)->getCString());
            particle->setPosition(ccp(parentX+93, parentY+48));
            if (i==5 || i==6) {
                particle->setPosition(ccp(parentX+93, parentY+48+20));
            }
            addSpeParticleToBatch(particle);
        }
        count = 2;
        for (int i=1; i<=count; i++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("Smoke_%d",i)->getCString());
            particle->setPosition(ccp(parentX+22, parentY+160));
            addSpeParticleToBatch(particle);
        }
    }
    
}

void DragonBuild::onPlayParticle()
{
    auto layer = dynamic_cast<DragonScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
    if (layer) {
        auto pt = layer->onGetSceenPt(parentX+mainWidth/2, parentY+mainHeight);
        layer->onFlyTroopParticle(pt.x, pt.y, 1);
//        if(m_buildingKey/1000!=FUN_BUILD_FORGE && m_buildingKey/1000!=FUN_BUILD_WORKSHOP)
//        {
//            
//        }
    }
}

void DragonBuild::showTmpBuild(int itemId)
{
    hideTmpBuild();
    
    auto build = DragonBuild::createTmpBuild(itemId, parentX, parentY, m_batchNode, m_zOrder, m_blentBatchNode);
    build->setTag(9);
    this->addChild(build);
}

void DragonBuild::hideTmpBuild()
{
    CCNode* node = this->getChildByTag(9);
    if (node) {
        DragonBuild* build = dynamic_cast<DragonBuild*>(node);
        if (build) {
            build->onBuildDelete();
        }
    }
    this->removeChildByTag(9);
}

void DragonBuild::onPlaySpeEffect(bool show)
{
    float dt = 2.0;
    if (show) {
        m_effectSpr->setOpacity(0);
        m_effectSpr->setVisible(true);
        m_effectSpr->runAction(CCFadeIn::create(dt));
        for (int i=0; i<m_speEffectVec.size(); i++) {
            m_speEffectVec[i]->setOpacity(0);
            m_speEffectVec[i]->setVisible(true);
            m_speEffectVec[i]->runAction(CCFadeIn::create(dt));
        }
    }
    else {
        m_effectSpr->runAction(CCFadeOut::create(dt));
        for (int i=0; i<m_speEffectVec.size(); i++) {
            m_speEffectVec[i]->runAction(CCFadeOut::create(dt));
        }
        scheduleOnce(schedule_selector(DragonBuild::onShowSpeEffect), dt);
    }
}
void DragonBuild::onShowSpeEffect(float _time)
{
    m_effectSpr->setVisible(isEffectRunning);
    for (int i=0; i<m_speEffectVec.size(); i++) {
        m_speEffectVec[i]->setVisible(isEffectRunning);
    }
}

CCNode* DragonBuild::getGuideNode(string _key)
{
    if(m_buildState) {
        return m_buildState->getGuideNode(_key);
    }
    return NULL;
}

void DragonBuild::checkIncreasedStatus(){
    double effectTime = m_info->effectTime;
    int gapTime = effectTime - GlobalData::shared()->getWorldTime();
    if(gapTime>0 && effectTime>0){
        if(m_gainSpr!=NULL){
            return;
        }
        int offX = 0;
        int offY = 0;
        float scale = 2.0;
        bool bst = false;
//        switch(m_info->type){
//            case FUN_BUILD_BARRACK1:
//                offX = 20;
//                offY = 5;
//                scale = 2.5;
//                bst = true;
//                break;
//            case FUN_BUILD_BARRACK2:
//                offX = 20;
//                offY = 15;
//                scale = 2.5;
//                bst = true;
//                break;
//            case FUN_BUILD_BARRACK3:
//                offX = 20;
//                offY = 10;
//                scale = 2.5;
//                bst = true;
//                break;
//            case FUN_BUILD_BARRACK4:
//                offX = 10;
//                offY = 5;
//                scale = 2.5;
//                bst = true;
//                break;
//            case FUN_BUILD_FORT:
//                offX = 20;
//                offY = 10;
//                scale = 2.5;
//                bst = true;
//                break;
//                
//        }
        offX = 20;
        offY = 5;
        scale = 2.5;
        bst = true;
        int tmpBatchPosX = parentX;
        int tmpBatchPosY = parentY;
        if(bst) {
            tmpBatchPosX = 0;
            tmpBatchPosY = 0;
        }
        m_gainSpr = CCSprite::create();
        CCCommonUtils::makeEffectSpr(m_gainSpr, "Gain_%d.png", 16,0.1);
        m_gainSpr->setScale(scale);
        m_gainSpr->setPosition(ccp(tmpBatchPosX+0+offX, tmpBatchPosY+0+offY));
        m_gainSpr->setOpacity(175);
        m_batchNode->addChild(m_gainSpr);
        m_sprArray->addObject(m_gainSpr);
        m_gainSpr->setZOrder(1000*m_zOrder);//1000*m_zOrder 1200
        string tmpStart = "IncreasedGlow_";
        int count = 3;
        for (int i=1; i<count; i++) {
            auto particle = ParticleController::createParticle(CCString::createWithFormat("%s%d",tmpStart.c_str(),i)->getCString());
            particle->setPosition(ccp(parentX+100+offX, parentY+60+offY));
            addSpeParticleToBatch(particle);
        }
    }else{
        if(m_gainSpr){
            m_gainSpr->removeFromParentAndCleanup(true);
        }
        onHideSpeParticle();
    }
}

void DragonBuild::showHarvestAni(CCObject* param){
    if(m_info==NULL) return ;
    CCString* time = dynamic_cast<CCString*>(param);
    addResTime = 0;
    if(time!=NULL){
        addResTime = atoi(time->getCString());
    }
    if(addResTime==0) return ;
    HarvestSkillAni* dropCCB = NULL;
    switch (m_info->type) {
//        case FUN_BUILD_STONE:
//            dropCCB = HarvestSkillAni::create(R_STONE);
//            break;
//        case FUN_BUILD_WOOD:
//            dropCCB = HarvestSkillAni::create(R_WOOD);
//            break;
//        case FUN_BUILD_IRON:
//            dropCCB = HarvestSkillAni::create(R_IRON);
//            break;
//        case FUN_BUILD_FOOD:
//            dropCCB = HarvestSkillAni::create(R_FOOD);
//            break;
    }
    if(dropCCB){
        dropCCB->setScale(0.5);
        m_upEffectNode->addChild(dropCCB);
        this->scheduleOnce(schedule_selector(DragonBuild::showFlyOut), 2.4);
    }
}

void DragonBuild::showFlyOut(float _time){
    int add = DragonBuildingController::getInstance()->getOutPutPerSecond(m_info->itemId)*addResTime;
    if(add<=0) return;
    int ot = add;
    string temp = "+";
    temp = temp+CC_ITOA(ot);
    CCCommonUtils::flyUiResText(temp.c_str(), this->m_signNode,ccp(70, 60) ,ccGREEN, floating_type_ui1, 28);
}

void DragonBuild::playFortSkillEffect(CCObject* params)
{
    playFortSkillEffect(0.0);
}
void DragonBuild::playFortSkillEffect(float _time){
//    if(m_info && m_info->type == FUN_BUILD_FORT)
//    {
//        string fortInfo =  GlobalData::shared()->m_skillFortInfo;
//        if(fortInfo==""){
//            return ;
//        }
//        std::vector<std::string> data;
//        CCCommonUtils::splitString(fortInfo,"|",data);
//        if(data.size()!=2) return ;
//        
//        auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
//        if(layer) {
//            layer->setUnMoveScence(false);
//            layer->onMoveToBuildAndOpen(m_buildingKey, TYPE_POS_UP);
//        }
//        for(int i=1;i<3;i++){
//            auto particle = ParticleController::createParticle(CCString::createWithFormat("Light_%d",i)->getCString());
//            particle->setPosition(ccp(-10, 460));
//            m_upEffectNode->addChild(particle);
//        }
//        for(int i=1;i<10;i++){
//            if(i==4 ||i==5) continue;
//            auto particle = ParticleController::createParticle(CCString::createWithFormat("CollectionOut_%d",i)->getCString());
//            particle->setPosition(ccp(0, -30));
//            m_upEffectNode->addChild(particle);
//        }
//        
//        auto particle = ParticleController::createParticle(CCString::createWithFormat("Harvest_10")->getCString());
//        particle->setPosition(ccp(-5, -55));
//        m_upEffectNode->addChild(particle);
//        
//        int freeNum = atoi(data[1].c_str());
//        string fortId = data[0];
//        map<std::string, ArmyInfo>::iterator it = GlobalData::shared()->fortList.find(fortId);
//        if(it!=GlobalData::shared()->fortList.end()){
//            it->second.free = it->second.free + freeNum;
//        }
//        string icon = "ico" + fortId+ ".png";
//        CCSprite* iconSpr = CCLoadSprite::createSprite(icon.c_str());
//        iconSpr->setPosition(ccp(0, 350));
//        iconSpr->setOpacity(0);
//        m_upEffectNode->addChild(iconSpr);
//        CCSequence* cs = CCSequence::create(CCDelayTime::create(0.15),CCFadeIn::create(0.2),CCMoveTo::create(0.4, ccp(0, 100)),CCFadeOut::create(0.25),CCRemoveSelf::create(),NULL);
//        iconSpr->runAction(cs);
//        GlobalData::shared()->m_skillFortInfo = "";
//        string addFort = "+" + data[1];
//        CCCommonUtils::flyUiResText(addFort.c_str(), this->m_signNode,ccp(130, 120) ,ccGREEN, floating_type_ui1, 28);
//    }
}

/////////////
DragonBuildState* DragonBuildState::create(int itemId)
{
    DragonBuildState *pRet = new DragonBuildState();
    if (pRet && pRet->init(itemId))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool DragonBuildState::init(int itemId)
{
    m_info = &((*DragonBuildingController::getInstance()->curDragonBuildsInfo)[itemId]);
    m_buildingKey = itemId;
    m_qid = QID_MAX;
    int bType = m_buildingKey/1000;
    isChange = true;
    bool b_isGet = false;
    m_qType = TYPE_DRAGON_BUILD;
    m_qid = QueueController::getInstance()->getMinTimeQidByType(m_qType, CC_ITOA(m_buildingKey));
    
    if (!b_isGet) {
        if (m_qid != QID_MAX && m_qType != TYPE_DRAGON_BUILD) {
            auto& qInfo = GlobalData::shared()->allQueuesInfo[m_qid];
            m_sumTime = qInfo.finishTime - qInfo.startTime;
            m_curTime = qInfo.finishTime - GlobalData::shared()->getWorldTime();
        }
        else {
            if(m_qType == TYPE_DRAGON_BUILD) {
                m_sumTime = m_info->des_time;
                m_curTime = m_info->updateTime - GlobalData::shared()->getWorldTime();
            }
            else {
            }
        }
    }
    
    CCBLoadFile("BuildStateView",this,this);
    m_freeLabel->setString(_lang("103672"));
    m_freeNode->setVisible(false);
    m_recNode->setVisible(false);
    m_helpNode->setVisible(false);
    m_getNode->setVisible(false);
    
    if (b_isGet) {
        int resType = Wood;
        string picStr = CCCommonUtils::getResourceIconByType(resType);
        auto pic = CCLoadSprite::createSprite(picStr.c_str());
        m_getIconNode->addChild(pic);
        m_getNode->setVisible(true);
    }
    else if (m_qType == TYPE_DRAGON_BUILD) {
        m_msgNode->setVisible(false);
        string iconStr = "icon_shengji1.png";
        if(m_info->state != FUN_BUILD_UPING) {
            iconStr = "icon_jianzao.png";
        }
        auto tIcon = CCLoadSprite::createSprite(iconStr.c_str());
        m_iconNode->addChild(tIcon);
    }
    else {
        if (m_qid!=QID_MAX) {
            int buildType = m_buildingKey/1000;
            auto& qInfo = GlobalData::shared()->allQueuesInfo[m_qid];
            int sid = QueueController::getInstance()->getItemId(qInfo.key);
            string name = CCCommonUtils::getNameById(CC_ITOA(sid));
            int num = 0;
            m_msgLabel->setString(name);
            m_msgLabel->setString(_lang("102114"));
            
            string pic = "icon_bubing.png";
            auto tIcon = CCLoadSprite::createSprite(pic.c_str());
            m_iconNode->addChild(tIcon);
            
            string recPic = "shangBing.png";
            auto utIcon = CCLoadSprite::createSprite(recPic.c_str());
            m_recIcon->addChild(utIcon);
        }
    }
    
    m_freeNode->setPositionY(115);
    m_helpNode->setPositionY(115);
    m_getNode->setPositionY(100);
    
    if (!b_isGet) {
        m_bar->setScaleX(0);
        m_bar2->setScaleX(0);
        onEnterFrame(0);
        onCheckAllianceHelp();
    }
    else {
        m_curTime = 0;
        m_upNode->setVisible(false);
        onPlayCanRec(4);
    }
    
    m_freeBtn->setTouchPriority(Touch_Imperial_City);
    m_helpBtn->setTouchPriority(Touch_Imperial_City);
    m_recBtn->setTouchPriority(Touch_Imperial_City);
    m_getBtn->setTouchPriority(Touch_Imperial_City);
    
    return true;
}

void DragonBuildState::onCheckAllianceHelp()
{
//    if(m_freeNode->isVisible() || m_recNode->isVisible()) {
//        return;
//    }
//    
//    if (m_qid!=QID_MAX && m_info->state != FUN_BUILD_DESTROY) {
//        auto& qInfo = GlobalData::shared()->allQueuesInfo[m_qid];
//        if (qInfo.type == TYPE_DRAGON_BUILD || qInfo.type == TYPE_HOSPITAL || qInfo.type == TYPE_SCIENCE || qInfo.type == TYPE_FORGE) {
//            if (GlobalData::shared()->playerInfo.isInAlliance() && qInfo.canHelp){
//                m_helpIconNode->removeAllChildren();
//                auto icon = CCLoadSprite::createSprite("alliances_help.png");
//                m_helpIconNode->addChild(icon);
//                m_helpNode->setVisible(true);
//                onPlayCanRec(3);
//            }
//        }
//    }
//    else if (m_info->type == FUN_BUILD_STABLE && GlobalData::shared()->playerInfo.allianceInfo.helpcount>0) {
//        m_helpIconNode->removeAllChildren();
//        auto icon = CCLoadSprite::createSprite("allianceHelpAll.png");
//        m_helpIconNode->addChild(icon);
//        m_helpNode->setVisible(true);
//        onPlayCanRec(3);
//        if(m_info->state == FUN_BUILD_NORMAL) {
//            m_upNode->setVisible(false);
//        }
//        else {
//            m_upNode->setVisible(true);
//        }
//    }
}

void DragonBuildState::onEnterFrame(float dt)
{
    if (m_qid == QID_MAX) {
        if (m_info->type == FUN_BUILD_STABLE) {
            return ;
        }
        if(isChange && DragonBuildingController::getInstance()->isFullOutPut(m_info->itemId)) {
            isChange = false;
            m_getIconNode->removeAllChildren();
            int resType = Wood;
            if(m_info->type==FUN_BUILD_WOOD){
                resType = Wood;
            }
            else if(m_info->type==FUN_BUILD_FOOD){
                resType = Food;
            }
            else if(m_info->type==FUN_BUILD_IRON){
                resType = Iron;
            }
            else if(m_info->type==FUN_BUILD_STONE){
                resType = Stone;
            }
            string picStr = CCCommonUtils::getResourceIconByType(resType);
            auto pic = CCLoadSprite::createSprite(picStr.c_str());
            //            CCCommonUtils::setSpriteMaxSize(pic, 80, true);
            m_getIconNode->addChild(pic);
            m_getNode->setVisible(true);
        }
        return;
    }
    if (m_curTime>=0) {
        auto& qInfo = GlobalData::shared()->allQueuesInfo[m_qid];
        int tmpCurTime = GlobalData::shared()->getWorldTime();
        if(qInfo.startTime==0){
            qInfo.startTime = tmpCurTime;
        }
        m_sumTime = qInfo.finishTime - qInfo.startTime;
        m_curTime = qInfo.finishTime - tmpCurTime;
        m_curTime = MAX(0, m_curTime);
        m_sumTime = MAX(1, m_sumTime);
        if (qInfo.finishTime<=0) {
            m_curTime = 0;
            m_sumTime = 1;
        }
        
        m_timeLabel->setString(CC_SECTOA(m_curTime));
        float pro = m_curTime*1.0/m_sumTime;
        pro = pro>1?1:pro;
        m_bar->setScaleX(1-pro);
        m_bar2->setScaleX(1-pro);
        if (m_qType == TYPE_DRAGON_BUILD && m_curTime<=GlobalData::shared()->GlobalData::shared()->freeSpdT)
        {
            string guideId = GuideController::share()->getCurrentId();
            if (guideId=="3071400" || guideId=="3072300" || guideId=="3072800" || guideId=="3074800"
                || guideId=="3071300" || guideId=="3072200" || guideId=="3072700" || guideId=="3074700")
            {
                m_freeNode->setVisible(false);
            }
            else {
                if (!m_freeNode->isVisible()) {
                    m_bar->setVisible(false);
                    
                    for (int i=1; i<=2; i++) {
                        auto particle = ParticleController::createParticle(CCString::createWithFormat("QueueGlow_%d",i)->getCString());
                        particle->setPosition(ccp(66, 66));
                        m_freeTouchNode->addChild(particle);
                    }
                    m_freeNode->setVisible(true);
                    m_helpNode->setVisible(false);
                    onPlayCanRec(1);
                }
            }
        }
        
        if (!qInfo.canHelp && m_helpNode->isVisible()) {
            m_helpNode->setVisible(false);
        }
        
        if(m_curTime<=0) {
            m_upNode->setVisible(false);
            m_freeTouchNode->removeAllChildren();
            m_freeNode->setVisible(false);
            m_helpNode->setVisible(false);
            
            if (m_qType != TYPE_DRAGON_BUILD) {
                m_recNode->setVisible(true);
                onPlayCanRec(2);
            }
        }
    }
}

void DragonBuildState::onPlayCanRec(int type)
{
    m_recNode->stopAllActions();
    m_freeNode->stopAllActions();
    m_helpNode->stopAllActions();
    m_getNode->stopAllActions();
    CCRotateBy* rt1 = CCRotateBy::create(0.1, 15);
    CCRotateBy* rt2 = CCRotateBy::create(0.1, -30);
    CCRotateBy* rt3 = CCRotateBy::create(0.1, 22);
    CCRotateBy* rt4 = CCRotateBy::create(0.1, -14);
    CCRotateBy* rt5 = CCRotateBy::create(0.1, 7);
    CCDelayTime* dt = CCDelayTime::create(5);
    CCSequence* seq = CCSequence::create(rt1, rt2, rt3, rt4, rt5, dt, NULL);
    if (type==1) {
        //        m_freeNode->runAction(CCRepeatForever::create(seq));
    }
    else if (type == 2) {
        m_recNode->runAction(CCRepeatForever::create(seq));
    }
    else if (type == 3) {
        m_helpNode->runAction(CCRepeatForever::create(seq));
    }
    else if (type == 4) {
        m_getNode->runAction(CCRepeatForever::create(seq));
    }
}

void DragonBuildState::onShowRecState()
{
    m_curTime=0;
    if (m_qType != TYPE_DRAGON_BUILD) {
        m_upNode->setVisible(false);
        m_recNode->setVisible(true);
        onPlayCanRec(2);
    }
}

void DragonBuildState::onCheckTime(CCObject* params)
{
    int itemId = dynamic_cast<CCInteger*>(params)->getValue();
    if (m_buildingKey!=itemId) {
        return;
    }
    
    int key = 0;
    if (m_qType == TYPE_DRAGON_BUILD) {
        m_qid = QueueController::getInstance()->getMinTimeQidByType(m_qType, CC_ITOA(m_buildingKey));
    }
    else {
        m_qid = QueueController::getInstance()->getMinTimeQidByType(m_qType);
    }
    
    if (m_qid != QID_MAX) {
        auto& qInfo = GlobalData::shared()->allQueuesInfo[m_qid];
        key = atoi(qInfo.key.c_str());
    }
    
    if (itemId == key) {
        auto& qInfo = GlobalData::shared()->allQueuesInfo[m_qid];
        m_sumTime = qInfo.finishTime - qInfo.startTime;
        m_curTime = qInfo.finishTime - GlobalData::shared()->getWorldTime();
    }
    
}

void DragonBuildState::onEnter() {
    CCNode::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DragonBuildState::onCheckTime), MSG_CHECK_TIME, NULL);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(DragonBuildState::onEnterFrame), this, 1.0,kCCRepeatForever, 0.0f, false);
}

void DragonBuildState::onExit() {
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_CHECK_TIME);
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(DragonBuildState::onEnterFrame), this);
    CCNode::onExit();
}

SEL_MenuHandler DragonBuildState::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{return NULL;}

SEL_CCControlHandler DragonBuildState::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onFreeBtnClick", DragonBuildState::onFreeBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onRecBtnClick", DragonBuildState::onRecBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onHelpBtnClick", DragonBuildState::onHelpBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onGetBtnClick", DragonBuildState::onGetBtnClick);
    return NULL;
}

bool DragonBuildState::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upNode", CCNode*, this->m_upNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bar", CCScale9Sprite*, this->m_bar);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeLabel", CCLabelIF*, this->m_timeLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msgNode", CCNode*, this->m_msgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msgLabel", CCLabelIF*, this->m_msgLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, this->m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bar2", CCScale9Sprite*, this->m_bar2);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_freeNode", CCNode*, this->m_freeNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_freeBtn", CCControlButton*, this->m_freeBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_freeLabel", CCLabelIF*, this->m_freeLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_helpNode", CCNode*, this->m_helpNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_helpBtn", CCControlButton*, this->m_helpBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_helpIconNode", CCNode*, this->m_helpIconNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_recNode", CCNode*, this->m_recNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_recBtn", CCControlButton*, this->m_recBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_recIcon", CCNode*, this->m_recIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_freeTouchNode", CCNode*, this->m_freeTouchNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_getNode", CCNode*, this->m_getNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_getBtn", CCControlButton*, this->m_getBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_getIconNode", CCNode*, this->m_getIconNode);
    return false;
}

bool DragonBuildState::isCanClick()
{
    if (m_freeNode->isVisible() || m_recNode->isVisible() || m_helpNode->isVisible() || m_getNode->isVisible()) {
        return true;
    }
    return false;
}

void DragonBuildState::ClickState()
{
    if (m_freeNode->isVisible() )
    {
        onFreeBtnClick(NULL,Control::EventType::TOUCH_DOWN);
    }
    else if (m_recNode->isVisible()) {
        onRecBtnClick(NULL,Control::EventType::TOUCH_DOWN);
    }
    else if (m_helpNode->isVisible()) {
        onHelpBtnClick(NULL,Control::EventType::TOUCH_DOWN);
    }
    else if (m_getNode->isVisible()) {
        onGetBtnClick(NULL,Control::EventType::TOUCH_DOWN);
    }
}


void DragonBuildState::onFreeBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if ( DragonBuildingController::getInstance()->costCD(m_buildingKey, "", 0) )
    {
    }
    else {
        CCLOG("dddd");
    }
}

void DragonBuildState::onHelpBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
//    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
//    if (m_info->type == FUN_BUILD_STABLE && GlobalData::shared()->playerInfo.allianceInfo.helpcount>0) {
//        if (m_qid!=QID_MAX) {
//            auto& qInfo = GlobalData::shared()->allQueuesInfo[m_qid];
//            if (GlobalData::shared()->playerInfo.isInAlliance() && qInfo.canHelp){//帮助请求，和处理别人帮助同时发送
//                m_helpNode->stopAllActions();
//                m_helpNode->setVisible(false);
//                AllianceManager::getInstance()->callHelp(m_qid);
//                AllianceManager::getInstance()->sendHelpAllToServer();
//            }
//        }
//        else {
//            m_helpNode->stopAllActions();
//            m_helpNode->setVisible(false);
//            AllianceManager::getInstance()->sendHelpAllToServer();
//        }
//    }
//    else {
//        m_helpNode->stopAllActions();
//        m_helpNode->setVisible(false);
//        AllianceManager::getInstance()->callHelp(m_qid);
//    }
}
void DragonBuildState::autoClickRecBtn(){
    this->onRecBtnClick(NULL,Control::EventType::TOUCH_DOWN);
}

void DragonBuildState::onRecBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
//    int btype = m_buildingKey/1000;
//    int qtype = CCCommonUtils::getQueueTypeByBuildType(btype);
//    if (qtype>0) {
//        int qid = QueueController::getInstance()->getCanRecQidByType(qtype);
//        if (qid != QID_MAX) {
//            unsigned long addPower = 0;
//            map<int, QueueInfo>::iterator it =  GlobalData::shared()->allQueuesInfo.find(qid);
//            if (it->second.type == TYPE_FORGE)
//            {
//                //收装备
//                SoundController::sharedSound()->playEffects(Music_Sfx_city_harvest_equipment);
//                QueueController::getInstance()->autoHavestEquip(qid, m_buildingKey);
//            }
//            else if (it->second.type == TYPE_MATE)
//            {
//                QueueController::getInstance()->autoHavestMateTool(qid, m_buildingKey);
//            }
//            else
//            {
//                //收兵
//                GlobalData::shared()->playerInfo.lastBattlePower =  GlobalData::shared()->playerInfo.battlePower;
//                if(it!=GlobalData::shared()->allQueuesInfo.end()){
//                    std::string armyId = CC_ITOA(atoi((it->second).key.c_str()) / 100);
//                    if(m_buildingKey/1000 == FUN_BUILD_FORT){
//                        map<std::string, ArmyInfo>::iterator armyIt = GlobalData::shared()->fortList.find(armyId);
//                        if(armyIt!=GlobalData::shared()->fortList.end()){
//                            addPower = (unsigned long)( ((float)armyIt->second.train) * armyIt->second.power);
//                            addPower = MAX(addPower, 0);
//                            GlobalData::shared()->playerInfo.fortPower = GlobalData::shared()->playerInfo.fortPower + addPower;
//                        }
//                    }else{
//                        map<std::string, ArmyInfo>::iterator armyIt = GlobalData::shared()->armyList.find(armyId);
//                        if(armyIt!=GlobalData::shared()->armyList.end()){
//                            addPower = (unsigned long)( ((float)armyIt->second.train) * armyIt->second.power);
//                            addPower = MAX(addPower, 0);
//                            GlobalData::shared()->playerInfo.armyPower = GlobalData::shared()->playerInfo.armyPower + addPower;
//                        }
//                    }
//                }
//                GlobalData::shared()->playerInfo.addPower = addPower;
//                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_COLLECT_SOLDIER_ADD_POWER);
//                QueueController::getInstance()->collectSolider(qid,m_buildingKey);
//                //ArmyController::getInstance()->startFinishForce(itemId, (it->second).uuid,buildingKey);
//                
//                if (GuideController::share()->getCurrentId() == "3074000") {
//                    auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
//                    if(layer) {
//                        layer->setUnMoveScence(false);
//                        layer->onMoveToPos(390, 600, TYPE_POS_MID, 7.0, 0.8, true);
//                    }
//                }
//                
//                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
//                                                                                       , CCString::createWithFormat("BU_%d_troop", m_buildingKey/1000));
//                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_REMOVE_STATE, CCInteger::create(m_buildingKey));
//            }
//        }
//        else {
//            if (btype == FUN_BUILD_WORKSHOP) {
//                auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
//                if (layer) {
//                    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MAINSCENCE_SAVEPOS);
//                    layer->setUnMoveScence(false);
//                    layer->onMoveToBuildAndOpen(m_buildingKey, TYPE_POS_MID);
//                    PopupViewController::getInstance()->addPopupView(ToolCreateView::create());
//                }
//            }
//        }
//    }
}

void DragonBuildState::onGetBtnClick(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (m_getNode->isVisible()) {
        DragonBuildingController::getInstance()->startGetFunBuildProduct(m_info->itemId);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                               , CCString::createWithFormat("BU_%d_collect", m_buildingKey/1000));
        m_getNode->stopAllActions();
        m_getNode->setVisible(false);
    }
}

void DragonBuildState::onHideBtn(int type)
{
    if(type == 4) {
        m_helpNode->stopAllActions();
        m_helpNode->setVisible(false);
    }
}

CCNode* DragonBuildState::getGuideNode(string _key)
{
    if(_key=="cd") {
        if (m_freeNode->isVisible()) {
            return m_freeBtn;
        }
        else {
            return NULL;
        }
    }
    else if (_key=="troop" && m_recNode->isVisible()) {
        return m_recBtn;
    }
    else if (_key=="collect" && m_getNode->isVisible()) {
        return m_getBtn;
    }
    return NULL;
}

bool DragonBuildState::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    int pop = PopupViewController::getInstance()->getCurrViewCount();
    if (pop <=0 && m_getBtn->isTouchInside(pTouch) && m_getNode->isVisible()) {
        CanDel = false;
        onGetBtnClick(NULL,Control::EventType::TOUCH_DOWN);
        return true;
    }
    return false;
}

void DragonBuildState::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    CanDel = true;
}
void DragonBuildState::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_RES_GET, pTouch);
}
void DragonBuildState::onTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    CanDel = true;
}
void DragonBuildState::onGetResAction(CCObject* params)
{
    CCTouch *pTouch = dynamic_cast<CCTouch*>(params);
    if (pTouch && m_getNode->isVisible() && m_getBtn->isTouchInside(pTouch)) {
        onGetBtnClick(NULL,Control::EventType::TOUCH_DOWN);
    }
}