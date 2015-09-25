//
//  GMCommand.h
//  IF
//
//  Created by 李锐奇 on 15/3/30.
//
//

#ifndef __IF__GMCommand__
#define __IF__GMCommand__

#include "CommandBase.h"

#define GM_CHANGE_SERVER_COMMAND "gm.ch.sv"

class GMCommand : public CommandBase{
public:
    GMCommand(std::string ip, std::string zone, int port, std::string gameUid, int serverId) :
    CommandBase(GM_CHANGE_SERVER_COMMAND)
    , info_ip(ip)
    , info_zone(zone)
    , info_port(port)
    , info_gameUid(gameUid)
    {
        putParam("server", CCInteger::create(serverId));
    };
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
private:
    std::string info_ip;
    std::string info_zone;
    int info_port;
    std::string info_gameUid;
};
#endif /* defined(__IF__GMCommand__) */
