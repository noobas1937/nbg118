/****************************************************************************
 Copyright (c) 2010-2012 cocos2d-x.org
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __HFHTTP_REQUEST_H__
#define __HFHTTP_REQUEST_H__

#include    "CommonInclude.h"
class HFHttpClient;
class HFHttpResponse;

//USING_NS_CC;

typedef void (CCObject::*SEL_HFHttpResponse)(HFHttpClient* client, HFHttpResponse* response);
#define httpresponse_selector_hf(_SELECTOR) (SEL_HFHttpResponse)(&_SELECTOR)

/** 
 @brief defines the object which users must packed for CCHttpClient::send(HttpRequest*) method.
 Please refer to samples/TestCpp/Classes/ExtensionTest/NetworkTest/HttpClientTest.cpp as a sample
 @since v2.0.2
 */
class HFHttpRequest : public CCHttpRequest
{
public:
    HFHttpRequest():m_uploadFile(""),m_pUserData(NULL){
    }
    virtual ~HFHttpRequest(){
//        CCLOGFUNC();
        CC_SAFE_RELEASE(m_pUserData);
    }
    
    inline void setResponseCallback(CCObject* pTarget, SEL_HFHttpResponse pSelector)
    {
        _pTarget = pTarget;
        _pHFSelector = pSelector;
        CC_SAFE_RETAIN(_pTarget);
    }
    
    inline void addHeader(const string& header)
    {
        _headers.push_back(header);
    }
    
    inline bool isUploadFile()
    {
        return ! m_uploadFile.empty();
    }
    
    inline void setRequestData(const string& data)
    {
        _requestData.assign(data.begin(), data.end());
    };
    
    inline SEL_HFHttpResponse getHFSelector(){
        return _pHFSelector;
    }

protected:
    CC_SYNTHESIZE_PASS_BY_REF(string, m_uploadFile, UploadFile);
    CC_SYNTHESIZE_RETAIN(CCObject*, m_pUserData, UserData);
    SEL_HFHttpResponse       _pHFSelector;      /// callback function, e.g. MyLayer::onHttpResponse(HFHttpClient *sender, CCHttpResponse * response)
};

#endif //__HFHTTP_REQUEST_H__
