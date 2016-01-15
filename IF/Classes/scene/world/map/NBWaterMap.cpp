//
//  NBWaterMap.cpp
//  IF
//
//  Created by 5ive on 15-10-28.
//
//

#include "NBWaterMap.h"

NBWaterMap::NBWaterMap()
:m_fUtime(0.0f)
,m_pTexture0(nullptr)
{
    
}

NBWaterMap::~NBWaterMap()
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, EVENT_COME_TO_FOREGROUND);
}

NBWaterMap* NBWaterMap::create(const char *pszFileName)
{
    NBWaterMap* pRet = new NBWaterMap();
    if (pRet && pRet->initWithFile(pszFileName)) {
        pRet->autorelease();
    }
    else {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

NBWaterMap* NBWaterMap::create(const char *pszFileName, const CCRect &rect)
{
    NBWaterMap* pRet = new NBWaterMap();
    if (pRet && pRet->initWithFile(pszFileName, rect)) {
        pRet->autorelease();
    }
    else {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool NBWaterMap::initWithTexture(CCTexture2D* texture, const CCRect&  rect)
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
                                                                       callfuncO_selector(NBWaterMap::listenBackToForeground),
                                                                      EVENT_COME_TO_FOREGROUND,
                                                                      NULL);
        //		m_pTexture0 = Director::getInstance()->getTextureCache()->addImage("map.png");
        
        initVertexData();
        this->initProgram();
        schedule(schedule_selector(NBWaterMap::update));
        return true;
    }
    
    return false;
}

void NBWaterMap::draw(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(NBWaterMap::onDraw, this, transform, flags);
    renderer->addCommand(&_customCommand);
}

void NBWaterMap::update(float dt)
{
    m_fUtime += dt*2;
}

void NBWaterMap::onDraw(const Mat4& transform, uint32_t flags)
{
    GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_TEX_COORD);
    BlendFunc blend = getBlendFunc();
    GL::blendFunc(blend.src, blend.dst);
    
    auto* pProgram = getShaderProgram();
    pProgram->use();
    pProgram->setUniformsForBuiltins(transform);
    
    pProgram->setUniformLocationWith1f(m_uUniforms[kMapUniformUTime], m_fUtime);
    
    //ccGLBindTexture2D(getTexture()->getName());
    ccGLBindTexture2DN(1, getTexture()->getName());
    if (m_pTexture0) {
        ccGLBindTexture2DN(2, m_pTexture0->getName());
    }
    
    //
    // Attributes
    //
    
    ccGLEnableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_TEX_COORD );
    
    drawByVertexs();
    CHECK_GL_ERROR_DEBUG();
    
    CC_INCREMENT_GL_DRAWS(1);
}

void NBWaterMap::initProgram()
{
    GLchar * fragSource = (GLchar*) CCString::createWithContentsOfFile(
                                                                       CCFileUtils::sharedFileUtils()->fullPathForFilename("shaders/shader_water_map.fsh").c_str())->getCString();
    GLchar* vertSource = (GLchar*) CCString::createWithContentsOfFile(
                                                                      CCFileUtils::sharedFileUtils()->fullPathForFilename("shaders/shader_water_map.vsh").c_str())->getCString();
    CCGLProgram* pProgram = new CCGLProgram();
    pProgram->initWithVertexShaderByteArray(vertSource, fragSource);
    
    setShaderProgram(pProgram);
    CC_SAFE_RELEASE(pProgram);
    
    CHECK_GL_ERROR_DEBUG();
    
    getShaderProgram()->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
    getShaderProgram()->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
    
    CHECK_GL_ERROR_DEBUG();
    
    getShaderProgram()->link();
    
    CHECK_GL_ERROR_DEBUG();
    
    getShaderProgram()->updateUniforms();
    
    CHECK_GL_ERROR_DEBUG();
    GLuint program = getShaderProgram()->getProgram();
    m_uUniforms[kMapUniformUTime] = glGetUniformLocation( program, kMapUniformUTime_s);
    m_uUniforms[kMapUniformTexture0] = glGetUniformLocation( program, kMapUniformTexture0_s);
    m_uUniforms[kMapUniformNormal] = glGetUniformLocation( program, kMapUniformNormal_s);
    
    CCLOG("uniform loc %s : %d", kMapUniformUTime_s, m_uUniforms[kMapUniformUTime]);
    CCLOG("uniform loc %s : %d", kMapUniformTexture0_s, m_uUniforms[kMapUniformTexture0]);
    CCLOG("uniform loc %s : %d", kMapUniformNormal_s, m_uUniforms[kMapUniformNormal]);
    
    getShaderProgram()->setUniformLocationWith1i(m_uUniforms[kMapUniformNormal], 1);
    getShaderProgram()->setUniformLocationWith1i(m_uUniforms[kMapUniformTexture0], 2);
    CHECK_GL_ERROR_DEBUG();
}

void NBWaterMap::listenBackToForeground(CCObject *obj)
{
    setShaderProgram(NULL);
    initProgram();
}

void NBWaterMap::initVertexData()
{
    V3F_T2F vert;
    float texCoordGridSizeU = 1.0f / MAP_GRID_COLUMN;
    float texCoordGridSizeV = 1.0f / MAP_GRID_ROWS;
    float fWidth = getContentSize().width;
    float fHeight = getContentSize().height; 
    float vertexGridSizeX = fWidth / MAP_GRID_COLUMN;
    float vertexGridSizeY = fHeight / MAP_GRID_ROWS;
    vert.vertices.z = 0;
    int arrIndex = 0;
    
    for(int i = 0; i<= MAP_GRID_COLUMN; ++i)
    {
        vert.vertices.y = vertexGridSizeY*i;
        vert.texCoords.v = texCoordGridSizeV*i ;
        
        for (int j = 0; j <= MAP_GRID_ROWS; ++j) {
            vert.vertices.x = vertexGridSizeX*j;
            vert.texCoords.u = texCoordGridSizeU*j ;
            m_vertArr[arrIndex++] = vert;
        }
    }
    CCLOG("init vertex count %d", arrIndex);
    arrIndex = 0;
    
    int ColumnCount = MAP_GRID_COLUMN + 1;
    for(int i =0; i< MAP_GRID_ROWS; ++i)
    {
        for (int j = 0; j < MAP_GRID_COLUMN; ++j) {
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

void NBWaterMap::drawByVertexs()
{
#define kVertElementSize sizeof(m_vertArr[0])
    long offset = (long)&m_vertArr[0];
    
    // vertex
    int diff = offsetof( V3F_T2F, vertices);
    glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, kVertElementSize, (void*) (offset + diff));
    
    // texCoods
    diff = offsetof( V3F_T2F, texCoords);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, kVertElementSize, (void*)(offset + diff));
    
    int nodeCount = sizeof(m_index) / sizeof(m_index[0]);
    
    glDrawElements(GL_TRIANGLES, nodeCount, GL_UNSIGNED_SHORT, (GLvoid*)(&m_index[0]));
}
