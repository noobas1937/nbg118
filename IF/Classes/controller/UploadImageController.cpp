//
//  UploadImageController.cpp
//  IF
//
//  Created by chenfubi on 15/4/16.
//
//

#include "UploadImageController.h"
#include <spine/Json.h>
#include "../view/guiExt/md5.h"

static UploadImageController* s_sharedUploadImageController = NULL;

UploadImageController* UploadImageController::shared(void)
{
	if (!s_sharedUploadImageController)
	{
		s_sharedUploadImageController = new UploadImageController();
	}
	return s_sharedUploadImageController;
}

void UploadImageController::purge()
{
	CC_SAFE_RELEASE_NULL(s_sharedUploadImageController);
}

UploadImageController::UploadImageController()
:m_uploadImageFlag(0)
{
//#if COCOS2D_DEBUG > 0
//    m_uploadImageFlag = 1;
//#endif
}

UploadImageController::~UploadImageController()
{
	
}

void UploadImageController::onUploadImage(int strCode)
{
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(UploadImageController::getHeadImgBack), MSG_GetHeadImgUrl, NULL);
    GlobalData::shared()->isUploadPic = true;
    CCCommonUtils::onUploadPhoto(GlobalData::shared()->playerInfo.uid, strCode, 1);
}

void UploadImageController::getHeadImgBack(cocos2d::CCObject *pObj)
{
    CCString* gPUid = dynamic_cast<CCString*>(pObj);
    if (gPUid != NULL)
    {
        string filePath = gPUid->getCString();
        HFHttpRequest* pHttpRequest = new HFHttpRequest();
        
        string url = "http://p1cok.elexapp.com/gameservice/www/gateway/json.php";
        pHttpRequest->setUrl(url.c_str());
        pHttpRequest->setRequestType(CCHttpRequest::Type::POST);
        pHttpRequest->setResponseCallback(this, httpresponse_selector_hf(UploadImageController::onHttpRequestCompleted));
        
        //  设置回调函数；
        if (!filePath.empty()) {
            CCLOGFUNCF("upload file: %s", filePath.c_str());
            filePath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filePath.c_str());
            CCLOGFUNCF("upload file2: %s", filePath.c_str());
            // initalize custom header list (stating that Expect: 100-continue is not wanted
            pHttpRequest->addHeader("Expect:");
            pHttpRequest->setUploadFile(filePath);
            
        }
        
        string uid = GlobalData::shared()->playerInfo.uid;
        int picVer = GlobalData::shared()->playerInfo.picVer;
        picVer = picVer%1000000;
        int photo_seq = picVer + 1;
        
        string strAuthKey = "";
        {
            string kvstr= "";
            kvstr.append("x645rGA7rnG5yOZkGijd");
            kvstr.append("data0commandNameuser/UploadPhoto");
            kvstr.append("params0");
            kvstr.append("gameuid").append(uid);
            kvstr.append("photo_seq");
            kvstr.append(CC_ITOA(photo_seq));
//            CCLOG("kvStr:%s", kvstr.c_str());
            MD5 md5(kvstr);
            strAuthKey = md5.toString();
        }
        ::transform(strAuthKey.begin(),strAuthKey.end(), strAuthKey.begin(), ::toupper);
        
        long enqueueTime = GlobalData::shared()->getWorldTime();
        
        /*data:  {"data":[{"commandName":"user/UploadPhoto","params":[{"photo_seq":"2","gameuid":1192855747000004}]}],"authkey":"6623C98E44958BE1F22BA5316EDA4DBF"}*/
        string data = "{\"data\":[{\"commandName\":\"user/UploadPhoto\",";
//        data.append("\"enqueueTime\":").append(CC_ITOA(enqueueTime)).append(",");
        data.append("\"params\":[{");
        data.append("\"photo_seq\":\"").append(CC_ITOA(photo_seq)).append("\",");
        data.append("\"gameuid\":").append(uid);
        data.append("}]}],");
        data.append("\"authkey\":\"");
        data.append(strAuthKey).append("\"");
        data.append("}");
        CCLOG("data:%s",data.c_str());
        pHttpRequest->setRequestData(data);
        
        pHttpRequest->setTag("UploadPhoto");
        
        HFHttpClient::getInstance()->send(pHttpRequest);
        pHttpRequest->release();
    }
    else
    {
        CCLOG("get userHead Img success, but no data get on notify");
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this , MSG_GetHeadImgUrl);
}

void UploadImageController::onHttpRequestCompleted(HFHttpClient *pSender, HFHttpResponse *pResponse)
{
    do
    {
        CCAssert(pResponse != NULL, "http response NULL");
        
#if defined(COCOS2D_DEBUG) && COCOS2D_DEBUG > 0
        std::vector<char>* headers = pResponse->getResponseHeader();
        string strHeader (headers->begin(),headers->end());
        CCLOGFUNCF("resp headers: %s",strHeader.c_str());
#endif
        CCDictionary* pDictContent = NULL;
        if (pResponse->isSucceed())
        {
            onLoadHeadCmd(parseResponseData(pResponse->getResponseData()));
        }
        else
        {
            CCDictionary* dict = CCDictionary::create();
            dict->setObject(ccs("-1"), "errorCode");
            onLoadHeadImgFail(dict);
        }
        
    } while (0);
}

CCDictionary* UploadImageController::parseResponseData(vector<char>* response)
{
    CCDictionary* pDictContent = NULL;
    do {
        CC_BREAK_IF(response == NULL);
        string m_sResponseData;
        m_sResponseData.clear();
        m_sResponseData.assign(response->begin(),response->end());
        CCLOG("m_sResponseData:%s", m_sResponseData.c_str());
        if (0 == m_sResponseData.size())
        {
            CCLOG("通信成功，但未返回任何数据。");
            //  通信失败；
            pDictContent = CCDictionary::create();
            CC_BREAK_IF(NULL == pDictContent);
            
            pDictContent->setObject(ccs("-1"), "errorCode");
            pDictContent->setObject(CCString::create("Invalid Response data!"), "errorMsg");
        }
        else
        {
            //  将应答包数据序列化为json对象；
            try
            {
                //todocfb
                Json *jsonObj = Json_create(m_sResponseData.c_str());
                Json *jsonResponse = NULL;
                Json *jsonData = NULL;
//                Json *jsonStatus = NULL;
                int status = -1;
                if (jsonObj)
                {
                    jsonResponse = Json_getItem(jsonObj, "response");
                }
                if (jsonResponse)
                {
                    jsonData = Json_getItemAt(jsonResponse, 0);
                }
                if (jsonData)
                {
                    status = Json_getInt(jsonData, "status", -1);
                }
                pDictContent = CCDictionary::create();
                if (status == 1)
                {
                    
                }
                else
                {
                    pDictContent->setObject(ccs("-1"), "errorCode");
                    pDictContent->setObject(CCString::create("Invalid Response data!"), "errorMsg");

                }
                
                Json_dispose(jsonObj);
            } catch (std::invalid_argument &ia)
            {
                CCLOGERROR("parse json error: %s data: %s",ia.what(),m_sResponseData.c_str());
                
                pDictContent = CCDictionary::create();
                CC_BREAK_IF(NULL == pDictContent);
                
                pDictContent->setObject(ccs("-1"), "errorCode");
                pDictContent->setObject(CCString::create("Invalid Response data!"), "errorMsg");
            }
        }
    } while (0);
    return pDictContent;
}

void UploadImageController::onLoadHeadCmd(cocos2d::CCObject *pObj)
{
    CCDictionary* dict = dynamic_cast<CCDictionary*>(pObj);
    if (dict->objectForKey("errorCode"))
    {
        onLoadHeadImgFail(pObj);
    }
    else
    {
        UpdatePicCommand* cmd = new UpdatePicCommand();
        cmd->sendAndRelease();
        if (GlobalData::shared()->picUploadFlag == 0) {
            CCCommonUtils::flyHint("", "", _lang("4100003"));
        }
        else{
        
        }
    }
}

void UploadImageController::onLoadHeadImgFail(cocos2d::CCObject *pObj)
{
    CCDictionary* dict = dynamic_cast<CCDictionary*>(pObj);
    if (dict->objectForKey("errorCode"))
    {
        CCCommonUtils::flyHint("", "", _lang("4100004"));
    }
}

//CLASS
bool UpdatePicCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare("user.update.picVer") != 0)
        return false;
    CCDictionary* params = _dict(dict->objectForKey("params"));
    if(params==NULL) return false;
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0)
    {
        CCCommonUtils::flyText(_lang(pStr->getCString()).c_str());
    }
    else
    {
        if (params->objectForKey("nextUpdateTime"))
        {
            
            GlobalData::shared()->playerInfo.pic_nextUpdateTime = params->valueForKey("nextUpdateTime")->doubleValue();
        }
        if (params->objectForKey("picVer"))
        {
            CCCommonUtils::flyHint("", "", _lang("105771"));
            GlobalData::shared()->playerInfo.picVer = params->valueForKey("picVer")->intValue();
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_UPLOADIMAGE_BACK);
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CHANGE_PLAYER_PIC);
            
        }
        
    }
    return true;
}