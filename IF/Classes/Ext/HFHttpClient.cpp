/****************************************************************************
 Copyright (c) 2010-2012 cocos2d-x.org
 Copyright (c) 2012 greathqy
 
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

#include "HFHttpClient.h"
#include <queue>
#include <pthread.h>
#include <errno.h>
#include "curl/curl.h"

static pthread_t        s_networkThreadHF;
static pthread_mutex_t  s_requestQueueMutexHF;
static pthread_mutex_t  s_responseQueueMutexHF;

static pthread_mutex_t	s_SleepMutexHF;
static pthread_cond_t	s_SleepConditionHF;

static unsigned long    s_asyncRequestCountHF = 0;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
typedef int int32_t;
#endif

static bool need_quitHF = false;

static CCArray* s_requestQueueHF = NULL;
static CCArray* s_responseQueueHF = NULL;

static HFHttpClient *s_pHttpClientHF = NULL; // pointer to singleton
static CURL* s_pCurl = NULL;

static char s_errorBufferHF[CURL_ERROR_SIZE];
static CURLcode s_errorCodeHF = CURLE_OK;
typedef size_t (*write_callback)(void *ptr, size_t size, size_t nmemb, void *stream);

// Callback function used by libcurl for collect response data
static size_t writeData(void *ptr, size_t size, size_t nmemb, void *stream)
{
    std::vector<char> *recvBuffer = (std::vector<char>*)stream;
    size_t sizes = size * nmemb;
    
    // add data to the end of recvBuffer
    // write data maybe called more than once in a single request
    recvBuffer->insert(recvBuffer->end(), (char*)ptr, (char*)ptr+sizes);
    
    return sizes;
}

// Callback function used by libcurl for collect header data
static size_t writeHeaderData(void *ptr, size_t size, size_t nmemb, void *stream)
{
    std::vector<char> *recvBuffer = (std::vector<char>*)stream;
    size_t sizes = size * nmemb;
    
    // add data to the end of recvBuffer
    // write data maybe called more than once in a single request
    recvBuffer->insert(recvBuffer->end(), (char*)ptr, (char*)ptr+sizes);
    
    return sizes;
}


static int processGetTask(HFHttpRequest *request, write_callback callback, void *stream, long *errorCode, write_callback headerCallback, void *headerStream);
static int processPostTask(HFHttpRequest *request, write_callback callback, void *stream, long *errorCode, write_callback headerCallback, void *headerStream);
static int processPutTask(HFHttpRequest *request, write_callback callback, void *stream, long *errorCode, write_callback headerCallback, void *headerStream);
static int processDeleteTask(HFHttpRequest *request, write_callback callback, void *stream, long *errorCode, write_callback headerCallback, void *headerStream);
static int processUploadTask(HFHttpRequest *request, write_callback callback, void *stream, long *errorCode, write_callback headerCallback, void *headerStream);
// int processDownloadTask(HttpRequest *task, write_callback callback, void *stream, int32_t *errorCode);


// Worker thread
static void* networkThread(void *data)
{
    HFHttpRequest *request = NULL;
    
    while (true)
    {
        if (need_quitHF)
        {
            break;
        }
                
        // step 1: send http request if the requestQueue isn't empty
        request = NULL;
        
        pthread_mutex_lock(&s_requestQueueMutexHF); //Get request task from queue
        if (0 != s_requestQueueHF->count())
        {
            request = dynamic_cast<HFHttpRequest*>(s_requestQueueHF->objectAtIndex(0));
            s_requestQueueHF->removeObjectAtIndex(0);
            CCLOG("get request tag: %s",request->getTag());
            // request's refcount = 1 here
        }
        pthread_mutex_unlock(&s_requestQueueMutexHF);
        
        if (NULL == request)
        {
        	// Wait for http request tasks from main thread
        	pthread_cond_wait(&s_SleepConditionHF, &s_SleepMutexHF);
            continue;
        }
        
        // step 2: libcurl sync access
    
        // Create a HttpResponse object, the default setting is http access failed
        HFHttpResponse *response = new HFHttpResponse(request);
        
        // request's refcount = 2 here, it's retained by HttpRespose constructor
        request->release();
        // ok, refcount = 1 now, only HttpResponse hold it.
        
        long responseCode = -1;
        int retValue = 0;

        // Process the request -> get response packet
        switch (request->getRequestType())
        {
            case CCHttpRequest::Type::GET: // HTTP GET
                retValue = processGetTask(request,
                                          writeData, 
                                          response->getResponseData(), 
                                          &responseCode,
                                          writeHeaderData,
                                          response->getResponseHeader());
                break;
            
            case CCHttpRequest::Type::POST: // HTTP POST
                if (request->isUploadFile()) {
                    retValue = processUploadTask(request,
                                                 writeData,
                                                 response->getResponseData(),
                                                 &responseCode,
                                                 writeHeaderData,
                                                 response->getResponseHeader());
                    
                }else{
                    retValue = processPostTask(request,
                                           writeData, 
                                           response->getResponseData(), 
                                           &responseCode,
                                           writeHeaderData,
                                           response->getResponseHeader());
                }
                break;

            case CCHttpRequest::Type::PUT:
                retValue = processPutTask(request,
                                          writeData,
                                          response->getResponseData(),
                                          &responseCode,
                                          writeHeaderData,
                                          response->getResponseHeader());
                break;

            case CCHttpRequest::Type::DELETE:
                retValue = processDeleteTask(request,
                                             writeData,
                                             response->getResponseData(),
                                             &responseCode,
                                             writeHeaderData,
                                             response->getResponseHeader());
                break;
            
            default:
                CCAssert(true, "HFHttpClient: unkown request type, only GET and POSt are supported");
                break;
        }
                
        // write data to HttpResponse
        response->setResponseCode(responseCode);
        
        if (retValue != 0) 
        {
            response->setSucceed(false);
            response->setErrorBuffer(s_errorBufferHF);
//            response->setErrorCode(s_errorCodeHF);
        }
        else
        {
            response->setSucceed(true);
        }
        
        // add response packet into queue
        pthread_mutex_lock(&s_responseQueueMutexHF);
        s_responseQueueHF->addObject(response);
        pthread_mutex_unlock(&s_responseQueueMutexHF);
        
        // resume dispatcher selector
        CCDirector::sharedDirector()->getScheduler()->resumeTarget(HFHttpClient::getInstance());
    }
    
    // cleanup: if worker thread received quit signal, clean up un-completed request queue
    pthread_mutex_lock(&s_requestQueueMutexHF);
    s_requestQueueHF->removeAllObjects();
    pthread_mutex_unlock(&s_requestQueueMutexHF);
    s_asyncRequestCountHF -= s_requestQueueHF->count();
    
    if (s_requestQueueHF != NULL) {
        
        pthread_mutex_destroy(&s_requestQueueMutexHF);
        pthread_mutex_destroy(&s_responseQueueMutexHF);
        
        pthread_mutex_destroy(&s_SleepMutexHF);
        pthread_cond_destroy(&s_SleepConditionHF);

        s_requestQueueHF->release();
        s_requestQueueHF = NULL;
        s_responseQueueHF->release();
        s_responseQueueHF = NULL;
    }

    pthread_exit(NULL);
    
    return 0;
}

//Configure curl's timeout property
static bool configureCURL(CURL *handle)
{
    if (!handle) {
        return false;
    }
    
    int32_t code;
    code = curl_easy_setopt(handle, CURLOPT_ERRORBUFFER, s_errorBufferHF);
    if (code != CURLE_OK) {
        return false;
    }
    code = curl_easy_setopt(handle, CURLOPT_TIMEOUT, HFHttpClient::getInstance()->getTimeoutForRead());
    if (code != CURLE_OK) {
        return false;
    }
    code = curl_easy_setopt(handle, CURLOPT_CONNECTTIMEOUT, HFHttpClient::getInstance()->getTimeoutForConnect());
    if (code != CURLE_OK) {
        return false;
    }
    curl_easy_setopt(handle, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(handle, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
    curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0L);

    return true;
}

class CURLRaiiHF
{
    /// Instance of CURL
    CURL *m_curl;
    /// Keeps custom header data
    curl_slist *m_headers;
public:
    CURLRaiiHF()
        : m_curl(s_pCurl)
        , m_headers(NULL)
    {
        if (HFHttpClient::getInstance()->getUsePersistConnection()) {
//            CCLOG("use persist connection");
            curl_easy_reset(m_curl);
        }else{
            m_curl = curl_easy_init();
        }
        
    }
    
    ~CURLRaiiHF()
    {
        if (m_curl && !HFHttpClient::getInstance()->getUsePersistConnection()) {
            curl_easy_cleanup(m_curl);
        }
        /* free the linked list for header data */
        if (m_headers)
            curl_slist_free_all(m_headers);
    }

    template <class T>
    bool setOption(CURLoption option, T data)
    {
        return CURLE_OK == curl_easy_setopt(m_curl, option, data);
    }

    /**
     * @brief Inits CURL instance for common usage
     * @param request Null not allowed
     * @param callback Response write callback
     * @param stream Response write stream
     */
    bool init(HFHttpRequest *request, write_callback callback, void *stream, write_callback headerCallback, void *headerStream)
    {
        if (!m_curl)
            return false;
        if (!configureCURL(m_curl)){
            CCLOGERROR("configure curl fail");
            return false;
        }
        /* get custom header data (if set) */
       	std::vector<std::string> headers=request->getHeaders();
        if(!headers.empty())
        {
            /* append custom headers one by one */
            for (std::vector<std::string>::iterator it = headers.begin(); it != headers.end(); ++it)
                m_headers = curl_slist_append(m_headers,it->c_str());
            /* set custom headers for curl */
            if (!setOption(CURLOPT_HTTPHEADER, m_headers))
                return false;
        }

        return setOption(CURLOPT_URL, request->getUrl())
                && setOption(CURLOPT_WRITEFUNCTION, callback)
                && setOption(CURLOPT_WRITEDATA, stream)
                && setOption(CURLOPT_HEADERFUNCTION, headerCallback)
                && setOption(CURLOPT_HEADERDATA, headerStream);
        
    }

    /// @param responseCode Null not allowed
    bool perform(long *responseCode)
    {
        CCLOGFUNC();
        s_errorCodeHF = curl_easy_perform(m_curl);
//        CCLOGFUNCF("HFHttpClient::curl perform result: %d",s_errorCodeHF);
        if (CURLE_OK != s_errorCodeHF){
            return false;
        }
        CURLcode code = curl_easy_getinfo(m_curl, CURLINFO_RESPONSE_CODE, responseCode);
#if defined(COCOS2D_DEBUG) && COCOS2D_DEBUG > 0
        double time;
        curl_easy_getinfo(m_curl, CURLINFO_TOTAL_TIME, &time);
        CCLOG("HFHttpClient::curl total time: %.3f",time);
        curl_easy_getinfo(m_curl, CURLINFO_NAMELOOKUP_TIME, &time);
        CCLOG("HFHttpClient::curl nslookup time: %.3f",time);
        curl_easy_getinfo(m_curl, CURLINFO_CONNECT_TIME, &time);
        CCLOG("HFHttpClient::curl connect time: %.3f",time);
        curl_easy_getinfo(m_curl, CURLINFO_PRETRANSFER_TIME, &time);
        CCLOG("HFHttpClient::curl pretransfer time: %.3f",time);
        curl_easy_getinfo(m_curl, CURLINFO_STARTTRANSFER_TIME, &time);
        CCLOG("HFHttpClient::curl start transfer time: %.3f",time);
#endif

        if (code != CURLE_OK || *responseCode != 200){
            return false;
        }
        return true;
    }
};

//Process Get Request
static int processGetTask(HFHttpRequest *request, write_callback callback, void *stream, long *responseCode, write_callback headerCallback, void *headerStream)
{
    CURLRaiiHF curl;
    bool ok = curl.init(request, callback, stream, headerCallback, headerStream)
            && curl.setOption(CURLOPT_CUSTOMREQUEST, NULL)
            && curl.setOption(CURLOPT_FOLLOWLOCATION, true)
            && curl.perform(responseCode);
    return ok ? 0 : 1;
}

//Process POST Request
static int processPostTask(HFHttpRequest *request, write_callback callback, void *stream, long *responseCode, write_callback headerCallback, void *headerStream)
{
    CURLRaiiHF curl;
    bool ok = curl.init(request, callback, stream, headerCallback, headerStream)
            && curl.setOption(CURLOPT_POST, 1)
            && curl.setOption(CURLOPT_ACCEPT_ENCODING, "")
            && curl.setOption(CURLOPT_POSTFIELDS, request->getRequestData())
            && curl.setOption(CURLOPT_POSTFIELDSIZE, request->getRequestDataSize())
            && curl.perform(responseCode);
    return ok ? 0 : 1;
}

//Process upload file Request
static int processUploadTask(HFHttpRequest *request, write_callback callback, void *stream, long *responseCode, write_callback headerCallback, void *headerStream)
{
    CURLRaiiHF curl;
        
    // construct the form data
    struct curl_httppost *formpost=NULL;
    struct curl_httppost *lastptr=NULL;
    string uploadFile = request->getUploadFile();
    /* Fill in the file upload field */
    curl_formadd(&formpost,
                 &lastptr,
                 CURLFORM_COPYNAME, "file",
                 CURLFORM_FILE, uploadFile.c_str(),
                 CURLFORM_END);
    
//    /* Fill in the filename field */
//    curl_formadd(&formpost,
//                 &lastptr,
//                 CURLFORM_COPYNAME, "filename",
//                 CURLFORM_COPYCONTENTS, "postit2.c",
//                 CURLFORM_END);
    curl_formadd(&formpost,
                 &lastptr,
                 CURLFORM_COPYNAME, "data",
                 CURLFORM_COPYCONTENTS, request->getRequestData(),
                 CURLFORM_CONTENTSLENGTH, request->getRequestDataSize(),
                 CURLFORM_END);
    
    /* Fill in the submit field too, even if this is rarely needed */
//    curl_formadd(&formpost,
//                 &lastptr,
//                 CURLFORM_COPYNAME, "submit",
//                 CURLFORM_COPYCONTENTS, "send",
//                 CURLFORM_END);
    
    
    bool ok = curl.init(request, callback, stream, headerCallback, headerStream)
    && curl.setOption(CURLOPT_HTTPPOST,formpost)
    && curl.perform(responseCode);
    curl_formfree(formpost);
    return ok ? 0 : 1;
}

//Process PUT Request
static int processPutTask(HFHttpRequest *request, write_callback callback, void *stream, long *responseCode, write_callback headerCallback, void *headerStream)
{
    CURLRaiiHF curl;
    bool ok = curl.init(request, callback, stream, headerCallback, headerStream)
            && curl.setOption(CURLOPT_CUSTOMREQUEST, "PUT")
            && curl.setOption(CURLOPT_POSTFIELDS, request->getRequestData())
            && curl.setOption(CURLOPT_POSTFIELDSIZE, request->getRequestDataSize())
            && curl.perform(responseCode);
    return ok ? 0 : 1;
}

//Process DELETE Request
static int processDeleteTask(HFHttpRequest *request, write_callback callback, void *stream, long *responseCode, write_callback headerCallback, void *headerStream)
{
    CURLRaiiHF curl;
    bool ok = curl.init(request, callback, stream, headerCallback, headerStream)
            && curl.setOption(CURLOPT_CUSTOMREQUEST, "DELETE")
            && curl.setOption(CURLOPT_FOLLOWLOCATION, true)
            && curl.perform(responseCode);
    return ok ? 0 : 1;
}

// HttpClient implementation
HFHttpClient* HFHttpClient::getInstance()
{
    if (s_pHttpClientHF == NULL) {
        s_pHttpClientHF = new HFHttpClient();
        s_pCurl = curl_easy_init();
    }
    
    return s_pHttpClientHF;
}

void HFHttpClient::destroyInstance()
{
    CCAssert(s_pHttpClientHF, "");
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(HFHttpClient::dispatchResponseCallbacks), s_pHttpClientHF);
    CC_SAFE_RELEASE_NULL(s_pHttpClientHF);
    curl_easy_cleanup(s_pCurl);
}

HFHttpClient::HFHttpClient()
: _timeoutForConnect(30)
, _timeoutForRead(60)
,m_bUsePersistConnection(true)
{
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(
                    schedule_selector(HFHttpClient::dispatchResponseCallbacks), this, 0, false);
    CCDirector::sharedDirector()->getScheduler()->pauseTarget(this);
}

HFHttpClient::~HFHttpClient()
{
    need_quitHF = true;
    
    if (s_requestQueueHF != NULL) {
    	pthread_cond_signal(&s_SleepConditionHF);
    }
    
    s_pHttpClientHF = NULL;
}

//Lazy create semaphore & mutex & thread
bool HFHttpClient::lazyInitThreadSemphore()
{
    if (s_requestQueueHF != NULL) {
        return true;
    } else {
        
        s_requestQueueHF = CCArray::create();
        CC_SAFE_RETAIN(s_requestQueueHF);
        
        s_responseQueueHF = CCArray::create();
        CC_SAFE_RETAIN(s_responseQueueHF);
        
        pthread_mutex_init(&s_requestQueueMutexHF, NULL);
        pthread_mutex_init(&s_responseQueueMutexHF, NULL);
        
        pthread_mutex_init(&s_SleepMutexHF, NULL);
        pthread_cond_init(&s_SleepConditionHF, NULL);

        pthread_create(&s_networkThreadHF, NULL, networkThread, NULL);
        pthread_detach(s_networkThreadHF);
        
        need_quitHF = false;
    }
    
    return true;
}

//Add a get task to queue
void HFHttpClient::send(HFHttpRequest* request)
{    
    if (false == lazyInitThreadSemphore()) 
    {
        return;
    }
    
    if (!request)
    {
        return;
    }
        
    ++s_asyncRequestCountHF;
    
    request->retain();
        
    pthread_mutex_lock(&s_requestQueueMutexHF);
    s_requestQueueHF->addObject(request);
    pthread_mutex_unlock(&s_requestQueueMutexHF);
    
    // Notify thread start to work
    pthread_cond_signal(&s_SleepConditionHF);
}

int HFHttpClient::getRequestQueueSize()
{
    return s_asyncRequestCountHF;
}
// Poll and notify main thread if responses exists in queue
void HFHttpClient::dispatchResponseCallbacks(float delta)
{
//    CCLog("HFHttpClient::dispatchResponseCallbacks is running");
    
    HFHttpResponse* response = NULL;
    
    pthread_mutex_lock(&s_responseQueueMutexHF);
    if (s_responseQueueHF->count())
    {
        response = dynamic_cast<HFHttpResponse*>(s_responseQueueHF->objectAtIndex(0));
        s_responseQueueHF->removeObjectAtIndex(0);
    }
    pthread_mutex_unlock(&s_responseQueueMutexHF);
    
    if (response)
    {
        --s_asyncRequestCountHF;
        
        HFHttpRequest *request = response->getRequest();
        CCObject *pTarget = request->getTarget();
        SEL_HFHttpResponse pSelector = request->getHFSelector();

        if (pTarget && pSelector) 
        {
            (pTarget->*pSelector)(this, response);
        }
//        CCLOGFUNCF("start release the response: %p",response);
        response->release();
//        CCLOGFUNC("end release the response");
    }
    
    
    if (0 == s_asyncRequestCountHF) 
    {
        CCDirector::sharedDirector()->getScheduler()->pauseTarget(this);
    }
    
}

