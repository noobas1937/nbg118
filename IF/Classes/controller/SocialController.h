//
//  SocialController.h
//  IF
//
//  Created by ganxiaohua on 14-10-22.
//
//

#ifndef __IF__SocialController__
#define __IF__SocialController__

#include "CommonInclude.h"

class SocialController : public CCObject{
public:
    static SocialController* getInstance();
    
    void parse(CCDictionary* dic);
    void parseCollect(CCDictionary* dic);
    int id;
    int exp;
    int miner;//矿工数
    double helpTime;//下一次帮助的时间
    double mineTime;//下一次挖矿的时间
    int goldReward;//0 没有领过，1领过
    int xml_lv;
    int xml_exp;
    int xml_gold;
    int xml_miner;
    double xml_weektime;
    int call;//能否呼叫
};
#endif /* defined(__IF__SocialController__) */
