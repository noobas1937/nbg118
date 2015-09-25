//
//  fireandcomman.h
//  IF
//
//  Created by jiaohongye on 14-2-24.
//
//

#ifndef __IF__fireandcomman__
#define __IF__fireandcomman__

#include "CommandBase.h"
#define FIRE "army.fire"
#define FORT_FIRE "fort.destory"
#define ARMY_NUM_CHANGE "armyNumChange"
#define GO_TO_HEALTHING "go.to.healthing"
#define HOSPITAL_FIRE "hospital.fire"

class fireandcomman: public CommandBase {
public:
    fireandcomman(std::string armyId, int num,std::string command=FIRE):
    CommandBase(command)
    {
        putParam("id", CCString::create(armyId));
        putParam("num", CCInteger::create(num));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};
#endif /* defined(__IF__WorldCommand__) */