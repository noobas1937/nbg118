//
//  OfficeInfo.h
//  IF
//
//  Created by ganxiaohua on 14-12-11.
//
//

#ifndef __IF__OfficeInfo__
#define __IF__OfficeInfo__
#include "CommonInclude.h"

class OfficeInfo : public CCObject{
public:
    CREATE_FUNC(OfficeInfo);
    void parse(CCDictionary *dict);
    OfficeInfo():id(0),name(string()),uid(string()),playerName(string()),playerIcon(string()),chNameFlag(false),time(0.0),allianceName(string()),kingdomName(string()),m_chBanner(false),picVer(0),appointTime(0){};
    ~OfficeInfo(){};
    string getPlayerIcon();
    int id;
    std::string name;
    std::vector<std::string> tips;
    std::vector<std::string> params;
    std::string icon;
    std::string uid;
    std::string playerName;
    std::string playerIcon;
    std::string allianceName;
    std::string kingdomName;
    std::string abbr;//联盟简称
    int picVer;
    bool chNameFlag;//1 改过，0没有改过
    bool m_chBanner;//1 改过，0没有改过
    double time;//修改时间
    double appointTime;//修改时间
    bool isServant;
private:
    virtual bool init(){return true;};
};
#endif /* defined(__IF__OfficeInfo__) */
