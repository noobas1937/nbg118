//
//  DragonCommand.h
//  IF
//
//  Created by 张军 on 15/6/8.
//
//
/*
 龙系统相关
 服务器需要配合做的接口：
 
 1.新建龙建筑
 2.升级龙建筑
 3.收获龙资源（农场.栖息地）
 4.买龙蛋
 5.合成龙蛋
 6.收获龙蛋（放置龙蛋）
 7.孵化龙
 8.收获龙（放置龙）
 9.卖龙/蛋
 10.喂龙
 11.加速（建造.升级.合成孵化）
 
 还可能需要：
 开启地块
 销毁龙建筑
 
 bool birthDragon(int f_attribute, int m_attribute);
 //孵化龙
 bool incubationDragon(int dragonId);
 
 另外：
 进入游戏的时候需要服务器把龙和地块相关信息加入到推送中
 */

#ifndef __IF__DragonCommand__
#define __IF__DragonCommand__

#include "CommandBase.h"
#define DRAGON_CMD_FEED "dragon.feed"   //喂龙
#define DRAGON_CMD_SELL "dragon.sell"   //卖龙

//喂龙
class DragonFeedCommand: public CommandBase {
public:
    DragonFeedCommand(string uuid, string cmd):
    CommandBase(cmd){
        putParam("uuid", CCString::create(uuid));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

//卖龙
class DragonSellCommand: public CommandBase {
public:
    DragonSellCommand(string uuid, string cmd):
    CommandBase(cmd){
        putParam("uuid", CCString::create(uuid));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#endif /* defined(__IF__DragonCommand__) */
