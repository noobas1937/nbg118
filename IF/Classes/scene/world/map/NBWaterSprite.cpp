//
//  NBWaterSprite.cpp
//  IF
//
//  Created by gj on 10/23/15.
//
//

#include "NBWaterSprite.hpp"

NBWaterSprite::NBWaterSprite():
m_fMovement(2.0f),
m_fWaveHeight(1.f),
m_fDivHeight(0.001),
m_fDivWidth(0.001),
m_fUtime(0),
m_pFoam(NULL),
m_pLightMap(NULL)
{
    
}

NBWaterSprite::~NBWaterSprite()
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, EVENT_COME_TO_FOREGROUND);
}


NBWaterSprite* NBWaterSprite::create(const char *pszFileName, const CCRect&  rect)
{
    NBWaterSprite* pRet = new NBWaterSprite();
    if (pRet && pRet->initWithFile(pszFileName,rect))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    
    return pRet;
}

NBWaterSprite* NBWaterSprite::create(const char *pszFileName)
{
    NBWaterSprite* pRet = new NBWaterSprite();
    if (pRet && pRet->initWithFile(pszFileName))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    
    return pRet;
}

bool NBWaterSprite::initWithTexture(CCTexture2D* texture, const CCRect& rect)
{
    CCTexture2D::TexParams texParams;
    texParams.minFilter = GL_LINEAR_MIPMAP_NEAREST;
    texParams.magFilter = GL_LINEAR;
    texParams.wrapS = GL_REPEAT;
    texParams.wrapT = GL_REPEAT;
    texture->setTexParameters(&texParams);
    glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
    texture->generateMipmap();
    if( CCSprite::initWithTexture(texture, rect) )
    {
        CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
                                                                      callfuncO_selector(NBWaterSprite::listenBackToForeground),
                                                                      EVENT_COME_TO_FOREGROUND,
                                                                      NULL);
        
        m_pFoam = Director::getInstance()->getTextureCache()->addImage("shaders/foam.png");
        m_pLightMap = getTexture();
        m_fMovement = 1.4;//1.4
        m_fWaveHeight = 0.3;//0.3
        m_fDivWidth = 0.008197;//0.008197
        m_fDivHeight = 0.008197;//0.008197
        
        initVertexData();
        this->initProgram();
        schedule(schedule_selector(NBWaterSprite::update));
        return true;
    }
    
    return false;
}

void NBWaterSprite::update(float dt)
{
    m_fUtime += dt*2;
}

void NBWaterSprite::listenBackToForeground(CCObject *obj)
{
    setShaderProgram(NULL);
    initProgram();
}

void NBWaterSprite::initProgram()
{
    GLchar * fragSource = (GLchar*) CCString::createWithContentsOfFile(
                                                                       CCFileUtils::sharedFileUtils()->fullPathForFilename("shaders/shader_water.fsh").c_str())->getCString();
    GLchar* vertSource = (GLchar*) CCString::createWithContentsOfFile(
                                                                      CCFileUtils::sharedFileUtils()->fullPathForFilename("shaders/shader_water.vsh").c_str())->getCString();
    CCGLProgram* pProgram = new CCGLProgram();
    pProgram->initWithVertexShaderByteArray(vertSource, fragSource);
    
    setShaderProgram(pProgram);
    pProgram->release();
    
    CHECK_GL_ERROR_DEBUG();
    
    getShaderProgram()->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
    getShaderProgram()->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
    getShaderProgram()->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
    
    CHECK_GL_ERROR_DEBUG();
    
    getShaderProgram()->link();
    
    CHECK_GL_ERROR_DEBUG();
    
    getShaderProgram()->updateUniforms();
    
    CHECK_GL_ERROR_DEBUG();
    GLuint program = getShaderProgram()->getProgram();
    m_uUniforms[kCCWaterUniformTime] = glGetUniformLocation( program, kCCWaterUniformTime_s);
    m_uUniforms[kCCWaterUniformlightPos] = glGetUniformLocation( program, kCCWaterUniformlightPos_s);
    m_uUniforms[kCCWaterUniformSEA_DARK] = glGetUniformLocation( program, kCCWaterUniformSEA_DARK_s);
    m_uUniforms[kCCWaterUniformSEA_LIGHT] = glGetUniformLocation( program, kCCWaterUniformSEA_LIGHT_s);
    m_uUniforms[kCCWaterUniformSHORE_LIGHT] = glGetUniformLocation( program, kCCWaterUniformSHORE_LIGHT_s);
    m_uUniforms[kCCWaterUniformSHORE_DARK] = glGetUniformLocation( program, kCCWaterUniformSHORE_DARK_s);
    m_uUniforms[kCCWaterUniformWAVE_HEIGHT] = glGetUniformLocation( program, kCCWaterUniformWAVE_HEIGHT_s);
    m_uUniforms[kCCWaterUniformWAVE_MOVEMENT] = glGetUniformLocation( program, kCCWaterUniformWAVE_MOVEMENT_s);
    m_uUniforms[kCCWaterUniformDivLevelWidth] = glGetUniformLocation( program, kCCWaterUniformDivLevelWidth_s);
    m_uUniforms[kCCWaterUniformDivLevelHeight] = glGetUniformLocation( program, kCCWaterUniformDivLevelHeight_s);
    m_uUniforms[kCCWaterUniformNormal] = glGetUniformLocation( program, kCCWaterUniformNormal_s);
    m_uUniforms[kCCWaterUniformlightMap] = glGetUniformLocation( program, kCCWaterUniformlightMap_s);
    m_uUniforms[kCCWaterUniformFoam] = glGetUniformLocation( program, kCCWaterUniformFoam_s);
    
    CCLOG("uniform loc %s : %d", kCCWaterUniformTime_s, m_uUniforms[kCCWaterUniformTime]);
    CCLOG("uniform loc %s : %d", kCCWaterUniformlightPos_s, m_uUniforms[kCCWaterUniformlightPos]);
    CCLOG("uniform loc %s : %d", kCCWaterUniformSEA_DARK_s, m_uUniforms[kCCWaterUniformSEA_DARK]);
    CCLOG("uniform loc %s : %d", kCCWaterUniformSEA_LIGHT_s, m_uUniforms[kCCWaterUniformSEA_LIGHT]);
    CCLOG("uniform loc %s : %d", kCCWaterUniformSHORE_LIGHT_s, m_uUniforms[kCCWaterUniformSHORE_LIGHT]);
    CCLOG("uniform loc %s : %d", kCCWaterUniformSHORE_DARK_s, m_uUniforms[kCCWaterUniformSHORE_DARK]);
    CCLOG("uniform loc %s : %d", kCCWaterUniformWAVE_HEIGHT_s, m_uUniforms[kCCWaterUniformWAVE_HEIGHT]);
    CCLOG("uniform loc %s : %d", kCCWaterUniformWAVE_MOVEMENT_s, m_uUniforms[kCCWaterUniformWAVE_MOVEMENT]);
    CCLOG("uniform loc %s : %d", kCCWaterUniformDivLevelWidth_s, m_uUniforms[kCCWaterUniformDivLevelWidth]);
    CCLOG("uniform loc %s : %d", kCCWaterUniformDivLevelHeight_s, m_uUniforms[kCCWaterUniformDivLevelHeight]);
    CCLOG("uniform loc %s : %d", kCCWaterUniformNormal_s, m_uUniforms[kCCWaterUniformNormal]);
    CCLOG("uniform loc %s : %d", kCCWaterUniformlightMap_s, m_uUniforms[kCCWaterUniformlightMap]);
    CCLOG("uniform loc %s : %d", kCCWaterUniformFoam_s, m_uUniforms[kCCWaterUniformFoam]);
    
    getShaderProgram()->setUniformLocationWith1i(m_uUniforms[kCCWaterUniformNormal], 1);
    //    getShaderProgram()->setUniformLocationWith1i(m_uUniforms[kCCWaterUniformlightMap], 2);
    //    getShaderProgram()->setUniformLocationWith1i(m_uUniforms[kCCWaterUniformFoam], 3);
    CHECK_GL_ERROR_DEBUG();
    setUniforms();
}
void NBWaterSprite::setUniforms()
{
    getShaderProgram()->setUniformLocationWith1f(m_uUniforms[kCCWaterUniformDivLevelHeight], m_fDivHeight);
    getShaderProgram()->setUniformLocationWith1f(m_uUniforms[kCCWaterUniformDivLevelWidth], m_fDivWidth);
    getShaderProgram()->setUniformLocationWith1f(m_uUniforms[kCCWaterUniformWAVE_MOVEMENT], m_fMovement);
    getShaderProgram()->setUniformLocationWith1f(m_uUniforms[kCCWaterUniformWAVE_HEIGHT], m_fWaveHeight);
    
    GLfloat shore_dark[3] = {0.0664062, 0.6875, 0.785156};//{0.0431373, 0.67451, 0.533333}
    GLfloat sea_dark[3] = {0.113725, 0.211765, 0.356863};//{0.113725, 0.211765, 0.356863}
    
    GLfloat shore_light[3] = {0.562500, 0.964843, 0.957031};//{0.752941, 0.956863, 0.890196}
    GLfloat sea_light[3] = {0.164706, 0.454902, 0.721569};//{0.164706, 0.454902, 0.721569}
    
    getShaderProgram()->setUniformLocationWith3fv(m_uUniforms[kCCWaterUniformSHORE_DARK], shore_dark, 1);
    getShaderProgram()->setUniformLocationWith3fv(m_uUniforms[kCCWaterUniformSHORE_LIGHT], shore_light, 1);
    getShaderProgram()->setUniformLocationWith3fv(m_uUniforms[kCCWaterUniformSEA_DARK], sea_dark, 1);
    getShaderProgram()->setUniformLocationWith3fv(m_uUniforms[kCCWaterUniformSEA_LIGHT], sea_light, 1);
    
    CHECK_GL_ERROR_DEBUG();
}

void NBWaterSprite::initVertexData()
{
    ccV3F_C4B_T2F vert;
    float texCoordGridSizeU = 1.0f / WATER_GRID_COLUMN;
    float texCoordGridSizeV = 1.0f / WATER_GRID_ROWS;
    float fWidth = getContentSize().width;
    float fHeight = getContentSize().height;
    float vertexGridSizeX = fWidth / WATER_GRID_COLUMN;
    float vertexGridSizeY = fHeight / WATER_GRID_ROWS;
    vert.vertices.z = 0;
    int arrIndex = 0;
    
    Image * pMyimg = new Image();
    pMyimg->useInitWithImageFileThreadSafe("shaders/water_normals.png");
    unsigned char *data_=pMyimg->getData();
    for(int i = 0; i<= WATER_GRID_COLUMN; ++i)
    {
        vert.vertices.y = -256 + vertexGridSizeY*i;
        vert.texCoords.v = texCoordGridSizeV*i;
        for (int j = 0; j <= WATER_GRID_ROWS; ++j) {
            vert.vertices.x = -256 + vertexGridSizeX*j;
            vert.texCoords.u = texCoordGridSizeU*j;
            ccColor4B color = ccColor4B(0, 0, 0, 64);//foam, wave, wind, depth
            if (i != 64 && j != 64) {
                unsigned int *pixel = (unsigned int *)data_;
                pixel = pixel + ((int)(vert.texCoords.v * getContentSize().width * getContentSize().width)) + (int)(vert.texCoords.u * getContentSize().height);
                //color.r = (*pixel & 0xff)/255;
                //CCLOG("R: %d", *pixel & 0xff);
                int nG = ((*pixel >> 8) & 0xff);
                int nTempG = (nG*nG/256);
                color.g = nTempG>=150 ? nTempG:0;
                //CCLOG("G: %d", (*pixel >> 8) & 0xff);
                int nB = ((*pixel >> 16) & 0xff);
                //color.b = (nB*nB/256 + 100)>=256 ? 256:(nB*nB/256 + 100);
                color.b = nB>128 ? 128:nB;
                //CCLOG("B: %d", (*pixel >> 16) & 0xff);
                //color.a = ((*pixel >> 24) & 0xff)/255;
                //CCLOG("A: %d", (*pixel >> 24) & 0xff);
            }
            vert.colors = color;
            m_vertArr[arrIndex++] = vert;
        }
    }
    CCLOG("init vertex count %d", arrIndex);
    arrIndex = 0;
    
    int ColumnCount = WATER_GRID_COLUMN + 1;
    for(int i =0; i< WATER_GRID_ROWS; ++i)
    {
        for (int j = 0; j < WATER_GRID_COLUMN; ++j) {
            GLushort a = (GLushort)(i * ColumnCount + j);
            GLushort b = (GLushort)((i + 1) * ColumnCount + j);
            GLushort c = (GLushort)((i + 1) * ColumnCount + (j + 1));
            GLushort d = (GLushort)(i * ColumnCount + (j + 1));
            
            
            if ((j & 1) ==0)
            {
                m_index[arrIndex++] = a;
                m_index[arrIndex++] = b;
                m_index[arrIndex++] = d;
                
                m_index[arrIndex++] = b;
                m_index[arrIndex++] = c;
                m_index[arrIndex++] = d;
            }else{
                m_index[arrIndex++] = a;
                m_index[arrIndex++] = b;
                m_index[arrIndex++] = c;
                
                m_index[arrIndex++] = c;
                m_index[arrIndex++] = d;
                m_index[arrIndex++] = a;
            }
            
        }
    }
    CCLOG("init index finished, index %d", arrIndex);
}

void NBWaterSprite::draw(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
    //    if (HFGameController::shared()->getIsInMapScene()) {
    //        return;
    //    }
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(NBWaterSprite::onDraw, this, transform, flags);
    renderer->addCommand(&_customCommand);
}

void NBWaterSprite::onDraw(const Mat4& transform, uint32_t flags)
{
    GL::enableVertexAttribs(cocos2d::GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);
    BlendFunc blend = getBlendFunc();
    GL::blendFunc(blend.src, blend.dst);
    
    auto* pProgram = getShaderProgram();
    pProgram->use();
    pProgram->setUniformsForBuiltins(transform);
    setUniforms();
    
    pProgram->setUniformLocationWith1f(m_uUniforms[kCCWaterUniformTime], m_fUtime);
    //TODO:移动光照
    //    GLfloat lightPos[3] = {-14.6388, 46.1505, 80};
    float fRate = 1.3; // GET_MAP_VIEWPORT()->getHorizontalRate();
    //    GLfloat minLightPos[3] = {-14.6388, 166.1505, 80};
    //    GLfloat maxLightPos[3] = {145.3612, 6.1505, 80};
    GLfloat minLightPos[3] = {16.3612, 96.1505, 80};
    GLfloat maxLightPos[3] = {115.3612, 6.1505, 80};
    
    GLfloat lightPos[3] = {minLightPos[0] + fRate*(maxLightPos[0] - minLightPos[0])
        , minLightPos[1] + fRate*(maxLightPos[1] - minLightPos[1]), 120};
    
    pProgram->setUniformLocationWith3fv(m_uUniforms[kCCWaterUniformlightPos], lightPos, 1);
    
    //ccGLBindTexture2D(getTexture()->getName());
    ccGLBindTexture2DN(1, getTexture()->getName());
    if (m_pLightMap) {
        ccGLBindTexture2DN(2, m_pLightMap->getName());
    }
    if (m_pFoam != NULL) {
        ccGLBindTexture2DN(3, m_pFoam->getName());
    }
    
    //
    // Attributes
    //
    
    ccGLEnableVertexAttribs(kCCVertexAttribFlag_PosColorTex );
    
    drawByVertexs();
    CHECK_GL_ERROR_DEBUG();
    
    CC_INCREMENT_GL_DRAWS(1);
}

void NBWaterSprite::drawByVertexs()
{
#define kVertElementSize sizeof(m_vertArr[0])
    long offset = (long)&m_vertArr[0];
    
    // vertex
    int diff = offsetof( ccV3F_C4B_T2F, vertices);
    glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, kVertElementSize, (void*) (offset + diff));
    
    // texCoods
    diff = offsetof( ccV3F_C4B_T2F, texCoords);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, kVertElementSize, (void*)(offset + diff));
    
    diff = offsetof( ccV3F_C4B_T2F, colors);
    glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, kVertElementSize, (void*)(offset + diff));
    int nodeCount = sizeof(m_index) / sizeof(m_index[0]);
    
    glDrawElements(GL_TRIANGLES, nodeCount, GL_UNSIGNED_SHORT, (GLvoid*)(&m_index[0]));
    
}
