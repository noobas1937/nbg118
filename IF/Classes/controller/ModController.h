//
//  ModController.h
//  IF
//
//  Created by 李锐奇 on 14/11/20.
//
//

#ifndef __IF__ModController__
#define __IF__ModController__

#include "CommonInclude.h"

class ModController{
public:
    static ModController *getInstance();
    void parseData(CCDictionary *dict);
    void purgeData();
    
    double nextRewardTime;
    int rewardGold;
    int m_currentSelectServer;
};

#endif /* defined(__IF__ModController__) */
