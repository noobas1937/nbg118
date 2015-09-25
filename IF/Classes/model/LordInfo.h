//
//  LordInfo.h
//  IF
//
//  Created by 李锐奇 on 13-11-1.
//
//

#ifndef __IF__LordInfo__
#define __IF__LordInfo__

#include "cocos2d.h"

class LordInfo{
public:
    LordInfo():
    inventoryNum(0)
    ,orgInventoryNum(20)
    ,goldPerBlank(0)
    ,recruitedGeneralCount(0)
    ,energy(0)
    ,nextEnergyRecoverTime(0)
    ,buyEnergyTimesDaily(0)
    ,physical_k1(0)
    ,physical_k2(0)
    ,physical_k3(0)
    ,physical_k4(0)
    ,feed_max(3)
    ,feed_count(0)
    {};
    int inventoryNum;//仓库位
    int orgInventoryNum;//初始仓库位
    int goldPerBlank;//开仓库位的金币数
    int recruitedGeneralCount;//武将可招募数量
    
    int energy;//能量点
    double nextEnergyRecoverTime;//恢复时间点
    int buyEnergyTimesDaily;//当天已经购买的次数
    int physical_k1;//能量点上限
    int physical_k2;//能量点自动回复时间（秒）
    int physical_k3;//当天购买次数上限
    int physical_k4;//每次购买能量数量
    std::map<std::string, int> physical_bug;//每次购买所花费的金币 键值 “k”+次数
    
    int feed_max;//feed每日发送上线
    int feed_count;//feed当日发送次数
    int levelUp;//升级领取到的级别
};

#endif /* defined(__IF__LordInfo__) */
