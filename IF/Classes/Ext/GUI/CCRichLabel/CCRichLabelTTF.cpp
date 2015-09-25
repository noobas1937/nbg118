/****************************************************************************
 Author: Luma (stubma@gmail.com)
 
 https://github.com/stubma/cocos2dx-better
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
#include "CCRichLabelTTF.h"
#include "CCDirector.h"
#include "renderer/CCGLProgram.h"
//#include "renderer/CCShaderCache.h"
#include "CCTexture2D_richlabel.h"
#include "CCImage_richlabel.h"
#include "CCMenuItemColor.h"
#include "CCRichLabelTTFLinkStateSynchronizer.h"
#include "CCTextureCache_richlabel.h"

NS_CC_BEGIN

#if CC_USE_LA88_LABELS
#define SHADER_PROGRAM kCCShader_PositionTextureColor
#else
#define SHADER_PROGRAM kCCShader_PositionTextureA8Color
#endif

// the start tag of menu item, for link tag
#define START_TAG_LINK_ITEM 0x80000
#define TAG_MENU 0x70000

//
//CCRichLabelTTF
//
CCRichLabelTTF::CCRichLabelTTF() :
m_hAlignment(kCCTextAlignmentCenter),
m_vAlignment(kCCVerticalTextAlignmentTop),
m_pFontName(NULL),
m_fFontSize(0.0),
m_string(""),
m_shadowEnabled(false),
m_shadowColor(0xff333333),
m_strokeEnabled(false),
m_textFillColor(ccWHITE),
m_stateListener(NULL),
m_textChanging(true) {
	m_stateListener = new CCRichLabelTTFLinkStateSynchronizer(this);
}

CCRichLabelTTF::~CCRichLabelTTF()
{
    CC_SAFE_DELETE(m_pFontName);
	
	// release callfunc
	CCMenu* menu = (CCMenu*)getChildByTag(TAG_MENU);
	if(menu) {
		CCObject* obj;
        const auto& children = menu->getChildren();
        for (const auto& child : children)
        {
            CCNode* item = (CCNode*)child;
            CCObject* data = (CCObject*)item->getUserData();
            CC_SAFE_RELEASE(data);
        }
//		CCARRAY_FOREACH(menu->getChildren(), obj) {
//			CCNode* item = (CCNode*)obj;
//			CCObject* data = (CCObject*)item->getUserData();
//			CC_SAFE_RELEASE(data);
//		}
	}
	
	// release other
	m_stateListener->release();
}

void CCRichLabelTTF::reloadAll() {
    CCTextureCache_richlabel::reloadAllTextures();
}

CCRichLabelTTF * CCRichLabelTTF::create()
{
    CCRichLabelTTF * pRet = new CCRichLabelTTF();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

CCRichLabelTTF * CCRichLabelTTF::create(const char *string, const char *fontName, float fontSize)
{
    return CCRichLabelTTF::create(string, fontName, fontSize,
                              CCSizeZero, kCCTextAlignmentCenter, kCCVerticalTextAlignmentTop);
}

CCRichLabelTTF * CCRichLabelTTF::create(const char *string, const char *fontName, float fontSize,
                                const CCSize& dimensions, CCTextAlignment hAlignment)
{
    return CCRichLabelTTF::create(string, fontName, fontSize, dimensions, hAlignment, kCCVerticalTextAlignmentTop);
}

CCRichLabelTTF* CCRichLabelTTF::create(const char *string, const char *fontName, float fontSize,
                               const CCSize &dimensions, CCTextAlignment hAlignment,
                               CCVerticalTextAlignment vAlignment)
{
    CCRichLabelTTF *pRet = new CCRichLabelTTF();
    if(pRet && pRet->initWithString(string, strcmp(fontName, "") == 0 ? "Helvetica" : fontName , fontSize, dimensions, hAlignment, vAlignment))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool CCRichLabelTTF::init()
{
    return this->initWithString("", "Helvetica", 12);
}

bool CCRichLabelTTF::initWithString(const char *label, const char *fontName, float fontSize,
                                const CCSize& dimensions, CCTextAlignment alignment)
{
    return this->initWithString(label, fontName, fontSize, dimensions, alignment, kCCVerticalTextAlignmentTop);
}

bool CCRichLabelTTF::initWithString(const char *label, const char *fontName, float fontSize)
{
    return this->initWithString(label, fontName, fontSize,
                                CCSizeZero, kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
}

bool CCRichLabelTTF::initWithString(const char *string, const char *fontName, float fontSize,
                                const CCSize &dimensions, CCTextAlignment hAlignment,
                                CCVerticalTextAlignment vAlignment)
{
    if (CCGradientSprite::init())
    {
        // shader program
        //this->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(SHADER_PROGRAM));
        this->setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP));
        m_tDimensions = CCSizeMake(dimensions.width, dimensions.height);
        m_hAlignment  = hAlignment;
        m_vAlignment  = vAlignment;
        m_pFontName   = new std::string(fontName);
        m_fFontSize   = fontSize;
        
        this->setString(string);
        
        return true;
    }
    
    return false;
}

bool CCRichLabelTTF::initWithStringAndTextDefinition(const char *string, ccRichFontDefinition &textDefinition)
{
    if (CCGradientSprite::init())
    {
        // shader program
        //this->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(SHADER_PROGRAM));
        this->setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP));
        
        // prepare everythin needed to render the label
        _updateWithTextDefinition(textDefinition, false);
        
        // set the string
        this->setString(string);
        
        //
        return true;
    }
    else
    {
        return false;
    }
}


void CCRichLabelTTF::setString(const std::string& string)
{
    if(string == "")
    {
        return;
    }
    CCAssert(string != "", "Invalid string");
    
    if (m_string.compare(string))
    {
        m_string = string;
        m_textChanging = true;
        this->updateTexture();
    }
}

const std::string& CCRichLabelTTF::getString(void) const
{
    return m_string;//.c_str();
}

const char* CCRichLabelTTF::description()
{
    return CCString::createWithFormat("<CCRichLabelTTF | FontName = %s, FontSize = %.1f>", m_pFontName->c_str(), m_fFontSize)->getCString();
}

CCTextAlignment CCRichLabelTTF::getHorizontalAlignment()
{
    return m_hAlignment;
}

void CCRichLabelTTF::setHorizontalAlignment(CCTextAlignment alignment)
{
    if (alignment != m_hAlignment)
    {
        m_hAlignment = alignment;
        
        // Force update
        if (m_string.size() > 0)
        {
            this->updateTexture();
        }
    }
}

CCVerticalTextAlignment CCRichLabelTTF::getVerticalAlignment()
{
    return m_vAlignment;
}

void CCRichLabelTTF::setVerticalAlignment(CCVerticalTextAlignment verticalAlignment)
{
    if (verticalAlignment != m_vAlignment)
    {
        m_vAlignment = verticalAlignment;
        
        // Force update
        if (m_string.size() > 0)
        {
            this->updateTexture();
        }
    }
}

CCSize CCRichLabelTTF::getDimensions()
{
    return m_tDimensions;
}

void CCRichLabelTTF::setDimensions(const CCSize &dim)
{
    if (dim.width != m_tDimensions.width || dim.height != m_tDimensions.height)
    {
        m_tDimensions = dim;
        
        // Force update
        if (m_string.size() > 0)
        {
            this->updateTexture();
        }
    }
}

float CCRichLabelTTF::getFontSize()
{
    return m_fFontSize;
}

void CCRichLabelTTF::setFontSize(float fontSize)
{
    if (m_fFontSize != fontSize)
    {
        m_fFontSize = fontSize;
        
        // Force update
        if (m_string.size() > 0)
        {
            this->updateTexture();
        }
    }
}

const char* CCRichLabelTTF::getFontName()
{
    return m_pFontName->c_str();
}

void CCRichLabelTTF::setFontName(const char *fontName)
{
    if (m_pFontName->compare(fontName))
    {
        delete m_pFontName;
        m_pFontName = new std::string(fontName);
        
        // Force update
        if (m_string.size() > 0)
        {
            this->updateTexture();
        }
    }
}

void CCRichLabelTTF::setColor(const ccColor4B& start, const ccColor4B& end, const CCPoint& v) {
    CCGradientSprite::setColor(start, end, v);
}

// Helper
bool CCRichLabelTTF::updateTexture()
{
    CCTexture2D_richlabel *tex;
    tex = new CCTexture2D_richlabel();
    
    if (!tex)
        return false;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
	ccRichFontDefinition texDef = _prepareTextDefinition(true);
	tex->initWithRichString( m_string.c_str(), &texDef );
    
#else
    
	tex->initWithRichString( m_string.c_str(),
						m_pFontName->c_str(),
						m_fFontSize * CC_CONTENT_SCALE_FACTOR(),
						CC_SIZE_POINTS_TO_PIXELS(m_tDimensions),
						m_hAlignment,
						m_vAlignment);
    
#endif
    
    // set the texture
    this->setTexture(tex);
    
    // release it
    tex->release();
    
    // set the size in the sprite
    CCRect rect =CCRectZero;
    rect.size   = _texture->getContentSize();
    this->setTextureRect(rect);
	
	// create link menu
	CCMenu* menu = (CCMenu*)getChildByTag(TAG_MENU);
	const LinkMetaList& linkMetas = tex->getLinkMetas();
	if(!linkMetas.empty() && m_textChanging) {
		// delete old menu if has
		if(menu) {
			menu->removeFromParent();
			menu = NULL;
		}
		
		// now create items for every link
		//CCArray* items = CCArray::create();
        Vector<MenuItem*> items;
		for(LinkMetaList::const_iterator iter = linkMetas.begin(); iter != linkMetas.end(); iter++) {
			const LinkMeta& meta = *iter;
			CCMenuItemColor* item = CCMenuItemColor::create(ccc4FromInt(meta.normalBgColor),
															ccc4FromInt(meta.selectedBgColor),
															this,
															menu_selector(CCRichLabelTTF::onLinkMenuItemClicked));
			item->setTag(START_TAG_LINK_ITEM + meta.tag);
			item->setPosition(ccp(meta.x + meta.width / 2,
								  meta.y + meta.height / 2));
			item->setContentSize(CCSizeMake(meta.width, meta.height));
			item->setStateListener(m_stateListener);
			items.pushBack(item);
		}
		
		// add menu in -1 z order so that it won't override label UI
		CCMenu* newMenu = CCMenu::createWithArray(items);
		newMenu->setPosition(CCPointZero);
		addChild(newMenu, -1, TAG_MENU);
		
		// clear flag
		m_textChanging = false;
	}
	
	// sync shadow stroke padding to link menu
	// the padding must divide scale factor. Because at this time, texture already scaled for label
	menu = (CCMenu*)getChildByTag(TAG_MENU);
	if(menu) {
		const CCPoint& p = tex->getShadowStrokePadding();
		menu->setPosition(ccp(p.x / CC_CONTENT_SCALE_FACTOR(),
							  p.y / CC_CONTENT_SCALE_FACTOR()));
	}
    
    //ok
    return true;
}

void CCRichLabelTTF::enableShadow(const CCSize &shadowOffset, int shadowColor, float shadowBlur, bool updateTexture)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
	bool valueChanged = false;
	
	if (false ==  m_shadowEnabled)
	{
        m_shadowEnabled = true;
		valueChanged    = true;
	}
	
	if ( ( m_shadowOffset.width != shadowOffset.width) || ( m_shadowOffset.height!=shadowOffset.height) )
	{
		 m_shadowOffset.width  = shadowOffset.width;
		 m_shadowOffset.height = shadowOffset.height;
		
		valueChanged = true;
	}
    
    if( m_shadowColor != shadowColor) {
         m_shadowColor = shadowColor;
        valueChanged = true;
    }
	
	if ( m_shadowBlur    != shadowBlur)
	{
		 m_shadowBlur = shadowBlur;
		valueChanged = true;
	}
	
	
	if ( valueChanged && updateTexture )
	{
		this->updateTexture();
	}
    
#else
	CCAssert(false, "Currently only supported on iOS and Android!");
#endif
    
}

void CCRichLabelTTF::disableShadow(bool updateTexture)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
	if ( m_shadowEnabled)
	{
		 m_shadowEnabled = false;
		
		if (updateTexture)
			this->updateTexture();
		
	}
    
#else
	CCAssert(false, "Currently only supported on iOS and Android!");
#endif
}

void CCRichLabelTTF::enableStroke(const ccColor3B &strokeColor, float strokeSize, bool updateTexture)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
	bool valueChanged = false;
	
	if(m_strokeEnabled == false)
	{
		m_strokeEnabled = true;
		valueChanged = true;
	}
	
	if ( (m_strokeColor.r != strokeColor.r) || (m_strokeColor.g != strokeColor.g) || (m_strokeColor.b != strokeColor.b) )
	{
		m_strokeColor = strokeColor;
		valueChanged = true;
	}
	
	if (m_strokeSize!=strokeSize)
	{
		m_strokeSize = strokeSize;
		valueChanged = true;
	}
	
	if ( valueChanged && updateTexture )
	{
		this->updateTexture();
	}
    
#else
	CCAssert(false, "Currently only supported on iOS and Android!");
#endif
    
}

void CCRichLabelTTF::disableStroke(bool updateTexture)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
	if (m_strokeEnabled)
	{
		m_strokeEnabled = false;
		
		if (updateTexture)
			this->updateTexture();
	}
    
#else
	CCAssert(false, "Currently only supported on iOS and Android!");
#endif
    
}

void CCRichLabelTTF::setFontFillColor(const ccColor3B &tintColor, bool updateTexture)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	if (m_textFillColor.r != tintColor.r || m_textFillColor.g != tintColor.g || m_textFillColor.b != tintColor.b)
	{
		m_textFillColor = tintColor;
		
		if (updateTexture)
			this->updateTexture();
	}
#else
	CCAssert(false, "Currently only supported on iOS and Android!");
#endif
}

void CCRichLabelTTF::setTextDefinition(ccRichFontDefinition *theDefinition)
{
    if (theDefinition)
    {
        _updateWithTextDefinition(*theDefinition, true);
    }
}

ccRichFontDefinition *CCRichLabelTTF::getTextDefinition()
{
    ccRichFontDefinition *tempDefinition = new ccRichFontDefinition;
    *tempDefinition = _prepareTextDefinition(false);
    return tempDefinition;
}

void CCRichLabelTTF::_updateWithTextDefinition(ccRichFontDefinition & textDefinition, bool mustUpdateTexture)
{
    m_tDimensions = CCSizeMake(textDefinition._dimensions.width, textDefinition._dimensions.height);
    m_hAlignment  = textDefinition._alignment;
    m_vAlignment  = textDefinition._vertAlignment;
    
    m_pFontName   = new std::string(textDefinition. _fontName);
    m_fFontSize   = textDefinition. _fontSize;
    
    
    // shadow
    if ( textDefinition._shadow._shadowEnabled )
    {
        enableShadow(textDefinition._shadow._shadowOffset, textDefinition.m_shadowColor, false);
    }
    
    // stroke
    if ( textDefinition._stroke._strokeEnabled )
    {
        enableStroke(textDefinition._stroke._strokeColor, textDefinition._stroke._strokeSize, false);
    }
    
    // fill color
    setFontFillColor(textDefinition._fontFillColor, false);
    
    if (mustUpdateTexture)
        updateTexture();
}

ccRichFontDefinition CCRichLabelTTF::_prepareTextDefinition(bool adjustForResolution)
{
    ccRichFontDefinition texDef;
    
    if (adjustForResolution)
        texDef._fontSize       =  m_fFontSize * CC_CONTENT_SCALE_FACTOR();
    else
        texDef._fontSize       =  m_fFontSize;
    
    texDef._fontName       = *m_pFontName;
    texDef._alignment      =  m_hAlignment;
    texDef._vertAlignment  =  m_vAlignment;
    
    
    if (adjustForResolution)
        texDef._dimensions     =  CC_SIZE_POINTS_TO_PIXELS(m_tDimensions);
    else
        texDef._dimensions     =  m_tDimensions;
    
    
    // stroke
    if ( m_strokeEnabled )
    {
        texDef._stroke._strokeEnabled = true;
        texDef._stroke._strokeColor   = m_strokeColor;
        
        if (adjustForResolution)
            texDef._stroke._strokeSize = m_strokeSize * CC_CONTENT_SCALE_FACTOR();
        else
            texDef._stroke._strokeSize = m_strokeSize;
        
        
    }
    else
    {
        texDef._stroke._strokeEnabled = false;
    }
    
    
    // shadow
    if ( m_shadowEnabled )
    {
        texDef._shadow._shadowEnabled         = true;
        texDef._shadow._shadowBlur            =  m_shadowBlur;
        texDef._shadow._shadowOpacity         =  m_shadowOpacity;
        texDef.m_shadowColor           =  m_shadowColor;
        
        if (adjustForResolution)
            texDef._shadow._shadowOffset = CC_SIZE_POINTS_TO_PIXELS( m_shadowOffset);
        else
            texDef._shadow._shadowOffset =  m_shadowOffset;
    }
    else
    {
        texDef._shadow._shadowEnabled = false;
    }
    
    // text tint
    texDef._fontFillColor = m_textFillColor;
    
    return texDef;
}

void CCRichLabelTTF::setLinkTarget(int index, CCCallFunc* func) {
    // if not found, do nothing
    CCMenu* menu = (CCMenu*)getChildByTag(TAG_MENU);
    if(!menu)
        return;
	
	CCObject* obj;
    auto& children = menu->getChildren();
    for (auto child : children)
    {
        CCMenuItemColor* item = (CCMenuItemColor*)child;
        if(item->getTag() == START_TAG_LINK_ITEM + index) {
            obj = (CCObject*)item->getUserData();
            if(obj) {
                CC_SAFE_RELEASE(obj);
            }
            item->setUserData(func);
            CC_SAFE_RETAIN(func);
        }
    }
//	CCARRAY_FOREACH(menu->getChildren(), obj) {
//		CCMenuItemColor* item = (CCMenuItemColor*)obj;
//		if(item->getTag() == START_TAG_LINK_ITEM + index) {
//			obj = (CCObject*)item->getUserData();
//			if(obj) {
//				CC_SAFE_RELEASE(obj);
//			}
//			item->setUserData(func);
//			CC_SAFE_RETAIN(func);
//		}
//	}
}
void CCRichLabelTTF::setLinkTargetForAll(CCCallFunc* func) {
	// if not found, do nothing
    CCMenu* menu = (CCMenu*)getChildByTag(TAG_MENU);
    if(!menu)
        return;
	
	CCObject* obj;
    auto& children = menu->getChildren();
    for (auto child : children)
    {
        CCMenuItemColor* item = (CCMenuItemColor*)child;
        obj = (CCObject*)item->getUserData();
        if(obj) {
            CC_SAFE_RELEASE(obj);
        }
        item->setUserData(func);
        CC_SAFE_RETAIN(func);
    }
//	CCARRAY_FOREACH(menu->getChildren(), obj) {
//		CCMenuItemColor* item = (CCMenuItemColor*)obj;
//		obj = (CCObject*)item->getUserData();
//		if(obj) {
//			CC_SAFE_RELEASE(obj);
//		}
//		item->setUserData(func);
//		CC_SAFE_RETAIN(func);
//	}
}

void CCRichLabelTTF::onLinkMenuItemClicked(CCObject* sender) {
	CCMenuItemColor* item = (CCMenuItemColor*)sender;
	CCCallFunc* func = (CCCallFunc*)item->getUserData();
    if(func){
        CCCallFuncO *funcO = dynamic_cast<CCCallFuncO*>(func);
        if(funcO){
            funcO->setObject(CCInteger::create(item->getTag() - START_TAG_LINK_ITEM));
        }
		func->execute();
    }
}

void CCRichLabelTTF::setLinkPriority(int p) {
	// if not found, do nothing
    CCMenu* menu = (CCMenu*)getChildByTag(TAG_MENU);
    if(!menu)
        return;
	
	//menu->setTouchPriority(p);
}

NS_CC_END
