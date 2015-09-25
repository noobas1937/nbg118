//
//  AllianceEventInfo.h
//  IF
//
//  Created by ganxiaohua on 14-9-24.
//
//

#ifndef __IF__AllianceEventInfo__
#define __IF__AllianceEventInfo__

#include "CommonInclude.h"

class AllianceEventInfo : public CCObject{
public:
    CREATE_FUNC(AllianceEventInfo);
    void parse(CCDictionary *dict);
    AllianceEventInfo():id(0),type(0),content(string()),isTime(false),posX(0),posY(0){};
    ~AllianceEventInfo(){};
    time_t createTime;
    int id;
    int type;//事件的类型占领为0，掠夺为1，科技为2，加入联盟为3，退出为4 公告为5 取代盟主为6  怪物攻城防守胜利7 怪物攻城防守失败8
    int posX;
    int posY;
    std::string content;
    bool isTime;
    std::string getContent();
    std::string srcTime;//原来没有转过的时间，要查找时，要把最后一条传给服务器的
    CC_SYNTHESIZE_SAFEOBJECT(CCArray, m_params, Params);
private:
    virtual bool init(){return true;};
};
#endif /* defined(__IF__AllianceEventInfo__) */
