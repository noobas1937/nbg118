//
//  TitanController.h
//  IF
//
//  Created by fusheng on 15-9-21.
//
//

#ifndef __IF__TitanController__
#define __IF__TitanController__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "TypeDefinition.h"
//enum TITAN_ERROR_CODE
//{
//    TITAN_NO_ENOUGH_TILI = 14000,//泰坦出征体力不足
//    TITAN_ALL_READLY_GO = 14001,//泰坦已经出征
//    TITAN_EXP_FULL = 14002,//泰坦经验已满
//    TITAN_NO_FEED_NUM = 14003,//泰坦没有喂食次数
//    TITAN_NEED_WAIT = 14004,//泰坦需要等待CD
//    TITAN_NOT_ENOUGH_FOOD = 14005,//没有足够的食物
//};
USING_NS_CC;
USING_NS_CC_EXT;

class TitanController : public Node{
public:
    static TitanController* getInstance();
    void TitanComplete(CCObject *obj);
    
    void handleErrorCode(int errorCode);

    TitanController();
    ~TitanController();
    
    void parse(CCDictionary* dict);
    
    void update(float dt);
    
    bool checkCanFeedFree();
    
    bool isUpdate;
private:
    
    bool isCanFeedFree;
    
    
};

#endif /* defined(__IF__TitanController__) */
