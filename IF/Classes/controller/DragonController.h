//
//  DragonController.h
//  IF
//
//  Created by 张军 on 15/6/5.
//
//

#ifndef __IF__DragonController__
#define __IF__DragonController__

#include "CommonInclude.h"
#include "DragonInfo.h"

class DragonController : public CCObject{
public:
    static DragonController* getInstance();
    DragonController();
    ~DragonController();

    void setDragonDataFromServer(CCDictionary* dict);
    //买龙蛋
    bool buyDragon(int attribute, int gold=0);
    //合成龙蛋
    bool birthDragon(int f_attribute, int m_attribute);
    //孵化龙
    bool incubationDragon(int dragonId);
    //卖龙/蛋
    bool startSellDragon(int dragonId);
    void endSellDragon(CCDictionary* dict);
    //得到龙的信息
    DragonInfo& getDragonById(int dragonId);
    //喂龙
    bool startFeedDragon(int dragonId);
    void endFeedDragon(CCDictionary* dict);
};
#endif /* defined(__IF__DragonController__) */
