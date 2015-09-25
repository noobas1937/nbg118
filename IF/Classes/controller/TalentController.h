//
//  TalentController.h
//  IF
//
//  Created by 李锐奇 on 14-3-20.
//
//

#ifndef __IF__TalentController__
#define __IF__TalentController__

#define TALENT_STATUS_CHANGE "talentStatusChange"

#include "CommonInclude.h"

class TalentController : public CCObject{
public:
    TalentController();
    ~TalentController();
    static TalentController *getInstance();
    void initController(CCDictionary *dict);
    
    void makeEndowment();
    void updateTalentLevel(int talentId);
    
    void saveTalentData();
    void reSetTalentData(string toolUuid);
    
    TalentInfo *getInfoById(int talentId);
    std::string getEffectStrByLevel(TalentInfo *info, int level);
    
    void openTalentPanel();
    
    map<int, TalentInfo> talentMap;
    int curEndowment;
    int sumEndowment;
};

#endif /* defined(__IF__TalentController__) */
