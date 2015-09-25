//
//  FBFriendInfo.h
//  IF
//
//  Created by ganxiaohua on 14-10-17.
//
//

#ifndef __IF__FBFriendInfo__
#define __IF__FBFriendInfo__

#include "CommonInclude.h"

class FBFriendInfo : public CCObject{
public:
    CREATE_FUNC(FBFriendInfo);
    FBFriendInfo():id(string()),name(string()),gUid(string()),server(0),help(0),url(string()),isCheck(false),add(false){};
    ~FBFriendInfo(){};
    
    std::string id;
    std::string name;
    std::string gUid;//游戏uid
    int server;//所在服
    int help;//是否可点 1 可点  0不可点
    std::string url;
    bool isCheck;
    bool add;

private:
    virtual bool init(){return true;};
};
#endif /* defined(__IF__FBFriendInfo__) */
