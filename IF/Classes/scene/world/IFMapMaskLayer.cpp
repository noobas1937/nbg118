//
//  IFMapMaskLayer.cpp
//  IF
//
//  Created by wangdianzhen on 15/7/10.
//
//

#include "IFMapMaskLayer.h"
#include "WorldController.h"

 
IFMapMaskLayer::IFMapMaskLayer(){

}
IFMapMaskLayer::~IFMapMaskLayer(){

}
IFMapMaskLayer * IFMapMaskLayer::create(const ccColor4B& color, GLfloat width, GLfloat height){
    IFMapMaskLayer* pRet = new IFMapMaskLayer();
    if (pRet && pRet->initWithColor(color, width, height)) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}