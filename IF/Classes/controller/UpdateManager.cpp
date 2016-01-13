//
//  UpdateManager.cpp
//  IF
//
//  Created by 邹 程 on 13-11-13.
//
//

#include "UpdateManager.h"
#include "GlobalData.h"
#include "LocalController.h"

#include <curl/easy.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include "external/unzip/unzip.h"

using namespace cocos2d;
using namespace std;

#define KEY_OF_APP_VERSION           "current-app-version-code"
#define KEY_OF_VERSION               "current-version-code"
#define KEY_OF_DOWNLOADED_VERSION    "downloaded-version-code"
#define TEMP_PACKAGE_FILE_NAME       "tmp_update.zip"
#define BUFFER_SIZE    8192
#define MAX_FILENAME   512

// Message type
#define UpdateMANAGER_MESSAGE_UPDATE_SUCCEED                0
#define UpdateMANAGER_MESSAGE_RECORD_DOWNLOADED_VERSION     1
#define UpdateMANAGER_MESSAGE_PROGRESS                      2
#define UpdateMANAGER_MESSAGE_ERROR                         3

#define XML_USE_LOCAL 0  //1 本地 0 线上

const std::string _xmlFileName = "database.local.xml";
const std::string _iniFileName = "text_";

// Some data struct for sending messages

struct ErrorMessage
{
    UpdateManager::UMErrorCode code;
    UpdateManager* manager;
};

struct ProgressMessage
{
    int percent;
    UpdateManager* manager;
};

// Implementation of UpdateManager

UpdateManager::UpdateManager()
: _storagePath("")
, _version("")
, _packageUrl("")
, _downloadedVersion("")
, _curl(NULL)
, _tid(NULL)
, _connectionTimeout(0)
, _delegate(NULL)
{
    _schedule = new Helper();
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(_schedule, 0, false);
    _storagePath = CCFileUtils::sharedFileUtils()->getWritablePath();
    string _hostIP = CCUserDefault::sharedUserDefault()->getStringForKey(ACCOUNT_IP,"");
    if (_hostIP == "") {
        _hostIP = SERVER_IP;
    }
#if COCOS2D_DEBUG > 0
    _packageUrl = "http://"+_hostIP+":8080/gameservice/getfile?appVersion=" + GlobalData::shared()->version;
#else
    _packageUrl = "http://cok.eleximg.com/cok/config/"+GlobalData::shared()->version;
#endif

    CCLOG("XMLURL = %s",_packageUrl.c_str());
}

UpdateManager::~UpdateManager()
{
    if (_schedule)
    {
        CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(_schedule);
        _schedule->release();
    }
}

bool UpdateManager::checkUpdate()
{
    if (XML_USE_LOCAL) {
        sendErrorMessage(kNoNewVersion);
        CCLOG("use local xml file");
        return false;
    }
    string _lang = LocalController::shared()->getLanguageFileName();
    string tmpIniName = _iniFileName + _lang + ".ini";
    CCLOG("iniFileName %s",tmpIniName.c_str());
    // check app version first
    string lastAppVersion = CCUserDefault::sharedUserDefault()->getStringForKey(KEY_OF_APP_VERSION);
    string currentAppVersion = GlobalData::shared()->version;
    if (lastAppVersion.compare(currentAppVersion) != 0) {
        auto del = [](string store,string name){
            string path = store + name;
            if (CCFileUtils::sharedFileUtils()->isFileExist(path)) {
                remove(path.c_str());
            }
        };
        // app version not compatible
        del(_storagePath,_xmlFileName);
        del(_storagePath,tmpIniName);
        CCUserDefault::sharedUserDefault()->setStringForKey(KEY_OF_APP_VERSION, currentAppVersion);
    }
    
    // Clear _version before assign new value.
    _version.clear();
    
    // get version from global data
    _version = GlobalData::shared()->xmlVersion;
    if (_version.empty()) {
        _version = CCUserDefault::sharedUserDefault()->getStringForKey(KEY_OF_VERSION);
    }
    
#if COCOS2D_DEBUG > 0
    _packageUrl = _packageUrl + "&xmlVersion=" + GlobalData::shared()->xmlVersion + "&xmlLang=" + _lang;
#else
    _packageUrl = _packageUrl + "/config_" + GlobalData::shared()->xmlVersion + "_" + _lang+".zip";
#endif

    CCLOG("XML URL %s",_packageUrl.c_str());
    string recordedVersion = CCUserDefault::sharedUserDefault()->getStringForKey(KEY_OF_VERSION);
    if (recordedVersion == _version && CCFileUtils::sharedFileUtils()->isFileExist(_storagePath+tmpIniName))
    {
        sendErrorMessage(kNoNewVersion);
        CCLOG("there is no new version");
        return false;
    }
    
    // before update , delete
    
    CCLOG("there is a new version: %s", _version.c_str());
    
    return true;
}

void* UpdateManagerDownloadAndUncompress(void *data)
{
    UpdateManager* self = (UpdateManager*)data;
    
    do
    {
        if (self->_downloadedVersion != self->_version)
        {
            if (! self->downLoad()) break;
            
            // Record downloaded version.
            UpdateManager::UMMessage *msg1 = new UpdateManager::UMMessage();
            msg1->what = UpdateMANAGER_MESSAGE_RECORD_DOWNLOADED_VERSION;
            msg1->obj = self;
            self->_schedule->sendMessage(msg1);
        }
        
        // Uncompress zip file.
        if (! self->uncompress())
        {
            CCUserDefault::sharedUserDefault()->setStringForKey(KEY_OF_DOWNLOADED_VERSION, "");
            CCUserDefault::sharedUserDefault()->flush();
            self->sendErrorMessage(UpdateManager::kUncompress);
            break;
        }
        
        // Record updated version and remove downloaded zip file
        UpdateManager::UMMessage *msg2 = new UpdateManager::UMMessage();
        msg2->what = UpdateMANAGER_MESSAGE_UPDATE_SUCCEED;
        msg2->obj = self;
        self->_schedule->sendMessage(msg2);
    } while (0);
    
    if (self->_tid)
    {
        delete self->_tid;
        self->_tid = NULL;
    }
    
    return NULL;
}

void UpdateManager::update()
{
    if (_tid) {
        sendErrorMessage(kPthread);
        CCLOG("pthread error!");
        return;
    }
    
    // Check if there is a new version.
    if (! checkUpdate()) return;
    
    // Is package already downloaded?
    _downloadedVersion = CCUserDefault::sharedUserDefault()->getStringForKey(KEY_OF_DOWNLOADED_VERSION);
    
    _tid = new pthread_t();
    pthread_create(&(*_tid), NULL, UpdateManagerDownloadAndUncompress, this);
}

bool UpdateManager::uncompress()
{
    // Open the zip file
    string outFileName = _storagePath + TEMP_PACKAGE_FILE_NAME;
    unzFile zipfile = unzOpen(outFileName.c_str());
    if (! zipfile)
    {
        CCLOG("can not open downloaded zip file %s", outFileName.c_str());
        sendErrorMessage(kUncompress);
        return false;
    }
    
    // Get info about the zip file
    unz_global_info global_info;
    if (unzGetGlobalInfo(zipfile, &global_info) != UNZ_OK)
    {
        CCLOG("can not read file global info of %s", outFileName.c_str());
        unzClose(zipfile);
        sendErrorMessage(kUncompress);
        return false;
    }
    
    // Buffer to hold data read from the zip file
    char readBuffer[BUFFER_SIZE];
    
    CCLOG("start uncompressing");
    
    // Loop to extract all files.
    uLong i;
    for (i = 0; i < global_info.number_entry; ++i)
    {
        // Get info about current file.
        unz_file_info fileInfo;
        char fileName[MAX_FILENAME];
        if (unzGetCurrentFileInfo(zipfile,
                                  &fileInfo,
                                  fileName,
                                  MAX_FILENAME,
                                  NULL,
                                  0,
                                  NULL,
                                  0) != UNZ_OK)
        {
            CCLOG("can not read file info");
            unzClose(zipfile);
            sendErrorMessage(kUncompress);
            return false;
        }
        
        string fullPath = _storagePath + fileName;
        
        // Check if this entry is a directory or a file.
        const size_t filenameLength = strlen(fileName);
        if (fileName[filenameLength-1] == '/')
        {
            // Entry is a direcotry, so create it.
            // If the directory exists, it will failed scilently.
            if (!createDirectory(fullPath.c_str()))
            {
                CCLOG("can not create directory %s", fullPath.c_str());
                unzClose(zipfile);
                sendErrorMessage(kUncompress);
                return false;
            }
        }
        else
        {
            // Entry is a file, so extract it.
            
            // Open current file.
            if (unzOpenCurrentFile(zipfile) != UNZ_OK)
            {
                CCLOG("can not open file %s", fileName);
                unzClose(zipfile);
                sendErrorMessage(kUncompress);
                return false;
            }
            
            // Create a file to store current file.
            FILE *out = fopen(fullPath.c_str(), "w+");
            if (! out)
            {
                CCLOG("can not open destination file %s", fullPath.c_str());
                unzCloseCurrentFile(zipfile);
                unzClose(zipfile);
                sendErrorMessage(kUncompress);
                return false;
            }
            
            // Write current file content to destinate file.
            int error = UNZ_OK;
            do
            {
                error = unzReadCurrentFile(zipfile, readBuffer, BUFFER_SIZE);
                if (error < 0)
                {
                    CCLOG("can not read zip file %s, error code is %d", fileName, error);
                    unzCloseCurrentFile(zipfile);
                    unzClose(zipfile);
                    sendErrorMessage(kUncompress);
                    return false;
                }
                
                if (error > 0)
                {
                    fwrite(readBuffer, error, 1, out);
                }
            } while(error > 0);
            
            fclose(out);
        }
        
        unzCloseCurrentFile(zipfile);
        
        // Goto next entry listed in the zip file.
        if ((i+1) < global_info.number_entry)
        {
            if (unzGoToNextFile(zipfile) != UNZ_OK)
            {
                CCLOG("can not read next file");
                unzClose(zipfile);
                sendErrorMessage(kUncompress);
                return false;
            }
        }
    }
    
    CCLOG("end uncompressing");
    
    return true;
}

/*
 * Create a direcotry is platform depended.
 */
bool UpdateManager::createDirectory(const char *path)
{
    mode_t processMask = umask(0);
    int ret = mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO);
    umask(processMask);
    if (ret != 0 && (errno != EEXIST))
    {
        return false;
    }
    
    return true;
}

static size_t downLoadPackage(void *ptr, size_t size, size_t nmemb, void *userdata)
{
    FILE *fp = (FILE*)userdata;
    size_t written = fwrite(ptr, size, nmemb, fp);
    return written;
}

int UpdateManagerProgressFunc(void *ptr, double totalToDownload, double nowDownloaded, double totalToUpLoad, double nowUpLoaded)
{
    UpdateManager* manager = (UpdateManager*)ptr;
    UpdateManager::UMMessage *msg = new UpdateManager::UMMessage();
    msg->what = UpdateMANAGER_MESSAGE_PROGRESS;
    
    ProgressMessage *progressData = new ProgressMessage();
    auto percent = (int)(nowDownloaded/totalToDownload*100);
    percent = percent < 0 ? 0 : percent;
    progressData->percent = percent;
    progressData->manager = manager;
    msg->obj = progressData;
    
    manager->_schedule->sendMessage(msg);
    
//    CCLOG("downloading... %d%%", percent);
    
    return 0;
}

bool UpdateManager::downLoad()
{
    _curl = curl_easy_init();
    if (! _curl)
    {
        CCLOG("can not init curl");
        return false;
    }

    // Create a file to save package.
    string outFileName = _storagePath + TEMP_PACKAGE_FILE_NAME;
    FILE *fp = fopen(outFileName.c_str(), "w+");
    if (! fp)
    {
        sendErrorMessage(kCreateFile);
        CCLOG("can not create file %s", outFileName.c_str());
        return false;
    }
    
    // Download pacakge
    CURLcode res;
    curl_easy_setopt(_curl, CURLOPT_URL, _packageUrl.c_str());
    curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, downLoadPackage);
    curl_easy_setopt(_curl, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(_curl, CURLOPT_NOPROGRESS, false);
    curl_easy_setopt(_curl, CURLOPT_PROGRESSFUNCTION, UpdateManagerProgressFunc);
    curl_easy_setopt(_curl, CURLOPT_PROGRESSDATA, this);
    res = curl_easy_perform(_curl);
    curl_easy_cleanup(_curl);
    if (res != 0)
    {
        sendErrorMessage(kNetwork);
        CCLOG("error when download package");
        fclose(fp);
        return false;
    }
    
    CCLOG("succeed downloading package %s", _packageUrl.c_str());
    
    fclose(fp);
    return true;
}

string UpdateManager::getVersion()
{
    return CCUserDefault::sharedUserDefault()->getStringForKey(KEY_OF_VERSION);
}

void UpdateManager::deleteVersion()
{
    CCUserDefault::sharedUserDefault()->setStringForKey(KEY_OF_VERSION, "");
}

void UpdateManager::setDelegate(UpdateManagerDelegateProtocol *delegate)
{
    _delegate = delegate;
}

void UpdateManager::setConnectionTimeout(unsigned int timeout)
{
    _connectionTimeout = timeout;
}

unsigned int UpdateManager::getConnectionTimeout()
{
    return _connectionTimeout;
}

void UpdateManager::sendErrorMessage(UpdateManager::UMErrorCode code)
{
    UMMessage *msg = new UMMessage();
    msg->what = UpdateMANAGER_MESSAGE_ERROR;
    
    ErrorMessage *errorMessage = new ErrorMessage();
    errorMessage->code = code;
    errorMessage->manager = this;
    msg->obj = errorMessage;
    
    _schedule->sendMessage(msg);
}

// Implementation of UpdateManagerHelper

UpdateManager::Helper::Helper()
{
    _messageQueue = new list<UMMessage*>();
    pthread_mutex_init(&_messageQueueMutex, NULL);
}

UpdateManager::Helper::~Helper()
{
    pthread_mutex_destroy(&_messageQueueMutex);
    delete _messageQueue;
}

void UpdateManager::Helper::sendMessage(UMMessage *msg)
{
    pthread_mutex_lock(&_messageQueueMutex);
    _messageQueue->push_back(msg);
    pthread_mutex_unlock(&_messageQueueMutex);
}

void UpdateManager::Helper::update(float dt)
{
    UMMessage *msg = NULL;
    
    if (!_messageQueue) {
        return;
    }
    
    // Returns quickly if no message
    pthread_mutex_lock(&_messageQueueMutex);
    if (0 == _messageQueue->size())
    {
        pthread_mutex_unlock(&_messageQueueMutex);
        return;
    }
    
    // Gets message
    msg = *(_messageQueue->begin());
    _messageQueue->pop_front();
    pthread_mutex_unlock(&_messageQueueMutex);
    
    switch (msg->what) {
        case UpdateMANAGER_MESSAGE_UPDATE_SUCCEED:
            handleUpdateSucceed(msg);
            
            break;
        case UpdateMANAGER_MESSAGE_RECORD_DOWNLOADED_VERSION:
            CCUserDefault::sharedUserDefault()->setStringForKey(KEY_OF_DOWNLOADED_VERSION,
                                                                ((UpdateManager*)msg->obj)->_version.c_str());
            CCUserDefault::sharedUserDefault()->flush();
            
            break;
        case UpdateMANAGER_MESSAGE_PROGRESS:
            if (((ProgressMessage*)msg->obj)->manager->_delegate)
            {
                ((ProgressMessage*)msg->obj)->manager->_delegate->onProgress(((ProgressMessage*)msg->obj)->percent);
            }
            
            delete (ProgressMessage*)msg->obj;
            
            break;
        case UpdateMANAGER_MESSAGE_ERROR:
            // error call back
            if (((ErrorMessage*)msg->obj)->manager->_delegate)
            {
                ((ErrorMessage*)msg->obj)->manager->_delegate->onError(((ErrorMessage*)msg->obj)->code);
            }
            
            delete ((ErrorMessage*)msg->obj);
            
            break;
        default:
            break;
    }
    
    delete msg;
}

void UpdateManager::Helper::handleUpdateSucceed(UMMessage *msg)
{
    UpdateManager* manager = (UpdateManager*)msg->obj;
    
    // Record new version code.
    CCUserDefault::sharedUserDefault()->setStringForKey(KEY_OF_VERSION, manager->_version.c_str());
    
    // Unrecord downloaded version code.
    CCUserDefault::sharedUserDefault()->setStringForKey(KEY_OF_DOWNLOADED_VERSION, "");
    CCUserDefault::sharedUserDefault()->flush();
        
    // Delete unloaded zip file.
    string zipfileName = manager->_storagePath + TEMP_PACKAGE_FILE_NAME;
    if (remove(zipfileName.c_str()) != 0)
    {
        CCLOG("can not remove downloaded zip file %s", zipfileName.c_str());
    }
    
    if (manager && manager->_delegate) manager->_delegate->onSuccess();
}
