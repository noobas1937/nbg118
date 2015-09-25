//
//  InviteTeleportCommand.h
//  IF
//
//  Created by xxrdsg on 15-2-28.
//
//

#ifndef __IF__InviteTeleportCommand__
#define __IF__InviteTeleportCommand__

#include "CommandBase.h"

#define AL_INVITE_TELEPORT "al.invite.move"
#define AL_REFUSE_TELEPORT "al.invite.move.refuse"
#define AL_NEAREST_POING_GET "point.nearest.find"

class InviteTeleportCommand: public CommandBase
{
public:
    InviteTeleportCommand(string uid, string allianceId,int cityIndex):CommandBase(AL_INVITE_TELEPORT)
    {
        putParam("targetUid", CCString::create(uid));
        putParam("allianceId", CCString::create(allianceId));
        if (cityIndex >= 0) {
            putParam("targetPoint", CCInteger::create(cityIndex));
        }
    }
private:
    virtual bool handleRecieve(cocos2d::CCDictionary* dict);
};

class RefuseTeleportCommand: public CommandBase
{
public:
    RefuseTeleportCommand(string mailUid): CommandBase(AL_REFUSE_TELEPORT)
    {
        putParam("mailId", CCString::create(mailUid));
    }
private:
    virtual bool handleRecieve(cocos2d::CCDictionary* dict);
};

class GetNearestPointCommand: public CommandBase
{
public:
    GetNearestPointCommand(int point, string mailuid): CommandBase(AL_NEAREST_POING_GET)
    , m_mailUid(mailuid)
    {
        putParam("point", CCInteger::create(point));
    }
private:
    string m_mailUid;
    virtual bool handleRecieve(CCDictionary* dict);
};

#endif /* defined(__IF__InviteTeleportCommand__) */
