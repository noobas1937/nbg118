//
//  HFUrlImage.h
//  HappyFarm
//
//  Created by 舒 圣林 on 14-2-25.
//
//

#ifndef __HappyFarm__HFUrlImage__
#define __HappyFarm__HFUrlImage__

#include "CommonInclude.h"

enum HFImageLoadState {
    kImageLoadInit,
    kImageLoading,
    kImageLoadOK ,
    kImageLoadFail
};

class HFUrlImage : public CCImage
{
public:
	virtual bool init();
	CREATE_FUNC(HFUrlImage);
	HFUrlImage();
    void startLoadImageAsync(const char* url,CCCallFuncO* callback);
	virtual ~HFUrlImage();
    CCTexture2D* getTexture();
    
private:
    void onLoadCompleted(CCHttpClient *sender, CCHttpResponse *response);
    CCCallFuncO* m_completedCallback;
    CCTexture2D* m_imageTexture;
    
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(string, m_imageUrl, ImageUrl);
    CC_SYNTHESIZE_READONLY(HFImageLoadState, m_state, ImageLoadStat);
    
};

#endif /* defined(__HappyFarm__HFUrlImage__) */
