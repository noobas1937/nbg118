//
//  LoadWebImg.cpp
//  IF
//
//  Created by ganxiaohua on 14-10-15.
//
//

#include "LoadWebImg.h"

bool LoadWebImg::init(){
	bool bRet = false;
    if (CCSprite::init())
    {
        m_urlImg = HFUrlImage::create();
		bRet = true;
    }
	return bRet;
}

void LoadWebImg::onEnter(){
    CCSprite::onEnter();
}

void LoadWebImg::onExit(){
    CCSprite::onExit();
}

void LoadWebImg::callBack(CCObject* p){
    CCTexture2D* texture = m_urlImg->getTexture();
    if(texture!=NULL){
        this->initWithTexture(texture);
    }
}

void LoadWebImg::setLoadURL(string imgURL){
    m_urlImg->startLoadImageAsync(imgURL.c_str(),CCCallFuncO::create(this, callfuncO_selector(LoadWebImg::callBack), NULL));
}