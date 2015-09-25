//
//  EquipmentController.h
//  IF
//
//  Created by fubin on 14-12-2.
//
//

#ifndef __IF__EquipmentController__
#define __IF__EquipmentController__

#include "CommonInclude.h"
#include "EquipmentInfo.h"

#define MSG_MATE_CRT_END "msg.mate.crt.end"
#define EQUIP_BAG_SHOW_DES "equip.bag.show.des"
#define EQUIP_CREATE_END "equip.create.end"

#define EQUIP_PUTON_MSG "equip.puton.msg"
#define EQUIP_TAKEOFF_MSG "equip.takeoff.msg"
#define EQUIP_CLOSE_VIEW "equip.close.view"
#define EQUIP_POWER_EFFECT_SHOW "equip.power.effect.show"

class EquipmentController : public CCObject{
public:
    static EquipmentController* getInstance();
    static void purgeData();
    EquipmentController();
    ~EquipmentController();
    
    void retInitData(CCDictionary* dict);
    void retInitConf(CCDictionary* dict);
    
    void addEquip(CCArray* equipArr);
    
    //合成装备
    bool startCreateEquipment(string uuid, string qItemId, vector<int> toolUuids);
    void retCreateEquipment(CCDictionary* dict);
    void startFinishCrtEquip(string quuid);
    void retFinishCrtEquip(CCDictionary* dict);
    
    //分解装备
    bool startDesEquipment(string uuid);
    void retDesEquipment(CCDictionary* dict);
    //卖龙装备
    bool startSellLongEquipment(string uuid);
    void retSellLongEquipment(CCDictionary* dict);
    
    //穿戴装备
    bool startPutOnEquipment(string uuid, string oldUuid);
    void retPutOnEquipment(CCDictionary* dict);
    
    //卸下装备
    bool startTakeOffEquipment(string uuid);
    void retTakeOffEquipment(CCDictionary* dict);
    
    //材料合成
    bool startMaterialCreate(int toolId,int type = 0);
    void retMaterialCreate(CCDictionary* dict);
    
    //计算锻造品质概率
    vector<float> MakeEquipmentPro(string uuid, string qItemId, vector<int> toolUuids, bool isUUid=true);
    
    //按部件 和 材料 查找装备
    vector<int> SelEquipmentsByType(int site=-1, int mate=-1);
    
    //按部件查找 现有 装备
    vector<string> FindEquipmentsByType(int site=-1);
    bool IsExsitEquipBySite(int site);
    bool IsExsitRealEquipBySite(int site);
    
    //按部件查找 现有 装备
    vector<string> FindEquipmentsByLv(int needLv1, int needLv2);
    
    //按部件查找 已经穿上的装备
    string FindOnEquipBySite(int site);
    
    //检查锻造列表里的装备 材料是否充足可锻造
    bool CheckSelEquipMate(int site, int mate);
    //检查可以锻造哪个部位的装备
    vector<int> CheckEquipSiteMake();
    
    //检查当前背包里是否有该类装备 可以是不同品质
    bool IsHaveEquipByLv(int lv, int selfLv);
    
    //检查是否有未使用的装备 且有佩戴位置
    bool IsCanSetEquipBySite(int site=-1);
    
    void MakeEffectValue();
    
    //显示穿戴装备战斗力特效
    void showPowerChangeEff(string, string);
    
    //提示可打造并穿戴装备
    bool isShowEquipTips(int site);
    
    string GetMateNameById(int mateId);
    
    CCSafeObject<CCCallFunc> m_callBackFunc;
    
    map<int, EquipmentInfo> EquipmentInfoMap;
    map<string, EquipmentInfo> LongEquipmentInfoMap;
    vector<int> SelEquipList;
    
    map<string, int> m_MyEquipmentMap;
    map<string, int> m_MyOnEquip;
    map<int, float> EquipEffMap;//装备作用
    
    vector<int> SelEquipListData;
    
    string m_curOptEquip;
    string m_oldEquip;
    vector<int> m_curOptMate;
    
    map<int,float> m_defPro;//保底值
    map<int,float> m_colorPro;//颜色衰减系数
    map<int,int> m_lvToCnt;//不同等级的装备 转换为相同品质材料的数量
    map<int,int> m_colorWeight;//颜色对应参数
    
    string m_curCreateCost;
private:
};

#endif /* defined(__IF__EquipmentController__) */
