//
//  CCGraySprite.h
//  Cocos2dxExt
//
//  Created by zhaohf on 14-3-19.
//
//

#include "CCGraySprite.h"
CCGraySprite::CCGraySprite()
{
    
}

CCGraySprite::~CCGraySprite()
{
    
}

CCGraySprite* CCGraySprite::create( const char* pszFileName )
{
	CCGraySprite* pSprGrayScale = new CCGraySprite;
	if (pSprGrayScale && pSprGrayScale->initWithFile(pszFileName))
    {
		pSprGrayScale->autorelease();
	}else{
		CC_SAFE_RELEASE(pSprGrayScale);
	}
	return pSprGrayScale;
}

CCGraySprite* CCGraySprite::createWithSpriteFrameName(const char *pszSpriteFrameName)
{
    CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pszSpriteFrameName);
    char msg[256] = {0};
    sprintf(msg, "Invalid spriteFrameName: %s", pszSpriteFrameName);
    CCAssert(pFrame != NULL, msg);
    return createWithSpriteFrame(pFrame);
}

CCGraySprite* CCGraySprite::createWithSpriteFrame(CCSpriteFrame *pSpriteFrame)
{
    CCGraySprite *pobSprite = new CCGraySprite();
    if (pobSprite && pobSprite->initWithSpriteFrame(pSpriteFrame))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

bool CCGraySprite::initWithTexture( cocos2d::CCTexture2D* pTexture, const cocos2d::CCRect& tRect )
{
	do{
		CC_BREAK_IF(!CCSprite::initWithTexture(pTexture, tRect));
        bool isETC = false;
		CCGLProgram* pProgram = new CCGLProgram();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if (pTexture && pTexture->getAlphaName()) {
            isETC = true;
            GLchar* pszFragSource =
            "#ifdef GL_ES \n \
            precision mediump float; \n \
            #endif \n \
            varying vec2 v_texCoord; \n \
            varying vec4 v_fragmentColor; \n \
            void main(void) \n \
            { \n \
            // Convert to greyscale using NTSC weightings \n \
            vec4 color = texture2D(CC_Texture0, v_texCoord); \n \
            float grey = dot(color.rgb, vec3(0.299, 0.587, 0.114)); \n \
            color.a = texture2D(CC_Texture1, v_texCoord).r;         \n\
            gl_FragColor = vec4(grey, grey, grey, color.a); \n \
            }";
            pProgram->initWithVertexShaderByteArray(ccPositionETC1A_vert, pszFragSource);
        }
        else
        {
            GLchar* pszFragSource =
            "#ifdef GL_ES \n \
            precision mediump float; \n \
            #endif \n \
            varying vec2 v_texCoord; \n \
            varying vec4 v_fragmentColor; \n \
            void main(void) \n \
            { \n \
            // Convert to greyscale using NTSC weightings \n \
            vec4 col = texture2D(u_texture, v_texCoord); \n \
            float grey = dot(col.rgb, vec3(0.299, 0.587, 0.114)); \n \
            gl_FragColor = vec4(grey, grey, grey, col.a); \n \
            }";
            pProgram->initWithVertexShaderByteArray(ccPositionTextureColor_vert, pszFragSource);
        }

#else
        GLchar* pszFragSource =
        "#ifdef GL_ES \n \
        precision mediump float; \n \
        #endif \n \
        uniform sampler2D u_texture; \n \
        varying vec2 v_texCoord; \n \
        varying vec4 v_fragmentColor; \n \
        void main(void) \n \
        { \n \
        // Convert to greyscale using NTSC weightings \n \
        vec4 col = texture2D(u_texture, v_texCoord); \n \
        float grey = dot(col.rgb, vec3(0.299, 0.587, 0.114)); \n \
        gl_FragColor = vec4(grey, grey, grey, col.a); \n \
        }";
        pProgram->initWithVertexShaderByteArray(ccPositionTextureColor_vert, pszFragSource);
#endif
		this->setShaderProgram(pProgram);
		pProgram->release();
		CHECK_GL_ERROR_DEBUG();
        
		this->getShaderProgram()->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
		this->getShaderProgram()->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
		this->getShaderProgram()->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
		CHECK_GL_ERROR_DEBUG();
        
		this->getShaderProgram()->link();
		CHECK_GL_ERROR_DEBUG();
        
		this->getShaderProgram()->updateUniforms(isETC);
		CHECK_GL_ERROR_DEBUG();
        
		return true;
	} while (0);
	return false;
}

void CCGraySprite::draw(Renderer *renderer, const kmMat4& transform, bool transformUpdated)
{
	ccGLEnableVertexAttribs(kCCVertexAttribFlag_PosColorTex );
	ccGLBlendFunc( _blendFunc.src, _blendFunc.dst );
    
    GL::bindTexture2D(_texture->getName());
    //modify by liudi
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (_texture->getAlphaName()) {
//        ccGLBindTexture2DN(0, _texture->getName());
//        ccGLBindTexture2DN(1, _texture->getAlphaName());
         GL::bindTexture2DN(1, _texture->getAlphaName());
    }
//#endif
    
    this->getShaderProgram()->use();
	this->getShaderProgram()->setUniformsForBuiltins();
    
#define kQuadSize sizeof(_quad.bl)
	long offset = (long)&_quad;
    
	// vertex
	int diff = offsetof( ccV3F_C4B_T2F, vertices);
	glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, kQuadSize, (void*) (offset + diff));
    
	// texCoods
	diff = offsetof( ccV3F_C4B_T2F, texCoords);
	glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, kQuadSize, (void*)(offset + diff));
    
	// color
	diff = offsetof( ccV3F_C4B_T2F, colors);
	glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (void*)(offset + diff));
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	CC_INCREMENT_GL_DRAWS(1);
}
