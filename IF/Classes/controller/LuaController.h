//
//  LuaController.h
//  IF
//
//  Created by Émilie.Jiang on 15/1/26.
//
//

#ifndef __IF__LuaController__
#define __IF__LuaController__

#include "CommonInclude.h"
#include "ActivityEventObj.h"
#include "GoldExchangeItem.h"

struct ExchangeGiftStruct{
    std::string exchangeId;
    std::string receiverId;
    std::string uid;
    double sendTime;
};
enum LuaDownloadType{
    LuaDownloadType_NONE,
    LuaDownloadType_LUA,
    LuaDownloadType_RESOURCE
};
class LuaController:public CCObject{
public:
    LuaController();    // for lua use
    ~LuaController();
    static LuaController* getInstance();
    static void doResourceByCommonIndex(int commonIndex,bool isLoad);
    static std::string getResourceNameByType(int itemid);
    static CCParticleSystemQuad* createParticleForLua(const char *name);
    static std::string getLang(std::string &msg);
    static std::string getLang1(std::string &msg, std::string &param);
    static std::string getDollarString(string &dollar,string &productId);
    static std::string getCMDLang(std::string &msg);
    static void addButtonLight(CCNode* node);
    static void addItemIcon(CCNode *node,std::string &itemid,CCLabelTTF *lable);
    static void addIconByType(CCNode *node,std::string &itemid,CCLabelTTF *lable,int type,int size);
    static int getWorldTime();
    static int getTimeStamp();
    static void playEffects(std::string name);
    static std::string getSECLang(int time);
    static void callPaymentToFriend(std::string &itemid,std::string touid,std::string toName);
    static void callPayment(std::string &itemid);
    static void removeAllPopup();
    static void removeLastPopup();
    static void showDetailPopup(std::string &itemid);
    static bool checkSkeletonFile(const char* fileName);
    static void addSkeletonAnimation(CCNode* node,const char* skeletonDataFile, const char* atlasFile,const char* animation,float scale);
    static int getStringLength(std::string &str);
    static std::string getUTF8SubString(std::string &str,int start,int end);
    static void toSelectUser(std::string itemid,bool removeAllPop,int backTo);
    static void setTitleName(std::string _name);
    static std::string getPlatform();
    static std::string getLanguage();
    CCNode* getSaleViewContainer();
    CCNode* getAdvCellContainer();
    CCNode* getAdvCell();
    CCNode* getIconContainer();
    
    //hd
    static bool isIosAndroidPad();
    
    void sendCMD(std::string cmdName,CCDictionary *dict);
    void backCMD(std::string cmdName,CCDictionary *dict);
    static CCDictionary* getCMDParams(std::string cmdName);
    
    // for c++ use
    void showExchangeSale(CCNode *node,GoldExchangeItem* item);
    CCRect createExchangeAdvCell(CCNode *node,std::string &info);
    void updateExchangeAdvCell(CCNode *node,std::string &info);
    void createExchangeIcon(CCNode *node,std::string &info);
    bool checkLuaValid(std::string popImg,std::string md5,std::string type);
    bool checkActLuaValid(ActivityEventObj* obj);
    bool showNoramlIcon(std::string popImg);
    friend void* LuaFileDownloadAndUncompress(void*);
    void downloadFinish();
    void unzipFinish();
    void changeUI(float t = 0.0);
//    bool checkStorageEnough(double checkSize);
//    void beginCheckStorage(float t = 0.0);
    
    //活动中心
    CCNode* createActivityView(ActivityEventObj* obj);
    CCNode* createActivityRwdView(ActivityEventObj* obj);
    CCNode* createActivityAdCell(ActivityEventObj* obj);
    CCNode* createActivityListCellSprite(ActivityEventObj* obj);
    void getActExcData();
    void showActRwdViewByActId(std::string actId);
    CCSafeObject<CCDictionary> m_cmdDic;
    static CCSprite* createGoodsIcon(std::string& itemId, int size);
    static void removeAllPopupView();
    static void openBagView(int type);
    static CCDictionary* getGoodsInfo(std::string& itemId);
    static std::string getObjectType(CCObject* obj);
    static std::string retReward(CCArray*);
    static void flyHint(std::string icon, std::string titleText, std::string context);
    static void showTipsView(std::string, CCTextAlignment align = kCCTextAlignmentCenter);
    
    std::string getActTips(std::string actId);
    std::string getCollectLabel(std::string actId);
    std::string getBuyBtnLabel(std::string actId);
    std::string getActRefTip(std::string actId);
    
    
    //google translate
    void setTranslateMark();
    
    void translate(std::string orgContent,std::string targeLan, int linkidx = 0);
    void onTranslateBack(std::string backStr);
    static size_t onTranslate(uint8_t* ptr,size_t size,size_t number,void *stream);

    
    string luaVersion;//动态更新活动的版本号
    string luaMD5Code;//动态更新活动zip md5校验码
    int luaSize;//lua解压后文件大小
    int luazipSize; //lua zip包大小
    string luapackMD5Code;
    
//
// Chat Notice
//
    void addChatNotice(CCDictionary *dict);
    void closeChatNotice();
    CCNode* getChatNoticeContainer();
    void showSceneNotice(bool b);
    void changeChatNotice(CCObject *ccObj = NULL);
    void addSystemUpdate(double countDown, bool isLogin,string tip,FlyHintType type,string icon);
    void ChatNoticeInView(CCObject *ccObj = NULL);
    void ChatNoticeInSceen(CCObject *ccObj = NULL);
    void clearChatNotice();
    
    //礼包赠送
    void updateExchangeGiftInfo(std::string toUID,std::string itemid, double sendTime);
    void initExchangeGiftInfo(CCArray *arr);
    bool canSendExchange(std::string &itemid,std::string uid,std::string toName,int type = 0,bool showTip = false);
    void backToExchange(int backTo);
    bool canTranslate();
    std::string encodeUrl(const std::string& szToEncode);
private:
    void addLuaPath();
    std::string getLuaPath();
    std::string getLuaFile();
    CCNode* m_saleViewContainer;
    CCNode* m_advCellContainer;
    CCNode* m_advCell;
    CCNode* m_iconContainer;
    bool m_isAddPath;
    
    bool m_isDownload;
    std::string m_clientVersion;
    std::string m_serverVersion;
    std::string m_downedVersion;
    void resetVersionAndUpdate();
    void updateNewResource();
    bool checkUpdate();
    void startDownload();
    bool unZipFile();
    bool downloadFile();
    bool createDirectory(const char *path);
    pthread_t *m_tid;
    CURL *m_curl;
    std::string m_writeablePath;
    std::string m_packagePath;
    bool isLuaOpen();
    
    bool initLua();
    bool m_initLuaEngine;
    std::string m_initVersion;
    
    LuaDownloadType m_downloadType;
    std::string m_packName;
    void replaceNewLuaFile();
    bool checkLuaFiles();
    void saveDownloadVersion(float fUpdate = 0.0f);
    void saveClientVersion(float fUpdate = 0.0f);
    void saveClientVersion1(float fUpdate = 0.0f);
    bool initMD5Info();
    int getVersionData(std::string filePath);
    std::map<std::string,std::string> m_md5Map;
    
    
    
    //chat notice
    CCNode *mChatNoticeNode;
    void createChatNotice(CCDictionary *ccDict);
    std::vector<CCDictionary*> mChatNoticeV;
    std::vector<ExchangeGiftStruct> mExGiftList;
    
    //google translate
    std::string m_orgMsg;
    bool m_canTranslate;
    
};


#endif /* defined(__IF__LuaController__) */
