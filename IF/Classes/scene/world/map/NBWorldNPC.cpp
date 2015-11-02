//
//  NBWorldNPC.cpp
//  IF
//
//  Created by gj on 10/16/15.
//
//

#include "NBWorldNPC.hpp"

#include "WorldController.h"
#include "WorldMapView.h"
#include "IFMapMaskLayer.h"
#include "DynamicTiledMap.h"

#define WORLD_CONTROLLER   WorldController::getInstance()
#define WORLD_MAP_VIEW     WorldMapView::instance()

RepeatForever* NBWorldNPC::createAnimation(const char * plist_filename, const char* animation_filename, int animation_start_id, int animation_end_id)
{
    auto spriteCache = SpriteFrameCache::getInstance();
    spriteCache->addSpriteFramesWithFile(plist_filename);
    Vector<SpriteFrame*> vsp;
    for (int i = animation_start_id; i <= animation_end_id; i++)
    {
        String *string = String::createWithFormat(animation_filename, i);
        SpriteFrame *spfr = spriteCache->getSpriteFrameByName(string->getCString());
        vsp.pushBack(spfr);
    }
    Animation *animation = Animation::createWithSpriteFrames(vsp, 0.1);
    Animate *animate = Animate::create(animation);
    auto *ac1 = RepeatForever::create(animate);
    
    return ac1;
}

#pragma mark - Octopus

class OctopusNode
: public Node
{
public:
    static OctopusNode * create()
    {
        OctopusNode * ret = new (std::nothrow) OctopusNode();
        if (ret && ret->init())
        {
            ret->autorelease();
        }
        else
        {
            CC_SAFE_DELETE(ret);
        }
        return ret;
    }
    
    Sprite* octopus;
    float show_time;
};
void NBWorldNPC::updateOctopus(const Vec2& currentTilePoint, Node* animation_layer)
{
    static const int SEA_MONSTER_AREA_X = 5;
    static const int SEA_MONSTER_AREA_Y = 10;
    static const int SEA_MONSTER_TAG = WM_BETWEEN_SERVER_MAP_TAG + 1;
    static const int SEA_MONSTER_GZ = -2;
    
    srand((unsigned int)time(0));
    int random_variable = rand() % 100;
    if (random_variable > 50) return;
    
    bool isX = abs(currentTilePoint.x - _tile_count_x) <= SEA_MONSTER_AREA_X;
    bool isY = abs(currentTilePoint.y - _tile_count_y) <= SEA_MONSTER_AREA_Y;
    if (!isX && !isY)
    {
        return;
    }
    
    int tx = _tile_count_x + 2;
    int ty = _tile_count_y + 2;
    float sx = 2 + rand() % 4;
    float sy = sx;
    float speedx = -10;
    float speedy = 5;
    float speedScale = 0.8 + 1.0 / (1.0 + rand() % 100);
    random_variable = rand() % 100;
    int offset_tile = 0;//(rand() % 2) * (random_variable > 50 ? 1 : -1);
    if (isX && !isY)
    {
        ty = currentTilePoint.y + offset_tile;
        
        if (random_variable > 50)
        {
            tx = _tile_count_x + 1;
            sx *= 1;
            sy *= -1;
            
            speedx *= speedScale;
            speedy *= -speedScale;
        }
        else
        {
            tx = _tile_count_x + 2;
            sx *= -1;
            sy *= 1;
            
            speedx *= -speedScale;
            speedy *= speedScale;
        }
    }
    else if (!isX && isY)
    {
        tx = currentTilePoint.x + offset_tile;
        
        if (random_variable > 50)
        {
            ty = _tile_count_y + 1;
            sx *= -1;
            sy *= -1;
            
            speedx *= -speedScale;
            speedy *= -speedScale;
        }
        else
        {
            ty = _tile_count_y + 2;
            sx *= 1;
            sy *= 1;
            
            speedx *= speedScale;
            speedy *= speedScale;
        }
    }
    
    auto fPos = WORLD_MAP_VIEW->m_map->getViewPointByTilePoint(Vec2(tx, ty));
    auto mSprite_pos = fPos;
    auto mSprite = OctopusNode::create();
    mSprite->setScaleX(sx);
    mSprite->setScaleY(sy);
    int random_offset_y = 128 / 2 + rand() % 30;
    mSprite->setPosition(mSprite_pos.x, mSprite_pos.y + random_offset_y);
    mSprite->setTag(SEA_MONSTER_TAG);
    animation_layer->addChild(mSprite);
    
    mSprite->octopus = Sprite::createWithSpriteFrameName("anima_sea_monster_01.png");
    mSprite->octopus->setOpacity(0);
    mSprite->addChild(mSprite->octopus);
    auto *ac1 = createAnimation("World/World_5.plist", "anima_sea_monster_0%d.png", 1, 8);
    mSprite->octopus->runAction(ac1);
    
    int total_time = 12.0 + rand() % 8;
    mSprite->show_time = total_time;
    
    mSprite->schedule([mSprite, speedx, speedy, total_time](float dt){
        if (mSprite->show_time < 0)
        {
            mSprite->removeFromParent();
            return;
        }
        
        mSprite->setPositionX(mSprite->getPositionX() + dt * speedx);
        mSprite->setPositionY(mSprite->getPositionY() + dt * speedy);
        
        // 慢慢消失
        if (mSprite->show_time <= 2.0)
        {
            int o = 255.0  * 30 / 100 * mSprite->show_time / 2.0;
            mSprite->octopus->setOpacity(o);
        }
        // 慢慢出现
        else if (mSprite->octopus->getOpacity() <= 255 * 30 / 100)
        {
            mSprite->octopus->setOpacity(mSprite->octopus->getOpacity() + 3);
        }
        mSprite->show_time -= dt;
    }, "move");
}

#pragma mark - birds

class BirdsNode
: public Node
{
public:
    static BirdsNode * create()
    {
        BirdsNode * ret = new (std::nothrow) BirdsNode();
        if (ret && ret->init())
        {
            ret->autorelease();
        }
        else
        {
            CC_SAFE_DELETE(ret);
        }
        return ret;
    }
    
    float show_time;
    int opacity;
};
void NBWorldNPC::updateBirds(const Vec2& currentTilePoint, Node* animation_layer)
{
    srand((unsigned int)time(0));
    
    int random_variable = rand() % 100;
    if (random_variable > 50) return;
    
    int pos_offset_X = (rand() % 300) * (rand() % 2 == 0 ? 1 : -1);
    int pos_offset_Y = (rand() % 600) * (rand() % 2 == 0 ? 1 : -1);
    auto current_pos = WORLD_MAP_VIEW->m_map->getViewPointByTilePoint(Vec2(currentTilePoint.x, currentTilePoint.y));
    BirdsNode* birdsNode = BirdsNode::create();
    birdsNode->setPosition(current_pos.x + pos_offset_X, current_pos.y + pos_offset_Y);
    birdsNode->opacity = 0;
    int total_time = 20.0 + rand() % 18;
    birdsNode->show_time = total_time;
    
    animation_layer->addChild(birdsNode, 100); // 尽量放在最上层
    
    int flipX = rand() % 2 == 0 ? 1 : -1;
    int flipY = rand() % 2 == 0 ? 1 : -1;
    auto *ac1 = createAnimation("World/World_5.plist", "anima_bird_0%d.png", 1, 8);
    int is = rand() % 5 + 1;
    int js = rand() % 5 + 1;
    int ii = rand() % 5 + 1 + is;
    int jj = rand() % 5 + 1 + js;
//    int kk = rand() % 3;
//    int ll = rand() % 3;
    for (int i = is; i < ii; i++)
    {
        for (int j = js; j < jj; j++)
        {
            auto bird = Sprite::createWithSpriteFrameName("anima_bird_01.png");
            bird->setOpacity(0);
            bird->setPosition(i * bird->getContentSize().width + rand() % (int)(bird->getContentSize().width / 2), j * bird->getContentSize().height + rand() % (int)(bird->getContentSize().height / 2));
            birdsNode->setScaleX(flipX);
            birdsNode->setScaleY(flipY);
            birdsNode->addChild(bird);
            bird->runAction(ac1->clone());
        }
    }
    
    float speed = 15.0 + 30.0 / (1.0 + rand() % 100);
    float speedx = speed * flipX;
    float speedy = speed * flipY;
    birdsNode->schedule([birdsNode, speedx, speedy, total_time](float dt){
        if (birdsNode->show_time < 0)
        {
            birdsNode->removeFromParent();
            return;
        }
        
        birdsNode->setPositionX(birdsNode->getPositionX() + dt * speedx);
        birdsNode->setPositionY(birdsNode->getPositionY() + dt * speedy);
        
        if (birdsNode->show_time <= 2.0)
        {
            int o = 255.0 * birdsNode->show_time / 2.0;
            for (auto it = birdsNode->getChildren().begin(); it != birdsNode->getChildren().end(); ++it)
            {
                (*it)->setOpacity(o);
            }
        }
        else if (birdsNode->opacity < 245)
        {
            birdsNode->opacity += 10;
            for (auto it = birdsNode->getChildren().begin(); it != birdsNode->getChildren().end(); ++it)
            {
                (*it)->setOpacity(birdsNode->opacity);
            }
        }
        birdsNode->show_time -= dt;
    }, "move");
}
