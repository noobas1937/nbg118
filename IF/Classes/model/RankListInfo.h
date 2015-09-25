//
//  RankListInfo.h
//  IF
//
//  Created by lifangkai on 14-9-16.
//
//

#ifndef __IF__RankListInfo__
#define __IF__RankListInfo__

#include "cocos2d.h"
#include "AllianceRankInfo.h"
#include "PlayerRankInfo.h"
USING_NS_CC;
class RankListInfo{
public:
    RankListInfo():playerRank(0),allianceRank(0){};
    ~RankListInfo(){};
   //void update(CCDictionary* dict);
    
    int playerRank; //个人排名
    int allianceRank;//个人联盟排名
    
    std::vector<AllianceRankInfo*> allianceRankList;
    std::vector<PlayerRankInfo*> playerRankList;
    
};

#endif /* defined(__IF__RankListInfo__) */
