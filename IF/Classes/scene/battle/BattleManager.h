//
//  BattleManager.h
//  IF
//
//  Created by ganxiaohua on 13-9-17.
//
//

#ifndef __IF__BattleManager__
#define __IF__BattleManager__

#include "cocos2d.h"
#include "PopupViewController.h"
#include "SceneController.h"
#include "RewardItem.h"
#include "SequenceResult.h"
#include <spine/Json.h>

static CCPoint offPos = ccp(100,10);

class BattleManager : public cocos2d::CCObject {
public:
    ~BattleManager(){};
    BattleManager():m_battleId(0),
    m_roomId(NULL),
    m_battleHeros(NULL),
    m_checkpointId(0),
    m_cityId(0),
    m_lastSence(SCENE_ID_MAIN),
    m_report(std::string())
    {
        
    };
    static BattleManager* shared();
    void requestBattleInfo(int cityId,int checkpointId=0,int cost=0);
    void goToBattle(CCObject* p);
    void parseBattleReport(CCObject* report);
    void mailFightReport(std::string reportUid);
    std::string getWalkDirect(std::string direct);
    
    bool isDuringRequestingBattleInfo;
    int m_battleId;
    int m_checkpointId;
    int m_cityId;
    int m_lastSence;
    std::string m_report;
protected:
    static BattleManager* instance;
    CC_SYNTHESIZE_SAFEOBJECT(cocos2d::CCArray,m_battleHeros,BattleRoles);
    CC_SYNTHESIZE_SAFEOBJECT(CCString, m_roomId, RoomId);
    CC_SYNTHESIZE_SAFEOBJECT(CCArray, m_arrAttackers, ArrAttackers);
    CC_SYNTHESIZE_SAFEOBJECT(CCArray, m_arrDefenders, ArrDefenders);
    
};
#endif /* defined(__IF__BattleManager__) */
