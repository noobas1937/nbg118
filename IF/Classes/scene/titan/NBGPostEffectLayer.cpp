#include "NBGPostEffectLayer.h"


using namespace std;

NBGPostEffectLayer::NBGPostEffectLayer()
{
	p = nullptr;
}

NBGPostEffectLayer::~NBGPostEffectLayer()
{
    
}

NBGPostEffectLayer* NBGPostEffectLayer::create(PostEffectType effectType)
{
	NBGPostEffectLayer *pRet = new NBGPostEffectLayer();
	if (pRet && pRet->init(effectType))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool NBGPostEffectLayer::init(PostEffectType effectType)
{
    if ( !Layer::init() )
    {
        return false;
    }
	
	Size visibleSize = Director::getInstance()->getVisibleSize();
    
    string pixelShaderFile = "";
    string vertexShaderFile = "";
    
    if (effectType == PostEffectType::POST_EFFECT_BLUR)
    {
        string pixelShaderFile = "shaders/Blur.fsh";
        string vertexShaderFile = "shaders/Blur.vsh";
        p = GLProgram::createWithFilenames(vertexShaderFile, pixelShaderFile);
        auto glprogramstate = GLProgramState::getOrCreateWithGLProgram(p);
        glprogramstate->setUniformVec2("blurSize", Vec2(0.002, 0.002));
        //glprogramstate->setUniformVec2("blurSize", Vec2(1.0 / visibleSize.width, 1.0 / visibleSize.height));
    }
    
    renderTexture = RenderTexture::create(visibleSize.width, visibleSize.height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH_COMPONENT16);
    
    addChild(renderTexture);
    
    rendTexSprite = Sprite::create();
    rendTexSprite->setTexture(renderTexture->getSprite()->getTexture());
    rendTexSprite->setTextureRect(Rect(0, 0, rendTexSprite->getTexture()->getContentSize().width, rendTexSprite->getTexture()->getContentSize().height));
    rendTexSprite->setPosition(Point::ZERO);
    rendTexSprite->setAnchorPoint(Point::ZERO);
    rendTexSprite->setFlippedY(true);
    
    addChild(rendTexSprite);
    
    rendTexSprite->setGLProgram(p);
    
    renderTexture->setKeepMatrix(true);
    
    return true;
}


void NBGPostEffectLayer::visit(
	Renderer *renderer,
	const Mat4& parentTransform,
	uint32_t parentFlags)
{
	
    CameraFlag cameraFlag =  (Camera::getVisitingCamera()->getCameraFlag());
    
    if(cameraFlag == CameraFlag::USER4)
    {
        renderTexture->setGlobalZOrder(-1);
        renderTexture->beginWithClear(0, 0, 0, 0, 1);
    }
    else
    {
        renderTexture->setGlobalZOrder(-1);
        renderTexture->begin();
    }
    
	for (auto child : getChildren())
	{
		if (child != renderTexture && child != rendTexSprite)
        {
			child->visit(renderer, parentTransform, parentFlags);
        }
	}
    
    if(cameraFlag == CameraFlag::DEFAULT)
    {
        renderTexture->setGlobalZOrder(0);
        renderTexture->end();
        rendTexSprite->setCameraMask((unsigned short)(cameraFlag));
        rendTexSprite->visit(renderer, parentTransform, parentFlags);
    }
    else
    {
        renderTexture->setGlobalZOrder(0);
        renderTexture->end();
        
    }
    
   
}

