//
//  PushInit.h
//  IF
//
//  Created by 邹 程 on 13-9-13.
//
//

#ifndef __IF__PushInit__
#define __IF__PushInit__

#include "cocos2d.h"
/// 初始化数据下发
class PushInit {
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};

class DataParse : public cocos2d::CCObject{
public:
    void parseObject(cocos2d::CCDictionary *dict);
    ~DataParse(){
        CCLOG("release");
    }
    void update(float time);
private:
    cocos2d::CCDictionary *m_dict;
    int startIndex;
    
    void killSelf(Ref *obj = NULL);
};

class PushUserOff {
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};

class PushUserGmOff {
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};

class PushUserKick {
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};

class PushInitError{
public:
    static void handleResponse(cocos2d::CCDictionary *dict);
};
#endif /* defined(__IF__PushInit__) */
