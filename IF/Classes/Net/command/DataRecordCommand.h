//
//  DataRecordCommand.h
//  IF
//
//  Created by 李锐奇 on 14-1-10.
//
//

#ifndef __IF__DataRecordCommand__
#define __IF__DataRecordCommand__

#include "CommandBase.h"
#include "CommonInclude.h"

enum OpenPanelType {
    GOLD_PANEL, GOLD_PANEL_SUCCESS, GOLD_BUTTON, GENERAL_OPEN, UPGRADE_OK, UPGRADE_ADD, ADD_TALENT, RECHARGE_ACT, MAIL_OPEN,POINT_ACTIVITY_OPEN,
    FB_SHARE_FEED = 17,
    FB_INVITE_FRIEND = 18
};

enum GameDataType {
    FUNCTION_USE, RANDOM_GENERAL, OPEN_PANEL
};

#define OPEN_PANEL_COMMAND "stat.log"
#define PAY_RECORD_COMMAND "pay.action"

class DataRecordCommand : public CommandBase{
public:
    DataRecordCommand(int type, CCArray* para, CCArray* data) : CommandBase(OPEN_PANEL_COMMAND){
        putParam("type", CCInteger::create(type));
        if(para->count() > 0){
            putParam("param", para);
        }
        if(data->count() > 0){
            putParam("data", data);
        }
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);

};

class PayRecordCommand : public CommandBase{
public:
    static bool isSendRecord;
    PayRecordCommand(std::string exchangeId,std::string platform) : CommandBase(PAY_RECORD_COMMAND){
        putParam("exchangeId", CCString::create(exchangeId));
        putParam("pf" , CCString::create(platform));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
};

#endif /* defined(__IF__DataRecordCommand__) */
