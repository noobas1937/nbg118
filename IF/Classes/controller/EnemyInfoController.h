//
//  EnemyInfoController.h
//  IF
//
//  Created by fubin on 14-2-24.
//
//

#ifndef IF_EnemyInfoController_h
#define IF_EnemyInfoController_h

#define ENEMY_INFO_CHANGE "enemy_info_change"
#define ENEMY_INFO_LIST_CHANGE "enemy_info_list_change"
#define ENEMY_TYPE_BATTLE 0    //攻击
#define ENEMY_TYPE_SCOUT 1     //侦查
#define ENEMY_TYPE_MONSTER 2
#define ENEMY_TYPE_TRADE 3     //交易
#define ENEMY_TYPE_TEAM 4      //组队战斗
#define ENEMY_TYPE_MEMBER 5    //组队防御

#define ENEMY_TYPE_MONSTER_ATTACK 8    //世界活动，怪物攻城

#include "CommonInclude.h"
#include "EnemyInfo.h"

class EnemyInfoController : public CCObject{
public:
    static EnemyInfoController* getInstance();
    static void purgeData();
    EnemyInfoController();
    ~EnemyInfoController();
    void clearAllEnemy();
    
    //初始化信息
    void initEnemyData(CCDictionary* dict);
    void pushEnemyData(CCDictionary* dict);
    void checkEnemyData();
    void onEnterFrame(float dt);
    bool checkMonsterWar();
    
    std::vector<EnemyInfo> m_enemyInfos;
    std::vector<EnemyInfo> m_ignorInfos;
    std::vector<EnemyInfo> m_enemyInSameAlliance;
    int m_curType;
private:
};

#endif
