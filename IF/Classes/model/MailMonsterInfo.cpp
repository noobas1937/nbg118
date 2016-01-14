//
//  MailMonsterInfo.cpp
//  IF
//
//  Created by lifangkai on 14-11-3.
//
//

#include "MailMonsterInfo.h"
#include "WorldController.h"

MailMonsterInfo* MailMonsterInfo::create(){
    MailMonsterInfo* pret = new MailMonsterInfo();
    if(pret){
        pret->init();
        pret->autorelease();
    }else{
        CC_SAFE_DELETE(pret);
    }
    return pret;
}
//
void MailMonsterInfo::parse(CCDictionary *dict){
    uid = dict->valueForKey("uid")->getCString();
    type = dict->valueForKey("type")->intValue();
    createTime =  dict->valueForKey("createTime")->doubleValue()/1000;

    auto monster = _dict(dict->objectForKey("def"));
    auto self = _dict(dict->objectForKey("att"));
    monsterTotalHp = monster->valueForKey("mmhp")->intValue();
    monsterCurrentHp = monster->valueForKey("mchp")->intValue();
    monsterId = monster->valueForKey("id")->getCString();
    exp = self->valueForKey("exp")->intValue();
    hurt = self->valueForKey("hurt")->intValue();
    survived = self->valueForKey("survived")->intValue();
    dead = self->valueForKey("dead")->intValue();
    total = self->valueForKey("total")->intValue();
    monsterDead = monster->valueForKey("dead")->intValue();
    int index = dict->valueForKey("xy")->intValue();
    pointId = index;
    firstKill = false;
    if(dict->objectForKey("firstKill") && dict->valueForKey("firstKill")->intValue() ==1){
        firstKill = true;
    }
    auto pt = WorldController::getPointByIndex(index);
    selfX = pt.x;
    selfY = pt.y;
    
    monsterX = pt.x;
    monsterY = pt.y;
    selfPowerLost = self->valueForKey("powerLost")->intValue();
    monsterPowerLost = monster->valueForKey("powerLost")->intValue();
    
    if(dict->objectForKey("reportUid")){
        reportUid = dict->valueForKey("reportUid")->getCString();
    }
    
    if(dict->objectForKey("rateReward")){
        normalReward = dynamic_cast<CCArray*>(dict->objectForKey("rateReward"));
    }
    if(!normalReward){
        normalReward = CCArray::create();
    }
    normalReward->retain();
    if(exp > 0){
        CCDictionary *dict = CCDictionary::create();
        dict->setObject(CCString::create(CC_ITOA(R_EXP)), "type");
        dict->setObject(CCString::create(CC_ITOA(exp)), "value");
        normalReward->addObject(dict);
//        CCDictionary *dict1 = CCDictionary::create();
//        dict1->setObject(CCString::create(CC_ITOA(R_EXP)), "type");
//        dict1->setObject(CCString::create(CC_ITOA(exp)), "value");
//        normalReward->addObject(dict1);
//        CCDictionary *dict2 = CCDictionary::create();
//        dict2->setObject(CCString::create(CC_ITOA(R_EXP)), "type");
//        dict2->setObject(CCString::create(CC_ITOA(exp)), "value");
//        normalReward->addObject(dict2);
//        CCDictionary *dict3 = CCDictionary::create();
//        dict3->setObject(CCString::create(CC_ITOA(R_EXP)), "type");
//        dict3->setObject(CCString::create(CC_ITOA(exp)), "value");
//        normalRieward->addObject(dict3);
    }
    if(dict->objectForKey("stat")){
        monsterResult = 4; //怪物消失
    }else{
        if(firstKill){
            monsterResult = 1;//首次杀
        }else if(monsterCurrentHp>0){//失败
            monsterResult = 2;//
        }else{
            monsterResult = 3;//
        }
    }
}

MailMonsterInfo::~MailMonsterInfo(){
    if(normalReward){
        normalReward->removeAllObjects();
        CC_SAFE_RELEASE_NULL(normalReward);
    }
}