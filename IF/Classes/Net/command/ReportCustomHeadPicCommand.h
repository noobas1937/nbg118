//
//  ReportCustomHeadPicCommand.h
//  IF
//
//  Created by xupengzhan on 15-9-1.
//
//

#ifndef __IF__ReportCustomHeadPicCommand__
#define __IF__ReportCustomHeadPicCommand__

#include "CommandBase.h"

#define REPORT_PICVER "report.picVer"

class ReportCustomHeadPicCommand : public CommandBase{
public:
    ReportCustomHeadPicCommand(std::string uid) :
    CommandBase(REPORT_PICVER){
        putParam("uid", CCString::create(uid));
    }
    
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};

#endif /* defined(__IF__ReportCustomHeadPicCommand__) */
