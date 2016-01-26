//
//  IFAllianceHelpReportInfo.hpp
//  IF
//
//  Created by wangdianzhen on 15/10/21.
//
//

#ifndef IFAllianceHelpReportInfo_hpp
#define IFAllianceHelpReportInfo_hpp

#include "cocos2d.h"

using namespace std;
class IFAllianceHelpReportInfo: public cocos2d::CCObject
{
public:
    CC_SYNTHESIZE(string, m_name, name);
    CC_SYNTHESIZE(int, m_count, count);
    CC_SYNTHESIZE(time_t, m_time, time);
    CC_SYNTHESIZE(string, m_uid, uid);
    CC_SYNTHESIZE(string, m_pic, pic);
    CC_SYNTHESIZE(int, m_picVer, picVer);
    
    void parseData(cocos2d::CCDictionary * data);
};

#endif /* IFAllianceHelpReportInfo_hpp */
