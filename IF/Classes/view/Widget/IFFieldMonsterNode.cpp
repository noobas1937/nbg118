//
//  IFFieldMonsterNode.cpp
//  IF
//
//  Created by lihua on 15/2/2.
//
//

#include "IFFieldMonsterNode.h"
#include "WorldMapView.h"
#include "DynamicResourceController.h"

bool IFFieldMonsterNode::init()
{
	animationObj = NULL;
	return true;
}

IFFieldMonsterNode::IFFieldMonsterNode()
:m_skeName("")
,m_aniName("")
,m_entry(nullptr)
{
}

IFSkeletonAnimation* IFFieldMonsterNode::setSpineState(string skeName, int state, int direction, bool repeat/*=true*/,CCCallFunc* callFunc/*=nullptr*/,int loopCnt /*= 1*/)
{
    m_skeName = skeName;
    m_state = state;
    m_loopCnt = loopCnt;
    //MonsterDead
    switch (state)
    {
        case MonsterBreath:
            m_aniName = "dj";
            break;
        case MonsterAttack:
        {
            m_aniName = "gj";
            if(m_skeName == "scws") {
                m_aniName = "gj";
            }
            break;
        }
        case MonsterDead:
            m_aniName = "sw";
            break;
        default:
            break;
    }
    
    if(!animationObj)
    {
        std::string strJsonFileName = getJsonPath(m_skeName);
        std::string strAtlasFilename = getAtlasPath();
        
        if (!isSkeExit(m_skeName) || !isSkeTextureExit())
        {
            return nullptr;
        }
        
        animationObj = new IFSkeletonAnimation(strJsonFileName.c_str(), strAtlasFilename.c_str());
        addChild(animationObj);
    }
    else
    {
        animationObj->setToSetupPose();
    }
    
    m_entry = animationObj->setAnimation(0,m_aniName.c_str(), repeat);
    if(m_entry){
        animationObj->setTimeScale(m_entry->endTime/1.0f);
        if(callFunc)
        {
            CC_SAFE_RELEASE_NULL(m_callBack);
            m_callBack = callFunc;
            CC_SAFE_RETAIN(m_callBack);
            animationObj->setCompleteListener( [&] (int trackIndex, int loopCount)
            {
                CC_SAFE_RETAIN(this);
                animationStateEvent(trackIndex,loopCount);
                CC_SAFE_RELEASE(this);
            });
        }
    }
    
    return animationObj;
}

std::string IFFieldMonsterNode::getAtlasPath(){
    std::string localPath = CCFileUtils::sharedFileUtils()->getWritablePath() + DynamicResource_Folder;
    std::string localAtlasPath = localPath + "World_3.atlas";
    if(!CCFileUtils::sharedFileUtils()->isFileExist(localAtlasPath)){
        localAtlasPath = "World/World_3.atlas";
    }
    return localAtlasPath;
}

std::string IFFieldMonsterNode::getJsonPath(std::string jsonName){
    std::string localPath = CCFileUtils::sharedFileUtils()->getWritablePath() + DynamicResource_Folder;
    std::string localAtlasPath = localPath + "World_3.atlas";
    std::string jsonPath = localPath + jsonName + ".json";
    if(!CCFileUtils::sharedFileUtils()->isFileExist(localAtlasPath)){
        jsonPath = CCString::createWithFormat("Spine/World/%s.json", jsonName.c_str())->getCString();
    }
    return jsonPath;
}

bool IFFieldMonsterNode::isSkeExit(string skeName)
{
    std::string localPath = CCFileUtils::sharedFileUtils()->getWritablePath() + DynamicResource_Folder;
    std::string localAtlasPath = localPath + "World_3.atlas";
    if(CCFileUtils::sharedFileUtils()->isFileExist(localAtlasPath)){
        std::string jsonPath = localPath + skeName + ".json";
        if (!CCFileUtils::sharedFileUtils()->isFileExist(jsonPath))
        {
            return false;
        }
    }else{
        char strJsonFileName[256] = {0};
        char strAtlasFilename[256] = {0};
        sprintf(strJsonFileName, "Spine/World/%s.json", skeName.c_str());
        if (!CCFileUtils::sharedFileUtils()->isFileExist(strJsonFileName))
        {
            return false;
        }
    }
    
    return true;
}

bool IFFieldMonsterNode::isSkeTextureExit(){
    std::string localPath = CCFileUtils::sharedFileUtils()->getWritablePath() + DynamicResource_Folder;
    std::string localAtlasPath = localPath + "World_3.atlas";
    if(CCFileUtils::sharedFileUtils()->isFileExist(localAtlasPath)){
        return true;
    }else{
        if (!CCFileUtils::sharedFileUtils()->isFileExist("World/World_3.atlas"))
        {
            return false;
        }
    }
    
    return true;

}

float IFFieldMonsterNode::getloopOneTime()
{
    if(m_entry)
    {
        return m_entry->endTime;
    }
    return 1.5f;
}

void IFFieldMonsterNode::animationStateEvent(int trackIndex, int loopCount)
{
    spTrackEntry* entry = spAnimationState_getCurrent(animationObj->getState(), trackIndex);
    if(m_callBack && entry)
    {
        if(loopCount == m_loopCnt)
        {
            m_callBack->execute();
            CC_SAFE_RELEASE_NULL(m_callBack);
        }
    }
}

IFFieldMonsterNode::~IFFieldMonsterNode()
{
	CC_SAFE_RELEASE_NULL(animationObj);
}