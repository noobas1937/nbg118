//
//  BattleArmy.cpp
//  IF
//
//  Created by ganxiaohua on 14-6-18.
//
//

#include "BattleArmy.h"

BattleArmy* BattleArmy::create(CCSpriteBatchNode* batchNode, CCSpriteBatchNode* soldierNode,CCPoint pt,int type,int side,int key){
    BattleArmy* ret = new BattleArmy(batchNode, soldierNode,pt,type,side,key);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool BattleArmy::init(){
    int rowWidth = 50;//不同行x偏移
    int colHeight = 40;//不同行y偏移
    int perX = 40;//同一行，每一个兵的x偏移
    int perY = 10;//同一行，每一个兵的y偏移
    m_row = 1;
    m_col = 1;
    m_soldiers = CCArray::create();
    
    float scale = 0.5;
    switch (m_type) {
        case 0:
            m_row = 6;
            m_col = 6;
            rowWidth = 16;
            colHeight = 8;
            perX = 16;
            perY = 8;
            m_icon = "a010";
            m_direct = "NW";
            break;
        case 1:
            m_row =  10;
            m_col = 1;
            rowWidth = 20;
            colHeight = 10;
            perX = 20;
            perY = 10;
            m_icon = "a030";
            m_direct = "SE";
            break;
        case 2:
            rowWidth = 20;
            colHeight = 10;
            perX = 30;
            perY = 15;
            m_row = 3;
            m_col = 3;
            m_icon = "a020";
            m_direct = "NW";
            break;
        case 3:
            scale = 1;
            m_row = 1;
            m_col = 1;
            rowWidth = 63;
            colHeight = 40;
            m_icon = "zhanche";
            m_direct = "NW";
            break;
        case 7:
            scale = 1;
            m_row = 1;
            m_col = 1;
            rowWidth = 63;
            colHeight = 40;
            m_icon = "zhanche";
            m_direct = "SE";
            break;
        case 4:
            rowWidth = 16;
            colHeight = 8;
            perX = 16;
            perY = 8;
            m_row = 6;
            m_col = 6;
            m_icon = "a060";
            m_direct = "NW";
            break;
        case 5:
            m_row = 1;
            m_col = 1;
            scale = 1.2;
            m_icon = "a020";
            m_direct = "NW";
            break;
        case 6:
            scale = 0.7;
            m_row = 1;
            m_col = 1;
            m_icon = "a020";
            m_direct = "SE";
            break;
        default:
            m_icon = "a020";
            m_direct = "NW";
            break;
    }
    
    for(int i=0;i<m_row;i++){
        for (int j=0; j<m_col; j++) {
            int rdx = 0;
            int rdy = 0;
            if(m_key == 99) {
                rdx = CCMathUtils::getRandomInt(-20, 20);
                rdy = CCMathUtils::getRandomInt(-10, 10);
            }
            auto soldier = BattleSoldier2::create(m_soldierNode, m_jianNode,m_type,m_side,m_icon,m_direct,m_type==5);
            soldier->setSprScale(scale);
            soldier->setSoldierPosition(m_pos+ccp(j*perX-i*rowWidth,-i*colHeight-j*perY)+ccp(rdx, rdy));
            soldier->setAnchorPoint(ccp(0.5, 0.5));
            this->addChild(soldier);
            soldier->playAnimation(ACTION_STAND);
            m_soldiers->addObject(soldier);
        }
    }
    return true;
}

int BattleArmy::getType()
{
    return m_type;
}

void BattleArmy::changeDirect(std::string direct,bool replay){
    m_direct = direct;
    int num = m_soldiers->count();
    for (int i=0; i<num; i++) {
        auto soldier = dynamic_cast<BattleSoldier2*>(m_soldiers->objectAtIndex(i));
        if(soldier){
            soldier->changeDirect(m_direct,replay);
        }
    }
}

void BattleArmy::playAnimation(ActionStatus status,float delayTime,CCCallFunc* completeFunc,int loopTimes, int moveType)
{
    if (status==ACTION_ATTACK && moveType ==0) {
        int num = m_soldiers->count();
        for (int i=0; i<num; i++) {
            auto soldier = dynamic_cast<BattleSoldier2*>(m_soldiers->objectAtIndex(i));
            if(soldier){
                float delayTime = 0;
                if (status==ACTION_ATTACK) {//&& (m_type==1||m_type==3||m_type==4)
                    delayTime = CCMathUtils::getRandom(0.1,1.2);
                }
                soldier->playAnimation(status,delayTime);
            }
        }
    }
    
    if (status==ACTION_MOVE && moveType ==0) {
        int num = m_soldiers->count();
        for (int i=0; i<num; i++) {
            auto soldier = dynamic_cast<BattleSoldier2*>(m_soldiers->objectAtIndex(i));
            if(soldier){
                vector<cocos2d::CCPoint> posV;
                
//                posV.push_back(soldier->getSoldierPosition()+ccp(-240,120));
                
                int dx = CCMathUtils::getRandomInt(-50, 50);
                int dy = CCMathUtils::getRandomInt(-40, 40);
                auto pt1 = ccp(2120,730)+ccp(dx, dy);
                posV.push_back(pt1);
                posV.push_back(pt1+ccp(-300, 150));
                
                int dd = CCMathUtils::getRandomInt(-10, 5);
                soldier->moveToPosition(posV, 0, 45+dd);
            }
        }
    }
    
    if (status==ACTION_MOVE && moveType ==1) {
        int num = m_soldiers->count();
        for (int i=0; i<num; i++) {
            auto soldier = dynamic_cast<BattleSoldier2*>(m_soldiers->objectAtIndex(i));
            if(soldier){
                vector<cocos2d::CCPoint> posV;
                posV.push_back(soldier->getSoldierPosition()+ccp(200,-100));
                soldier->moveToPosition(posV);
            }
        }
    }
    
    if (status==ACTION_ATTACK && moveType ==1) {
        int num = m_soldiers->count();
        for (int i=0; i<num; i++) {
            auto soldier = dynamic_cast<BattleSoldier2*>(m_soldiers->objectAtIndex(i));
            if(soldier){
                soldier->playAnimation(status,0,completeFunc,1);
            }
        }
    }
    
    this->unschedule(schedule_selector(BattleArmy::handeDieSoldierHandle));
    if (m_type == 1 || m_type == 4 || m_type == 0) {
        if(m_type == 1) {
            this->schedule(schedule_selector(BattleArmy::handeDieSoldierHandle), 1);
        }
        else {
            this->schedule(schedule_selector(BattleArmy::handeDieSoldierHandle), 0.5);
        }
    }
}

void BattleArmy::hideSoldier(float dt)
{
    this->unschedule(schedule_selector(BattleArmy::handeDieSoldierHandle));
    int num = m_soldiers->count();
    for (int i=0; i<num; i++) {
        auto soldier = dynamic_cast<BattleSoldier2*>(m_soldiers->objectAtIndex(i));
        if(soldier){
            soldier->removeFromBatchNode(dt);
        }
    }
}

void BattleArmy::destory(){
    this->unschedule(schedule_selector(BattleArmy::handeDieSoldierHandle));
    int num = m_soldiers->count();
    for (int i=0; i<num; i++) {
        auto soldier = dynamic_cast<BattleSoldier2*>(m_soldiers->objectAtIndex(i));
        if(soldier){
            soldier->removeFromParentAndCleanup(true);
        }
    }
    m_soldiers->removeAllObjects();
}

void BattleArmy::handeDieSoldierHandle(float _time){
    int index = CCMathUtils::getRandom(0,m_soldiers->count()-1);
    if(index>=0 && m_soldiers->count()>0){
        auto soldier = dynamic_cast<BattleSoldier2*>(m_soldiers->objectAtIndex(index));
        soldier->playDie();
        m_soldiers->removeObjectAtIndex(index);
    }
}
