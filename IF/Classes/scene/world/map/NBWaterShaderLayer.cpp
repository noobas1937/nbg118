//
//  NBWaterShaderLayer.cpp
//
//

#include "NBWaterShaderLayer.h"

bool NBWaterShaderLayer::init()
{
    m_pMapSprite = SpriteMap::create("shaders/worldmap_water_nonpremultipliedalpha.png");
    m_pMapSprite->setScale(25.0f);
    this->addChild(m_pMapSprite);
    CCLOG("width:%f, height:%f", m_pMapSprite->getContentSize().width, m_pMapSprite->getContentSize().height);

	return true;
}

NBWaterShaderLayer::NBWaterShaderLayer()
: m_pMapSprite(nullptr)
{
	
}

NBWaterShaderLayer::~NBWaterShaderLayer()
{
	
}
void NBWaterShaderLayer::tick(float time)
{
    if (m_pMapSprite)
    {
        m_pMapSprite->update(time);
    }
}

/*
 * SpriteMap
 */

SpriteMap::SpriteMap()
:m_fUtime(0.0f)
,m_pTexture0(nullptr)
,m_nVAOname(0)
{
    memset(m_arrBuffersVBO, 0, sizeof(m_arrBuffersVBO));
}

SpriteMap::~SpriteMap()
{
    
}

SpriteMap* SpriteMap::create(const char *pszFileName)
{
    SpriteMap* pRet = new SpriteMap();
    if (pRet && pRet->initWithFile(pszFileName)) {
        pRet->autorelease();
    }
    else {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

SpriteMap* SpriteMap::create(const char *pszFileName, const Rect &rect)
{
    SpriteMap* pRet = new SpriteMap();
    if (pRet && pRet->initWithFile(pszFileName, rect)) {
        pRet->autorelease();
    }
    else {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool SpriteMap::initWithTexture(Texture2D* texture, const Rect& rect)
{
    Texture2D::TexParams texParams;
    texParams.minFilter = GL_LINEAR;
    texParams.magFilter = GL_LINEAR;
    texParams.wrapS = GL_REPEAT;
    texParams.wrapT = GL_REPEAT;
    texture->setTexParameters(texParams);
    if (Sprite::initWithTexture(texture, rect))
    {
        initVertexData();
        if (Configuration::getInstance()->supportsShareableVAO())
        {
            setupVBOandVAO();
        }
        this->initProgram();
//        m_pTexture0 = Director::getInstance()->getTextureCache()->addImage(IMAGE_BASE_PATH "worldmap_water_dark.png");
        m_pTexture0 = Director::getInstance()->getTextureCache()->addImage("shaders/light_nonpremultipliedalpha.png");
        m_pTexture0->retain();
        return true;
    }
    
    return false;
}

void SpriteMap::draw(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(SpriteMap::onDraw, this, transform, flags);
    renderer->addCommand(&_customCommand);
}

void SpriteMap::update(float dt)
{
    m_fUtime += dt * 2;
}

void SpriteMap::onDraw(const Mat4& transform, uint32_t flags)
{
    GL::blendFunc(_blendFunc.src, _blendFunc.dst);
    GL::bindTexture2D(_texture->getName());
    if (m_pTexture0)
    {
        GL::bindTexture2DN(1, m_pTexture0->getName());
    }
    auto* pProgram = getGLProgram();
    pProgram->use();
    pProgram->setUniformsForBuiltins(transform);
    pProgram->setUniformLocationWith1f(m_uUniforms[kMapUniformUTime], m_fUtime);
    
    
    if (Configuration::getInstance()->supportsShareableVAO())
    {
        GL::bindVAO(m_nVAOname);
        int nodeCount = sizeof(m_index) / sizeof(m_index[0]);
        glDrawElements(GL_TRIANGLES, nodeCount, GL_UNSIGNED_SHORT, (GLvoid*)0);
        GL::bindVAO(0);
    }
    else
    {
        //
        // Attributes
        //
        GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_TEX_COORD);
        
        drawByVertexs();
        CHECK_GL_ERROR_DEBUG();
        
        CC_INCREMENT_GL_DRAWS(1);
    }
}

void SpriteMap::initProgram()
{
    GLProgram* pProgram = new GLProgram();
    
    pProgram->initWithFilenames("shaders/shader_map.vsh", "shaders/shader_map.fsh");
    
    pProgram->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
    pProgram->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORD);
    pProgram->link();
    pProgram->updateUniforms();
    GLuint program = pProgram->getProgram();
    m_uUniforms[kMapUniformUTime] = glGetUniformLocation( program, kMapUniformUTime_s);
    m_uUniforms[kMapUniformTexture0] = glGetUniformLocation( program, kMapUniformTexture0_s);
    m_uUniforms[kMapUniformNormal] = glGetUniformLocation( program, kMapUniformNormal_s);
    
    CCLOG("uniform loc %s : %d", kMapUniformUTime_s, m_uUniforms[kMapUniformUTime]);
    CCLOG("uniform loc %s : %d", kMapUniformTexture0_s, m_uUniforms[kMapUniformTexture0]);
    CCLOG("uniform loc %s : %d", kMapUniformNormal_s, m_uUniforms[kMapUniformNormal]);
    
    pProgram->setUniformLocationWith1i(m_uUniforms[kMapUniformNormal], 0);
    pProgram->setUniformLocationWith1i(m_uUniforms[kMapUniformTexture0], 1);
    
    CHECK_GL_ERROR_DEBUG();
    
    setGLProgram(pProgram);
    pProgram->release();
}

void SpriteMap::initVertexData()
{
    V2F_T2F vert;
    float texCoordGridSizeU = 1.0f / MAP_GRID_COLUMN;
    float texCoordGridSizeV = 1.0f / MAP_GRID_ROWS;
    float fWidth = getContentSize().width;
    float fHeight = getContentSize().height;
    float vertexGridSizeX = fWidth / MAP_GRID_COLUMN;
	float vertexGridSizeY = fHeight / MAP_GRID_ROWS;
    int arrIndex = 0;
    
    for(int i = 0; i<= MAP_GRID_COLUMN; ++i)
    {
        vert.vertices.y = vertexGridSizeY*i;
        vert.texCoords.v = texCoordGridSizeV*i;
        for (int j = 0; j <= MAP_GRID_ROWS; ++j) {
			vert.vertices.x = vertexGridSizeX*j;
			vert.texCoords.u = texCoordGridSizeU*j;
            m_vertArr[arrIndex++] = vert;
        }
    }
    CCLOG("init vertex count %d", arrIndex);
    arrIndex = 0;
    
    int columnCount = MAP_GRID_COLUMN + 1;
    for(int i =0; i< MAP_GRID_ROWS; ++i)
    {
        for (int j = 0; j < MAP_GRID_COLUMN; ++j) {
            GLushort a = (GLushort)(i * columnCount + j);
            GLushort b = (GLushort)((i + 1) * columnCount + j);
            GLushort c = (GLushort)((i + 1) * columnCount + (j + 1));
            GLushort d = (GLushort)(i * columnCount + (j + 1));
            
            
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

void SpriteMap::drawByVertexs()
{
    long offset = (long)&m_vertArr[0];
    
    // vertex
    int diff = offsetof( V2F_T2F, vertices);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(m_vertArr[0]), (void*) (offset + diff));
    
    // texCoods
    diff = offsetof( V2F_T2F, texCoords);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(m_vertArr[0]), (void*)(offset + diff));
    
    int nodeCount = sizeof(m_index) / sizeof(m_index[0]);
	
    glDrawElements(GL_TRIANGLES, nodeCount, GL_UNSIGNED_SHORT, (GLvoid*)(&m_index[0]));
}

void SpriteMap::setupVBOandVAO()
{
    // clean VAO
    glDeleteBuffers(2, &m_arrBuffersVBO[0]);
    glDeleteVertexArrays(1, &m_nVAOname);
    GL::bindVAO(0);
    
    glGenVertexArrays(1, &m_nVAOname);
    GL::bindVAO(m_nVAOname);
    
    glGenBuffers(2, &m_arrBuffersVBO[0]);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_arrBuffersVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertArr), m_vertArr, GL_STATIC_DRAW);
    
    // vertices
    glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_POSITION);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(m_vertArr[0]), (GLvoid*) offsetof( V2F_T2F, vertices));
    
    // tex coords
    glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_TEX_COORD);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(m_vertArr[0]), (GLvoid*)offsetof( V2F_T2F, texCoords));
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_arrBuffersVBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_index), m_index, GL_STATIC_DRAW);
    
    // Must unbind the VAO before changing the element buffer.
    GL::bindVAO(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    CHECK_GL_ERROR_DEBUG();
}

