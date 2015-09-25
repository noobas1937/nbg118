//
//  TroopsController.h
//  IF
//
//  Created by fubin on 14-3-6.
//
//

#ifndef IF_TroopsController_h
#define IF_TroopsController_h

#include "CommonInclude.h"

class TroopsController : public CCObject{
public:
    static TroopsController* getInstance();
    static void purgeData();
    TroopsController();
    ~TroopsController();
    
    //初始化信息
    void initDefenceData(CCDictionary* dict);
    
    //保存城墙防御数据
    bool saveDefence(int autoType, bool isBtn=false);
    void retSaveDefence(CCDictionary* dict);
    void reFreshDefInfo();
    
    //保存城墙自动防御数据
    bool saveAutoDefence(int autoType);
    void retSaveAutoDefence(CCDictionary* dict);
    
    //保存出征数据
    CCDictionary* saveBattle();
    bool isHaveDefHeroInBattle();
    void retSaveBattle();
    
    void pushDefenceData(CCDictionary* dict);
    int updateTmpDefenceData(string key, int num, string soldierId);
    int updateTmpBattleData(string key, int num, string soldierId);
    
    int updateTmpDefenceHeroData(string key, int type);
    
    void makeCurDefenceSoldiersType();
    bool selHero(int num);
    bool isFullHero();
    void makeLoadNum(string soldierId, int num, bool isIncludeGeneralEffect);
    void changeArrTime();
    
    void sortByIsHaveArmy(CCArray* arr, bool reverse = false);
    
    void reSetAutoDefHero();
    void resetGeneralToSend();
    int getMaxSoilder();
    int getMaxSendTime();
    string m_curInfo;
    std::map<string,int> m_defenceInfos;
    std::map<string,int> m_prepareInfos;
    std::map<string,int> m_battleInfos;
    
    std::map<string,int> m_tmpDefenceInfos;
    std::map<string,int> m_tmpPrepareInfos;
    std::map<string,int> m_tmpBattleInfos;
    
    std::map<string,int> m_tmpFreeSoldiers;
    std::map<string,int> m_tmpConfSoldiers;
    
    std::map<string,int> m_tmpAutoHeros;
    std::map<int, std::string> m_generalToSend;
    
    int m_curPrepareNum;
    int m_prepareLimitNum;
    
    int m_curSelHeroNum;
    int m_limitHeroNum;
    float m_curLoadNum;
    int m_minSpeed;
    int m_autoType;
    
    bool m_isFirstCancelAuto;
    bool m_isBtn;
    
    bool m_isStartBattle;
    bool m_isNotice;
    int m_saveSoldierNum;
private:
};

#endif
