//
//  DynamicResourceController.cpp
//  IF
//
//  Created by Émilie.Jiang on 15/3/25.
//
//

#include "DynamicResourceController.h"
#include "ToolController.h"
#include "../Ext/CCDevice.h"
#include <curl/easy.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include "../view/guiExt/md5.h"
#include <stdio.h>
#include "external/unzip/unzip.h"
#include "DRConfigCommand.h"

#define DynamicResource_Client_VersionKey "DRVersion_client_"
#define DynamicResource_Download_VersionKey "DRVersion_download_"
#define DynamicResource_Zip "tmpZR_"
#define DynamicResource_BUFFER_SIZE    8192
#define DynamicResource_MAX_FILENAME   512

#define DynamicSound_BG "bgsound"
#define DynamicSound_EFFECT "effsound"

#define DynamicMD5_Version "dynamicmd5version.bin"
#define DynamicMD5Unzip_Version "dynamicmd5UnzipVersion.bin"

static DynamicResourceController *_instance = NULL;
DynamicResourceController* DynamicResourceController::getInstance() {
    if (!_instance) {
        _instance = new DynamicResourceController();
    }
    return _instance;
}
DynamicResourceController::DynamicResourceController():m_isDownloading(false),m_isInitedCommon(false){
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    m_writeablePath = cocos2d::extension::CCDevice::getSDcardPath();
//    CCLog("sdcard path [%s]",m_writeablePath.c_str());
//#endif
    if(m_writeablePath.empty()){
        m_writeablePath = CCFileUtils::sharedFileUtils()->getWritablePath();
    }
    initMD5Map();
}
void DynamicResourceController::initGoodsInfo(cocos2d::CCDictionary *ccDict){
    if(!ccDict)
        return;
    
    DynamicResource dr;
    dr.name = DynamicResource_GOODS_TEXTURE;
    if (ccDict->objectForKey("goodsVersion") && ccDict->objectForKey("goodsMd5Android") && ccDict->objectForKey("goodsMd5Ios")) {
        std::string version = ccDict->valueForKey("goodsVersion")->getCString();
        if(!version.empty()){
            if(version.compare("0.0.0") == 0){
                return;
            }
            dr.newV = version;
        }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        std::string code = ccDict->valueForKey("goodsMd5Android")->getCString();
        if(!code.empty()){
            dr.md5 = code;
        }
#else
        std::string code = ccDict->valueForKey("goodsMd5Ios")->getCString();
        if(!code.empty()){
            dr.md5 =code;
        }
#endif
        mServerQueue.push_back(dr);
    }
}
bool DynamicResourceController::checkGoodsResource(){
    auto iter = mServerQueue.begin();
    while (iter!=mServerQueue.end()) {
        if((*iter).name.compare(DynamicResource_GOODS_TEXTURE) == 0){
            return hasVersionTypeResource(DynamicResource_GOODS_TEXTURE,(*iter).newV,1024,(*iter).md5);
        }
        ++iter;
    }
    return false;
}

void DynamicResourceController::initMonsterInfo(CCDictionary* ccDict){
    if(!ccDict)
        return;
    
    DynamicResource dr;
    dr.name = DynamicResource_MONSTER_TEXTURE;
    if (ccDict->objectForKey("monsterVersion") && ccDict->objectForKey("monsterMd5Android") && ccDict->objectForKey("monsterMd5Ios")) {
        std::string version = ccDict->valueForKey("monsterVersion")->getCString();
        if(!version.empty()){
            if(version.compare("0.0.0") == 0){
                return;
            }
            dr.newV = version;
        }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        std::string code = ccDict->valueForKey("monsterMd5Android")->getCString();
        if(!code.empty()){
            dr.md5 = code;
        }
#else
        std::string code = ccDict->valueForKey("monsterMd5Ios")->getCString();
        if(!code.empty()){
            dr.md5 =code;
        }
#endif
        mServerQueue.push_back(dr);
        checkMonsterResource();
    }
}

bool DynamicResourceController::checkMonsterResource(){
    auto iter = mServerQueue.begin();
    while (iter!=mServerQueue.end()) {
        if((*iter).name.compare(DynamicResource_MONSTER_TEXTURE) == 0){
            return hasVersionTypeResource(DynamicResource_MONSTER_TEXTURE,(*iter).newV,1024,(*iter).md5);
        }
        ++iter;
    }
    return false;
}

void DynamicResourceController::initThorneInfo(CCDictionary *ccDict){
    if(!ccDict)
        return;
    
    DynamicResource dr;
    dr.name = DynamicResource_THORNE_TEXTURE;
    if (ccDict->objectForKey("thorneVersion") && ccDict->objectForKey("thorneMd5Android") && ccDict->objectForKey("thorneMd5Ios")) {
        std::string version = ccDict->valueForKey("thorneVersion")->getCString();
        if(!version.empty()){
            if(version.compare("0.0.0") == 0){
                return;
            }
            dr.newV = version;
        }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        std::string code = ccDict->valueForKey("thorneMd5Android")->getCString();
        if(!code.empty()){
            dr.md5 = code;
        }
#else
        std::string code = ccDict->valueForKey("thorneMd5Ios")->getCString();
        if(!code.empty()){
            dr.md5 =code;
        }
#endif
        mServerQueue.push_back(dr);
        checkThorneResource();
    }
}

bool DynamicResourceController::checkThorneResource(){
    auto iter = mServerQueue.begin();
    while (iter!=mServerQueue.end()) {
        if((*iter).name.compare(DynamicResource_THORNE_TEXTURE) == 0){
            return hasVersionTypeResource(DynamicResource_THORNE_TEXTURE,(*iter).newV,1024,(*iter).md5);
        }
        ++iter;
    }
    return false;
}
void DynamicResourceController::sendGetCommonCMD(){
    if(m_isInitedCommon == false){
        DRConfigCommand *drCommand = new DRConfigCommand();
        drCommand->sendAndRelease();
    }
}

void DynamicResourceController::downLoadAllCommon(){
    if(CCTexture2D::useDownloadResource()==true){
        int index=0;
        while (index<CC_SAMLL_TEXTURE_CNT) {
            char buffer[16];
            sprintf(buffer,"Common_%d",CCTexture2D::SmallCommonList[index]);
            std::string name = buffer;
            auto iter = mServerQueue.begin();
            while (iter!=mServerQueue.end()) {
                if((*iter).name.compare(name) == 0){
                    //CCLog("android_test_checkCommonResource[%s]",name.c_str());
                    bool isNew = hasNameTypeResource(name,(*iter).needSize,(*iter).md5);
                    auto pos = name.find("_");
                    if(pos != string::npos){
                        int commonidx = atoi(name.substr(pos+1).c_str());
                        saveCommonStatus(commonidx,isNew);
                    }
                }
                ++iter;
            }
            ++index;
        }
    }
}
void DynamicResourceController::initCommonInfo(CCObject *obj){
    if(!obj){
        m_isInitedCommon=true;
        return;
    }
    CCArray* array = dynamic_cast<CCArray*>(obj);
    if(array){
        CCObject* ccObj = NULL;
        CCARRAY_FOREACH(array, ccObj){
            CCDictionary* _dict = dynamic_cast<CCDictionary*>(ccObj);
            if(!_dict)
                continue;
            if(_dict->objectForKey("name") && _dict->objectForKey("version") && _dict->objectForKey("checksize") && _dict->objectForKey("iosmd5") && _dict->objectForKey("androidmd5")){
                
                bool isIn = false;
                int index0=0;
                while (index0<CC_SAMLL_TEXTURE_CNT) {
                    char buffer[16];
                    sprintf(buffer,"Common_%d",CCTexture2D::SmallCommonList[index0]);
                    string aa = _dict->valueForKey("name")->getCString();
                    if(aa.compare(buffer) == 0){
                        isIn = true;
                        break;
                    }
                    ++index0;
                }
                if(isIn ==false){
                    string name = _dict->valueForKey("name")->getCString();
                    if(name == DynamicSound_BG ||  name  == DynamicSound_EFFECT){
                        isIn=true;
                    }else{
                        continue;
                    }
                }
                DynamicResource dr;
                dr.name = _dict->valueForKey("name")->getCString();
                dr.needSize = _dict->valueForKey("checksize")->intValue();
                dr.newV = _dict->valueForKey("version")->getCString();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                std::string code = _dict->valueForKey("androidmd5")->getCString();
                if(!code.empty()){
                    dr.md5 = code;
                }
#else
                std::string code = _dict->valueForKey("iosmd5")->getCString();
                if(!code.empty()){
                    dr.md5 =code;
                }
#endif
                if(!dr.md5.empty()){
                    mServerQueue.push_back(dr);
                }
            }
        }
    }
    m_isInitedCommon=true;
    downLoadAllCommon();
    checkSoundResource();
}
bool DynamicResourceController::checkCommonResource(string name){
    std::string oldV = "";
//    if(m_md5Map.find("dr_common_oldv")!=m_md5Map.end()){
//        oldV = m_md5Map["dr_common_oldv"];
//    }else{
        oldV = CCUserDefault::sharedUserDefault()->getStringForKey("dr_common_oldv");
        if(!oldV.empty()){
            m_md5Map["dr_common_oldv"] = oldV;
            saveMD5Map();
        }
//    }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    int idx = 0;
    bool isIn = false;
    while (idx < CC_SAMLL_TEXTURE_CNT) {
        string cmpName = string("Common_")+CC_ITOA(CCTexture2D::SmallCommonList[idx]);
        if(name.compare(cmpName) == 0){
            if(oldV.empty() || (idx>=oldV.size())){
                return false;
            }else{
                if(oldV.at(idx) == '1'){
                    return false;
                }else{
                    std::string fPath  =  m_writeablePath + "dresource/_alpha_"+ cmpName + ".pkm";
                    if(CCFileUtils::sharedFileUtils()->isFileExist(fPath)){
                        return true;
                    }else{
                        return false;
                    }
                }
            }
            break;
        }
        ++idx;
    }
#else
    if(CCTexture2D::useDownloadResource()){
        int idx = 0;
        bool isIn = false;
        while (idx < CC_SAMLL_TEXTURE_CNT) {
            string cmpName = string("Common_")+CC_ITOA(CCTexture2D::SmallCommonList[idx]);
            if(name.compare(cmpName) == 0){
                if(oldV.empty() || (idx>=oldV.size())){
                    return false;
                }else{
                    if(oldV.at(idx) == '1'){
                        return false;
                    }else{
                        std::string fPath  =  m_writeablePath + "dresource/"+ cmpName + ".pvr.ccz";
                        if(CCFileUtils::sharedFileUtils()->isFileExist(fPath)){
                            return true;
                        }else{
                            return false;
                        }
                    }
                }
                break;
            }
            ++idx;
        }
    }
#endif
    return false;
}
void DynamicResourceController::saveCommonStatus(int commonidx,bool downloaded){
    std::string oldV = "";
//    if(m_md5Map.find("dr_common_oldv")!=m_md5Map.end()){
//        oldV = m_md5Map["dr_common_oldv"];
//    }else{
        oldV = CCUserDefault::sharedUserDefault()->getStringForKey("dr_common_oldv");
        if(!oldV.empty()){
            m_md5Map["dr_common_oldv"] = oldV;
            saveMD5Map();
        }
//    }
    if(oldV.size() < CC_SAMLL_TEXTURE_CNT){
        int index=oldV.size();
        while (index<CC_SAMLL_TEXTURE_CNT) {
            oldV.append("0");
            ++index;
        }
    }
    bool isfind = false;
    int index=0;
    while (index<CC_SAMLL_TEXTURE_CNT) {
        if(commonidx == CCTexture2D::SmallCommonList[index]){
            isfind=true;
            break;
        }
        ++index;
    }
    if(isfind==true){
        oldV.replace(index, 1, downloaded?"0":"1");
        CCUserDefault::sharedUserDefault()->setStringForKey("dr_common_oldv",oldV);
        CCUserDefault::sharedUserDefault()->flush();
        m_md5Map["dr_common_oldv"]=oldV;
        saveMD5Map();
    }
}
void DynamicResourceController::checkSoundResource(){
    if(m_isInitedCommon==false){
        return;
    }
    vector<DynamicResource> pushDr;
    auto iter = mServerQueue.begin();
    while (iter!=mServerQueue.end()) {
        if((*iter).name.compare(DynamicSound_BG) == 0 || (*iter).name.compare(DynamicSound_EFFECT) == 0){
            std::string vClient = "";
            std::string vServer = (*iter).md5;
            std::string vDownload = "";
//            if(m_md5Map.find((*iter).name)!=m_md5Map.end()){
//                vClient = m_md5Map[(*iter).name];
//            }else{
                std::string keyclient = DynamicResource_Client_VersionKey + (*iter).name;
                vClient = CCUserDefault::sharedUserDefault()->getStringForKey(keyclient.c_str());
                m_md5Map[(*iter).name]=vClient;
                saveMD5Map();
//            }
//            if(m_md5unzipMap.find((*iter).name) !=m_md5unzipMap.end()){
//                vDownload =m_md5unzipMap[(*iter).name];
//            }else{
                std::string keydownload = DynamicResource_Download_VersionKey + (*iter).name;
                vDownload = CCUserDefault::sharedUserDefault()->getStringForKey(keydownload.c_str());
                m_md5unzipMap[(*iter).name]=vDownload;
                saveUnzipMD5Map();
//            }
            
            if(vClient.compare(vServer)!=0){
                if(vDownload.compare(vServer) == 0){
                    if(!cocos2d::extension::CCDevice::hasEnoughSpace((*iter).needSize)){
                        CCLOG("Do not has enough space to unzip DynamicResource file!");
                        ++iter;
                        continue;
                    }
                    m_zipFile = m_writeablePath + DynamicResource_Zip + (*iter).name + ".zip";
                    if(checkZipMD5(m_zipFile, (*iter).md5) && unZipFile(false)){
                        unzipFinish();
                        ++iter;
                        continue;
                    }
                    std::remove(m_zipFile.c_str());
                }
                pushDr.push_back((*iter));
            }else{
                if((*iter).name.compare(DynamicSound_BG) == 0){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                    string file1 = m_writeablePath + DynamicSound_Folder + "background/m_city.ogg";
                    string file2 = m_writeablePath + DynamicSound_Folder + "background/m_field.ogg";
                    if(CCFileUtils::sharedFileUtils()->isFileExist(file1) &&CCFileUtils::sharedFileUtils()->isFileExist(file2)){
                        ++iter;
                        continue;
                    }
                    pushDr.push_back((*iter));
#else
                    string file1 = m_writeablePath + DynamicSound_Folder + "background/m_city.aac";
                    string file2 = m_writeablePath + DynamicSound_Folder + "background/m_field.aac";
                    if(CCFileUtils::sharedFileUtils()->isFileExist(file1) &&CCFileUtils::sharedFileUtils()->isFileExist(file2)){
                        ++iter;
                        continue;
                    }
                    pushDr.push_back((*iter));
#endif
                }else{
                    ++iter;
                    continue;
                }
            }
        }
        ++iter;
    }
    auto iter0 = pushDr.begin();
    while (iter0!=pushDr.end()) {
        bool isIn = false;
        auto iter = mQueue.begin();
        while (iter!=mQueue.end()) {
            if((*iter).name.compare((*iter0).name) == 0){
                isIn = true;
                break;
            }
            ++iter;
        }
        if(!isIn){
            DynamicResource dr = {(*iter0).name,"",(*iter0).md5,(*iter0).md5,(*iter0).needSize,true,DynamicResourceType_None};
            mQueue.push_back(dr);
        }
        ++iter0;
    }
    popFromQueue();
}
void DynamicResourceController::downloadSoundResource(string name){
    auto iter1 = mQueue.begin();
    while (iter1!=mQueue.end()) {
        if((*iter1).name.compare(name) == 0){
            return;
        }
        ++iter1;
    }
    
    auto iter0 = mServerQueue.begin();
    while (iter0!=mServerQueue.end()) {
        if((*iter0).name.compare(name) == 0){
            DynamicResource dr = {name,"",(*iter0).md5,(*iter0).md5,1024,true,DynamicResourceType_None};
            mQueue.push_back(dr);
            popFromQueue();
            break;
        }
        ++iter0;
    }
}

bool DynamicResourceController::checkMainCityResource(string name, string md5){
    return hasNameTypeResource(name,1024,md5,false);
}
void DynamicResourceController::initMainCityConfig(string name,map<string,string> &skinMap){
    string drPath = getDownloadPath() + "skinparticle/";
    string configfile = drPath + name + ".config";
    if(CCFileUtils::sharedFileUtils()->isFileExist(configfile)){
        ifstream ifile(configfile);
        string line;
        vector<string> mVector;
        while(getline(ifile,line)){
            if(!line.empty()){
                mVector.push_back(line);
            }
        }
        ifile.close();
        auto iter = mVector.begin();
        while (iter!=mVector.end()) {
            string tmpStr = (*iter);
            if(!tmpStr.empty() && tmpStr.find("=")!=string::npos){
                vector<std::string> mTmpV;
                CCCommonUtils::splitString(tmpStr, "=", mTmpV);
                if(mTmpV.size()>1){
                    skinMap[mTmpV.at(0)]=mTmpV.at(1);
                }
            }
            ++iter;
        }
    }
}
void DynamicResourceController::initServerFightInfo(CCDictionary* ccDict){
    if(!ccDict)
        return;
    
    DynamicResource dr;
    dr.name = DynamicResource_SERVERFIGHT_TEXTURE;
    if (ccDict->objectForKey("ckf_mapVersion") && ccDict->objectForKey("ckf_mapMd5Android") && ccDict->objectForKey("ckf_mapMd5Ios")) {
        std::string version = ccDict->valueForKey("ckf_mapVersion")->getCString();
        if(!version.empty()){
            if(version.compare("0.0.0") == 0){
                return;
            }
            dr.newV = version;
        }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        std::string code = ccDict->valueForKey("ckf_mapMd5Android")->getCString();
        if(!code.empty()){
            dr.md5 = code;
        }
#else
        std::string code = ccDict->valueForKey("ckf_mapMd5Ios")->getCString();
        if(!code.empty()){
            dr.md5 =code;
        }
#endif
        mServerQueue.push_back(dr);
        checkServerFightResource();
    }
}

bool DynamicResourceController::checkServerFightResource(){
    auto iter = mServerQueue.begin();
    while (iter!=mServerQueue.end()) {
        if((*iter).name.compare(DynamicResource_SERVERFIGHT_TEXTURE) == 0){
            return hasVersionTypeResource(DynamicResource_SERVERFIGHT_TEXTURE,(*iter).newV,1024,(*iter).md5);
        }
        ++iter;
    }
    return false;
}

void DynamicResourceController::initMinimapViewInfo(CCDictionary* ccDict){
    if(!ccDict)
        return;
    
    DynamicResource dr;
    dr.name = DynamicResource_MINIMAPVIEW_TEXTURE;
    if (ccDict->objectForKey("miniMapView_version") && ccDict->objectForKey("miniMapView_md5_android") && ccDict->objectForKey("miniMapView_md5_ios")) {
        std::string version = ccDict->valueForKey("miniMapView_version")->getCString();
        if(!version.empty()){
            if(version.compare("0.0.0") == 0){
                return;
            }
            dr.newV = version;
        }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        std::string code = ccDict->valueForKey("miniMapView_md5_android")->getCString();
        if(!code.empty()){
            dr.md5 = code;
        }
#else
        std::string code = ccDict->valueForKey("miniMapView_md5_ios")->getCString();
        if(!code.empty()){
            dr.md5 =code;
        }
#endif
        mServerQueue.push_back(dr);
        checkMinimapViewResource();
    }
}

bool DynamicResourceController::checkMinimapViewResource(){
    auto iter = mServerQueue.begin();
    while (iter!=mServerQueue.end()) {
        if((*iter).name.compare(DynamicResource_MINIMAPVIEW_TEXTURE) == 0){
            return hasVersionTypeResource(DynamicResource_MINIMAPVIEW_TEXTURE,(*iter).newV,1024,(*iter).md5);
        }
        ++iter;
    }
    return false;
}

// lzy
void DynamicResourceController::initHDCCBInfo(CCDictionary* ccDict){
    if(!ccDict)
        return;
    
    DynamicResource dr;
    dr.name = DynamicResource_HD_CCB;
    if (ccDict->objectForKey("HDCCBVersion")) {
        std::string version = ccDict->valueForKey("HDCCBVersion")->getCString();
        if(!version.empty()){
            if(version.compare("0.0.0") == 0){
                return;
            }
            dr.newV = version;
        }
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        std::string code = ccDict->valueForKey("HDCCBMd5Android")->getCString();
        if(!code.empty()){
            dr.md5 = code;
        }
#else
        std::string code = ccDict->valueForKey("HDCCBMd5Ios")->getCString();
        if(!code.empty()){
            dr.md5 =code;
        }
#endif
        mServerQueue.push_back(dr);
        checkHDCCB();
    }
}

bool DynamicResourceController::checkHDCCB(){
    auto iter = mServerQueue.begin();
    while (iter!=mServerQueue.end()) {
        if((*iter).name.compare(DynamicResource_HD_CCB) == 0){
            return hasVersionTypeResource(DynamicResource_HD_CCB,(*iter).newV,1024,(*iter).md5);
        }
        ++iter;
    }
    
    
    return false;
}

void DynamicResourceController::initHDResourceInfo(CCDictionary* ccDict){
    if(!ccDict)
        return;
    
    DynamicResource dr;
    dr.name = DynamicResource_HD_RES;
    if (ccDict->objectForKey("HDResVersion")) {
        std::string version = ccDict->valueForKey("HDResVersion")->getCString();
        if(!version.empty()){
            if(version.compare("0.0.0") == 0){
                return;
            }
            dr.newV = version;
        }

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        std::string code = ccDict->valueForKey("HDResMd5Android")->getCString();
        if(!code.empty()){
            dr.md5 = code;
        }
#else
        std::string code = ccDict->valueForKey("HDResMd5Ios")->getCString();
        if(!code.empty()){
            dr.md5 =code;
        }
#endif
        
        
        
        mServerQueue.push_back(dr);
        checkHDResource();
    }
}

bool DynamicResourceController::checkHDResource(){
    auto iter = mServerQueue.begin();
    while (iter!=mServerQueue.end()) {
        if((*iter).name.compare(DynamicResource_HD_RES) == 0){
            return hasVersionTypeResource(DynamicResource_HD_RES,(*iter).newV,1024,(*iter).md5);
        }
        ++iter;
    }
    
    
    return false;
}

void DynamicResourceController::initMonsterLayerInfo(CCDictionary* ccDict){
    if(!ccDict)
        return;
    
    DynamicResource dr;
    dr.name = DynamicResource_MONSTER_TEXTURE_layer;
    if (ccDict->objectForKey("monsterLayerVersion") && ccDict->objectForKey("monsterLayerMd5Android") && ccDict->objectForKey("monsterLayerMd5Ios")) {
        std::string version = ccDict->valueForKey("monsterLayerVersion")->getCString();
        if(!version.empty()){
            if(version.compare("0.0.0") == 0){
                return;
            }
            dr.newV = version;
        }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        std::string code = ccDict->valueForKey("monsterLayerMd5Android")->getCString();
        if(!code.empty()){
            dr.md5 = code;
        }
#else
        std::string code = ccDict->valueForKey("monsterLayerMd5Ios")->getCString();
        if(!code.empty()){
            dr.md5 =code;
        }
#endif
        mServerQueue.push_back(dr);
        checkMonsterLayerRes();
    }
}

bool DynamicResourceController::checkMonsterLayerRes(){
    auto iter = mServerQueue.begin();
    while (iter!=mServerQueue.end()) {
        if((*iter).name.compare(DynamicResource_MONSTER_TEXTURE_layer) == 0){
            return hasVersionTypeResource(DynamicResource_MONSTER_TEXTURE_layer,(*iter).newV,1024,(*iter).md5);
        }
        ++iter;
    }
    return false;
}
bool DynamicResourceController::hasNameTypeResource(std::string name,int needSize,std::string md5,bool platform){
    auto iter = mQueue.begin();
    while (iter!=mQueue.end()) {
        if((*iter).name.compare(name) == 0){
            return false;
        }
        ++iter;
    }
    
    std::string vServer = md5;
    std::string vClient = "";
    std::string vDownload = "";
    bool isOld=false;
//    auto iter0 = m_md5Map.find(name);
//    if(iter0!=m_md5Map.end()){
//        vClient = (*iter0).second;
//    }else{
        std::string keyclient = DynamicResource_Client_VersionKey + name;
        vClient = CCUserDefault::sharedUserDefault()->getStringForKey(keyclient.c_str());
        m_md5Map[name]=vClient;
        saveMD5Map();
//    }
//    auto iter1 = m_md5unzipMap.find(name);
//    if(iter1!=m_md5unzipMap.end()){
//        vDownload = (*iter1).second;
//    }else{
        std::string keydownload = DynamicResource_Download_VersionKey + name;
        vDownload = CCUserDefault::sharedUserDefault()->getStringForKey(keydownload.c_str());
        m_md5unzipMap[name]=vDownload;
        saveUnzipMD5Map();
//    }
    
    if(vClient.compare(vServer)!=0){
        if(vDownload.compare(vServer) == 0){
            if(!cocos2d::extension::CCDevice::hasEnoughSpace(needSize)){
                CCLOG("Do not has enough space to unzip DynamicResource file!");
                return false;
            }
            m_zipFile = m_writeablePath + DynamicResource_Zip + name + ".zip";
            if(checkZipMD5(m_zipFile, md5) && unZipFile(false)){
                unzipFinish();
                return false;
            }
            std::remove(m_zipFile.c_str());
        }
    }else{
        if(checkResourceFile(name,platform)){
            return true;
        }
    }
    
    DynamicResource dr = {name,"",md5,md5,needSize,platform,DynamicResourceType_Name};
    mQueue.push_back(dr);
    popFromQueue();
    return false;
}
bool DynamicResourceController::hasVersionTypeResource(std::string name,std::string newV,int needSize,std::string md5){
    
    auto iter = mQueue.begin();
    while (iter!=mQueue.end()) {
        if((*iter).name.compare(name) == 0){
            return false;
        }
        ++iter;
    }
    
    std::string keyclient = DynamicResource_Client_VersionKey + name;
    std::string keydownload = DynamicResource_Download_VersionKey + name;
    
    std::string vClient = CCUserDefault::sharedUserDefault()->getStringForKey(keyclient.c_str());
    std::string vServer = newV;
    std::string vDownload = CCUserDefault::sharedUserDefault()->getStringForKey(keydownload.c_str());

    if(vClient.compare(vServer) != 0){
        if(vDownload.compare(vServer) == 0){
            if(!cocos2d::extension::CCDevice::hasEnoughSpace(needSize)){
                CCLOG("Do not has enough space to unzip DynamicResource file!");
                return false;
            }
            m_zipFile = m_writeablePath + DynamicResource_Zip + name + ".zip";
            if(checkZipMD5(m_zipFile, md5) && unZipFile(false)){
                unzipFinish();
                return false;
            }
            std::remove(m_zipFile.c_str());
        }
    }else{
        if(checkResourceFile(name)){
            return true;
        }
    }
    
    
    
    DynamicResource dr = {name,vClient,newV,md5,needSize,true,DynamicResourceType_Version};
    mQueue.push_back(dr);
    
    popFromQueue();
    return false;
}
bool DynamicResourceController::checkResourceFile(std::string name,bool platform){
    std::vector<std::string> filelist;
    std::string fileName1 = m_writeablePath + DynamicResource_Folder + name + ".plist";
    if(name == DynamicResource_MONSTER_TEXTURE){
        fileName1 = m_writeablePath + DynamicResource_Folder + "World_3.atlas";
    }else if(name == DynamicResource_THORNE_TEXTURE){
        fileName1 = m_writeablePath + DynamicResource_Folder + "Common_301.plist";
    } else if (CCCommonUtils::isPad() && DynamicResource_HD_RES == name)
    {
         fileName1 = m_writeablePath + DynamicResource_Folder + "Common/Common_101_hd.plist";
    }
    else if (CCCommonUtils::isPad() && DynamicResource_HD_CCB == name)
    {
        fileName1 = m_writeablePath + DynamicResource_Folder + "ccbi/gameUI_HD.ccbi";
    }
    
    filelist.push_back(fileName1);
   
    if(platform==true){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    std::string fileName2 = m_writeablePath + DynamicResource_Folder +"_alpha_" + name + ".pkm";
    std::string fileName3 = m_writeablePath + DynamicResource_Folder +"_alpha_" + name + "_alpha.pkm";
    if(name == DynamicResource_MONSTER_TEXTURE){
        fileName2 = m_writeablePath + DynamicResource_Folder + "_alpha_World_3.pkm";
        fileName3 = m_writeablePath + DynamicResource_Folder + "_alpha_World_3_alpha.pkm";
    }else if(name == DynamicResource_THORNE_TEXTURE){
        fileName2 = m_writeablePath + DynamicResource_Folder + "_alpha_Common_301.pkm";
        fileName3 = m_writeablePath + DynamicResource_Folder + "_alpha_Common_301_alpha.pkm";
    }
    else if(name == DynamicResource_HD_RES){
        fileName2 = m_writeablePath + DynamicResource_Folder + "Common/_alpha_Common_101_hd.pkm";
        fileName3 = m_writeablePath + DynamicResource_Folder + "Common/_alpha_Common_101_hd_alpha.pkm";
    }
    else if(name == DynamicResource_HD_CCB){
        fileName2 = m_writeablePath + DynamicResource_Folder + "hdccbi/gameUI_HD.ccbi";
        fileName3 = m_writeablePath + DynamicResource_Folder + "hdccbi/gameUI_HD.ccbi";
    }
    
    filelist.push_back(fileName2);
    filelist.push_back(fileName3);

#else
     std::string fileName2 = m_writeablePath + DynamicResource_Folder + name + ".pvr.ccz";
    if(name == DynamicResource_MONSTER_TEXTURE){
        fileName2 = m_writeablePath + DynamicResource_Folder + "World_3.pvr.ccz";
    }else if(name == DynamicResource_THORNE_TEXTURE){
        fileName2 = m_writeablePath + DynamicResource_Folder + "Common_301.pvr.ccz";
    }else if(name == DynamicResource_HD_RES){
        fileName2 = m_writeablePath + DynamicResource_Folder + "Common/Common_101_hd.pvr.ccz";
    }else if(name == DynamicResource_HD_CCB){
        fileName2 = m_writeablePath + DynamicResource_Folder + "hdccbi/gameUI_HD.ccbi";
    }

        filelist.push_back(fileName2);
#endif
    }else{
        std::string fileName2 = m_writeablePath + DynamicResource_Folder + name + ".pvr.ccz";
        if(name == DynamicResource_MONSTER_TEXTURE){
            fileName2 = m_writeablePath + DynamicResource_Folder + "World_3.pvr.ccz";
        }else if(name == DynamicResource_THORNE_TEXTURE){
            fileName2 = m_writeablePath + DynamicResource_Folder + "Common_301.pvr.ccz";
        }else if(name == DynamicResource_HD_RES){
            fileName2 = m_writeablePath + DynamicResource_Folder + "Common/Common_101_hd.pvr.ccz";
        }else if(name == DynamicResource_HD_CCB){
            fileName2 = m_writeablePath + DynamicResource_Folder + "hdccbi/gameUI_HD.ccbi";
        }
        filelist.push_back(fileName2);
    }
    auto iter = filelist.begin();
    while (iter!=filelist.end()) {
        if(!CCFileUtils::sharedFileUtils()->isFileExist((*iter))){
            return false;
        }
        ++iter;
    }
    return true;
}
void DynamicResourceController::popFromQueue(){
    if (false == get_is_download_flag())
    {
        mQueue.clear();
        return;
    }
    if(m_isDownloading==false && mQueue.size()>0){
        mDownLoadDR = mQueue.at(0);
//        m_zipFile = m_writeablePath +mDownLoadDR.name+"tmp.zip";
        m_zipFile = m_writeablePath + DynamicResource_Zip + mDownLoadDR.name+".zip";
        startDownload();
    }
}
void* DynamicResourceFileDownloadAndUncompress(void *data)
{
    DynamicResourceController* self = (DynamicResourceController*)data;
    if (!self) {
        return NULL;
    }
    
    do
    {
        if (!self->downloadFile()) {
            self->dealwithError();
            break;
        }
        self->downloadFinish();
        // Uncompress zip file.
        if (!self->unZipFile()){
            self->dealwithError();
            break;
        }
        self->unzipFinish();
        
    } while (0);
    
    if (self->m_tid)
    {
        delete self->m_tid;
        self->m_tid = NULL;
    }
    return NULL;
}
static size_t downLoadDynamicResourcePackage(void *ptr, size_t size, size_t nmemb, void *userdata){
    FILE *fp = (FILE*)userdata;
    size_t written = fwrite(ptr, size, nmemb, fp);
    return written;
}
int DynamicResourceDownloadProgressFunc(void *ptr, double totalToDownload, double nowDownloaded, double totalToUpLoad, double nowUpLoaded){
    auto percent = (int)(nowDownloaded/totalToDownload*100);
    percent = percent < 0 ? 0 : percent;
//    CCLOG("DynamicResource___downloading... %d%%", percent);
    return 0;
}
void DynamicResourceController::startDownload(){
    if(m_isDownloading==false){
        std::string platform="";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        platform = "android";
#else
        platform = "ios";
#endif
        m_isDownloading = true;
        if(platform.empty() || !cocos2d::extension::CCDevice::hasEnoughSpace(mDownLoadDR.needSize)){
            finishAndtryNext();
            return;
        }
        // COCOS2D_DEBUG : 0是外网,1是内网,2是beta
#if COCOS2D_DEBUG == 1
        string hostIP = CCUserDefault::sharedUserDefault()->getStringForKey(ACCOUNT_IP);
        CCLOG("lua_host_url [%s]",hostIP.c_str());
        if (hostIP.empty()) {
            hostIP = SERVER_IP;
        }
        if(mDownLoadDR.type == DynamicResourceType_Version){
            m_packagePath = "http://"+hostIP+":8080/gameservice/getfile?drVersion=" + mDownLoadDR.name + mDownLoadDR.newV;
        }else{
            m_packagePath = "http://"+hostIP+":8080/gameservice/getfile?drVersion=" + mDownLoadDR.name;
        }
        if(mDownLoadDR.platform==true){
            m_packagePath.append("_");
            m_packagePath.append(platform);
        }
#else
        m_packagePath = "http://cok.eleximg.com/cok/lua/dr_" + mDownLoadDR.name + mDownLoadDR.newV;
        if(mDownLoadDR.platform==true){
            m_packagePath.append("_");
            m_packagePath.append(platform);
        }
        m_packagePath.append(".zip");
#endif
        CCLOG("DynamicResource_package_url [%s]",m_packagePath.c_str());
        
        m_tid = new pthread_t();
        pthread_create(&(*m_tid), NULL, DynamicResourceFileDownloadAndUncompress, this);
    }
}
bool DynamicResourceController::checkZipMD5(string zipfile, string md5str){
    std::ifstream ifs;
    ifs.open(m_zipFile);
    MD5 md5;
    md5.update(ifs);
    std::string zipStr = md5.toString();
    std::string checkStr = md5str;
    if(zipStr.compare(checkStr) != 0){
        CCLOG("zip file %s is not correct. check code [%s], download code [%s]",m_zipFile.c_str(),checkStr.c_str(),zipStr.c_str());
        return false;
    }
    return true;
}
bool DynamicResourceController::unZipFile(bool checkMD5){
    if(!CCFileUtils::sharedFileUtils()->isFileExist(m_zipFile)){
        CCLOG("zip file %s is not exit",m_zipFile.c_str());
        return false;
    }
    if(checkMD5){
        checkZipMD5(m_zipFile,mDownLoadDR.md5);
    }
    unzFile zipfile = unzOpen(m_zipFile.c_str());
    if (! zipfile)
    {
        CCLOG("can not open downloaded zip file %s", m_zipFile.c_str());
        return false;
    }
    
    // Get info about the zip file
    unz_global_info global_info;
    if (unzGetGlobalInfo(zipfile, &global_info) != UNZ_OK)
    {
        CCLOG("can not read file global info of %s", m_zipFile.c_str());
        unzClose(zipfile);
        return false;
    }
    
    // Buffer to hold data read from the zip file
    char readBuffer[DynamicResource_BUFFER_SIZE];
    
    CCLOG("start uncompressing");
    
    // Loop to extract all files.
    uLong i;
    for (i = 0; i < global_info.number_entry; ++i)
    {
        // Get info about current file.
        unz_file_info fileInfo;
        char fileName[DynamicResource_MAX_FILENAME];
        if (unzGetCurrentFileInfo(zipfile,
                                  &fileInfo,
                                  fileName,
                                  DynamicResource_MAX_FILENAME,
                                  NULL,
                                  0,
                                  NULL,
                                  0) != UNZ_OK)
        {
            CCLOG("can not read file info");
            unzClose(zipfile);
            return false;
        }
        
        string fullPath = m_writeablePath + fileName;
        
        // Check if this entry is a directory or a file.
        const size_t filenameLength = strlen(fileName);
        if (fileName[filenameLength-1] == '/')
        {
            // Entry is a direcotry, so create it.
            // If the directory exists, it will failed scilently.
            if (!createDirectory(fullPath.c_str()))
            {
                CCLOG("can not create directory %s", fullPath.c_str());
                unzClose(zipfile);
                return false;
            }
        }
        else
        {
            // Entry is a file, so extract it.
            
            // Open current file.
            if (unzOpenCurrentFile(zipfile) != UNZ_OK)
            {
                CCLOG("can not open file %s", fileName);
                unzClose(zipfile);
                return false;
            }
            
            // Create a file to store current file.
            FILE *out = fopen(fullPath.c_str(), "w+");
            if (! out)
            {
                CCLOG("can not open destination file %s", fullPath.c_str());
                unzCloseCurrentFile(zipfile);
                unzClose(zipfile);
                return false;
            }
            
            // Write current file content to destinate file.
            int error = UNZ_OK;
            do
            {
                error = unzReadCurrentFile(zipfile, readBuffer, DynamicResource_BUFFER_SIZE);
                if (error < 0)
                {
                    CCLOG("can not read zip file %s, error code is %d", fileName, error);
                    unzCloseCurrentFile(zipfile);
                    unzClose(zipfile);
                    return false;
                }
                
                if (error > 0)
                {
                    fwrite(readBuffer, error, 1, out);
                }
            } while(error > 0);
            
            fclose(out);
        }
        
        unzCloseCurrentFile(zipfile);
        
        // Goto next entry listed in the zip file.
        if ((i+1) < global_info.number_entry)
        {
            if (unzGoToNextFile(zipfile) != UNZ_OK)
            {
                CCLOG("can not read next file");
                unzClose(zipfile);
                return false;
            }
        }
    }
    CCLOG("end uncompressing");
    return true;
}
bool DynamicResourceController::createDirectory(const char *path){
    mode_t processMask = umask(0);
    int ret = mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO);
    umask(processMask);
    if (ret != 0 && (errno != EEXIST))
    {
        return false;
    }
    
    return true;
}

bool DynamicResourceController::downloadFile(){
    m_curl = curl_easy_init();
    if (! m_curl)
    {
        CCLOG("can not init curl");
        return false;
    }
    if(m_packagePath.empty())
        return false;
    try {
        // Create a file to save package.
        CCLOG("lua_package_save_path [%s]",m_zipFile.c_str());
        FILE *fp = fopen(m_zipFile.c_str(), "w+");
        if (! fp){
            CCLOG("can not create file %s", m_zipFile.c_str());
            return false;
        }
        
        // Download pacakge
        CURLcode res;
        curl_easy_setopt(m_curl, CURLOPT_URL, m_packagePath.c_str());
        curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, downLoadDynamicResourcePackage);
        curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, fp);
        curl_easy_setopt(m_curl, CURLOPT_NOPROGRESS, false);
        curl_easy_setopt(m_curl, CURLOPT_PROGRESSFUNCTION, DynamicResourceDownloadProgressFunc);
        curl_easy_setopt(m_curl, CURLOPT_PROGRESSDATA, this);
        res = curl_easy_perform(m_curl);
        curl_easy_cleanup(m_curl);
        if (res != 0)
        {
            CCLOG("error when download package");
            fclose(fp);
            return false;
        }
        
        CCLOG("succeed downloading package %s", m_packagePath.c_str());
        
        fclose(fp);
        return true;
    } catch (...) {
        CCLog("curl error when download package");
    }
    return false;
}
void DynamicResourceController::downloadFinish(){
//    if(mDownLoadDR.type == DynamicResourceType_Version){
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(DynamicResourceController::saveDownloadVersion), this, 1, 0, 0, false);
//    }else if(mDownLoadDR.type == DynamicResourceType_Name){
    
//    }
}
void DynamicResourceController::unzipFinish(){
//    if(mDownLoadDR.type == DynamicResourceType_Version){
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(DynamicResourceController::saveClientVersion), this, 1, 0, 0, false);
//    }else if(mDownLoadDR.type == DynamicResourceType_Name){
//        std::remove(m_zipFile.c_str());
//        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(DynamicResourceController::sendNotification), this, 2, 0, 0, false);
//    }
}
void DynamicResourceController::saveDownloadVersion(float fUpdate){
    std::string keydownload = DynamicResource_Download_VersionKey + mDownLoadDR.name;
    if(mDownLoadDR.type == DynamicResourceType_Version){
        CCUserDefault::sharedUserDefault()->setStringForKey(keydownload.c_str(), mDownLoadDR.newV);
    }else if (mDownLoadDR.type == DynamicResourceType_Name){
        CCUserDefault::sharedUserDefault()->setStringForKey(keydownload.c_str(), mDownLoadDR.md5);
        m_md5unzipMap[mDownLoadDR.name]=mDownLoadDR.md5;
        saveUnzipMD5Map();
    }else{
        if(mDownLoadDR.name.find("sound") != string::npos){
            CCUserDefault::sharedUserDefault()->setStringForKey(keydownload.c_str(), mDownLoadDR.newV);
            m_md5unzipMap[mDownLoadDR.name]=mDownLoadDR.newV;
            saveUnzipMD5Map();
        }
    }
    
    if (mDownLoadDR.name == DynamicResource_HD_CCB)  
    {
        CCUserDefault::sharedUserDefault()->setBoolForKey(DOWNLOAD_HD_CCB, true);
    }
    if (mDownLoadDR.name == DynamicResource_HD_RES)
    {
        CCUserDefault::sharedUserDefault()->setBoolForKey(DOWNLOAD_HD_RES, true);
    }
    CCUserDefault::sharedUserDefault()->flush();
}
void DynamicResourceController::saveClientVersion(float fUpdate){
    std::string keyclient = DynamicResource_Client_VersionKey + mDownLoadDR.name;
    std::string keydownload = DynamicResource_Download_VersionKey + mDownLoadDR.name;
    if(mDownLoadDR.type == DynamicResourceType_Version){
        CCUserDefault::sharedUserDefault()->setStringForKey(keyclient.c_str(), mDownLoadDR.newV);
        CCUserDefault::sharedUserDefault()->setStringForKey(keydownload.c_str(), "");
    }else if (mDownLoadDR.type == DynamicResourceType_Name){
        CCUserDefault::sharedUserDefault()->setStringForKey(keyclient.c_str(), mDownLoadDR.md5);
        CCUserDefault::sharedUserDefault()->setStringForKey(keydownload.c_str(), "");
        m_md5unzipMap[mDownLoadDR.name]="";
        saveUnzipMD5Map();
        m_md5Map[mDownLoadDR.name]=mDownLoadDR.md5;
        saveMD5Map();
    }else{
        if(mDownLoadDR.name.find("sound") != string::npos){
            CCUserDefault::sharedUserDefault()->setStringForKey(keyclient.c_str(), mDownLoadDR.newV);
            CCUserDefault::sharedUserDefault()->setStringForKey(keydownload.c_str(), "");
            m_md5unzipMap[mDownLoadDR.name]="";
            saveUnzipMD5Map();
            m_md5Map[mDownLoadDR.name]=mDownLoadDR.newV;
            saveMD5Map();
        }
    }
    if(mDownLoadDR.name.find("Common")!=string::npos){
        auto pos = mDownLoadDR.name.find("_");
        if(pos != string::npos){
            int commonidx = atoi(mDownLoadDR.name.substr(pos+1).c_str());
            saveCommonStatus(commonidx,true);
        }
    }
    
    CCUserDefault::sharedUserDefault()->flush();
    std::remove(m_zipFile.c_str());
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(DynamicResourceController::sendNotification), this, 2, 0, 0, false);
}
void DynamicResourceController::sendNotification(float t){
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_DYNAMIC_DOWNLOAD_FINISH,CCString::create(mDownLoadDR.name));
    finishAndtryNext();
}
void DynamicResourceController::finishAndtryNext(float t){
    m_isDownloading = false;
    m_zipFile="";
    m_packagePath="";
    mDownLoadDR= {"","","","",0,false,DynamicResourceType_None};
    if(mQueue.size()>0){
        mQueue.erase(mQueue.begin());
        popFromQueue();
    }
}
void DynamicResourceController::dealwithError(){
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(DynamicResourceController::finishAndtryNext), this, 2, 0, 0, false);
}
void DynamicResourceController::loadNameTypeResource(std::string name, bool release){
    std::string fileName = m_writeablePath + DynamicResource_Folder + name + ".plist";
    CCLOG("android_test_loadNameTypeResourceName[%s]",fileName.c_str());
    
    if(!CCFileUtils::sharedFileUtils()->isFileExist(fileName)){
         CCLog("android_test_loadNameTypeResourceName[%s]_file_not_exite",fileName.c_str());
        return;
    }
    
    if (release == false)
    {
        cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(fileName.c_str());
        CCLoadSprite::retain_texture(fileName);
    }
    else if (CCLoadSprite::release_texture(fileName))
    {
        cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(fileName.c_str());
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        std::string textName1 = m_writeablePath + DynamicResource_Folder +"_alpha_" + name + ".pkm";
        CCTexture2D *texture1 =  CCTextureCache::sharedTextureCache()->textureForKey(textName1.c_str());
        if (texture1)
        {
            CCTextureCache::sharedTextureCache()->removeTexture(texture1);
            CCLog("DynamicResource_texutre_remove:[%s]",textName1.c_str());
        }
//        std::string textName2 = m_writeablePath + DynamicResource_Folder +"_alpha_" + name + "_alpha.pkm";
//        CCTexture2D *texture2 =  CCTextureCache::sharedTextureCache()->textureForKey(textName2.c_str());
//        if(texture2){
//            CCTextureCache::sharedTextureCache()->removeTexture(texture2);
//            CCLog("DynamicResource_texutre_remove:[%s]",textName2.c_str());
//        }
#else
        std::string textName = m_writeablePath + DynamicResource_Folder + name + ".pvr.ccz";
        CCTexture2D *texture =  CCTextureCache::sharedTextureCache()->textureForKey(textName.c_str());
        if (texture)
        {
            CCTextureCache::sharedTextureCache()->removeTexture(texture);
        }
#endif
        
    }
}

void DynamicResourceController::initMD5Map(){
    if(m_md5Map.size()>0)
        return;
    std::string oVersionFile = m_writeablePath + DynamicResource_Folder +DynamicMD5_Version;
    std::string oVersionFile1 = m_writeablePath + DynamicResource_Folder +DynamicMD5Unzip_Version;
    if(CCFileUtils::sharedFileUtils()->isFileExist(oVersionFile)){
        ifstream ifile(oVersionFile);
        string line;
        vector<string> mVector;
        while(getline(ifile,line)){
            if(!line.empty()){
                mVector.push_back(line);
            }
        }
        ifile.close();
        auto iter = mVector.begin();
        while (iter!=mVector.end()) {
            string tmpStr = (*iter);
            if(!tmpStr.empty() && tmpStr.find("=")!=string::npos){
                vector<std::string> mTmpV;
                CCCommonUtils::splitString(tmpStr, "=", mTmpV);
                if(mTmpV.size()>1){
                    m_md5Map[mTmpV.at(0)]=mTmpV.at(1);
                }
            }
            ++iter;
        }
    }
    if(CCFileUtils::sharedFileUtils()->isFileExist(oVersionFile1)){
        ifstream ifile(oVersionFile1);
        string line;
        vector<string> mVector;
        while(getline(ifile,line)){
            if(!line.empty()){
                mVector.push_back(line);
            }
        }
        ifile.close();
        auto iter = mVector.begin();
        while (iter!=mVector.end()) {
            string tmpStr = (*iter);
            if(!tmpStr.empty() && tmpStr.find("=")!=string::npos){
                vector<std::string> mTmpV;
                CCCommonUtils::splitString(tmpStr, "=", mTmpV);
                if(mTmpV.size()>1){
                    m_md5unzipMap[mTmpV.at(0)]=mTmpV.at(1);
                }else if(mTmpV.size() == 1){
                    m_md5unzipMap[mTmpV.at(0)]="";
                }
            }
            ++iter;
        }
    }
}
void DynamicResourceController::saveMD5Map(){
    std::string oVersionFile = m_writeablePath + DynamicResource_Folder +DynamicMD5_Version;
//    if(CCFileUtils::sharedFileUtils()->isFileExist(oVersionFile)){
        ofstream ofile(oVersionFile,ofstream::out);
        auto iterMD5 = m_md5Map.begin();
        while (iterMD5!=m_md5Map.end()) {
            string writeStr =(*iterMD5).first + "=" + (*iterMD5).second;
            ofile<<writeStr<<endl;
            ++iterMD5;
        }
        ofile.close();
//    }
}
void DynamicResourceController::saveUnzipMD5Map(){
    std::string oVersionFile = m_writeablePath + DynamicResource_Folder +DynamicMD5Unzip_Version;
//    if(CCFileUtils::sharedFileUtils()->isFileExist(oVersionFile)){
        ofstream ofile(oVersionFile,ofstream::out);
        auto iterMD5 = m_md5unzipMap.begin();
        while (iterMD5!=m_md5unzipMap.end()) {
            string writeStr =(*iterMD5).first + "=" + (*iterMD5).second;
            ofile<<writeStr<<endl;
            ++iterMD5;
        }
        ofile.close();
//    }
}
string DynamicResourceController::getDownloadPath(){
    return m_writeablePath + DynamicResource_Folder;
}