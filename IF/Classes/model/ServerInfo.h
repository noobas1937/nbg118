//
//  ServerInfo.h
//  IF
//
//  Created by wangzhenlei on 13-9-27.
//
//

#ifndef __IF__ServerInfo__
#define __IF__ServerInfo__
#include "cocos2d.h"

USING_NS_CC;
using namespace std;
class ServerInfo{
public:
    ServerInfo(CCDictionary* dict);
    ServerInfo():
    id (-1),
    name (""),
    bRecommend(false),
    bNew (false),
    zone (""),
    ip (""),
    port(8088),
    platformId(""),
    nickName(""),
    level(-1),
    gameUid("")
    {
    }
    int id;
    string name;
    bool bRecommend;
    bool bNew;
    string zone;
    string ip;
    int port;
    string platformId;
    string nickName;
    int level;
    string gameUid;
};

#endif /* defined(__IF__ServerInfo__) */
