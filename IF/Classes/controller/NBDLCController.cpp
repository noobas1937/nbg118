//
//  NBDLCController.cpp
//  IF
//
//  Created by gj on 11/13/15.
//
//

#include "NBDLCController.hpp"
#include "../Ext/CCDevice.h"
#include <map>

static map<string, bool> current_dlc_paths;

void NBDLCController::clean()
{
    current_dlc_paths.clear();
}

NBDLCController* NBDLCController::create(string manifest_file_path,
                                         string version_filename,
                                         string temp_manifest_filename,
                                         string manifest_filename)
{
    string dlc_path = FileUtils::getInstance()->getWritablePath() + "dlc_" + /*cocos2d::extension::CCDevice::getDLCVersionName() + "_" +*/ manifest_filename;
    auto it = current_dlc_paths.find(dlc_path);
    if (it == current_dlc_paths.end()) FileUtils::getInstance()->addSearchPath(dlc_path, true); // 查找文件时优先搜索从服务器下载下来的文件
    current_dlc_paths[dlc_path] = true;
    
    if (FileUtils::getInstance()->isFileExist(dlc_path + "/" + manifest_filename))
    {
        manifest_file_path = dlc_path + "/" + manifest_filename;
    }
    
    auto p = new NBDLCController();
    p->m_pAssetsManagerEx = AssetsManagerEx::create(manifest_file_path, dlc_path, version_filename, temp_manifest_filename, manifest_filename);
    p->m_pAssetsManagerEx->retain();
    
    CCLOG("NBDLCController : manifest_file_path %s", manifest_file_path.c_str());
    CCLOG("NBDLCController : DLC path %s", dlc_path.c_str());
    
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
        CCLOG("NBDLCController : 没有有效的配置文件 %s", manifest_file_path_as_key.c_str());
        
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
            CCLOG("NBDLCController : %s, eventCode:%d, message:%s, assetId:%s", manifest_file_path_as_key.c_str(), eventCode, event->getMessage().c_str(), assetId.c_str());
            
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
                    CCLOG("    NBDLCController : %s, %s", "check new version", manifest_file_path_as_key.c_str());
                }
                else if (assetId == AssetsManagerEx::MANIFEST_ID)
                {
                    CCLOG("    NBDLCController : downloading manifest %s", manifest_file_path_as_key.c_str());
                }
                else
                {
                    CCLOG("    NBDLCController : downloading %s assetId:%s, percent:%.2f%%", manifest_file_path_as_key.c_str(), assetId.c_str(), event->getPercent());
                }
            }
            else if (eventCode == EventAssetsManagerEx::EventCode::ERROR_DOWNLOAD_MANIFEST || eventCode == EventAssetsManagerEx::EventCode::ERROR_PARSE_MANIFEST)
            {
                CCLOG("    NBDLCController : downloading manifest failed, %s", manifest_file_path_as_key.c_str());
                if (callback)
                {
                    callback(manifest_file_path_as_key, event);
                }
                onCompleted();
            }
            else if (eventCode == EventAssetsManagerEx::EventCode::ALREADY_UP_TO_DATE)
            {
                CCLOG("    NBDLCController : already up to date %s", manifest_file_path_as_key.c_str());
                if (callback)
                {
                    callback(manifest_file_path_as_key, event);
                }
                onCompleted();
            }
            else if (eventCode == EventAssetsManagerEx::EventCode::UPDATE_FINISHED)
            {
                CCLOG("    NBDLCController : update finished %s", manifest_file_path_as_key.c_str());
                if (callback)
                {
                    callback(manifest_file_path_as_key, event);
                }
                onCompleted();
            }
            else if (eventCode == EventAssetsManagerEx::EventCode::ERROR_UPDATING)
            {
                CCLOG("    NBDLCController : downloading %s, %s failed, error:%s", manifest_file_path_as_key.c_str(), assetId.c_str(), event->getMessage().c_str());
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
