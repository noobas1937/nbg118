//
//  SoundController.h
//  IF
//
//  Created by zhaohf on 13-8-30.
//
//

#ifndef __IF__SoundController__
#define __IF__SoundController__

//#include "CommonInclude.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "SoundDefine.h"
#include "audio/include/SimpleAudioEngine.h"
using namespace std;
USING_NS_CC;
USING_NS_CC_EXT;
#define BG_MUSIC_ON "isBGMusicOn"
#define EFFECT_MUSIC_ON "isEffectOn"

class SoundController : public cocos2d::CCObject
{
public:
    virtual ~SoundController();
    SoundController();
    static SoundController* sharedSound();

    std::string getMusicFileName(const char *);
    std::string getEffectFileName(const char *);
    
    void unloadEffects(const char *);
    
    void stopAllMusic();
    void stopAllEffects();
    
    void setMusicVolume(float volume);
    void setEffectsVolume(float volume);
    
    void playBGMusic(const char *);
    void playEffects(const char *);
    void playEffects(const char *, bool);
    void end();
    CC_SYNTHESIZE(string, m_bgName, BG_NAME);
    void update(float delta);
private:
    static SoundController* m_sSound;
    void init();
    void loadMusicByName(std::string name);
    void loadEffectByName(std::string name);

    
    CCArray *_musicArr;
    CCArray *_effectArr;
    void initArr();
    string m_writePath;
};


#endif /* defined(__IF__SoundController__) */
