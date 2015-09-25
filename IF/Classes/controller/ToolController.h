//
//  ToolController.h
//  IF
//
//  Created by fubin on 14-2-27.
//
//

#ifndef IF_ToolController_h
#define IF_ToolController_h

#include "CommonInclude.h"
#include "ToolInfo.h"
#include "ToolCommand.h"

#define ITEM_TYPE_QUEUE 1
#define ITEM_TYPE_SPD 2
#define ITEM_TYPE_RES 3

#define ITEM_QUEUE_OPEN_BEGIN 200100
#define ITEM_QUEUE_OPEN_END 200105
#define ITEM_AFFAIR_REFRESH 200014

#define ITEM_RESET_TALENT 200040
#define ITEM_RENAME 200021
#define ITEM_RANDOM 200001
#define ITEM_CHANGE_PIC 200026
#define ITEM_VIP 200027
#define ITEM_ADD_STAMINE 200381
#define ITEM_SPD_CCD 200200
#define ITEM_WOOD_SPD 200027
#define ITEM_FOOD_SPD 200028
#define ITEM_IRON_SPD 200029
#define ITEM_STONE_SPD 200030

#define ITEM_MOVE_CITY 200002
#define ITEM_FREE_MOVE_CITY 200005
#define ITEM_CROSS_MOVE_CITY 200002
#define ITEM_CROSS_FREE_CITY 200005

#define ITEM_INCREASE_GATHER_SPEED 200427

#define ITEM_SEND_GIFT 200010
#define ITEM_SEND_GIFTMORE 200013
#define ITEM_SEND_GIFTALLIANCE 200014
//宝藏道具
#define ITEM_TREASURE1 200042
#define ITEM_TREASURE2 200043
#define ITEM_TREASURE3 200044
#define ITEM_TREASURE4 200045
#define ITEM_TREASURE5 200046
//烟花道具
#define ITEM_FIREWORK1 200680
#define ITEM_FIREWORK2 200681

#define ITEM_SKILL_SPEED 610003
#define ITEM_SKILL_MOVE 610002


#define ITEM_SEND_NOTICE 200011
#define ITEM_MOVE_BUILD 200032
#define ITEM_CITY_DEF_FULL 200033

#define ITEM_ALLIANCE_CITY_MOVE 200008 //联盟迁城道具
#define ITEM_TRIAL_TICKET 209182 //试炼券
#define ITEM_TRIAL_RWD_NORMAL 209183 //试炼场奖励 普通宝箱
#define ITEM_TRIAL_RWD_SILVER 209184 //试炼场奖励 白银宝箱
#define ITEM_TRIAL_RWD_GOLD 209185 //试炼场奖励 黄金宝箱

#define USE_TOOL_CITY_MOVE "city.move"
#define USE_TOOL_MARCH_CANCEL "march.cancel"
#define USE_TOOL_MARCH_CANCEL_RALLY "march.cancel.rally"//高级行军召回
#define MSG_TOOL_CHANGE "msg.tool.change"
#define MSG_ITME_STATUS_TIME_CHANGE "msg.item.status.time.change"
#define USE_TOOL_CHANGE_NAME "change.nickName"
#define USE_TOOL_CHANGE_PIC "change.pic"
#define USE_TOOL_VIP_PIC "use.vip"
#define USE_TOOL_VIP_ACTIVITY "use.vip.activity"
#define USE_TOOL_MATE_BOX "use.mate.box"
#define MSG_TOOL_HOT_GET_RETURN "msg.tool.hotget.return"
#define MSG_TOOL_MERCHANTE_LEAVE "msg.tool.merchante.leave"
#define USE_TOOL_LOTTERY1 "lottery.buy1"
#define USE_TOOL_LOTTERY2 "lottery.buy2"

#define CCD_MARCH_TYPE -1
#define CCD_QUEUE_OPEN -2
#define CCD_TRAIN_GENERAL_TYPE 100000

#define TOOL_MERCHANTE_SAVE_INDEX_KEY "tool.merchante.save.index"
#define MSG_HAS_NEW_GOOD "msg.has.new.good"

#define MATETOOL_CREATE_END "mateTool.create.end"

enum ItemSpdMenu{
    ItemSpdMenu_ALL = 1, //——城建，造兵，科技，造陷阱，治疗
    ItemSpdMenu_Troop = 2,  //——行军
    ItemSpdMenu_Soldier = 3, //——造兵
    ItemSpdMenu_Heal = 4, // 治疗
    ItemSpdMenu_Trap = 5, // 造陷阱
    ItemSpdMenu_Science = 6, // 科技
    ItemSpdMenu_City = 7, // 城建
    ItemSpdMenu_DuanZao = 8 //锻造
};
struct ToolCustomSkin{
    string md5;
    string skin;
};

class ToolController : public CCObject{
public:
    static ToolController* getInstance();
    static void purgeData();
    static bool sortMerchantItemsByOder(const MerchantToolInfo& itemId1,const MerchantToolInfo& itemId2);
    
    ToolController();
    ~ToolController();
    
    //初始化信息
    void initToolData(CCDictionary* dict);
    void initToolReward(CCDictionary* dict);
    //买龙晶装备
    bool buyLongJingEquip(int itemId, int num, CCCallFunc* callBackFunc, float price,bool buyAndUse,bool batchbuy,string fromView);
    void retBuyLongJingEquip(CCDictionary* dict,bool buyAndUse,int buyCount);
    //购买道具
    bool buyTool(int itemId, int num=1, CCCallFunc* callBackFunc=NULL, float price=0,bool buyAndUse=false,bool batchbuy = false,string fromView="");
    bool buyHotTool(int itemId, int num=1, CCCallFunc* callBackFunc=NULL, float price=0,string fromView="");
    void retBuyTool(CCDictionary* dict,bool buyAndUse=false,int buyCount =1);
    
    //添加道具
    void addTool(int itemId, int num=1, string uuid="");
    void pushAddTool(CCDictionary* dict);
    void pushDelTool(CCDictionary* dict);
    //添加建造材料空位
    bool addMakeToolQueue(int num);
    void endAddMakeToolQueue(CCDictionary* dict);
    //取消建造材料
    bool cancelMakeTool(int index);
    void endCancelMakeTool(CCDictionary* dict);
    //制作道具
    bool startMakeTool(int itemId);
    void retMakeTool(CCDictionary* dict);
    //收货道具
    bool getMakeTool(string quuid);
    void retGetMakeTool(CCDictionary* dict);
    
    //使用道具
    bool useTool(int itemId, int num=1, bool isSend = false, bool isByBuy=false);
    void retUseTool(CCDictionary* dict);
    //热卖道具
    void retHotItems(CCDictionary* dict);
    std::vector<ToolRewardInfo>* findToolRewards(int itemId);
    
    bool checkUseStateTool(int itemId,CCCallFunc* callFunc);
    void onOkUse();
    
    bool useItemChnageName(int itemId,std::string nickName);
    bool useItemChnagePic(int itemId,std::string pic);
    
    int getTotalCountInBag();
    //获取道具信息
    ToolInfo& getToolInfoById(int itemId);
    
    //获取 某一类 装备材料数量
    int getMateCntByType(int mateType);
    
    CCSafeObject<CCCallFunc> m_callBackFunc;
    
    std::map<int, std::vector<ToolRewardInfo>> m_toolReward;
    
    std::map<int,ToolInfo> m_toolInfos;
    std::vector<int> m_warTools;
    std::vector<int> m_spdTools;
    std::vector<int> m_resTools;
    std::vector<int> m_othTools;
    std::vector<int> m_mateTools;
    std::vector<int> m_allTools;
    
    std::vector<int> m_makeTools;//可制作的道具

    std::vector<int> m_allianceTools;
    std::map<int,CCDictionary*> m_statusItems;
    std::map<int,CCArray*> m_typeItems;
    
    std::map<int, std::vector<int> > m_typeTools;
    std::map<int, std::vector<int> > m_statusGroup;
    std::map<int,ToolCustomSkin> m_customSkinGroup;
    int m_willUseItem;
    
    int m_canBuy;
    int m_lastTime;
    int m_currentUseItemId;//defaut 0
    int m_mallCurrItemId;
    
    
    //行走商人
    void initMerchanteInfo(CCDictionary* dict);
    std::vector<MerchantToolInfo> m_toolMerchantInfos;
    void buyMerchantTool(const string itemId,const int itemNum,const int price,const int priceType,CCCallFunc* callBackFunc=NULL,const int num = 1);
    void retMerchantItems(CCDictionary* dict);
    void retBuyMerchantTool(CCDictionary* dict);
    int m_merchantRefreshCost;
    double m_merchantRefreshTime;
    
    int m_merchantCurrIndex;
    
    MerchantToolInfo merchantBestTool;
//    MerchantToolType m_merchantBestType;
//    int m_merchantBestColor;
//    int m_merchantBestPriceType;
//    int m_merchantBestPrice;
//    int m_merchantBestID;
//    int m_merchantBestNum;
    void saveMerchanteIndex();
    
    int getSPDItem(int speedType);
    
    
    //商店新分类规则
    std::vector<int> m_warList;
    std::vector<int> m_resList;
    std::vector<int> m_buffList;
    std::vector<int> m_otherList;
    std::vector<int> m_hotList;
    
    //新获得的物品列表,仅用于前台展示
    std::vector<int> m_newGoodsList;
    void addToNewGoodsList(int itemid);
    void removeFromNewGoodsList(int itemid);
    void flyToolReward(int itemId);
    
    
    //主城换皮
    void checkMaincityCustom();
    void setMaincityCustom(CCObject *ccObj);
    void resetMaincityCustom(float fupdate);
    void cancelCustomSkin();
    bool canChangeSkin();
    std::map<string,string> maincityConfig;
private:
    
    CC_SYNTHESIZE(bool, m_merchante_animation , Merchante_animation);
    //旅行商人状态 0-leave 1-in 2-not open
    CC_SYNTHESIZE(int, m_merchante_state, Merchante_state);
    CC_SYNTHESIZE_READONLY(int, m_merchante_costTip, MerchanteCostTip);
//    CC_SYNTHESIZE_READONLY(double, m_merchante_time_s1, Merchante_time_s1);
//    CC_SYNTHESIZE_READONLY(double, m_merchante_time_e1, Merchante_time_e1);
    
    bool isNewVersionGoods(std::string checkV);
    
    void aniComplete(CCObject* p);
    void removeLayerColor(CCObject* p);
    
    //主城换皮
    CC_SYNTHESIZE_READONLY(string, m_maincitySkin, MaincitySkin);
    bool m_bMaincityResource;
};

#endif
