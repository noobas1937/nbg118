//
//  IFMapMaskLayer.h
//  IF
//
//  Created by wangdianzhen on 15/7/10.
//
//

#ifndef __IF__IFMapMaskLayer__
#define __IF__IFMapMaskLayer__

#include "CommonInclude.h"
class IFMapMaskLayer : public CCLayerColor{
public:
    static IFMapMaskLayer * create(const ccColor4B& color, GLfloat width, GLfloat height);
    IFMapMaskLayer();
    ~IFMapMaskLayer();
    CC_SYNTHESIZE(int, m_serverId, serverId);
};

#endif /* defined(__IF__IFMapMaskLayer__) */
