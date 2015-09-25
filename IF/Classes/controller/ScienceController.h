//
//  ScienceController.h
//  IF
//
//  Created by fubin on 14-3-19.
//
//

#ifndef IF_ScienceController_h
#define IF_ScienceController_h

#define MSG_SCIENCE_UPDATE "msg_science_update"
#define MSG_SCIENCE_CELL_STATE "msg_science_cell_state"
#define MSG_SCIENCE_DONATE "msg_science_donate"
#define MSG_AL_SCIENCE_PUSH "msg_al_science_push"
#define MSG_AL_SCIENCE_POINT "msg_al_science_point"
#define MSG_SCIENCE_POWER_ADD "msg_science_power_add"

#include "CommonInclude.h"
#include "ScienceInfo.h"

class ScienceController : public CCObject{
public:
    static ScienceController* getInstance();
    static void purgeData();
    ScienceController();
    ~ScienceController();
    
    bool costCD(int itemId, string toolUUID);
    
    bool startResearchScience(int scienceid, int gold=0);
    void retResearchScience(CCDictionary* dict);
    
    bool startDirResearchScience(int scienceid);
    void retDirResearchScience(CCDictionary* dict);
    
    bool startFinishScience(int scienceid, string qUUid);
    void retFinishScience(CCDictionary* dict);
    int getScienceLevel(int scienceId);
    CCSafeObject<CCCallFunc> m_callBackFunc;
    
    std::vector<int> m_scienceType;
    
    void initAllianceScienceData(CCArray* array);
    int getTotalAllScPoints();
    
    bool startResearchAllSc(int sid);
    void endResearchAccSc(CCDictionary* dict);
    
    bool startDonate(int sid, int type);
    void endDonate(CCDictionary* dict);
    
    bool startDonateRefresh(int sid);
    void endDonateRefresh(CCDictionary* dict);
    
    void pushUpdateAllInfo(CCDictionary* dict);
    void onQuitAlliance();
    int  findResearchingAlSc();
    
    bool costCDDonate();
    void endCDDonate(CCDictionary* dict);
    
    void updateSciencePower(int power);
    
    map<int, AllianceScienceInfo> allianceScienceMap;
    map<int, vector<int>> aScienceFloorMap;
    map<int, float> alScEffMap;
    
private:
};

#endif
