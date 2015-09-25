//
//  FunBuildInfo.h
//  IF
//
//  Created by fubin on 13-11-1.
//
//

#ifndef IF_FunBuildInfo_h
#define IF_FunBuildInfo_h

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

class FunBuildInfo : public cocos2d::CCObject {
public:
    FunBuildInfo(CCDictionary* dict);
    FunBuildInfo(FunBuildInfo* oldInfo);
    FunBuildInfo():itemId(0)
    ,uuid("")
    ,name("")
    ,time_need(0)
    ,des_time(0)
    ,plevel(0)
    ,description("")
    ,deno("")
    ,introduce("")
    ,information("")
    ,icon("")
    ,pic("")
    ,pos(0)
    ,open(0)
    ,state(1)
    ,severST(0)
    ,isError(0)
    ,level(0)
    ,updateTime(0)
    ,currentEffect("")
    ,type(0)
    ,exp(0)
    ,oldExp(0)
//    ,power(0)
    ,silver_need(0)
    ,wood_need(0)
    ,stone_need(0)
    ,food_need(0)
    ,iron_need(0)
    ,tool_need("")
    ,limitNum(1)
    ,button_txt("")
    ,building("")
    ,starNum(0)
    ,starRate(0)
    ,para1(0)
    ,para2(0)
    ,para3(0)
    ,para4(0)
    ,para5(0)
    ,refreshTime(0)
    ,nextLevelParas("")
    ,open_arms("")
    ,effectTime(0)
    ,is_Dirc(false)
    ,putId("")
    ,dragonGold_need(0)
    {};
    
    int itemId;
    string uuid;
    string name;
    int time_need;
    int des_time;
    int plevel;
    string description;
    string destip;
    string tip;
    string deno;
    string introduce;
    string information;
    string icon;
    string pic;
    int pos;
    int open;
    
    int state;
    int severST;
    int isError;
    int level;
    double updateTime;//秒
    
//    string effect;
    string currentEffect;
//    string nextEffect;
    
    int type;
    int exp;
    int oldExp;
//    int power;
    int silver_need;
    int wood_need;
    int stone_need;
    int food_need;
    int iron_need;
    string tool_need;
    int limitNum;
    string button_txt;
    string building;
    
    int starNum;
    float starRate;
    
    int para1;
    int para2;
    int para3;
    float para4;
    float para5;
    double refreshTime;
    string nextLevelParas;
    map<int, string> para;
    map<int, int> mapPower;
    vector<string> picVec;
    std::string open_arms;
    bool is_Dirc;
    double effectTime;//农田等加速效果结束的时间
    void SetInfoFromXml(CCDictionary* dict);
    void SetInfoFromServer(CCDictionary* dict);
    bool isUnLock();
    
    string getInfomationByLv(int lv);
    
    //龙建筑相关
    string putId;//放了的植物id
    int dragonGold_need;//升级需要的资源
    
protected:
    
};
#endif
