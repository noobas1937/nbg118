//
//  HFUrlImage.cpp
//  HappyFarm
//
//  Created by 舒 圣林 on 14-2-25.
//
//

#include "HFUrlImage.h"

bool HFUrlImage::init()
{
	bool bRet = false;
	do
	{
		// TODO: auto generated code
		bRet = true;
	}while(0);
	return bRet;
}

void HFUrlImage::startLoadImageAsync(const char *url, cocos2d::CCCallFuncO *callback)
{
    CCAssert(url != NULL, "url cant be null");
    if (url == NULL) {
        if (callback != NULL) {
            callback->setObject(NULL);
            callback->execute();
        }
        return;
    }
    m_imageUrl.assign(url);
    
//    CCLOGFUNCF("start load image : %s",url);
    CCHttpRequest* request = new CCHttpRequest();
    request->setUrl(url);
    request->setRequestType(CCHttpRequest::Type::GET);
    request->setResponseCallback(this, httpresponse_selector(HFUrlImage::onLoadCompleted));
    CCHttpClient::getInstance()->send(request);
    request->release();
    m_completedCallback = callback;
    CC_SAFE_RETAIN(m_completedCallback);
    m_state = kImageLoading;
}
CCTexture2D* HFUrlImage::getTexture()
{
    if (m_imageTexture != NULL) {
        return m_imageTexture;
    }
    if (m_state != kImageLoadOK) {
        return NULL;
    }
    m_imageTexture = new CCTexture2D();
    if(m_imageTexture->initWithImage(this) == false){
        CC_SAFE_RELEASE_NULL(m_imageTexture);
    }
    return m_imageTexture;
}

void HFUrlImage::onLoadCompleted(CCHttpClient *sender, CCHttpResponse *response)
{
    do {
        if(response == NULL || response->isSucceed() == false){
            m_state = kImageLoadFail;
            break;
        }
        vector<char> *buffer = response->getResponseData();
        //create image
        if(initWithImageData((unsigned char*)buffer->data(),buffer->size()) == true){
            m_state = kImageLoadOK;
        }else{
            m_state = kImageLoadFail;
        }
    } while (0);
    
    if (m_completedCallback != NULL) {
        m_completedCallback->setObject(this);
        m_completedCallback->execute();
        CC_SAFE_RELEASE_NULL(m_completedCallback);
    }
}

HFUrlImage::HFUrlImage():
m_completedCallback(NULL)
,m_state(kImageLoadInit)
,m_imageUrl("")
,m_imageTexture(NULL)
{
	
}

HFUrlImage::~HFUrlImage()
{
	CC_SAFE_RELEASE(m_imageTexture);
}
