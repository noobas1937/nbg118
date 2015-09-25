//
//  GeneralConfig.h
//  IF
//
//  Created by 李锐奇 on 13-11-27.
//
//

#ifndef __IF__GeneralConfig__
#define __IF__GeneralConfig__

#include "cocos2d.h"

class GeneralConfig{
public:
    GeneralConfig() :
    greenNum(0),
    blueNum(0),
    purpleNum(0),
    orangeNum(0),
    k5(0.0),
    up1(0.0),
    up2(0.0),
    up3(0.0),
    up4(0.0),
    picUploadLv_limit(1),
    picModelTime(0),
    oneTimeCost(0),
    tenTimeCost(0),
    oneTimeFree(0),
    tenTimeFree(0),
    freePerDay(0),
    maxGeneral(0),
    firstRndGenFlag(1),
    bagMaxGeneralNum(16),
    bagMaxGoldBuyNum(3),
    bagFreeNum(3)
    {};
    int greenNum;
    int blueNum;
    int purpleNum;
    int orangeNum;
    
    float k5;
    float picModelTime;
    float up1;
    float up2;
    float up3;
    float up4;
    int picUploadLv_limit;
    int oneTimeCost;
    int tenTimeCost;
    int oneTimeFree;
    int tenTimeFree;
    int freePerDay;
    int maxGeneral;
    int firstRndGenFlag;
    std::map<int, int> formationOpen;
    
    //以下为武将背包
    int bagMaxGeneralNum;
    int bagMaxGoldBuyNum;
    int bagFreeNum;
    std::map<int, int> posPrice;
    
    //灵魂石获取个数
    int soul0;
    int soul1;
    int soul2;
    int soul3;
    
    //武将能力相关
    std::map<int, int> skillPointAddPerLevel;//每级增加的技能点
    std::map<int, int> abilityOpenedLevel;//功能开放等级
    std::map<int, int> generalMaxAbility;//武将品质决定所开功能的总数
    
    int abilityChangeGold;//更换能力
    int abilityResetGold;//遗忘nengli
    int skillResetGold;//重置技能
    
};

#endif /* defined(__IF__GeneralConfig__) */
