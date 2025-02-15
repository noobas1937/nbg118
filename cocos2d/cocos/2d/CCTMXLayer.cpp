/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.

http://www.cocos2d-x.org

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

#include "2d/CCTMXLayer.h"
#include "2d/CCTMXTiledMap.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "renderer/CCTextureCache.h"
#include "renderer/CCGLProgram.h"
#include "deprecated/CCString.h" // For StringUtils::format

NS_CC_BEGIN


// TMXLayer - init & alloc & dealloc

// Modifiedy by ChenFei 2014-12-26 V3.2 support
//TMXLayer * TMXLayer::create(TMXTilesetInfo *tilesetInfo, TMXLayerInfo *layerInfo, TMXMapInfo *mapInfo)
TMXLayer * TMXLayer::create(TMXTilesetInfo *tilesetInfo, TMXLayerInfo *layerInfo, TMXMapInfo *mapInfo, unsigned int capacity, int loopSize)
{
    TMXLayer *ret = new (std::nothrow) TMXLayer();
    if (ret->initWithTilesetInfo(tilesetInfo, layerInfo, mapInfo, capacity,loopSize))
    {
        ret->autorelease();
        return ret;
    }
    return nullptr;
}
bool TMXLayer::initWithTilesetInfo(TMXTilesetInfo *tilesetInfo, TMXLayerInfo *layerInfo, TMXMapInfo *mapInfo, unsigned int capacity, int loopSize)
{
    // XXX: is 35% a good estimate ?
    Size size = layerInfo->_layerSize;
    float totalNumberOfTiles = size.width * size.height;
    if(loopSize == -1){
        m_loopSize = size.width;
    }else{
        m_loopSize = loopSize;
    }
    // Modifiedy by ChenFei 2014-12-26 V3.2 support
    //float capacity = totalNumberOfTiles * 0.35f + 1; // 35 percent is occupied ?
    float tmpCapacity = 0.0;
    if (capacity != 0) {
        tmpCapacity = capacity;
        m_isDynamicAdd = true;
    } else {
        tmpCapacity = totalNumberOfTiles * 0.35f + 1; // 35 percent is occupied ?
    }
    Texture2D *texture = nullptr;
    if( tilesetInfo )
    {
        texture = Director::getInstance()->getTextureCache()->addImage(tilesetInfo->_sourceImage.c_str());
    }
    
    // Modifiedy by ChenFei 2014-12-26 V3.2 support
    //if (SpriteBatchNode::initWithTexture(texture, static_cast<ssize_t>(capacity)))
    if (texture && SpriteBatchNode::initWithTexture(texture, static_cast<ssize_t>(tmpCapacity)))
    {
        // layerInfo
        _layerName = layerInfo->_name;
        _layerSize = size;
        _tiles = layerInfo->_tiles;
        _opacity = layerInfo->_opacity;
        setProperties(layerInfo->getProperties());
        _contentScaleFactor = Director::getInstance()->getContentScaleFactor();
        
        // tilesetInfo
        _tileSet = tilesetInfo;
        CC_SAFE_RETAIN(_tileSet);
        
        // mapInfo
        _mapTileSize = mapInfo->getTileSize();
        _layerOrientation = mapInfo->getOrientation();
        
        // offset (after layer orientation is set);
        Vec2 offset = this->calculateLayerOffset(layerInfo->_offset);
        this->setPosition(CC_POINT_PIXELS_TO_POINTS(offset));
        
        if (!m_isDynamicAdd) {
            _atlasIndexArray = ccCArrayNew((unsigned int)totalNumberOfTiles);
        }
        
        _atlasIndexArray = ccCArrayNew(totalNumberOfTiles);
        
        this->setContentSize(CC_SIZE_PIXELS_TO_POINTS(Size(_layerSize.width * _mapTileSize.width, _layerSize.height * _mapTileSize.height)));
        
        _useAutomaticVertexZ = false;
        _vertexZvalue = 0;
        
        return true;
    }
    return false;
}

TMXLayer::TMXLayer()
:_layerName("")
,_opacity(0)
,_vertexZvalue(0)
,_useAutomaticVertexZ(false)
,_reusedTile(nullptr)
,_atlasIndexArray(nullptr)
,_contentScaleFactor(1.0f)
,_layerSize(Size::ZERO)
,_mapTileSize(Size::ZERO)
,_tiles(nullptr)
,_tileSet(nullptr)
,_layerOrientation(TMXOrientationOrtho)
// Added by ChenFei 2014-12-26 V3.2 support
,m_isDynamicAdd(false)
,m_isShaderForTileSprite(false)
{}

TMXLayer::~TMXLayer()
{
    CC_SAFE_RELEASE(_tileSet);
    CC_SAFE_RELEASE(_reusedTile);

    if (_atlasIndexArray)
    {
        ccCArrayFree(_atlasIndexArray);
        _atlasIndexArray = nullptr;
    }

    CC_SAFE_DELETE_ARRAY(_tiles);
}

void TMXLayer::releaseMap()
{
    if (_tiles)
    {
        delete [] _tiles;
        _tiles = nullptr;
    }

    if (_atlasIndexArray)
    {
        ccCArrayFree(_atlasIndexArray);
        _atlasIndexArray = nullptr;
    }
}

// TMXLayer - setup Tiles
void TMXLayer::setupTiles()
{    
    // Optimization: quick hack that sets the image size on the tileset
    _tileSet->_imageSize = _textureAtlas->getTexture()->getContentSizeInPixels();

    // By default all the tiles are aliased
    // pros:
    //  - easier to render
    // cons:
    //  - difficult to scale / rotate / etc.
    _textureAtlas->getTexture()->setAliasTexParameters();

    //CFByteOrder o = CFByteOrderGetCurrent();

    // Parse cocos2d properties
    this->parseInternalProperties();

    for (int y=0; y < _layerSize.height; y++)
    {
        for (int x=0; x < _layerSize.width; x++)
        {
            int pos = static_cast<int>(x + _layerSize.width * y);
            int gid = _tiles[ pos ];

            // gid are stored in little endian.
            // if host is big endian, then swap
            //if( o == CFByteOrderBigEndian )
            //    gid = CFSwapInt32( gid );
            /* We support little endian.*/

            // FIXME:: gid == 0 --> empty tile
            if (gid != 0) 
            {
                this->appendTileForGID(gid, Vec2(x, y));
            }
        }
    }
}

// Added by ChenFei 2014-12-26 V3.2 support
void TMXLayer::initSetupTiles() {
    _tileSet->_imageSize = _textureAtlas->getTexture()->getContentSizeInPixels();
    this->parseInternalProperties();
}

#pragma mark - 新追加方法

// Added by ChenFei 2014-12-26 V3.2 support
/**
 @param tileCoordinate 中心点
 以参数为中心点，在其周围选取略大于屏幕显示范围的区域进行瓷砖图块的绘制
 */
void TMXLayer::setupTilesByCoordinate(const cocos2d::Point &tileCoordinate, const cocos2d::Point &mapStartPt)
{
    CCLOG("TMXLayer start");
    // first , check over zone
    int col = tileCoordinate.x;
    int row = tileCoordinate.y;
    
    int designWidthCol = 11;
    int designHeightRow = 26;
    
    unsigned int index = 0;
    
    // second , remove all tiles
    m_currentCenter = tileCoordinate;
    
    this->removeAllChildren();
    m_indexForDynamic.clear();
    
    // third , add display tiles
    auto DoAddAtlas = [&](int i,int j) {
        auto pos = Point(i, j);
        unsigned int gid = 0;
        if (i<0||j<0||i>=m_loopSize||j>=m_loopSize) {
            auto newPos = Point((i + m_loopSize) % m_loopSize, (j + m_loopSize) % m_loopSize);
            if(newPos.x >= _layerSize.width || newPos.y >= _layerSize.height){
                gid = 0;
            }else{
                gid = tileGIDAt(newPos);
            }
//            if (i<0 && j<0) {
//                newPos = ccp(-i,-j);
//            } else if (i<0 && j>=0 && j<m_tLayerSize.height) {
//                newPos = ccp(-i, j);
//            } else if (i<0 && j>=m_tLayerSize.height) {
//                newPos = ccp(-i, 2*m_tLayerSize.height - 1 - j);
//            } else if (i>=0 && i<m_tLayerSize.width && j<0) {
//                newPos = ccp(i, -j);
//            } else if (i>=0 && i<m_tLayerSize.width && j>=m_tLayerSize.height) {
//                newPos = ccp(i, 2*m_tLayerSize.height - 1 - j);
//            } else if (i>=m_tLayerSize.width && j<0) {
//                newPos = ccp(2*m_tLayerSize.width - 1 - i, -j);
//            } else if (i>=m_tLayerSize.width && j>=m_tLayerSize.height) {
//                newPos = ccp(2*m_tLayerSize.width - 1 - i, 2*m_tLayerSize.height - 1 - j);
//            } else {
//                newPos = ccp(2*m_tLayerSize.width - 1 - i, j);
//            }
//            
//            unsigned int z = (unsigned int)(newPos.x + newPos.y * m_tLayerSize.width);
//            if (m_originTilesGID.find(z) != m_originTilesGID.end()) {
//                gid = m_originTilesGID[z];
//            } else {
//                gid = tileGIDAt(newPos);
//                if(gid > 3){
//                    gid = 3;
//                }
//            }
        } else {
            if(i >= _layerSize.width || j >= _layerSize.height){
                gid = 0;
            }else{
                gid = tileGIDAt(pos);
            }
        }
        
        if (!gid) {
            return;
        }
        
        // get atlas index
        int indexForZ = index++;
        int globalIndex = i+j*_layerSize.width;
        m_indexForDynamic.insert(std::pair<int,unsigned int>((int)(1+globalIndex),indexForZ));
        
        Rect rect = _tileSet->getRectForGID(gid);
        // guojiang
        if (gid == 64)
        {
//            {{4,1210},{256,128}}
            int k = (i + j) % 8;
            if (k == 0) {}
            else if (k == 1) rect = Rect(528,1210, 164,101);
            else if (k == 2) rect = Rect(698,1210, 256,128);
            else rect = Rect(0,2048-128, 256,128);
//            else if (k == 3) rect = Rect(422,1428, 112,92);
//            else if (k == 4) rect = Rect(540,1428, 150,131);
//            {{528,1210},{164,101}}
//            {{698,1210},{256,128}}
//            {{422,1428},{112,92}}
//            {{540,1428},{150,131}}
        }
        rect = CC_RECT_PIXELS_TO_POINTS(rect);
        TMXTiledSprite *tile = reusedTileWithRect(rect);
        setupTileSprite(tile, pos, gid);
       // CCLOG("    Dump tile:gid:%d [%f,%f](%d) => [%f,%f]", gid, tileCoordinate.x, tileCoordinate.y, indexForZ, tile->getPosition().x, tile->getPosition().y);
         tile->setPosition(mapStartPt + tile->getPosition());
        // Optimization: add the quad without adding a child
        this->insertQuadFromSprite(tile, indexForZ);
        
        // guo.jiang
//        if (m_isShaderForTileSprite) {
//            tile->applySeaShader();
//            setGLProgramState(tile->getGLProgramState());
//        }
    };
    
    if (_layerOrientation == TMXOrientationIso)
    {
        // 斜坐标系
        int value1 = designWidthCol/2 + designHeightRow/2;
        int value2 = designHeightRow/2 - designWidthCol/2;
        for (int i=col-value1; i<col+value1; i++) {
            int startY = 0;
            int countY = 0;
            int thisRow = i - col + value1;
            if (thisRow < designWidthCol) {
                startY = row - value2 - 1 - thisRow;
                countY = (thisRow + 1)*2;
            } else if (thisRow < designHeightRow) {
                startY = row - value1 + thisRow - designWidthCol;
                countY = 2*designWidthCol;
            } else {
                startY = row - value1 + thisRow - designWidthCol;
                countY = (designWidthCol + designHeightRow - thisRow)*2;
            }
            for (int j=startY;j<startY+countY;j++) {
                DoAddAtlas(i,j);
            }
        }
    }
    else if (_layerOrientation == TMXOrientationOrtho)
    {
        // 直角坐标系
        int value1 = designWidthCol/2;
        int value2 = designHeightRow / 2;
        for (int i = col - value1; i < col + value1; i ++) {
            for (int j = row - value2; j < row + value2; j ++) {
                DoAddAtlas(i, j);
            }
        }
    }
    
    
    CCLOG("TMXLayer end");
}




// TMXLayer - Properties
Value TMXLayer::getProperty(const std::string& propertyName) const
{
    if (_properties.find(propertyName) != _properties.end())
        return _properties.at(propertyName);
    
    return Value();
}

void TMXLayer::parseInternalProperties()
{
    // if cc_vertex=automatic, then tiles will be rendered using vertexz

    auto vertexz = getProperty("cc_vertexz");
    if (!vertexz.isNull())
    {
        std::string vertexZStr = vertexz.asString();
        // If "automatic" is on, then parse the "cc_alpha_func" too
        if (vertexZStr == "automatic")
        {
            _useAutomaticVertexZ = true;
            auto alphaFuncVal = getProperty("cc_alpha_func");
            float alphaFuncValue = alphaFuncVal.asFloat();
            setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_ALPHA_TEST));

            GLint alphaValueLocation = glGetUniformLocation(getGLProgram()->getProgram(), GLProgram::UNIFORM_NAME_ALPHA_TEST_VALUE);

            // NOTE: alpha test shader is hard-coded to use the equivalent of a glAlphaFunc(GL_GREATER) comparison
            
            // use shader program to set uniform
            getGLProgram()->use();
            getGLProgram()->setUniformLocationWith1f(alphaValueLocation, alphaFuncValue);
            CHECK_GL_ERROR_DEBUG();
        }
        else
        {
            _vertexZvalue = vertexz.asInt();
        }
    }
}

void TMXLayer::setupTileSprite(TMXTiledSprite* sprite, Vec2 pos, int gid)
{
    sprite->setPosition(getPositionAt(pos));
    sprite->setPositionZ((float)getVertexZForPos(pos));
    sprite->setAnchorPoint(Vec2::ZERO);
    sprite->setOpacity(_opacity);

    //issue 1264, flip can be undone as well
    sprite->setFlippedX(false);
    sprite->setFlippedY(false);
    sprite->setRotation(0.0f);
    sprite->setAnchorPoint(Vec2(0,0));

    // Rotation in tiled is achieved using 3 flipped states, flipping across the horizontal, vertical, and diagonal axes of the tiles.
    if (gid & kTMXTileDiagonalFlag)
    {
        // put the anchor in the middle for ease of rotation.
        sprite->setAnchorPoint(Vec2(0.5f,0.5f));
        sprite->setPosition(getPositionAt(pos).x + sprite->getContentSize().height/2,
           getPositionAt(pos).y + sprite->getContentSize().width/2 );

        int flag = gid & (kTMXTileHorizontalFlag | kTMXTileVerticalFlag );

        // handle the 4 diagonally flipped states.
        if (flag == kTMXTileHorizontalFlag)
        {
            sprite->setRotation(90.0f);
        }
        else if (flag == kTMXTileVerticalFlag)
        {
            sprite->setRotation(270.0f);
        }
        else if (flag == (kTMXTileVerticalFlag | kTMXTileHorizontalFlag) )
        {
            sprite->setRotation(90.0f);
            sprite->setFlippedX(true);
        }
        else
        {
            sprite->setRotation(270.0f);
            sprite->setFlippedX(true);
        }
    }
    else
    {
        if (gid & kTMXTileHorizontalFlag)
        {
            sprite->setFlippedX(true);
        }

        if (gid & kTMXTileVerticalFlag)
        {
            sprite->setFlippedY(true);
        }
    }
}

TMXTiledSprite* TMXLayer::reusedTileWithRect(Rect rect)
{
    if (! _reusedTile) 
    {
        _reusedTile = TMXTiledSprite::createWithTexture(_textureAtlas->getTexture(), rect);
        _reusedTile->setBatchNode(this);
        _reusedTile->retain();
    }
    else
    {
        // FIXME: HACK: Needed because if "batch node" is nil,
        // then the Sprite'squad will be reset
        _reusedTile->setBatchNode(nullptr);
        
        // Re-init the sprite
        _reusedTile->setTextureRect(rect, false, rect.size);
//        Rect rectformat;
//        rectformat.origin.x = rect.origin.x + rect.size.width * 0.05;
//        rectformat.origin.y = rect.origin.y + rect.size.height * 0.05;
//        rectformat.size.width = rect.size.width * (1-0.05 * 2);
//        rectformat.size.height = rect.size.height *(1-0.05 *2);
//        _reusedTile->setTextureCoords(rectformat);
        _reusedTile->setScale(1.01, 1.01);
        // restore the batch node
        _reusedTile->setBatchNode(this);
    }

    return _reusedTile;
}

// TMXLayer - obtaining tiles/gids
TMXTiledSprite * TMXLayer::getTileAt(const Vec2& pos)
{
    CCASSERT(pos.x < _layerSize.width && pos.y < _layerSize.height && pos.x >=0 && pos.y >=0, "TMXLayer: invalid position");
    CCASSERT(_tiles && _atlasIndexArray, "TMXLayer: the tiles map has been released");

    // Added by ChenFei 2014-12-26 V3.2 support
    if (!m_isDynamicAdd) {
        CCAssert(pos.x < _layerSize.width && pos.y < _layerSize.height && pos.x >=0 && pos.y >=0, "TMXLayer: invalid position");
        CCAssert(_tiles && _atlasIndexArray, "TMXLayer: the tiles map has been released");
    }
    
    TMXTiledSprite *tile = nullptr;
    
    // Added by ChenFei 2014-12-26 V3.2 support
    if (pos.x >= _layerSize.width || pos.y >= _layerSize.height || pos.x < 0 || pos.y < 0) {
        return tile;
    }

    
    int gid = this->getTileGIDAt(pos);

    // if GID == 0, then no tile is present
    if (gid) 
    {
        int z = (int)(pos.x + pos.y * _layerSize.width);
        tile = static_cast<TMXTiledSprite*>(this->getChildByTag(z));

        // tile not created yet. create it
        if (! tile) 
        {
            Rect rect = _tileSet->getRectForGID(gid);
            rect = CC_RECT_PIXELS_TO_POINTS(rect);

            tile = TMXTiledSprite::createWithTexture(this->getTexture(), rect);
            tile->setBatchNode(this);
            tile->setPosition(getPositionAt(pos));
            tile->setPositionZ((float)getVertexZForPos(pos));
            tile->setAnchorPoint(Vec2::ZERO);
            tile->setOpacity(_opacity);

            ssize_t indexForZ = atlasIndexForExistantZ(z);
            this->addSpriteWithoutQuad(tile, static_cast<int>(indexForZ), z);
        }
    }
    
    return tile;
}

uint32_t TMXLayer::getTileGIDAt(const Vec2& pos, TMXTileFlags* flags/* = nullptr*/)
{
    
    // Added by ChenFei 2014-12-26 V3.2 support
    if (!m_isDynamicAdd) {
        CCAssert(pos.x < _layerSize.width && pos.y < _layerSize.height && pos.x >=0 && pos.y >=0, "TMXLayer: invalid position");
        CCAssert(_tiles && _atlasIndexArray, "TMXLayer: the tiles map has been released");
    }
//    CCASSERT(pos.x < _layerSize.width && pos.y < _layerSize.height && pos.x >=0 && pos.y >=0, "TMXLayer: invalid position");
//    CCASSERT(_tiles && _atlasIndexArray, "TMXLayer: the tiles map has been released");

    ssize_t idx = static_cast<int>((pos.x + pos.y * _layerSize.width));
    // Bits on the far end of the 32-bit global tile ID are used for tile flags
    uint32_t tile = _tiles[idx];

    // issue1264, flipped tiles can be changed dynamically
    if (flags) 
    {
        *flags = (TMXTileFlags)(tile & kTMXFlipedAll);
    }
    
    return (tile & kTMXFlippedMask);
}

// TMXLayer - adding helper methods
TMXTiledSprite * TMXLayer::insertTileForGID(uint32_t gid, const Vec2& pos)
{
    if (gid != 0 && (static_cast<int>((gid & kTMXFlippedMask)) - _tileSet->_firstGid) >= 0)
    {
        Rect rect = _tileSet->getRectForGID(gid);
        rect = CC_RECT_PIXELS_TO_POINTS(rect);
        
        intptr_t z = (intptr_t)(pos.x + pos.y * _layerSize.width);
        
        TMXTiledSprite *tile = reusedTileWithRect(rect);
        
        setupTileSprite(tile, pos, gid);
        
        // get atlas index
        ssize_t indexForZ = atlasIndexForNewZ(static_cast<int>(z));
        
        // Optimization: add the quad without adding a child
        this->insertQuadFromSprite(tile, indexForZ);
        
        // insert it into the local atlasindex array
        ccCArrayInsertValueAtIndex(_atlasIndexArray, (void*)z, indexForZ);
        
        // update possible children
        
        for(const auto &child : _children) {
            Sprite* sp = static_cast<Sprite*>(child);
            ssize_t ai = sp->getAtlasIndex();
            if ( ai >= indexForZ )
            {
                sp->setAtlasIndex(ai+1);
            }
        }
        
        _tiles[z] = gid;
        return tile;
    }
    
    return nullptr;
}

TMXTiledSprite * TMXLayer::updateTileForGID(uint32_t gid, const Vec2& pos)
{
    Rect rect = _tileSet->getRectForGID(gid);
    rect = Rect(rect.origin.x / _contentScaleFactor, rect.origin.y / _contentScaleFactor, rect.size.width/ _contentScaleFactor, rect.size.height/ _contentScaleFactor);
    int z = (int)(pos.x + pos.y * _layerSize.width);

    TMXTiledSprite *tile = reusedTileWithRect(rect);

    setupTileSprite(tile ,pos ,gid);

    // get atlas index
//    ssize_t indexForZ = atlasIndexForExistantZ(z);
    // get atlas index
    // Modified by ChenFei 2014-12-26 V3.2 support
    //    ssize_t indexForZ = atlasIndexForExistantZ(z);
    unsigned int indexForZ = 0;
    if (!m_isDynamicAdd) {
        indexForZ = atlasIndexForExistantZ(z);
    } else {
        indexForZ = atlasIndexForDynamicAdd(z);
        if (indexForZ == -1) {
            return NULL;
        }
    }
    tile->setAtlasIndex(indexForZ);
    tile->setDirty(true);
    tile->updateTransform();
    if (m_originTilesGID.find(z) == m_originTilesGID.end()) {
        m_originTilesGID.insert(std::pair<unsigned int,unsigned int>(z,_tiles[z]));
    }
    _tiles[z] = gid;

    return tile;
}

// used only when parsing the map. useless after the map was parsed
// since lot's of assumptions are no longer true
TMXTiledSprite * TMXLayer::appendTileForGID(uint32_t gid, const Vec2& pos)
{
    if (gid != 0 && (static_cast<int>((gid & kTMXFlippedMask)) - _tileSet->_firstGid) >= 0)
    {
        Rect rect = _tileSet->getRectForGID(gid);
        rect = CC_RECT_PIXELS_TO_POINTS(rect);
        
        intptr_t z = (intptr_t)(pos.x + pos.y * _layerSize.width);
        
        TMXTiledSprite *tile = reusedTileWithRect(rect);
        
        setupTileSprite(tile ,pos ,gid);
        
        // optimization:
        // The difference between appendTileForGID and insertTileforGID is that append is faster, since
        // it appends the tile at the end of the texture atlas
        ssize_t indexForZ = _atlasIndexArray->num;
        
        // don't add it using the "standard" way.
        insertQuadFromSprite(tile, indexForZ);
        
        // append should be after addQuadFromSprite since it modifies the quantity values
        ccCArrayInsertValueAtIndex(_atlasIndexArray, (void*)z, indexForZ);
        
        return tile;
    }
    
    return nullptr;
}

// Added by ChenFei 2014-12-26 V3.2 support
int TMXLayer::atlasIndexForDynamicAdd(unsigned int z) {
    unsigned index = z+1;
    if (m_indexForDynamic.find(index) != m_indexForDynamic.end()) {
        return m_indexForDynamic[index];
    }
    return -1;
}

// Added by ChenFei 2014-12-26 V3.2 support
void TMXLayer::revertTileGID(const cocos2d::Point &tileCoordinate) {
    unsigned int z = (unsigned int)(tileCoordinate.x + tileCoordinate.y * _layerSize.width);
    if (m_originTilesGID.find(z) == m_originTilesGID.end()) {
        return;
    }
    setTileGID(m_originTilesGID[z], tileCoordinate,(TMXTileFlags)0);
}


// TMXLayer - atlasIndex and Z
static inline int compareInts(const void * a, const void * b)
{
    return ((*(int*)a) - (*(int*)b));
}

ssize_t TMXLayer::atlasIndexForExistantZ(int z)
{
    int key=z;
    int *item = (int*)bsearch((void*)&key, (void*)&_atlasIndexArray->arr[0], _atlasIndexArray->num, sizeof(void*), compareInts);

    CCASSERT(item, "TMX atlas index not found. Shall not happen");

    ssize_t index = ((size_t)item - (size_t)_atlasIndexArray->arr) / sizeof(void*);
    return index;
}

ssize_t TMXLayer::atlasIndexForNewZ(int z)
{
    // FIXME:: This can be improved with a sort of binary search
    ssize_t i=0;
    for (i=0; i< _atlasIndexArray->num ; i++) 
    {
        ssize_t val = (size_t) _atlasIndexArray->arr[i];
        if (z < val)
        {
            break;
        }
    } 
    
    return i;
}

// TMXLayer - adding / remove tiles
void TMXLayer::setTileGID(uint32_t gid, const Vec2& pos)
{
    setTileGID(gid, pos, (TMXTileFlags)0);
}

void TMXLayer::setTileGID(uint32_t gid, const Vec2& pos, TMXTileFlags flags)
{
    CCASSERT(pos.x < _layerSize.width && pos.y < _layerSize.height && pos.x >=0 && pos.y >=0, "TMXLayer: invalid position");
    if (!m_isDynamicAdd) {
        CCAssert(_tiles && _atlasIndexArray, "TMXLayer: the tiles map has been released");
    }
    CCASSERT(_tiles && _atlasIndexArray, "TMXLayer: the tiles map has been released");
    CCASSERT(gid == 0 || (int)gid >= _tileSet->_firstGid, "TMXLayer: invalid gid" );

    TMXTileFlags currentFlags;
    uint32_t currentGID = getTileGIDAt(pos, &currentFlags);

    if (currentGID != gid || currentFlags != flags) 
    {
        uint32_t gidAndFlags = gid | flags;

        // setting gid=0 is equal to remove the tile
        if (gid == 0)
        {
            removeTileAt(pos);
        }
        // empty tile. create a new one
        else if (currentGID == 0)
        {
            insertTileForGID(gidAndFlags, pos);
        }
        // modifying an existing tile with a non-empty tile
        else 
        {
            int z = pos.x + pos.y * _layerSize.width;
            TMXTiledSprite *sprite = static_cast<TMXTiledSprite*>(getChildByTag(z));
            if (sprite)
            {
                Rect rect = _tileSet->getRectForGID(gid);
                rect = CC_RECT_PIXELS_TO_POINTS(rect);

                sprite->setTextureRect(rect, false, rect.size);
                Rect rectformat;
                rectformat.origin.x = rect.origin.x + rect.size.width * 0.05;
                rectformat.origin.y = rect.origin.y + rect.size.height * 0.05;
                rectformat.size.width = rect.size.width * (1-0.05 * 2);
                rectformat.size.height = rect.size.height *(1-0.05 *2);
                
//                sprite->setTextureCoords(rectformat);
                _reusedTile->setScale(1.01, 1.01);

                if (flags)
                {
                    setupTileSprite(sprite, sprite->getPosition(), gidAndFlags);
                }
                // Added by ChenFei 2014-12-26 V3.2 support
                if (m_originTilesGID.find(z) == m_originTilesGID.end()) {
                    m_originTilesGID.insert(std::pair<unsigned int,unsigned int>(z,_tiles[z]));
                }
                
                _tiles[z] = gidAndFlags;
            } 
            else 
            {
                updateTileForGID(gidAndFlags, pos);
            }
        }
    }
}

void TMXLayer::addChild(Node * child, int zOrder, int tag)
{
    CC_UNUSED_PARAM(child);
    CC_UNUSED_PARAM(zOrder);
    CC_UNUSED_PARAM(tag);
    CCASSERT(0, "addChild: is not supported on TMXLayer. Instead use setTileGID:at:/tileAt:");
}

void TMXLayer::removeChild(Node* node, bool cleanup)
{
    // Added by ChenFei 2014-12-26 V3.2 support
    if (m_isDynamicAdd) {
        CCLOG("not implement");
        return;
    }
    
    Sprite *sprite = (Sprite*)node;
    // allows removing nil objects
    if (! sprite)
    {
        return;
    }

    CCASSERT(_children.contains(sprite), "Tile does not belong to TMXLayer");

    ssize_t atlasIndex = sprite->getAtlasIndex();
    ssize_t zz = (ssize_t)_atlasIndexArray->arr[atlasIndex];
    _tiles[zz] = 0;
    ccCArrayRemoveValueAtIndex(_atlasIndexArray, atlasIndex);
    SpriteBatchNode::removeChild(sprite, cleanup);
}

void TMXLayer::removeTileAt(const Vec2& pos)
{
    CCASSERT(pos.x < _layerSize.width && pos.y < _layerSize.height && pos.x >=0 && pos.y >=0, "TMXLayer: invalid position");
    // Added by ChenFei 2014-12-26 V3.2 support
    if (!m_isDynamicAdd) {
        CCAssert(_tiles && _atlasIndexArray, "TMXLayer: the tiles map has been released");
    } else {
        CCLOG("not implement!");if (!m_isDynamicAdd) {
            CCAssert(_tiles && _atlasIndexArray, "TMXLayer: the tiles map has been released");
        } else {
            CCLOG("not implement!");
            return;
        }
        return;
    }

    int gid = getTileGIDAt(pos);

    if (gid) 
    {
        int z = pos.x + pos.y * _layerSize.width;
        ssize_t atlasIndex = atlasIndexForExistantZ(z);

        // remove tile from GID map
        _tiles[z] = 0;

        // remove tile from atlas position array
        ccCArrayRemoveValueAtIndex(_atlasIndexArray, atlasIndex);

        // remove it from sprites and/or texture atlas
        Sprite *sprite = (Sprite*)getChildByTag(z);
        if (sprite)
        {
            SpriteBatchNode::removeChild(sprite, true);
        }
        else 
        {
            _textureAtlas->removeQuadAtIndex(atlasIndex);

            // update possible children
            for(const auto &obj : _children) {
                Sprite* child = static_cast<Sprite*>(obj);
                ssize_t ai = child->getAtlasIndex();
                if ( ai >= atlasIndex )
                {
                    child->setAtlasIndex(ai-1);
                }
            }
        }
    }
}

//CCTMXLayer - obtaining positions, offset
Vec2 TMXLayer::calculateLayerOffset(const Vec2& pos)
{
    Vec2 ret;
    switch (_layerOrientation) 
    {
    case TMXOrientationOrtho:
        ret.set( pos.x * _mapTileSize.width, -pos.y *_mapTileSize.height);
        break;
    case TMXOrientationIso:
        ret.set((_mapTileSize.width /2) * (pos.x - pos.y),
                  (_mapTileSize.height /2 ) * (-pos.x - pos.y));
        break;
    case TMXOrientationHex:
        CCASSERT(pos.isZero(), "offset for hexagonal map not implemented yet");
        break;
    case TMXOrientationStaggered:
        {
            float diffX = 0;
            if ((int)std::abs(pos.y) % 2 == 1)
            {
                diffX = _mapTileSize.width/2;
            }
            ret.set(pos.x * _mapTileSize.width + diffX,
                         (-pos.y) * _mapTileSize.height/2);
        }
        break;
    }
    return ret;    
}

Vec2 TMXLayer::getPositionAt(const Vec2& pos)
{
    Vec2 ret;
    switch (_layerOrientation)
    {
    case TMXOrientationOrtho:
        ret = getPositionForOrthoAt(pos);
        break;
    case TMXOrientationIso:
        ret = getPositionForIsoAt(pos);
        break;
    case TMXOrientationHex:
        ret = getPositionForHexAt(pos);
        break;
    case TMXOrientationStaggered:
        ret = getPositionForStaggeredAt(pos);
        break;
    }
    ret = CC_POINT_PIXELS_TO_POINTS( ret );
    return ret;
}

Vec2 TMXLayer::getPositionForOrthoAt(const Vec2& pos)
{
    return Vec2(pos.x * _mapTileSize.width,
                            (_layerSize.height - pos.y - 1) * _mapTileSize.height);
}

Vec2 TMXLayer::getPositionForIsoAt(const Vec2& pos)
{
    return Vec2(_mapTileSize.width /2 * (_layerSize.width + pos.x - pos.y - 1),
                             _mapTileSize.height /2 * ((_layerSize.height * 2 - pos.x - pos.y) - 2));
}

Vec2 TMXLayer::getPositionForHexAt(const Vec2& pos)
{
    float diffY = 0;
    if ((int)pos.x % 2 == 1)
    {
        diffY = -_mapTileSize.height/2 ;
    }

    Vec2 xy(
        pos.x * _mapTileSize.width*3/4,
                            (_layerSize.height - pos.y - 1) * _mapTileSize.height + diffY);
    return xy;
}

Vec2 TMXLayer::getPositionForStaggeredAt(const Vec2 &pos)
{
    float diffX = 0;
    if ((int)pos.y % 2 == 1)
    {
        diffX = _mapTileSize.width/2;
    }
    return Vec2(pos.x * _mapTileSize.width + diffX,
                (_layerSize.height - pos.y - 1) * _mapTileSize.height/2);
}

int TMXLayer::getVertexZForPos(const Vec2& pos)
{
    int ret = 0;
    int maxVal = 0;
    if (_useAutomaticVertexZ)
    {
        switch (_layerOrientation) 
        {
        case TMXOrientationIso:
            maxVal = static_cast<int>(_layerSize.width + _layerSize.height);
            ret = static_cast<int>(-(maxVal - (pos.x + pos.y)));
            break;
        case TMXOrientationOrtho:
            ret = static_cast<int>(-(_layerSize.height-pos.y));
            break;
        case TMXOrientationStaggered:
            ret = static_cast<int>(-(_layerSize.height-pos.y));
            break;
        case TMXOrientationHex:
            CCASSERT(0, "TMX Hexa zOrder not supported");
            break;
        default:
            CCASSERT(0, "TMX invalid value");
            break;
        }
    } 
    else
    {
        ret = _vertexZvalue;
    }
    
    return ret;
}

std::string TMXLayer::getDescription() const
{
    return StringUtils::format("<TMXLayer | tag = %d, size = %d,%d>", _tag, (int)_mapTileSize.width, (int)_mapTileSize.height);
}

//void TMXLayer::revertTileGID(const Vec2& PointileCoordinate)
//{
//    //TODO::liudi
//}
NS_CC_END
