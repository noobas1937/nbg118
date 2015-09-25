//
//  UploadImageController.h
//  IF
//
//  Created by chenfubi on 15/4/16.
//
//

#ifndef __IF__UploadImageController__
#define __IF__UploadImageController__

#include "CommonInclude.h"
#include "HFHttpRequest.h"
#include "HFHttpClient.h"

#define MSG_UPLOADIMAGE_BACK "MSG_UPLOADIMAGE_BACK"

class UploadImageController : public CCObject
{
public:
	static UploadImageController * shared();
	void purge();

    void onUploadImage(int srcCord);
private:
	UploadImageController();
	virtual ~UploadImageController();
	UploadImageController(const UploadImageController &);
    UploadImageController &operator=(const UploadImageController &);
    void getHeadImgBack(CCObject* pObj);
    void onHttpRequestCompleted(HFHttpClient *pSender, HFHttpResponse *pResponse);
    CCDictionary* parseResponseData(vector<char>* response);
    void onLoadHeadCmd(CCObject* pObj);
    void onLoadHeadImgFail(CCObject* pObj);
    
public:
    CC_SYNTHESIZE(int, m_uploadImageFlag, UploadImageFlag);
};


#include "CommandBase.h"
class UpdatePicCommand : public CommandBase
{
public:
    UpdatePicCommand():CommandBase("user.update.picVer")
    {
        
    }
protected:
    virtual bool handleRecieve(cocos2d::CCDictionary *dict);
};
#endif /* defined(__IF__UploadImageController__) */
