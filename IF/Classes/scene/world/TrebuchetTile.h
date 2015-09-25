//
//  TrebuchetTile.h
//  IF
//
//  Created by 李锐奇 on 14/12/4.
//
//

#ifndef __IF__TrebuchetTile__
#define __IF__TrebuchetTile__

#include "NewBaseTileInfo.h"
#include "ThroneTile.h"

class TrebuchetTile : public ThroneTile {
public:
    
    TrebuchetTile(WorldCityInfo& info):
    ThroneTile(info)
    {}
    
    static TrebuchetTile* create(WorldCityInfo& info) {
        auto ret = new TrebuchetTile(info);
        if (ret->init()) {
            ret->autorelease();
        } else {
            CC_SAFE_DELETE(ret);
        }
        return ret;
    }
    
protected:
    virtual bool init();
    virtual std::string getShowText();
    virtual int getOwnerFlag();
    virtual void setPeaceTimeButtons();
    virtual void setNotOpenTimeButtons();
    virtual void setDefenderWarTimeButtons();
    virtual void setAttackerWarTimeButtons();
    virtual bool isSelfBuild();
};

#endif /* defined(__IF__TrebuchetTile__) */
