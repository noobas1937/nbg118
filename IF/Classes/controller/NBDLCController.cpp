//
//  NBDLCController.cpp
//  IF
//
//  Created by gj on 11/13/15.
//
//

#include "NBDLCController.hpp"

NBDLCController* NBDLCController::create(string manifest_file_path)
{
    auto p = new NBDLCController();
    p->m_dlcPath = FileUtils::getInstance()->getWritablePath() + "dlc";
    p->m_serverAssetsPath = p->m_dlcPath; // 文件下载解压后存放的目录
    FileUtils::getInstance()->addSearchPath(p->m_serverAssetsPath, true); // 查找文件时优先搜索从服务器下载下来的文件
    p->m_pAssetsManagerEx = AssetsManagerEx::create(manifest_file_path, p->m_dlcPath);
    p->m_pAssetsManagerEx->retain();
    
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

void NBDLCController::start()
{
    if (false == m_pAssetsManagerEx->getLocalManifest()->isLoaded())
    {
        CCLOG("NBDLCController : 没有有效的配置文件");
        onCompleted();
    }
    else
    {
        auto listener = EventListenerAssetsManagerEx::create(m_pAssetsManagerEx, [this](EventAssetsManagerEx * event){
            auto eventCode = event->getEventCode();
            auto message = event->getMessage();
            auto assetId = event->getAssetId();
            CCLOG("NBDLCController : eventCode:%d, message:%s, assetId:%s", eventCode, event->getMessage().c_str(), assetId.c_str());
            
            if (eventCode == EventAssetsManagerEx::EventCode::ERROR_NO_LOCAL_MANIFEST)
            {
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
                onCompleted();
            }
            else if (eventCode == EventAssetsManagerEx::EventCode::ALREADY_UP_TO_DATE)
            {
                CCLOG("    NBDLCController : already up to date");
                onCompleted();
            }
            else if (eventCode == EventAssetsManagerEx::EventCode::UPDATE_FINISHED)
            {
                CCLOG("    NBDLCController : update finished");
                onCompleted();
            }
            else if (eventCode == EventAssetsManagerEx::EventCode::ERROR_UPDATING)
            {
                CCLOG("    NBDLCController : downloading %s failed, error:%s", assetId.c_str(), event->getMessage().c_str());
                onCompleted();
            }
        });
        Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);
        m_pAssetsManagerEx->update();
    }
}

void NBDLCController::onCompleted()
{
    
}
