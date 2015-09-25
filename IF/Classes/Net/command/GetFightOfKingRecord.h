//
//  GetFightOfKingRecord.h
//  IF
//
//  Created by ganxiaohua on 15/1/29.
//
//

#ifndef __IF__GetFightOfKingRecord__
#define __IF__GetFightOfKingRecord__

#include "CommandBase.h"
#define GET_FIGHT_OF_KING "Get.FightOfKing.Record"

class GetFightOfKingRecord: public CommandBase {
public:
    GetFightOfKingRecord(int serverId,int page) : CommandBase(GET_FIGHT_OF_KING){
        putParam("page", CCInteger::create(page));
        putParam("serverId", CCInteger::create(serverId));
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
    
private:
};
#endif /* defined(__IF__GetFightOfKingRecord__) */
