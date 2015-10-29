//
//  MainCityArmy.cpp
//  IF
//
//  Created by ganxiaohua on 14-7-4.
//
//

#include "MainCityArmy.h"
#include "FunBuildController.h"
#include "GuideController.h"

MainCityArmy* MainCityArmy::create(CCLayer* parent, CCSpriteBatchNode* soldierBatch, int buildId){
    MainCityArmy* ret = new MainCityArmy();
    if(ret && ret->init(parent, soldierBatch, buildId)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool MainCityArmy::init(CCLayer* parent, CCSpriteBatchNode* soldierBatch, int buildId){
    bool bRet = false;
    if(CCLayer::init()){
        this->setTouchEnabled(true);
        bRet = true;
        
        int soldierType = 0;
        int bType = buildId/1000;
        m_soldierBatch = soldierBatch;
        
        m_isInGuide = false;
        string guideId = GuideController::share()->getCurGuideID();
        if (GuideController::share()->isInTutorial() && (guideId=="3071700" || guideId=="3074100")) {
            m_isInGuide = true;
        }
        
        std::string path = CCString::createWithFormat("%s_%d_%s_move_0.png","a010",0,"S")->getCString();
        
        auto& bInfo = FunBuildController::getInstance()->getFunbuildById(buildId);
        if ( FunBuildController::getInstance()->posToPath.find(bInfo.pos) != FunBuildController::getInstance()->posToPath.end() ) {
            m_posV = FunBuildController::getInstance()->posToPath[bInfo.pos];
            m_zOrders = FunBuildController::getInstance()->posToZorders[bInfo.pos];
            if (bType == FUN_BUILD_BARRACK1) {
                soldierType = 0;
                int pos = FunBuildController::getInstance()->lastSqerPos[1];
                if (pos < FunBuildController::getInstance()->sqerPoints.size()) {
                    CCPoint lpt = FunBuildController::getInstance()->sqerPoints[pos];
                    m_posV.push_back(lpt);
                }
            }
            else if (bType == FUN_BUILD_BARRACK2) {
                soldierType = 2;
                int pos = FunBuildController::getInstance()->lastSqerPos[2];
                if (pos < FunBuildController::getInstance()->sqerPoints.size()) {
                    CCPoint lpt = FunBuildController::getInstance()->sqerPoints[pos];
                    m_posV.push_back(lpt);
                }
            }
            else if (bType == FUN_BUILD_BARRACK3) {
                soldierType = 4;
                int pos = FunBuildController::getInstance()->lastSqerPos[3];
                if (pos < FunBuildController::getInstance()->sqerPoints.size()) {
                    CCPoint lpt = FunBuildController::getInstance()->sqerPoints[pos];
                    m_posV.push_back(lpt);
                }
            }
            else if (bType == FUN_BUILD_BARRACK4) {
                soldierType = 3;
                int pos = FunBuildController::getInstance()->lastSqerPos[4];
                if (pos < FunBuildController::getInstance()->sqerPoints.size()) {
                    CCPoint lpt = FunBuildController::getInstance()->sqerPoints[pos];
                    m_posV.push_back(lpt);
                }
            }
        }
        else {
            return false;
        }
        
        m_curBatch = CCSpriteBatchNode::createWithTexture(CCLoadSprite::loadResource(path.c_str())->getTexture());
        m_curBatch->setAnchorPoint(ccp(0, 0));
        parent->addChild(m_curBatch);
        m_curBatch->setZOrder(m_zOrders[0]+10);
        
        int tmp_curSCnt = FunBuildController::getInstance()->colTroopsNum[bType];
        m_soldierCnt = int(powf(tmp_curSCnt, 0.35));
        m_soldierCnt = max(1, m_soldierCnt);
        m_soldierCnt = min(52, m_soldierCnt);
//        m_soldierCnt = 20;
        m_stopCnt = 0;
        for(int i=0;i<m_soldierCnt;i++){
            MackSoldier(soldierType, i);
        }
        
    }
    return bRet;
}

void MainCityArmy::onEnter(){
    CCLayer::onEnter();
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MainCityArmy::checkZOrder), MSG_CHECK_ZORDER, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(MainCityArmy::soldierStop), MSG_SOLDIER_STOP, NULL);
}

void MainCityArmy::onExit(){
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_CHECK_ZORDER);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SOLDIER_STOP);
    CCLayer::onExit();
}

void MainCityArmy::checkZOrder(CCObject* obj)
{
    BattleSoldier2* soldier = dynamic_cast<BattleSoldier2*>(obj);
    if(soldier && this==soldier->getParent()) {
        int mIdx = soldier->m_moveIndex;
        int curOd = m_curBatch->getZOrder()/100;
        if (mIdx<m_zOrders.size() && curOd != (m_zOrders)[mIdx]) {
            int ad = m_curBatch->getZOrder()%100;
            m_curBatch->setZOrder((m_zOrders)[mIdx]+ad);
        }
    }
}

void MainCityArmy::soldierStop(CCObject* obj)
{
    BattleSoldier2* soldier = dynamic_cast<BattleSoldier2*>(obj);
    if(soldier && this==soldier->getParent()) {
        m_stopCnt++;
        soldier->removeFromBatchNode(0.5);
        if (m_soldierCnt == m_stopCnt) {
            changeBatchNode();
        }
    }
}

void MainCityArmy::changeBatchNode()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_TROOPS_CHANGE);
    m_curBatch->removeFromParent();
    this->removeFromParent();
    return;
    
//    auto arrSpr = m_curBatch->getChildren();
//    if (arrSpr) {
//        for (int i=0; i<arrSpr->count(); i++) {
//            CCSprite* tmpSpr = dynamic_cast<CCSprite*>(arrSpr->objectAtIndex(i));
//            if (tmpSpr) {
//                tmpSpr->retain();
//                tmpSpr->removeFromParent();
//                m_soldierBatch->addChild(tmpSpr);
//                tmpSpr->release();
//                i--;
//            }
//        }
//    }
//    m_curBatch->removeFromParent();
//    m_curBatch = NULL;
//    CCLOG("stop end");
}

bool MainCityArmy::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    
    return true;
}

void MainCityArmy::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}

void MainCityArmy::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    
}

void MainCityArmy::restart(){
}

bool MainCityArmy::MackSoldier(int type, int idx){
    // tao.yu 跑路的兵
    string m_icon = "";
    int tmpPx = 70;
    int tmpPy = 40;
    float scale = 2;
    switch (type) {
        case 0://步兵
            m_icon = "a010";
            break;
        case 1://弓兵
            m_icon = "a060";
            break;
        case 2://骑兵
            m_icon = "a020";
            break;
        case 3://战车
            m_icon = "zhanche";
            break;
        case 4://长弓兵
            m_icon = "a060";
            break;
        case 5:
            m_icon = "a020";
            break;
        default:
            m_icon = "a020";
            break;
    }
    
    if (true) {
        int addx = 0;
        int addy = 0;
        int tId = idx%9;
        if (tId == 4) {
        }else if (tId == 0) {
            addx = -tmpPx;
            addy = tmpPy;
        }else if (tId == 2) {
            addy = tmpPy;
        }else if (tId == 5) {
            addx = tmpPx;
            addy = tmpPy;
        }else if (tId == 7) {
            addx = tmpPx;
        }else if (tId == 8) {
            addx = tmpPx;
            addy = -tmpPy;
        }else if (tId == 6) {
            addy = -tmpPy;
        }else if (tId == 3) {
            addx = -tmpPx;
            addy = -tmpPy;
        }else if (tId == 1) {
            addx = -tmpPx;
        }
        
        vector<cocos2d::CCPoint> posV = m_posV;
        for(int i=0; i<posV.size(); i++) {
            posV[i].x += addx;
            posV[i].y += addy;
        }
        
        BattleSoldier2* soldier = BattleSoldier2::create(m_curBatch, NULL,0,0,m_icon,"NE",false);
        soldier->setSoldierPosition(posV[0]);
        soldier->setAnchorPoint(ccp(0.5, 0.5));
        this->addChild(soldier);
        soldier->setSprScale(scale);
        soldier->playAnimation(ACTION_MOVE);
        float spd = 130;
        if (m_isInGuide) {
            spd = 200;
        }
        soldier->moveToPosition(posV,idx/9*1.3, spd);
    }
    return true;
}