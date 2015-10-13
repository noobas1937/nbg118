//
//  CCTMXTiledSprite.hpp
//  cocos2d_libs
//
//  Created by gj on 10/12/15.
//
//

#ifndef CCTMXTiledSprite_hpp
#define CCTMXTiledSprite_hpp

#include "2d/CCSprite.h"
#include "renderer/CCQuadCommand.h"

NS_CC_BEGIN

class TMXTiledSprite
: public Sprite {
    
public:
    
    static TMXTiledSprite* create();
    
    /**
     * Creates a sprite with an image filename.
     *
     * After creation, the rect of sprite will be the size of the image,
     * and the offset will be (0,0).
     *
     * @param   filename A path to image file, e.g., "scene1/monster.png".
     * @return  An autoreleased sprite object.
     */
    static TMXTiledSprite* create(const std::string& filename);
    
    /**
     * Creates a polygon sprite with a polygon info.
     *
     * After creation, the rect of sprite will be the size of the image,
     * and the offset will be (0,0).
     *
     * @param   polygonInfo A path to image file, e.g., "scene1/monster.png".
     * @return  An autoreleased sprite object.
     */
    static TMXTiledSprite* create(const PolygonInfo& info);
    
    /**
     * Creates a sprite with an image filename and a rect.
     *
     * @param   filename A path to image file, e.g., "scene1/monster.png".
     * @param   rect     A subrect of the image file.
     * @return  An autoreleased sprite object.
     */
    static TMXTiledSprite* create(const std::string& filename, const Rect& rect);
    
    /**
     * Creates a sprite with a Texture2D object.
     *
     * After creation, the rect will be the size of the texture, and the offset will be (0,0).
     *
     * @param   texture    A pointer to a Texture2D object.
     * @return  An autoreleased sprite object.
     */
    static TMXTiledSprite* createWithTexture(Texture2D *texture);
    
    /**
     * Creates a sprite with a texture and a rect.
     *
     * After creation, the offset will be (0,0).
     *
     * @param   texture     A pointer to an existing Texture2D object.
     *                      You can use a Texture2D object for many sprites.
     * @param   rect        Only the contents inside the rect of this texture will be applied for this sprite.
     * @param   rotated     Whether or not the rect is rotated.
     * @return  An autoreleased sprite object.
     */
    static TMXTiledSprite* createWithTexture(Texture2D *texture, const Rect& rect, bool rotated=false);
    
    /**
     * Creates a sprite with an sprite frame.
     *
     * @param   spriteFrame    A sprite frame which involves a texture and a rect.
     * @return  An autoreleased sprite object.
     */
    static TMXTiledSprite* createWithSpriteFrame(SpriteFrame *spriteFrame);
    
    /**
     * Creates a sprite with an sprite frame name.
     *
     * A SpriteFrame will be fetched from the SpriteFrameCache by spriteFrameName param.
     * If the SpriteFrame doesn't exist it will raise an exception.
     *
     * @param   spriteFrameName A null terminated string which indicates the sprite frame name.
     * @return  An autoreleased sprite object.
     */
    static TMXTiledSprite* createWithSpriteFrameName(const std::string& spriteFrameName);
    
    TMXTiledSprite();
    ~TMXTiledSprite();
    
    void applySeaShader(const std::string& textureLight = "shaders/caustics.png",
                        const std::string& fShaderFilename = "shaders/water.fsh");
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void update(float dt) override;
    virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
    void onDraw(const Mat4 &transform, uint32_t flags);
    
protected:
    bool _AutoScrollU = true;
    float _AutoScrollSpeedU = 0;
    bool _AutoScrollV = false;
    float _AutoScrollSpeedV = 0;
    float _AutoScrollCountU = 0;
    float _AutoScrollCountV = 0;
    CustomCommand _customCommand;
};

NS_CC_END

#endif /* CCTMXTiledSprite_hpp */
