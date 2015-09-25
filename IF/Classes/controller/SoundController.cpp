//
//  SoundController.cpp
//  IF
//
//  Created by zhaohf on 13-8-30.
//
//

#include "SoundController.h"
#include "ChatServiceCocos2dx.h"
#include "DynamicResourceController.h"

USING_NS_CC;
USING_NS_CC_EXT;

SoundController* SoundController::m_sSound = NULL;
SoundController::~SoundController()
{
    _musicArr->removeAllObjects();
    _musicArr->release();
    _effectArr->removeAllObjects();
    _effectArr->release();
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
    m_sSound = NULL;
}

SoundController::SoundController() : _musicArr(NULL), _effectArr(NULL)
{

}

SoundController* SoundController::sharedSound()
{
    if (!m_sSound)
    {
        m_sSound = new SoundController();
        m_sSound->init();
    }
    
    return m_sSound;
}

void SoundController::init()
{
    initArr();
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
    loadMusicByName(Music_Begin_Music);
    setMusicVolume(MUSIC_DEFAULT_VOLUME);
    setEffectsVolume(EFFECT_DEFAULT_VOLUME);
    m_writePath = CCFileUtils::sharedFileUtils()->getWritablePath();
}

void SoundController::initArr(){
    _musicArr = CCArray::create();
    _musicArr->retain();
    _effectArr = CCArray::create();
    _effectArr->retain();
    _musicArr->addObject(CCString::create(Music_M_city_1));
    _musicArr->addObject(CCString::create(Music_M_battle_1));
    
    _effectArr->addObject(CCString::create(Music_Sfx_city_mithril));
    _effectArr->addObject(CCString::create(Music_Sfx_city_iron));
    _effectArr->addObject(CCString::create(Music_Sfx_city_wood));
    _effectArr->addObject(CCString::create(Music_Sfx_city_farm));
    _effectArr->addObject(CCString::create(Music_Sfx_city_harvest_mithril));
    _effectArr->addObject(CCString::create(Music_Sfx_city_harvest_iron));
    _effectArr->addObject(CCString::create(Music_Sfx_city_harvest_wood));
    _effectArr->addObject(CCString::create(Music_Sfx_city_harvest_farm));
    _effectArr->addObject(CCString::create(Music_Sfx_harvest_soldier));
    _effectArr->addObject(CCString::create(Music_Sfx_city_train));
    _effectArr->addObject(CCString::create(Music_Sfx_city_building));
    _effectArr->addObject(CCString::create(Music_Sfx_city_market));
    _effectArr->addObject(CCString::create(Music_Sfx_city_hospital));
    _effectArr->addObject(CCString::create(Music_Sfx_city_military));
    _effectArr->addObject(CCString::create(Music_Sfx_city_wishing));
    _effectArr->addObject(CCString::create(Music_Sfx_city_walls));
    _effectArr->addObject(CCString::create(Music_Sfx_city_turret));
    _effectArr->addObject(CCString::create(Music_Sfx_city_watchtower));
    _effectArr->addObject(CCString::create(Music_Sfx_city_blacksmith));
    _effectArr->addObject(CCString::create(Music_Sfx_city_details));
    _effectArr->addObject(CCString::create(Music_Sfx_city_fortress));
    _effectArr->addObject(CCString::create(Music_Sfx_city_hall));
    _effectArr->addObject(CCString::create(Music_Sfx_city_depot));
    _effectArr->addObject(CCString::create(Music_Sfx_city_embassy));
    _effectArr->addObject(CCString::create(Music_Sfx_city_college));
    _effectArr->addObject(CCString::create(Music_Sfx_city_drill));
    _effectArr->addObject(CCString::create(Music_Sfx_city_chariot));
    _effectArr->addObject(CCString::create(Music_Sfx_city_range));
    _effectArr->addObject(CCString::create(Music_Sfx_city_stable));
    _effectArr->addObject(CCString::create(Music_Sfx_city_barrack));
    _effectArr->addObject(CCString::create(Music_Sfx_city_castle));
    _effectArr->addObject(CCString::create(Music_Sfx_city_complete));
    _effectArr->addObject(CCString::create(Music_Sfx_city_build));
    _effectArr->addObject(CCString::create(Music_Sfx_city_space));
    _effectArr->addObject(CCString::create(Music_Sfx_guide_speech));
    _effectArr->addObject(CCString::create(Music_Sfx_guide_attack_city));
    _effectArr->addObject(CCString::create(Music_Sfx_scene_change));
    _effectArr->addObject(CCString::create(Music_Sfx_world_click_space));
    _effectArr->addObject(CCString::create(Music_Sfx_world_click_attack));
    _effectArr->addObject(CCString::create(Music_Sfx_world_click_monster));
    _effectArr->addObject(CCString::create(Music_Sfx_world_attack));
    _effectArr->addObject(CCString::create(Music_Sfx_world_click_march));
    _effectArr->addObject(CCString::create(Music_Sfx_click_button));
    _effectArr->addObject(CCString::create(Music_Sfx_button_click_cancel));
    _effectArr->addObject(CCString::create(Music_Sfx_button_click_reward));
    _effectArr->addObject(CCString::create(Music_Sfx_UI_levelup));
    _effectArr->addObject(CCString::create(Music_Sfx_UI_collect_item));
    _effectArr->addObject(CCString::create(Music_Sfx_UI_secret));
    _effectArr->addObject(CCString::create(Music_Sfx_UI_drafting));
    _effectArr->addObject(CCString::create(Music_Sfx_UI_roulette));
    _effectArr->addObject(CCString::create(Music_Sfx_UI_shuffle));
    _effectArr->addObject(CCString::create(Music_Sfx_UI_loteryrwd));
    _effectArr->addObject(CCString::create(Music_Sfx_UI_putdown));
    _effectArr->addObject(CCString::create(Music_Sfx_UI_scroll));
    _effectArr->addObject(CCString::create(Music_Sfx_UI_jewel));
    _effectArr->addObject(CCString::create(Music_Sfx_UI_putonequipment));
    _effectArr->addObject(CCString::create(Music_Sfx_city_harvest_equipment));
    _effectArr->addObject(CCString::create(Music_Sfx_UI_blacksmith));
    _effectArr->addObject(CCString::create(Music_Sfx_UI_slide));
    _effectArr->addObject(CCString::create(Music_Sfx_UI_foodreward));
    _effectArr->addObject(CCString::create(Music_Sfx_UI_woodreward));
    _effectArr->addObject(CCString::create(Music_Sfx_city_harvest_bowman));
    _effectArr->addObject(CCString::create(Music_Sfx_city_harvest_cavalryman));
    _effectArr->addObject(CCString::create(Music_Sfx_city_harvest_infantry));
    _effectArr->addObject(CCString::create(Music_Sfx_city_harvest_chariot));
    _effectArr->addObject(CCString::create(Music_Sfx_UI_buildfinish));
}

void SoundController::update(float delta){
    if(!_effectArr){
        return;
    }
    if (_effectArr->count() > 0) {
        std::string effectStr = dynamic_cast<CCString*>(_effectArr->objectAtIndex(0))->getCString();
        loadEffectByName(effectStr);
        _effectArr->removeObjectAtIndex(0);
        return;
    }
//    if(_musicArr->count() > 0){
//        std::string musicStr = dynamic_cast<CCString*>(_musicArr->objectAtIndex(0))->getCString();
//        loadMusicByName(musicStr);
//        _musicArr->removeObjectAtIndex(0);
//        return;
//    }
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
}

std::string SoundController::getMusicFileName(const char * name)
{
    std::string musicFile;
    musicFile.append(MUSIC_PATH);
    musicFile.append(name);
    musicFile.append(MUSIC_EXT);
    
    std::string fullpath = m_writePath;
    fullpath.append(musicFile);
    
    if(CCFileUtils::sharedFileUtils()->isFileExist(fullpath)){
        musicFile = fullpath;
    }
    return musicFile;
}

std::string SoundController::getEffectFileName(const char * name)
{
    std::string effectFile;
    effectFile.append(EFFECTS_PATH);
    effectFile.append(name);
    effectFile.append(EFFECT_EXT);
    
    std::string fullpath = m_writePath;
    fullpath.append(effectFile);
    
    if(CCFileUtils::sharedFileUtils()->isFileExist(fullpath)){
        effectFile = fullpath;
    }
    return effectFile;
}

void SoundController::loadMusicByName(std::string name){
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(getMusicFileName(name.c_str()).c_str());
}

void SoundController::loadEffectByName(std::string name){
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(getEffectFileName(name.c_str()).c_str());
}

void SoundController::unloadEffects(const char * name)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->unloadEffect(getEffectFileName(name).c_str());
}

void SoundController::playBGMusic(const char * bgName)
{
    m_bgName = bgName;
    if (CCUserDefault::sharedUserDefault()->getBoolForKey(BG_MUSIC_ON, true))
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(getMusicFileName(bgName).c_str(), true);
    }
}

void SoundController::stopAllMusic()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
}

void SoundController::stopAllEffects()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();
}

void SoundController::setMusicVolume(float volume)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(volume);
}

void SoundController::setEffectsVolume(float volume)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(volume);
}

void SoundController::playEffects(const char * effectName)
{
    if (CCUserDefault::sharedUserDefault()->getBoolForKey(EFFECT_MUSIC_ON, true))
    {
        string fileName = getEffectFileName(effectName);
        if(!CCFileUtils::sharedFileUtils()->isFileExist(fileName)){
            DynamicResourceController::getInstance()->downloadSoundResource("effsound");
            return;
        }
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if(!ChatServiceCocos2dx::isChatShowing)
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(fileName.c_str());
#else
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(fileName.c_str());
#endif
    }
}

void SoundController::playEffects(const char *effectName, bool bLoop)
{
    if (CCUserDefault::sharedUserDefault()->getBoolForKey(EFFECT_MUSIC_ON,true))
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getEffectFileName(effectName).c_str(), bLoop);
    }
}

void SoundController::end()
{
    CocosDenshion::SimpleAudioEngine::end();
}
