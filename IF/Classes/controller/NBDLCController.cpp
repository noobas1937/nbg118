//
//  NBDLCController.cpp
//  IF
//
//  Created by gj on 11/13/15.
//
//

#include "NBDLCController.hpp"
#include "../Ext/CCDevice.h"

static string current_dlc_path = "";

NBDLCController* NBDLCController::create(string manifest_file_path,
                                         string version_filename,
                                         string temp_manifest_filename,
                                         string manifest_filename)
{
    auto p = new NBDLCController();
    p->m_dlcPath = FileUtils::getInstance()->getWritablePath() + "dlc" + cocos2d::extension::CCDevice::getDLCVersionName();
    p->m_serverAssetsPath = p->m_dlcPath; // 文件下载解压后存放的目录
    if (current_dlc_path != p->m_serverAssetsPath) FileUtils::getInstance()->addSearchPath(p->m_serverAssetsPath, true); // 查找文件时优先搜索从服务器下载下来的文件
    current_dlc_path = p->m_serverAssetsPath;
    p->m_pAssetsManagerEx = AssetsManagerEx::create(manifest_file_path, p->m_dlcPath, version_filename, temp_manifest_filename, manifest_filename);
    p->m_pAssetsManagerEx->retain();
    
    CCLOG("NBDLCController : manifest_file_path %s", manifest_file_path.c_str());
    CCLOG("NBDLCController : DLC path %s", p->m_dlcPath.c_str());
    CCLOG("NBDLCController : 文件下载解压后存放的目录 %s", p->m_serverAssetsPath.c_str());
    
    return p;
}

NBDLCController::NBDLCController()
: m_pAssetsManagerEx(nullptr)
{
    
}

NBDLCController::~NBDLCController()
{
    CC_SAFE_RELEASE_NULL(m_pAssetsManagerEx);
}

void NBDLCController::start(string manifest_file_path_as_key, std::function<void(string, EventAssetsManagerEx*)> callback)
{
    if (false == m_pAssetsManagerEx->getLocalManifest()->isLoaded())
    {
        CCLOG("NBDLCController : 没有有效的配置文件");
        
        if (callback)
        {
            callback(manifest_file_path_as_key, nullptr);
        }
        onCompleted();
    }
    else
    {
        auto listener = EventListenerAssetsManagerEx::create(m_pAssetsManagerEx, [this, manifest_file_path_as_key, callback](EventAssetsManagerEx * event){
            auto eventCode = event->getEventCode();
            auto message = event->getMessage();
            auto assetId = event->getAssetId();
            CCLOG("NBDLCController : eventCode:%d, message:%s, assetId:%s", eventCode, event->getMessage().c_str(), assetId.c_str());
            
            if (eventCode == EventAssetsManagerEx::EventCode::ERROR_NO_LOCAL_MANIFEST)
            {
                if (callback)
                {
                    callback(manifest_file_path_as_key, event);
                }
                onCompleted();
            }
            else if (eventCode == EventAssetsManagerEx::EventCode::UPDATE_PROGRESSION)
            {
                if (assetId == AssetsManagerEx::VERSION_ID)
                {
                    CCLOG("    NBDLCController : %s", "check new version");
                }
                else if (assetId == AssetsManagerEx::MANIFEST_ID)
                {
                    CCLOG("    NBDLCController : downloading manifest");
                }
                else
                {
                    CCLOG("    NBDLCController : downloading assetId:%s, percent:%.2f%%", assetId.c_str(), event->getPercent());
                }
            }
            else if (eventCode == EventAssetsManagerEx::EventCode::ERROR_DOWNLOAD_MANIFEST || eventCode == EventAssetsManagerEx::EventCode::ERROR_PARSE_MANIFEST)
            {
                CCLOG("    NBDLCController : downloading manifest failed");
                if (callback)
                {
                    callback(manifest_file_path_as_key, event);
                }
                onCompleted();
            }
            else if (eventCode == EventAssetsManagerEx::EventCode::ALREADY_UP_TO_DATE)
            {
                CCLOG("    NBDLCController : already up to date");
                if (callback)
                {
                    callback(manifest_file_path_as_key, event);
                }
                onCompleted();
            }
            else if (eventCode == EventAssetsManagerEx::EventCode::UPDATE_FINISHED)
            {
                CCLOG("    NBDLCController : update finished");
                if (callback)
                {
                    callback(manifest_file_path_as_key, event);
                }
                onCompleted();
            }
            else if (eventCode == EventAssetsManagerEx::EventCode::ERROR_UPDATING)
            {
                CCLOG("    NBDLCController : downloading %s failed, error:%s", assetId.c_str(), event->getMessage().c_str());
                if (callback)
                {
                    callback(manifest_file_path_as_key, event);
                }
                onCompleted();
            }
        });
        Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);
        m_pAssetsManagerEx->update();
    }
}

void NBDLCController::onCompleted()
{
    release();
}
