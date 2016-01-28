//
//  CCCommonUtils.h
//  IF
//
//  Created by 邹 程 on 13-9-16.
//
//

#ifndef __IF__CCCommonUtils__
#define __IF__CCCommonUtils__

#include <string>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "EquipInfo.h"
#include "GeneralInfo.h"
#include "CCFloatingText.h"
#include "GlobalData.h"
#include "cocosextV3.h"
#define _dict(_obj) CCCommonUtils::castDict((_obj))

#define GOODS_ICON_TAG 99
#define GOODS_BG_TAG 100
#define SERVER_SWITCH_HD "SERVER_SWITCH_HD"
#define SERVER_SWITCH_HD_2 "SERVER_SWITCH_HD_2"
#define DOWNLOAD_HD_CCB "DOWNLOAD_HD_CCB"
#define DOWNLOAD_HD_RES "DOWNLOAD_HD_RES"

namespace CCCommonUtils {
    enum InitOperations{
        InitStart,//起始标志，其他的要放到这个下面
        InitOpenUUID,//uuid更新标识，其他放在这个下面
        InitTime,
        InitEffect,
        InitTranslation,
        InitGaid,
        InitEffectState,
        InitStatus,
        InitConfigData,
        InitPlayerInfo,
        InitWorld,
        InitPray,
        InitLord,
        InitPushSetting,
        InitBusinessMan,
        InitMonthLyCards,
        InitMails,
        InitTrain,
        InitResource,
        InitEnemyInfo,
        InitAffair,
        InitWall,
        InitActivity,
        InitTitan,//fusheng 初始化泰坦数据  在queue前面初始化
        InitQueue,
        InitArmy,
        InitBuilding,
        InitDragonBuilding,
        InitDragonsInfo,
        InitAppLibHelper,
        InitGeneral,
        InitAllianceSceice,
        InitFort,
        InitTreat,
        InitExchange,
        InitEquip,
        InitItems,
        InitScience,
        InitTalent,
        InitGCMReward,
        InitTask,
        InitServerStopTime,
        InitMixInfo,
        InitFBShare,
        DoWhenInitComplete,
        GetGameDataAfterInit,
        InitTestSwitches,
        InitNewPlayerReward,
        InitAssets,//跨服胜点对象数据
        InitEnd,//结束标志，其他的要放到这个上面
    };
//    std::string to_string(int d);
//    std::string to_string(float d);
//    std::string to_string(double d);
    std::string timeStampToUTCDate(time_t &t);
    std::string timeStampToUTCHSM(time_t &t);
    std::string timeStampToDate(time_t &t);
    std::string timeLeftToCountDown(int time,const char* separator = ":");
    
    std::string timeStampToDHM(time_t &t);
    std::string timeStampToHour(time_t &t);
    std::string timeStampToMD(time_t &t);
    std::string timeStampToYMD(time_t &t);
    std::string timeStampToHMS(time_t &t);
    
    long UTCDataToTimeStamp(const char* timeStamp);
    
    void setButtonTitle(CCControlButton* button,const char* str);
    void setButtonTitleColor(CCControlButton *button, ccColor3B color);
    void setButtonSprite(CCControlButton* button,const char* str);
    void setButtonTitlePoint(CCControlButton* button,const CCPoint point);
    CCArray* split(const char* src, const char* sym);
    void splitString(const std::string &strSrc, const std::string &strFind, std::vector<std::string> &arSplit);
    std::string replaceString(const std::string &str,const std::string &src,const std::string &des);
    void flyText(std::string, cocos2d::ccColor3B color=cocos2d::ccRED, float time = 0.5);
    void flyHint(std::string icon, std::string titleText, std::string contentText, float time = 3, float dy=0, bool isMid=false, float proY=0, std::string mailId="");
    void flyHintWithDefault(std::string icon, std::string titleText, std::string contentText,float time = 3,float dy=0);
    void flySystemUpdateHint(double countDown, bool isLogin,string tip,FlyHintType type,std::string icon="");
    void flyTextOnParent(std::string, cocos2d::CCNode *parent,cocos2d::CCPoint pos,cocos2d::ccColor3B color=cocos2d::ccRED);
    void flyUiResText(std::string, cocos2d::CCNode *parent,cocos2d::CCPoint pos,cocos2d::ccColor3B color=cocos2d::ccRED, FloatingTypeEnum floatType=floating_type_normal, int fontSize=20);
    void createGoodsIcon(int toolID, CCNode* mParent,CCSize defSize,CCLabelIF* mNum = NULL, CCLabelIF* mName = NULL, CCLabelIF* mDes =NULL,float op =255.0);
    std::string getBonusString(cocos2d::CCArray* rewards);
    //上传头像
    void onUploadPhoto(string uid, int srcCode, int idx);
    //heibai
    CCSprite *createGraySprite(const char* imageName);
    //武将头像
    std::string getGeneralPicById(std::string generalId);
    //武将半身像
    std::string getGeneralBustPicById(std::string generalId);
    //名字
    std::string getNameById(std::string xmlId);
    //获得xml配置相对应的属性
    std::string getPropById(std::string xmlId, std::string propName);
    std::string getQueueIconByType(int queueType);
    std::string getQueueNameByType(int queueType);
    int getQueueSortByType(int queueType);
    
    std::string getResourceIconByType(int resourceType);
    std::string getResourceNameByType(int resourceType);
    std::string getResourceProductByType(int resourceType);

    bool isEnoughResourceByType(int resourceType, int num);
    int getCurResourceByType(int resourceType);
    bool isPushNotify(int type);
    //获得兵种图标
    std::string getArmIconByType(int armType);
    //物品背景图片
    std::string getToolBgByColor(int color);
    //英雄头像背景图片
    std::string getGenHeadBgByColor(int color);
    
    void fitContainerSizeAndAddChild(cocos2d::CCNode *container,cocos2d::CCNode *child,int type = 0);

    //更改Sprite变灰
    void setSpriteGray(CCSprite *sprite, bool gray);
    
    //更改Sprite3D变灰
    void setSprite3DGray(Sprite3D *pSprite3D, bool gray);
    
    //检查武将状态
    bool checkGeneralState(int _state);
    //icon字段读取
    std::string getIcon(std::string);
    //是否是装备物品
    bool isEquip(int itemid);
    //获得物品颜色
    cocos2d::ccColor3B getItemColor(int color);
    //获得颜色名称
    string getColorName(int color);
    //
    bool isLackOfGold(int i);
    //获得地形天赋字符串
    std::string getTalentStr(std::string talent, int type);
    //获得地形加成字符串
    std::string getTacticsStr(std::string talent, int type);
    //获得物品洗练属性字符串
    std::string getItemPropStr(EquipInfo* info, std::string att, int value);
    //获得碎片合成所需碎片数
    int getSoulExChangeNeedByColor(int color);
    //获得武将技能
    std::string getGeneralSkill(std::string generalId);
    //监测布阵格是否开放
    bool isFormationOpen(int index);
    //返回布阵已开几个格
    int getFormationOpenNum();
    //返回下次开布阵位的等级
    int getNextFormationOpenLevel();
    //根据建筑类型获得队列类型
    int getQueueTypeByBuildType(int buildType);
    
    //根据用户头像名称获取加载哪个大图
    int getBustIdxByName(string name);
    //localFN 本地取系统的语言的简称 如en zh，通过这个方法，取得对应的语言的全称，如 zh 通过多语言等，取得 简体中文
    std::string getLanguageFNByLocalSN(std::string localSN);
    
    //获取作用号
    float getEffectValueByNum(int num, bool isInludeGeneral = true);
    float getVipEffectValueByNum(int num);
    float getOfficerValueByNum(int num);
    float getTerritoryEffectValueByNum(int num);
    float getTerritoryNegativeEffectValueByNum(int num);
    //功能按钮是否开启
    bool isBtnOpen(int btnIndex);
    //记录打点
    void recordStep(std::string index);
//    void recordStepByHttp(std::string index);
    //获得祭祀
    int getNormalPray();
    int getIronPray();
    
    //获得cd时间所花费的金币
    int getGoldByTime(int t);
    //获得某种资源所花费的金币
    int getResGlodByType(int type, int num);
    //获得购买物品的金币
    int getGoldBuyItem(std::string itemId);
    //设置图片最大宽度
    CCSprite* setSpriteMaxSize(CCSprite* spr, int limitNum, bool isForce=false);
    CCSprite* getRomanSprite(int num, int type=0);
    //变灰色
    CCSprite* graylightWithCCSprite(CCSprite* oldSprite,bool isLight);
    
    //是否是测试版本
    bool isTestPlatformAndServer(string key="");
    
    //是否有资源采集道具加成
    bool hasSpeedEffect(int type);
    double getSpeedEffectTime(int type);
    
    CCPoint getPointByString(std::string string,std::string separator=";");
    
    void getRGBAProtocols(CCNode* parent, vector<CCNode*> * proVec);
    
    std::string changeTimeAddUnit(int secs);
    std::string subStrByUtf8(const string &str,int index,int len);
    
    std::string getLanguage();
    //获取多语言的type与系统字段的对应关系
    std::string getMoreLanguageType(string sysTemKey);
    //生成序列帧动画
    void makeEffectSpr(CCSprite* spr, string path, int num, float dt=0.07, int idx=0);
    void makeOnceEffectSpr(CCSprite* spr, string path, int num, float delate=0, float dt=0.07);
    void makeTwoAnimatSpr(CCSprite* spr, string path, int num, int splitNum, float dt=0.07, int idx=0);
    void makeInternalEffectSpr(CCSprite* spr, string path, int num, float dt=0.07, int idx=0,float delaytime=0);
    
    // safe cast dict
    CCDictionary* castDict(CCObject* obj);
    
    CCPoint getCrossPoint(vector<CCPoint> &vector, CCPoint &c, CCPoint &d);
    std::vector<CCPoint> getCrossPoints(vector<CCPoint> &vector, CCPoint &c, CCPoint &d);
    CCSprite *addFilterSprite(std::string fileName, const ccColor3B &color, float bright = 0.0f, float contrast = 1.0f, float saturation = 1.0f);
    ccColor3B covertHSBToRGB(int h, float s, float v);
    CCSprite *getItemBGByColor(int color);
    void changeAllianceFlagBGByRank(CCSprite *sprite, int index, bool isGray);
    string getResourceStr(int resource);
    int getResourceLoadByType(int type);
    bool isServerCrossOpen();
    bool isAdriodPad();
    bool isIOSPad();
    bool isIosAndroidPad();
    bool isPad();
    void setIsUseHD(bool isHD);
    void setServerHDSwitch();
    bool getIsUseHD();
    void setIsHDViewPort(bool isHD);
    bool getIsHDViewPort();
    void addResSearchPath();
    void parseData(cocos2d::CCDictionary *params);
    //以下为初始化数据调用的函数，因为解析时间过长，分段解析
    void resetData(CCDictionary *params);//数据重置
    void refreshOpenUUID(CCDictionary *params);
    void initConfigData(CCDictionary *params);//config数据初始化
    void initTime(CCDictionary *params);//初始化时间
    void initEffect(CCDictionary *params);
    void initTranslation(CCDictionary *params);
    void initGaid(CCDictionary *params);
    void initEffectState(CCDictionary *params);
    void initStatus(CCDictionary *params);
    void initWorld(CCDictionary *params);
    void initPray(CCDictionary *params);
    void initLord(CCDictionary *params);
    void initPushSetting(CCDictionary *params);
    void initBusinessMan(CCDictionary *params);
    void initMonthLyCards(CCDictionary *params);
    void initPlayerInfo(CCDictionary *params);
    void initMails(CCDictionary *params);
    void initTrain(CCDictionary *params);
    void initResource(CCDictionary *params);
    void initEnemyInfo(CCDictionary *params);
    void initAffair(CCDictionary *params);
    void initWall(CCDictionary *params);
    void initActivity(CCDictionary *params);
    void initQueue(CCDictionary *params);
    void initBuilding(CCDictionary *params);
    void initDragonBuilding(CCDictionary *params);//初始化龙建筑信息
    void initDragonsInfo(CCDictionary *params);//初始化龙信息
    void initAppLibHelper(CCDictionary *params);
    void initGeneral(CCDictionary *params);
    void initAllianceSceice(CCDictionary *params);
    void initArmy(CCDictionary *params);
    void initFort(CCDictionary *params);
    void initTreat(CCDictionary *params);
    void initExchange(CCDictionary *params);
    void initEquip(CCDictionary *params);
    void initItems(CCDictionary *params);
    void initScience(CCDictionary *params);
    void initTalent(CCDictionary *params);
    void initGCMReward(CCDictionary *params);
    void initNewPlayerReward(CCDictionary *params);
    void initTask(CCDictionary *params);
    void initServerStopTime(CCDictionary *params);
    void initMixInfo(CCDictionary *params);
    void initFBShare(CCDictionary *params);
    void doWhenInitComplete(CCDictionary *params);
    void getGameDataAfterInit();
    void initOperation(int initType, CCDictionary *params);
    void initTestSwitches(CCDictionary *params);
    void initAssets(CCDictionary *params);
    void initTitan(CCDictionary *params);
    bool checkTaiWanFlag();
    std::string changeChinaFlag(string flag);//对大陆国旗的处理显示
    bool isShowFlagForChinese();
    bool checkVersion(string version);//检测开关 当前版本大于等于version 返回true
    //字符串替换函数
    std::string& StringReplace(std::string& strBig, const std::string& strsrc, const std::string& strdst);
    
    int getBindType(std::string bindStr);
    bool payParseData(cocos2d::CCDictionary *params);
    CCArray* getTypeArray(int type);
    string getCustomPicUrl(string uid, int picVer);
    string getShakePicUrl(string picName);
    bool isUseCustomPic(int picVer);
//    cocos2d::CCDictionary* getHelpShiftDefaultMeta(); simon
//    cocos2d::CCArray* getHelpShiftDefaultTags();
    cocos2d::ValueMap getHelpShiftDefaultMeta();
    cocos2d::ValueVector getHelpShiftDefaultTags();
    void showHelpShiftFAQ();
    void showHelpShiftFAQ(string itemId);
    void showBanHelpShift();
    void showHelpShiftSingleFAQ(string itemId);
    class ButtonNode: public Layer
    {
    public:
        
        static ButtonNode* create(cocos2d::CCSize touchSize) {
            auto ret = new ButtonNode();
            if (ret && ret->init(touchSize)) {
                ret->autorelease();
            } else {
                CC_SAFE_DELETE(ret);
            }
            return ret;
        }
        
        bool init(cocos2d::CCSize touchSize) {
            bool ret = false;
            if (cocos2d::CCNode::init()) {
                m_touchSize = touchSize;
                ret = true;
            }
            return ret;
        }
        
        void setSwallow(bool isSwallow);
        void setPriority(int priority);
        
        bool isTouchInNode(cocos2d::CCTouch *pTouch);
        
        virtual bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
        virtual void onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
        virtual void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
        virtual void onTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
        
        virtual void onEnter();
        virtual void onExit();
        
    private:
        
        ButtonNode():
        cocos2d::Layer()
        ,m_touchSize(cocos2d::CCSizeZero)
        ,m_swallow(false)
        ,m_priority(1)
        ,m_touchBegan(NULL)
        ,m_touchMoved(NULL)
        ,m_touchEnded(NULL)
        ,m_touchCancelled(NULL)
        ,m_touchDoBegin(NULL)
        ,m_touchDoMove(NULL)
        ,m_touchDoEnd(NULL)
        ,m_touchDoCancel(NULL)
        {_ignoreAnchorPointForPosition = false;}
        
        void resetTouch();

        CC_SYNTHESIZE(cocos2d::CCSize, m_touchSize,TouchSize);
        CC_SYNTHESIZE_READONLY(bool, m_swallow,Swallow);
        CC_SYNTHESIZE_READONLY(int, m_priority,Priority);
        CC_SYNTHESIZE(std::function<bool(ButtonNode* that,cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)>, m_touchBegan,TouchBegan);
        CC_SYNTHESIZE(std::function<void(ButtonNode* that,cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)>, m_touchMoved,TouchMoved);
        CC_SYNTHESIZE(std::function<void(ButtonNode* that,cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)>, m_touchEnded,TouchEnded);
        CC_SYNTHESIZE(std::function<void(ButtonNode* that,cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)>, m_touchCancelled,TouchCancelled);
        CC_SYNTHESIZE(std::function<void(bool isTouchInNode)>, m_touchDoBegin,TouchDoBegin);
        CC_SYNTHESIZE(std::function<void(bool isTouchInNode)>, m_touchDoMove,TouchDoMove);
        CC_SYNTHESIZE(std::function<void(bool isTouchInNode)>, m_touchDoEnd,TouchDoEnd);
        CC_SYNTHESIZE(std::function<void(bool isTouchInNode)>, m_touchDoCancel,TouchDoCancel);
    };
    
};

#endif /* defined(__IF__CCCommonUtils__) */
