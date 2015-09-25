//
//  DynamicResourceController.h
//  IF
//
//  Created by Émilie.Jiang on 15/3/25.
//
//

#ifndef __IF__DynamicResourceController__
#define __IF__DynamicResourceController__

#include "CommonInclude.h"


#define MSG_DYNAMIC_DOWNLOAD_FINISH "msg.dynamic.download.finish"
#define DynamicResource_Folder "dresource/"
#define DynamicSound_Folder "sounds/"
#define DynamicResource_GOODS_TEXTURE "goods"
#define DynamicResource_MONSTER_TEXTURE "monster"
#define DynamicResource_SERVERFIGHT_TEXTURE "ckf_map"
#define DynamicResource_MONSTER_TEXTURE_layer "monsterLayer"
#define DynamicResource_MINIMAPVIEW_TEXTURE "miniMapView"
#define DynamicResource_THORNE_TEXTURE "thorne"
#define DynamicResource_HD_CCB "hdccb"
#define DynamicResource_HD_RES "hdres"

enum DynamicResourceType{
    DynamicResourceType_None,
    DynamicResourceType_Version,
    DynamicResourceType_Name
};

struct DynamicResource{
    std::string name;
    std::string oldV;
    std::string newV;
    std::string md5;
    int needSize;
    bool platform;
    DynamicResourceType type;
};

class DynamicResourceController:public CCObject{
public:
    DynamicResourceController();    // for lua use
    static DynamicResourceController* getInstance();
    string getDownloadPath();
    
    void initGoodsInfo(CCDictionary* ccDict);
    bool checkGoodsResource();
    
    void initMonsterInfo(CCDictionary* ccDict);
    bool checkMonsterResource();
    
    void initServerFightInfo(CCDictionary* ccDict);
    bool checkServerFightResource();
    
    void initMinimapViewInfo(CCDictionary* ccDict);
    bool checkMinimapViewResource();
    
//    lzy 高清ccb
    void initHDCCBInfo(CCDictionary* ccDict);
    bool checkHDCCB();
    
    void initHDResourceInfo(CCDictionary* ccDict);
    bool checkHDResource();
    
    void initMonsterLayerInfo(CCDictionary* ccDict);
    bool checkMonsterLayerRes();
    
    void initThorneInfo(CCDictionary *dict);
    bool checkThorneResource();
    
    void sendGetCommonCMD();
    void downLoadAllCommon();
    void initCommonInfo(CCObject* ccDict);
    bool checkCommonResource(string name);
    
    //sound
    void checkSoundResource();
    void downloadSoundResource(string name);
    
    //maincity
    bool checkMainCityResource(string name, string md5);
    void initMainCityConfig(string name,map<string,string> &skinMap);
    
    bool hasNameTypeResource(std::string name,int needSize,std::string md5,bool platform=true);
    bool hasVersionTypeResource(std::string name,std::string newV,int needSize,std::string md5);
    friend void* DynamicResourceFileDownloadAndUncompress(void*);
    void downloadFinish();
    void unzipFinish();
    void dealwithError();
    void loadNameTypeResource(std::string name, bool release);
    
protected:
    bool checkResourceFile(std::string fileName,bool platform = true);
    bool checkMD5();
    void saveDownloadVersion(float fUpdate = 0.0f);
    void saveClientVersion(float fUpdate = 0.0f);
    void sendNotification(float t = 0.0);
    void finishAndtryNext(float fUpdate = 0.0f);
private:
    bool m_isDownloading;
    std::string m_writeablePath;
    std::string m_packagePath;
    std::string m_zipFile;
    void startDownload();
    bool unZipFile(bool checkMD5=true);
    bool downloadFile();
    bool createDirectory(const char *path);
    pthread_t *m_tid;
    CURL *m_curl;
    vector<DynamicResource> mQueue;
    void popFromQueue();
    
    DynamicResource mDownLoadDR;
    
    vector<DynamicResource> mServerQueue;
    
    CC_SYNTHESIZE_READONLY(bool, m_isInitedCommon, InitedCommon);
    
    void saveCommonStatus(int idx,bool downloaded);
    
    bool checkZipMD5(string zipfile,string md5);
    //md5类型的文件版本号信息，存放在dresource里
    std::map<string,string> m_md5Map;
    std::map<string,string> m_md5unzipMap;
    void initMD5Map();
    void saveMD5Map();
    void saveUnzipMD5Map();
};
#endif /* defined(__IF__DynamicResourceController__) */
