//
//  NBSprite3D.h
//  IF
//
//  Created by guojiang on 8/19/15.
//
//

#ifndef __IF__NBSprite3D__
#define __IF__NBSprite3D__

#include "cocos2d.h"

class NBSprite3D
: public cocos2d::Sprite3D
{
public:
    NBSprite3D():
    m_shining_texture(nullptr)
    {};
    virtual ~NBSprite3D(){releaseTex();};
    static NBSprite3D* create(const std::string &modelPath);
    
    // speed : 变化速度
    // min, max : 变化区间 [min, max]
    void setEmittingShaderDuration(float speed = 7.0, float min = 0.0, float max = 10.0);
    // caustics_color: titan 发光的颜色
    // texture1: 用于调整模型贴图颜色的灰度图文件路径
    // texture0: 给模型换新的贴图文件路径
    void applyEmittingShader(const cocos2d::Vec3& caustics_color,
                             const std::string& textureLight,
                             const std::string& vShaderFilename = "shaders/titan.vsh",
                             const std::string& fShaderFilename = "shaders/titan.fsh");
    
    // test
    void applySeaShader(const std::string& texture1,
                        const std::string& textureLight,
                        const std::string& vShaderFilename = "shaders/UVAnimation.vsh",
                        const std::string& fShaderFilename = "shaders/UVAnimation.fsh");
    
    void releaseTex(){
        if (m_shining_texture)
        {
            CC_SAFE_RELEASE(m_shining_texture);
        }
    }
protected:
    
    // shader start-----------
    void titanUpdate(float dt);
    
    float m_shining_duration; // 取值范围 [min_duraion, max_duraion]
    float m_shining_duration_step;
    float m_min_duration;
    float m_max_duration;
    bool m_fade_in;
    // shader end ------------
    cocos2d::Texture2D* m_shining_texture;
    
    // test
    void updateSeaShader(float dt);
    cocos2d::Vec2 m_LightAni;
};

#endif /* defined(__IF__NBSprite3D__) */
