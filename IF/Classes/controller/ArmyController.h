//
//  ArmyController.h
//  IF
//
//  Created by 李锐奇 on 14-3-5.
//
//

#ifndef __IF__ArmyController__
#define __IF__ArmyController__

#include "CommonInclude.h"

enum ArmType{
    ARMY,
    FORT,
    TREAT_ARMY,
};

class ArmyController : public CCObject{
    
public:
    static ArmyController* getInstance();
    ArmyController() :
    treatFinishTime(0){};
    
    int getMaxNumByType(int type, bool isOri=false);
    
    int getTrainTime(int num, ArmyInfo &info);
    int getTrainSilver(int num, ArmyInfo &info);
    int getTrainWood(int num, ArmyInfo &info);
    int getTrainFood(int num, ArmyInfo &info);
    int getTrainIron(int num, ArmyInfo &info);
    int getTrainStone(int num, ArmyInfo &info);
    
    int getTreatTime(int num, TreatInfo &info);
    int getTreatSilver(int num, TreatInfo &info);
    int getTreatWood(int num, TreatInfo &info);
    int getTreatFood(int num, TreatInfo &info);
    int getTreatIron(int num, TreatInfo &info);
    int getTreatStone(int num, TreatInfo &info);
    int getTreatSolidierMaxNum();
    
    int getTotalArmy();
    int getTotalFree();
    int getTotalTreat();
    int getTotalDead();
    
    int getTotalFreeByType(int type);
    void refreshFortFreeNum(CCDictionary *dict);
    void refreshArmyFreeNum(CCDictionary *dict,bool refresh = true);
    void refreshTreatDeadNum(CCDictionary *dict);
    void refreshTreat(CCDictionary *dict);
    
    void setTreatFinishTime(time_t time);
    void setTreatEndTime(double time);
    void cancelTreate();
    
    int getTotalUpKeep();
    int getUpKeep(ArmyInfo *info);
    int getUpKeep(ArmyInfo *info, int num);
    
    float getLoad(ArmyInfo *info, bool isIncludeGeneralEffect = true, int num = 1);
    
    void startFinishForce(std::string armyId, std::string uid,int buildingKey);
    void startFinishFort(std::string armyId, std::string uid);
    void startFinishHospital(std::string uid);
    
    vector<int> getSoldierSqerNum();
    int getTotalFortNum();
    time_t treatFinishTime;
    bool costCD(int itemId, string toolUUID);
    
    vector<string> getCreateSoldierIds(string sildiers, bool isFort);
};
#endif /* defined(__IF__ArmyController__) */
