//
//  VipUtil.h
//  IF
//
//  Created by chenliang on 14-3-20.
//
//

#ifndef __IF__VipUtil__
#define __IF__VipUtil__

#include "VipConstants.h"

class VipUtil
{
public:
    static int getVipLevel(long vipPoints);
    static int getVipItemValueByKey(std::string itemId,std::string key);
    static int getCurVipValueByKey(std::string key);
    
    static bool showVIPActiveView();
};

#endif /* defined(__IF__VipUtil__) */
