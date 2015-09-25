//
//  HFHttpResponse.cpp
//  HappyFarm
//
//  Created by 舒 圣林 on 10/21/13.
//
//

#include "HFHttpResponse.h"

USING_NS_CC_EXT;

HFHttpResponse::HFHttpResponse(HFHttpRequest* request):
CCHttpResponse(NULL),
m_pRequest(request)
{
    CC_SAFE_RETAIN(request);
}

HFHttpRequest* HFHttpResponse::getRequest()
{
    return m_pRequest;
}

HFHttpRequest* HFHttpResponse::getHttpRequest()
{
    return m_pRequest;
}

HFHttpResponse::~HFHttpResponse()
{
//    CCLOGFUNC();
    CC_SAFE_RELEASE_NULL(m_pRequest);
}
