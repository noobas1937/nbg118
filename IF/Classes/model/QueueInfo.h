//
//  QueueInfo.h
//  IF
//
//  Created by fubin on 14-2-19.
//
//

#ifndef IF_QueueInfo_h
#define IF_QueueInfo_h

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

enum QueueType{
    TYPE_BUILDING,  //建筑
    TYPE_FORCE,     //步
    TYPE_FORT,      //城防
    TYPE_HOSPITAL,  //医院
    TYPE_AFFAIRS,   //事务
    TYPE_MARCH,     //出征
    TYPE_SCIENCE,   //科技
    TYPE_ALLIANCE_AFFAIRS, //联盟事件
    TYPE_RIDE_SOLDIER, //骑
    TYPE_BOW_SOLDIER, //弓
    TYPE_CAR_SOLDIER, //车
    TYPE_FORGE,//锻造队列
    TYPE_MATE,//造材料
    TYPE_DRAGON_BUILD,//龙建筑
    TYPE_DRAGON_EGG,//产龙蛋
    TYPE_DRAGON_BRON,//龙孵化
    
    TYPE_OCCUPY_RESOURCE,//占领资源
    TYPE_OCCUPY_MAZE,//占领迷宫
    TYPE_OCCUPY_CAMP,//帐篷
    TYPE_ARMY_MASS,//集结中
    TYPE_OCCUPY_ALLIANCE,//协助防守
    TYPE_BUILDING_TERRITORY,//建造领地
    TYPE_REPAIR_TERRITORY,//修理领地
    TYPE_STATION_TERRITORY,//驻守领地
    TYPE_DESTROY_TERRITORY,//摧毁领地
    TYPE_OCCUPY_TERRITORY_RESOURCE,//占领超级矿
};

class QueueInfo : public cocos2d::CCObject {
public:
    QueueInfo(CCDictionary* dict);
    QueueInfo(const QueueInfo& info);
    QueueInfo():
    qid(0)
    ,type(20)
    ,endTime(0)
    ,finishTime(0)
    ,startTime(0)
    ,key("")
    ,uuid("")
    ,canHelp(false)
    ,para(0)
    ,allId("")
    {};
    
    int qid;//队列id
    int type;//队列类型 建筑 科技 造兵
    int itemId;
    bool canHelp;
    double endTime;//队列有效时间点
    double finishTime;//队列工作完成时间点
    double startTime;//队列工作开始时机
    string key;  // when it is TYPE_MARCH , this is display name
    string uuid; // when it is TYPE_MARCH , this is march id
    
    //材料工坊 建造装备材料
    int para;//当前建造位置
    string allId; //全部材料id
    
    double getTime();
};

#endif
