//
//  QueueCommand.h
//  IF
//
//  Created by fubin on 14-2-20.
//
//

#ifndef IF_QueueCommand_h
#define IF_QueueCommand_h

#include "CommandBase.h"
#define QUEUE_OPEN_COMMAND "queue.open"
#define QUEUE_CCD_COMMAND "queue.ccd"
#define QUEUE_FINISH_COMMAND "queue.finish"
#define QUEUE_CANCEL_COMMAND "queue.cancel"
#define QUEUE_RENT_COMMAND "queue.rent"
#define QUEUE_MATE_REFRESH_COMMAND "queue.tool"

class QueueCCDCommand;
class QueueFinishCommand;
class QueueCancelCommand;

class QueueOpenCommand: public CommandBase {
public:
    QueueOpenCommand(string cmd, string uuid):
    CommandBase(cmd){
        putParam("uuid", CCString::create(uuid));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};

class QueueCCDCommand: public CommandBase {
public:
    QueueCCDCommand(string cmd, string qUUID, string itemUUID, int isGold):
    CommandBase(cmd){
        putParam("qUUID", CCString::create(qUUID));
        putParam("itemUUID", CCString::create(itemUUID));
        putParam("isGold", CCInteger::create(isGold));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};

class QueueFinishCommand: public CommandBase {
public:
    QueueFinishCommand(string cmd, string uuid):
    CommandBase(cmd){
        putParam("uuid", CCString::create(uuid));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};

class QueueCancelCommand: public CommandBase {
public:
    QueueCancelCommand(string cmd, string uuid):
    CommandBase(cmd){
        putParam("uuid", CCString::create(uuid));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};

class QueueRentCommand: public CommandBase {
public:
    QueueRentCommand(string qUUID, int time):
    CommandBase(QUEUE_RENT_COMMAND){
        putParam("qUUID", CCString::create(qUUID));
        putParam("rentTime", CCInteger::create(time));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};
//材料队列刷新
class QueueRefreshCommand: public CommandBase {
public:
    QueueRefreshCommand(string cmd):
    CommandBase(cmd){
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
};

#endif
