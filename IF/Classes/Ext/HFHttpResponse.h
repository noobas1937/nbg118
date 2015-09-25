//
//  HFHttpResponse.h
//  HappyFarm
//
//  Created by 舒 圣林 on 10/21/13.
//
//

#ifndef __HappyFarm__HFHttpResponse__
#define __HappyFarm__HFHttpResponse__

#include    "CommonInclude.h"
#include "HFHttpRequest.h"

class HFHttpResponse : public CCHttpResponse
{
public:
    HFHttpResponse(HFHttpRequest* request);
    virtual ~HFHttpResponse();
    HFHttpRequest* getRequest();
    HFHttpRequest* getHttpRequest();
    
protected:
    HFHttpRequest* m_pRequest;
};


#endif /* defined(__HappyFarm__HFHttpResponse__) */
