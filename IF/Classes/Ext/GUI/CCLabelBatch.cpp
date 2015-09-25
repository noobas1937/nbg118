//
//  CCLabelBatch.cpp
//  IF
//
//  Created by 邹 程 on 14-4-9.
//
//

#include "CCLabelBatch.h"

NS_CC_BEGIN

//void CCLabelBatchNode::purgeCachedData()
//{
//    FNTConfigRemoveCache();
//}
//
//
//bool CCLabelBatchNode::init(const char *fntFile)
//{    
//    CCTexture2D *texture = NULL;
//    
//    if (fntFile)
//    {
//        CCBMFontConfiguration *newConf = FNTConfigLoadFile(fntFile);
//        if (!newConf)
//        {
//            CCLOG("cocos2d: WARNING. CCLabelBatch: Impossible to create font. Please check file: '%s'", fntFile);
//            release();
//            return false;
//        }
//        
//        newConf->retain();
//        CC_SAFE_RELEASE(m_pConfiguration);
//        m_pConfiguration = newConf;
//        
//        m_sFntFile = fntFile;
//        
//        texture = CCTextureCache::sharedTextureCache()->addImage(m_pConfiguration->getAtlasName());
//    }
//
//    if (CCSpriteBatchNode::initWithTexture(texture, 512))
//    {
//        m_obContentSize = CCSizeZero;
//        m_obAnchorPoint = ccp(0.0f, 0.0f);
//        
//        m_pReusedChar = new CCSprite();
//        m_pReusedChar->initWithTexture(m_pobTextureAtlas->getTexture(), CCRectMake(0, 0, 0, 0), false);
//        m_pReusedChar->setBatchNode(this);
//        
//        return true;
//    }
//    return false;
//}
//
//CCLabelBatchNode::CCLabelBatchNode()
//: m_pConfiguration(NULL)
//, m_pReusedChar(NULL)
//{}
//
//CCLabelBatchNode::~CCLabelBatchNode()
//{
//    CC_SAFE_RELEASE(m_pReusedChar);
//    CC_SAFE_RELEASE(m_pConfiguration);
//}
//
//
//
//// The return value needs to be deleted by CC_SAFE_DELETE_ARRAY.
//static unsigned short* copyUTF16StringN(unsigned short* str)
//{
//    int length = str ? cc_wcslen(str) : 0;
//    unsigned short* ret = new unsigned short[length+1];
//    for (int i = 0; i < length; ++i) {
//        ret[i] = str[i];
//    }
//    ret[length] = 0;
//    return ret;
//}
//
//CCLabelBatch::CCLabelBatch()
//: m_sString(NULL)
//, m_sInitialString(NULL)
//, m_pAlignment(kCCTextAlignmentCenter)
//, m_fWidth(-1.0f)
//, m_bLineBreakWithoutSpaces(false)
//, m_cDisplayedOpacity(255)
//, m_cRealOpacity(255)
//, m_tDisplayedColor(ccWHITE)
//, m_tRealColor(ccWHITE)
//, m_bCascadeColorEnabled(true)
//, m_bCascadeOpacityEnabled(true)
//, m_bIsOpacityModifyRGB(false)
//{}
//
//CCLabelBatch::~CCLabelBatch()
//{
//    CC_SAFE_DELETE_ARRAY(m_sString);
//    CC_SAFE_DELETE_ARRAY(m_sInitialString);
//}
//
//bool CCLabelBatch::initWithBatchNode(const char *str, CCLabelBatchNode* node , float width, CCTextAlignment alignment) {
//    
//    m_batchNode = node;
//    
//    CCSprite::initWithTexture(m_batchNode->getTexture(),CCRectZero);
//    m_batchNode->addChild(this);
//    
//    if (str == NULL)
//    {
//        str = "";
//    }
//    
//    m_pConfiguration = m_batchNode->getConf();
//    m_pReusedChar = m_batchNode->getReusedChar();
//    
//    m_fWidth = width;
//    m_pAlignment = alignment;
//
//    m_cDisplayedOpacity = m_cRealOpacity = 255;
//    m_tDisplayedColor = m_tRealColor = ccWHITE;
//    m_bCascadeOpacityEnabled = true;
//    m_bCascadeColorEnabled = true;
//    
//    m_obContentSize = CCSizeZero;
//    
//    m_bIsOpacityModifyRGB = m_batchNode->getTexture()->hasPremultipliedAlpha();
//    m_obAnchorPoint = ccp(0.5f, 0.5f);
//
//    this->setString(str, true);
//    
//    return true;
//}
//
//bool CCLabelBatch::initWithBatch(const char *str, CCLabelBatch* node , float width, CCTextAlignment alignment) {
//    
//    m_batchNode = node->m_batchNode;
//    
//    CCSprite::initWithTexture(m_batchNode->getTexture(),CCRectZero);
//    node->addChild(this);
//    
//    if (str == NULL)
//    {
//        str = "";
//    }
//    
//    m_pConfiguration = m_batchNode->getConf();
//    m_pReusedChar = m_batchNode->getReusedChar();
//    
//    m_fWidth = width;
//    m_pAlignment = alignment;
//    
//    m_cDisplayedOpacity = m_cRealOpacity = 255;
//    m_tDisplayedColor = m_tRealColor = ccWHITE;
//    m_bCascadeOpacityEnabled = true;
//    m_bCascadeColorEnabled = true;
//    
//    m_obContentSize = CCSizeZero;
//    
//    m_bIsOpacityModifyRGB = m_batchNode->getTexture()->hasPremultipliedAlpha();
//    m_obAnchorPoint = ccp(0.5f, 0.5f);
//    
//    this->setString(str, true);
//    
//    return true;
//}
//
//int CCLabelBatch::kerningAmountForFirst(unsigned short first, unsigned short second)
//{
//    int ret = 0;
//    unsigned int key = (first<<16) | (second & 0xffff);
//    
//    if( m_pConfiguration->m_pKerningDictionary ) {
//        tCCKerningHashElement *element = NULL;
//        HASH_FIND_INT(m_pConfiguration->m_pKerningDictionary, &key, element);
//        if(element)
//            ret = element->amount;
//    }
//    return ret;
//}
//
//void CCLabelBatch::createFontChars()
//{
//    int nextFontPositionX = 0;
//    int nextFontPositionY = 0;
//    unsigned short prev = -1;
//    int kerningAmount = 0;
//    
//    CCSize tmpSize = CCSizeZero;
//    
//    int longestLine = 0;
//    unsigned int totalHeight = 0;
//    
//    unsigned int quantityOfLines = 1;
//    unsigned int stringLen = m_sString ? cc_wcslen(m_sString) : 0;
//    if (stringLen == 0)
//    {
//        return;
//    }
//    
//    std::set<unsigned int> *charSet = m_pConfiguration->getCharacterSet();
//    
//    for (unsigned int i = 0; i < stringLen - 1; ++i)
//    {
//        unsigned short c = m_sString[i];
//        if (c == '\n')
//        {
//            quantityOfLines++;
//        }
//    }
//    
//    totalHeight = m_pConfiguration->m_nCommonHeight * quantityOfLines;
//    nextFontPositionY = 0-(m_pConfiguration->m_nCommonHeight - m_pConfiguration->m_nCommonHeight * quantityOfLines);
//    
//    CCRect rect;
//    ccBMFontDef fontDef;
//    
//    for (unsigned int i= 0; i < stringLen; i++)
//    {
//        unsigned short c = m_sString[i];
//        
//        if (c == '\n')
//        {
//            nextFontPositionX = 0;
//            nextFontPositionY -= m_pConfiguration->m_nCommonHeight;
//            continue;
//        }
//        
//        if (charSet->find(c) == charSet->end())
//        {
//            CCLOGWARN("cocos2d::CCLabelBatch: Attempted to use character not defined in this bitmap: %d", c);
//            continue;
//        }
//        
//        kerningAmount = this->kerningAmountForFirst(prev, c);
//        
//        tCCFontDefHashElement *element = NULL;
//        
//        // unichar is a short, and an int is needed on HASH_FIND_INT
//        unsigned int key = c;
//        HASH_FIND_INT(m_pConfiguration->m_pFontDefDictionary, &key, element);
//        if (! element)
//        {
//            CCLOGWARN("cocos2d::CCLabelBatch: characer not found %d", c);
//            continue;
//        }
//        
//        fontDef = element->fontDef;
//        
//        rect = fontDef.rect;
//        rect = CC_RECT_PIXELS_TO_POINTS(rect);
//                
//        CCSprite *fontChar;
//        
//        bool hasSprite = true;
//        fontChar = (CCSprite*)(this->getChildByTag(i));
//        if(fontChar )
//        {
//            // Reusing previous Sprite
//			fontChar->setVisible(true);
//        }
//        else
//        {
//            // New Sprite ? Set correct color, opacity, etc...
//            if( 0 )
//            {
//				/* WIP: Doesn't support many features yet.
//				 But this code is super fast. It doesn't create any sprite.
//				 Ideal for big labels.
//				 */
//				fontChar = m_pReusedChar;
//				fontChar->setBatchNode(NULL);
//				hasSprite = false;
//			}
//            else
//            {
//                fontChar = new CCSprite();
//                fontChar->initWithTexture(m_batchNode->getTexture(), rect);
//                addChild(fontChar, i, i);
//                fontChar->release();
//			}
//            
//            // Apply label properties
//			fontChar->setOpacityModifyRGB(m_bIsOpacityModifyRGB);
//            
//			// Color MUST be set before opacity, since opacity might change color if OpacityModifyRGB is on
//			fontChar->updateDisplayedColor(m_tDisplayedColor);
//			fontChar->updateDisplayedOpacity(m_cDisplayedOpacity);
//        }
//        
//        // updating previous sprite
//        fontChar->setTextureRect(rect, false, rect.size);
//        
//        // See issue 1343. cast( signed short + unsigned integer ) == unsigned integer (sign is lost!)
//        int yOffset = m_pConfiguration->m_nCommonHeight - fontDef.yOffset;
//        CCPoint fontPos = ccp( (float)nextFontPositionX + fontDef.xOffset + fontDef.rect.size.width*0.5f + kerningAmount,
//                              (float)nextFontPositionY + yOffset - rect.size.height*0.5f * CC_CONTENT_SCALE_FACTOR() );
//        fontChar->setPosition(CC_POINT_PIXELS_TO_POINTS(fontPos));
//        
//        // update kerning
//        nextFontPositionX += fontDef.xAdvance + kerningAmount;
//        prev = c;
//        
//        if (longestLine < nextFontPositionX)
//        {
//            longestLine = nextFontPositionX;
//        }
//        
//        if (! hasSprite)
//        {
////            updateQuadFromSprite(fontChar, i);
//        }
//    }
//    
//    // If the last character processed has an xAdvance which is less that the width of the characters image, then we need
//    // to adjust the width of the string to take this into account, or the character will overlap the end of the bounding
//    // box
//    if (fontDef.xAdvance < fontDef.rect.size.width)
//    {
//        tmpSize.width = longestLine + fontDef.rect.size.width - fontDef.xAdvance;
//    }
//    else
//    {
//        tmpSize.width = longestLine;
//    }
//    tmpSize.height = totalHeight;
//    
//    this->setContentSize(CC_SIZE_PIXELS_TO_POINTS(tmpSize));
//}
//
//void CCLabelBatch::setString(const char *newString)
//{
//    this->setString(newString, true);
//}
//
//void CCLabelBatch::setString(const char *newString, bool needUpdateLabel)
//{
//    if (newString == NULL) {
//        newString = "";
//    }
//    if (needUpdateLabel) {
//        m_sInitialStringUTF8 = newString;
//    }
//    unsigned short* utf16String = cc_utf8_to_utf16(newString);
//    setString(utf16String, needUpdateLabel);
//    CC_SAFE_DELETE_ARRAY(utf16String);
//}
//
//void CCLabelBatch::setString(unsigned short *newString, bool needUpdateLabel)
//{
//    if (!needUpdateLabel)
//    {
//        unsigned short* tmp = m_sString;
//        m_sString = copyUTF16StringN(newString);
//        CC_SAFE_DELETE_ARRAY(tmp);
//    }
//    else
//    {
//        unsigned short* tmp = m_sInitialString;
//        m_sInitialString = copyUTF16StringN(newString);
//        CC_SAFE_DELETE_ARRAY(tmp);
//    }
//    
//    if (m_pChildren && m_pChildren->count() != 0)
//    {
//        CCObject* child;
//        CCARRAY_FOREACH(m_pChildren, child)
//        {
//            CCNode* pNode = (CCNode*) child;
//            if (pNode)
//            {
//                pNode->setVisible(false);
//            }
//        }
//    }
//    this->createFontChars();
//    
//    if (needUpdateLabel) {
//        updateLabel();
//    }
//}
//
//const char* CCLabelBatch::getString(void)
//{
//    return m_sInitialStringUTF8.c_str();
//}
//
//const ccColor3B& CCLabelBatch::getColor()
//{
//    return m_tRealColor;
//}
//
//const ccColor3B& CCLabelBatch::getDisplayedColor()
//{
//    return m_tDisplayedColor;
//}
//
//void CCLabelBatch::setColor(const ccColor3B& color)
//{
//	m_tDisplayedColor = m_tRealColor = color;
//	
//	if( m_bCascadeColorEnabled ) {
//		ccColor3B parentColor = ccWHITE;
//        CCRGBAProtocol* pParent = dynamic_cast<CCRGBAProtocol*>(m_pParent);
//        if (pParent && pParent->isCascadeColorEnabled())
//        {
//            parentColor = pParent->getDisplayedColor();
//        }
//        this->updateDisplayedColor(parentColor);
//	}
//}
//
//GLubyte CCLabelBatch::getOpacity(void)
//{
//    return m_cRealOpacity;
//}
//
//GLubyte CCLabelBatch::getDisplayedOpacity(void)
//{
//    return m_cDisplayedOpacity;
//}
//
///** Override synthesized setOpacity to recurse items */
//void CCLabelBatch::setOpacity(GLubyte opacity)
//{
//	m_cDisplayedOpacity = m_cRealOpacity = opacity;
//    
//	if( m_bCascadeOpacityEnabled ) {
//		GLubyte parentOpacity = 255;
//        CCRGBAProtocol* pParent = dynamic_cast<CCRGBAProtocol*>(m_pParent);
//        if (pParent && pParent->isCascadeOpacityEnabled())
//        {
//            parentOpacity = pParent->getDisplayedOpacity();
//        }
//        this->updateDisplayedOpacity(parentOpacity);
//	}
//}
//
//void CCLabelBatch::setOpacityModifyRGB(bool var)
//{
//    m_bIsOpacityModifyRGB = var;
//    if (m_pChildren && m_pChildren->count() != 0)
//    {
//        CCObject* child;
//        CCARRAY_FOREACH(m_pChildren, child)
//        {
//            CCNode* pNode = (CCNode*) child;
//            if (pNode)
//            {
//                CCRGBAProtocol *pRGBAProtocol = dynamic_cast<CCRGBAProtocol*>(pNode);
//                if (pRGBAProtocol)
//                {
//                    pRGBAProtocol->setOpacityModifyRGB(m_bIsOpacityModifyRGB);
//                }
//            }
//        }
//    }
//}
//bool CCLabelBatch::isOpacityModifyRGB()
//{
//    return m_bIsOpacityModifyRGB;
//}
//
//void CCLabelBatch::updateDisplayedOpacity(GLubyte parentOpacity)
//{
//	m_cDisplayedOpacity = m_cRealOpacity * parentOpacity/255.0;
//    
//	CCObject* pObj;
//	CCARRAY_FOREACH(m_pChildren, pObj)
//    {
//        CCSprite *item = (CCSprite*)pObj;
//		item->updateDisplayedOpacity(m_cDisplayedOpacity);
//	}
//}
//
//void CCLabelBatch::updateDisplayedColor(const ccColor3B& parentColor)
//{
//	m_tDisplayedColor.r = m_tRealColor.r * parentColor.r/255.0;
//	m_tDisplayedColor.g = m_tRealColor.g * parentColor.g/255.0;
//	m_tDisplayedColor.b = m_tRealColor.b * parentColor.b/255.0;
//    
//    CCObject* pObj;
//	CCARRAY_FOREACH(m_pChildren, pObj)
//    {
//        CCSprite *item = (CCSprite*)pObj;
//		item->updateDisplayedColor(m_tDisplayedColor);
//	}
//}
//
//bool CCLabelBatch::isCascadeColorEnabled()
//{
//    return false;
//}
//
//void CCLabelBatch::setCascadeColorEnabled(bool cascadeColorEnabled)
//{
//    m_bCascadeColorEnabled = cascadeColorEnabled;
//}
//
//bool CCLabelBatch::isCascadeOpacityEnabled()
//{
//    return false;
//}
//
//void CCLabelBatch::setCascadeOpacityEnabled(bool cascadeOpacityEnabled)
//{
//    m_bCascadeOpacityEnabled = cascadeOpacityEnabled;
//}
//
//void CCLabelBatch::setAnchorPoint(const CCPoint& point)
//{
//    if( ! point.equals(m_obAnchorPoint))
//    {
//        CCSprite::setAnchorPoint(point);
//        updateLabel();
//    }
//}
//
//void CCLabelBatch::updateLabel()
//{
//    this->setString(m_sInitialString, false);
//    
//    if (m_fWidth > 0)
//    {
//        // Step 1: Make multiline
//        std::vector<unsigned short> str_whole = cc_utf16_vec_from_utf16_str(m_sString);
//        unsigned int stringLength = str_whole.size();
//        std::vector<unsigned short> multiline_string;
//        multiline_string.reserve( stringLength );
//        std::vector<unsigned short> last_word;
//        last_word.reserve( stringLength );
//        
//        unsigned int line = 1, i = 0;
//        bool start_line = false, start_word = false;
//        float startOfLine = -1, startOfWord = -1;
//        int skip = 0;
//        
//        CCArray* children = getChildren();
//        if (children) {
//            for (unsigned int j = 0; j < children->count(); j++)
//            {
//                CCSprite* characterSprite;
//                unsigned int justSkipped = 0;
//                
//                while (!(characterSprite = (CCSprite*)this->getChildByTag(j + skip + justSkipped)))
//                {
//                    justSkipped++;
//                }
//                
//                skip += justSkipped;
//                
//                if (!characterSprite->isVisible())
//                    continue;
//                
//                if (i >= stringLength)
//                    break;
//                
//                unsigned short character = str_whole[i];
//                
//                if (!start_word)
//                {
//                    startOfWord = getLetterPosXLeft( characterSprite );
//                    start_word = true;
//                }
//                if (!start_line)
//                {
//                    startOfLine = startOfWord;
//                    start_line = true;
//                }
//                
//                // Newline.
//                if (character == '\n')
//                {
//                    cc_utf8_trim_ws(&last_word);
//                    
//                    last_word.push_back('\n');
//                    multiline_string.insert(multiline_string.end(), last_word.begin(), last_word.end());
//                    last_word.clear();
//                    start_word = false;
//                    start_line = false;
//                    startOfWord = -1;
//                    startOfLine = -1;
//                    i+=justSkipped;
//                    line++;
//                    
//                    if (i >= stringLength)
//                        break;
//                    
//                    character = str_whole[i];
//                    
//                    if (!startOfWord)
//                    {
//                        startOfWord = getLetterPosXLeft( characterSprite );
//                        start_word = true;
//                    }
//                    if (!startOfLine)
//                    {
//                        startOfLine  = startOfWord;
//                        start_line = true;
//                    }
//                }
//                
//                // Whitespace.
//                if (isspace_unicode(character))
//                {
//                    last_word.push_back(character);
//                    multiline_string.insert(multiline_string.end(), last_word.begin(), last_word.end());
//                    last_word.clear();
//                    start_word = false;
//                    startOfWord = -1;
//                    i++;
//                    continue;
//                }
//                
//                // Out of bounds.
//                if ( getLetterPosXRight( characterSprite ) - startOfLine > m_fWidth )
//                {
//                    if (!m_bLineBreakWithoutSpaces)
//                    {
//                        last_word.push_back(character);
//                        
//                        int found = cc_utf8_find_last_not_char(multiline_string, ' ');
//                        if (found != -1)
//                            cc_utf8_trim_ws(&multiline_string);
//                        else
//                            multiline_string.clear();
//                        
//                        if (multiline_string.size() > 0)
//                            multiline_string.push_back('\n');
//                        
//                        line++;
//                        start_line = false;
//                        startOfLine = -1;
//                        i++;
//                    }
//                    else
//                    {
//                        cc_utf8_trim_ws(&last_word);
//                        
//                        last_word.push_back('\n');
//                        multiline_string.insert(multiline_string.end(), last_word.begin(), last_word.end());
//                        last_word.clear();
//                        start_word = false;
//                        start_line = false;
//                        startOfWord = -1;
//                        startOfLine = -1;
//                        line++;
//                        
//                        if (i >= stringLength)
//                            break;
//                        
//                        if (!startOfWord)
//                        {
//                            startOfWord = getLetterPosXLeft( characterSprite );
//                            start_word = true;
//                        }
//                        if (!startOfLine)
//                        {
//                            startOfLine  = startOfWord;
//                            start_line = true;
//                        }
//                        
//                        j--;
//                    }
//                    
//                    continue;
//                }
//                else
//                {
//                    // Character is normal.
//                    last_word.push_back(character);
//                    i++;
//                    continue;
//                }
//            }
//        }
//        
//        multiline_string.insert(multiline_string.end(), last_word.begin(), last_word.end());
//        
//        int size = multiline_string.size();
//        unsigned short* str_new = new unsigned short[size + 1];
//        
//        for (int i = 0; i < size; ++i)
//        {
//            str_new[i] = multiline_string[i];
//        }
//        
//        str_new[size] = '\0';
//        
//        this->setString(str_new, false);
//        
//        CC_SAFE_DELETE_ARRAY(str_new);
//    }
//    
//    // Step 2: Make alignment
//    if (m_pAlignment != kCCTextAlignmentLeft)
//    {
//        int i = 0;
//        
//        int lineNumber = 0;
//        int str_len = cc_wcslen(m_sString);
//        std::vector<unsigned short> last_line;
//        for (int ctr = 0; ctr <= str_len; ++ctr)
//        {
//            if (m_sString[ctr] == '\n' || m_sString[ctr] == 0)
//            {
//                float lineWidth = 0.0f;
//                unsigned int line_length = last_line.size();
//				// if last line is empty we must just increase lineNumber and work with next line
//                if (line_length == 0)
//                {
//                    lineNumber++;
//                    continue;
//                }
//                int index = i + line_length - 1 + lineNumber;
//                if (index < 0) continue;
//                
//                CCSprite* lastChar = (CCSprite*)getChildByTag(index);
//                if ( lastChar == NULL )
//                    continue;
//                
//                lineWidth = lastChar->getPosition().x + lastChar->getContentSize().width/2.0f;
//                
//                float shift = 0;
//                switch (m_pAlignment)
//                {
//                    case kCCTextAlignmentCenter:
//                        shift = getContentSize().width/2.0f - lineWidth/2.0f;
//                        break;
//                    case kCCTextAlignmentRight:
//                        shift = getContentSize().width - lineWidth;
//                        break;
//                    default:
//                        break;
//                }
//                
//                if (shift != 0)
//                {
//                    for (unsigned j = 0; j < line_length; j++)
//                    {
//                        index = i + j + lineNumber;
//                        if (index < 0) continue;
//                        
//                        CCSprite* characterSprite = (CCSprite*)getChildByTag(index);
//                        characterSprite->setPosition(ccpAdd(characterSprite->getPosition(), ccp(shift, 0.0f)));
//                    }
//                }
//                
//                i += line_length;
//                lineNumber++;
//                
//                last_line.clear();
//                continue;
//            }
//            
//            last_line.push_back(m_sString[ctr]);
//        }
//    }
//}
//
//// LabelBMFont - Alignment
//void CCLabelBatch::setAlignment(CCTextAlignment alignment)
//{
//    this->m_pAlignment = alignment;
//    updateLabel();
//}
//
//void CCLabelBatch::setWidth(float width)
//{
//    this->m_fWidth = width;
//    updateLabel();
//}
//
//void CCLabelBatch::setLineBreakWithoutSpace( bool breakWithoutSpace )
//{
//    m_bLineBreakWithoutSpaces = breakWithoutSpace;
//    updateLabel();
//}
//
//void CCLabelBatch::setScale(float scale)
//{
//    CCSprite::setScale(scale);
//    updateLabel();
//}
//
//void CCLabelBatch::setScaleX(float scaleX)
//{
//    CCSprite::setScaleX(scaleX);
//    updateLabel();
//}
//
//void CCLabelBatch::setScaleY(float scaleY)
//{
//    CCSprite::setScaleY(scaleY);
//    updateLabel();
//}
//
//float CCLabelBatch::getLetterPosXLeft( CCSprite* sp )
//{
//    return sp->getPosition().x * m_fScaleX - (sp->getContentSize().width * m_fScaleX * sp->getAnchorPoint().x);
//}
//
//float CCLabelBatch::getLetterPosXRight( CCSprite* sp )
//{
//    return sp->getPosition().x * m_fScaleX + (sp->getContentSize().width * m_fScaleX * sp->getAnchorPoint().x);
//}

NS_CC_END
