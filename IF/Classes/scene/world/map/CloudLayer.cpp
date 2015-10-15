//
//  CloudLayer.cpp
//  IF
//
//  Created by gj on 10/15/15.
//
//

#include "CloudLayer.hpp"

void CloudLayer::visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags)
{
    auto map = getParent()->getParent();
    auto pos = map->getPosition();
    if (!pos.equals(_lastPosition)) {
        
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        
        pos.x -= winSize.width/2;
        pos.y -= winSize.height/2;
        
        for(unsigned int i=0; i < _parallaxArray->num; i++ )
        {
            auto point = dynamic_cast<PointObject*>(_parallaxArray->arr[i]);
            float x = (pos.x/map->getScaleX() + point->getOffset().x) * point->getRatio().x + point->getOffset().x;
            float y = (pos.y/map->getScaleY() + point->getOffset().y) * point->getRatio().y + point->getOffset().y;
            point->getChild()->setPosition(ccp(x,y));
        }
        
        _lastPosition = pos;
    }
    CCNode::visit(renderer,parentTransform,parentFlags);
}
