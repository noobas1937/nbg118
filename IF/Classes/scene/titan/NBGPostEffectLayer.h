#ifndef __NBGPostEffectLayer_H__
#define __NBGPostEffectLayer_H__

#include "cocos2d.h"
#include <string>
USING_NS_CC;

enum PostEffectType{
    POST_EFFECT_BLUR,
};

class NBGPostEffectLayer : public cocos2d::Layer
{
	
public:
	NBGPostEffectLayer();
	~NBGPostEffectLayer();
    using cocos2d::Layer::init;
    virtual bool init(PostEffectType effectType);
	static NBGPostEffectLayer* create(PostEffectType effectType);
	
	virtual void visit(
		Renderer *renderer,
		const Mat4& parentTransform,
		uint32_t parentFlags) override;

    
    
private:
    RenderTexture* renderTexture;
    Sprite* rendTexSprite;
    GLProgram* p;
    
};

#endif
