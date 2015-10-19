//
//  NBSlider.h
//  clipTest
//
//  Created by fusheng on 15-10-16.
//
//

#ifndef __clipTest__NBSlider__
#define __clipTest__NBSlider__

#include "cocos2d.h"
#include "cocos-ext.h"


USING_NS_CC;
using namespace cocos2d::ui;

class NBSlider : public Node
{
   
    
    
public:
    enum class EventType
    {
        ON_PERCENTAGE_CHANGED
    };
    
    enum class TextureResType
    {
        LOCAL = 0,
        PLIST = 1
    };
    typedef std::function<void(Ref*,EventType)> nbSliderCallBack;
    
    NBSlider():m_minValue(0),m_maxValue(100),_eventCallback(nullptr),_textureResType(TextureResType::LOCAL),isMoved(false){}
    float getValue();
    
    void setValue(float value);
    
    void setMinimumValue(float minValue);
    
    void setMaximumValue(float maxValue);
    
    
    bool onTouchBegan(Touch *touch, Event *unusedEvent) ;
    void onTouchMoved(Touch *touch, Event *unusedEvent) ;
    void onTouchEnded(Touch *touch, Event *unusedEvent) ;
    void onTouchCancelled(Touch *touch, Event *unusedEvent) ;
    
    void addEventListener(const nbSliderCallBack& callback);
    

    static NBSlider* create(const std::string& backGroundTextureName,
                            const std::string& barTextureName,
                            const std::string& thumbTextureName,
                          TextureResType resType = TextureResType::LOCAL);
    
    bool init(const std::string& backGroundTextureName,
                    const std::string& barTextureName,
                    const std::string& thumbTextureName,
                    TextureResType resType = TextureResType::LOCAL
                    );

    virtual void setContentSize(const Size & var) override;
    
    void setCapInsets(const Rect &capInsets);
    
    void setCapInsetsBarRenderer(const Rect &capInsets);
    
    void setCapInsetProgressBarRebderer(const Rect &capInsets);
private:
    
    float m_minValue;
    
    float m_maxValue;
    
    float _value;
    
    Scale9Sprite*  _barRenderer;
    Scale9Sprite* _progressBarRenderer;
    
    Sprite* _thumbRenderer;
    
    Rect _capInsetsBarRenderer;
    Rect _capInsetsProgressBarRenderer;
    
    
    nbSliderCallBack _eventCallback;
    
    TextureResType _textureResType;
    
    
    float _thumbMaxLength;
    
    bool isMoved;

};


#endif /* defined(__clipTest__NBSlider__) */
