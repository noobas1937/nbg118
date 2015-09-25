//
//  TrainInfo.h
//  IF
//
//  Created by ganxiaohua on 14-3-17.
//
//

#ifndef __IF__TrainInfo__
#define __IF__TrainInfo__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;


class TrainInfo:public cocos2d::CCObject{
    
public:
    TrainInfo(CCDictionary* dict);

    TrainInfo():m_trainId(string())
    ,m_genUid(string())
    ,m_finish(0)
    ,m_end(-1.0)
    ,m_name(string())
    ,m_cost(-1)
    ,m_icon(string())
    ,m_rateExp(0)
    ,m_open(false)
    {}
    
    void updateTrainInfo(CCDictionary* dict);
    std::string m_trainId;//训练位的ID
    std::string m_genUid;//正在训练将军UID
    double m_finish;//训练结束时间(默认是0)
    double m_end;//训练位的使用结束时间(默认0)
    
    std::string m_name;
    int m_cost;
    std::string m_icon;
    int m_rateExp;
    int m_totalTime;
    int m_spending;
    int m_suddenly;
    double m_entryTime;
    bool m_open;
};


#endif /* defined(__IF__TrainInfo__) */
