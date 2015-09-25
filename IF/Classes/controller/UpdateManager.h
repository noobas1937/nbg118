//
//  UpdateManager.h
//  IF
//
//  Created by 邹 程 on 13-11-13.
//
//

#ifndef __IF__UpdateManager__
#define __IF__UpdateManager__


#include <curl/curl.h>
#include <pthread.h>
#include "cocos2d.h"


class UpdateManagerDelegateProtocol;

/*
 *  This class is used to auto update resources, such as pictures or scripts.
 *  The updated package should be a zip file. And there should be a file named
 *  version in the server, which contains version code.
 */
class UpdateManager
{
public:
    enum UMErrorCode
    {
        // Error caused by creating a file to store downloaded data
        kCreateFile,
        /** Error caused by network
         -- network unavaivable
         -- timeout
         -- ...
         */
        kNetwork,
        /** There is not a new version
         */
        kNoNewVersion,
        /** Error caused in uncompressing stage
         -- can not open zip file
         -- can not read file global information
         -- can not read file information
         -- can not create a directory
         -- ...
         */
        kUncompress,
        kPthread
    };
    
    /* @brief Creates a UpdateManager with new package url, version code url and storage path.
     *
     * @param packageUrl URL of new package, the package should be a zip file.
     * @param versionFileUrl URL of version file. It should contain version code of new package.
     * @param storagePath The path to store downloaded resources.
     */
    UpdateManager();
    
    ~UpdateManager();
    
    /* @brief Check out if there is a new version resource.
     *        You may use this method before updating, then let user determine whether
     *        he wants to update resources.
     */
    bool checkUpdate();
    
    /* @brief Download new package if there is a new version, and uncompress downloaded zip file.
     *        Ofcourse it will set search path that stores downloaded files.
     */
    void update();
    
    /* @brief Gets current version code.
     */
    std::string getVersion();
    
    /* @brief Deletes recorded version code.
     */
    void deleteVersion();
    
    /** @brief Sets delegate, the delegate will receive messages
     */
    void setDelegate(UpdateManagerDelegateProtocol *delegate);
    
    /** @brief Sets connection time out in seconds
     */
    void setConnectionTimeout(unsigned int timeout);
    
    /** @brief Gets connection time out in secondes
     */
    unsigned int getConnectionTimeout();
    
    /* downloadAndUncompress is the entry of a new thread
     */
    friend void* UpdateManagerDownloadAndUncompress(void*);
    friend int UpdateManagerProgressFunc(void *, double, double, double, double);
    
protected:
    bool downLoad();
    bool uncompress();
    bool createDirectory(const char *path);
    void sendErrorMessage(UMErrorCode code);
    
private:
    typedef struct _Message
    {
    public:
        _Message() : what(0), obj(NULL){}
        unsigned int what; // message type
        void* obj;
    } UMMessage;
    
    class Helper : public cocos2d::CCObject
    {
    public:
        Helper();
        ~Helper();
        
        virtual void update(float dt);
        void sendMessage(UMMessage *msg);
        
    private:
        void handleUpdateSucceed(UMMessage *msg);
        
        std::list<UMMessage*> *_messageQueue;
        pthread_mutex_t _messageQueueMutex;
    };
    
private:
    //! The path to store downloaded resources.
    std::string _storagePath;
    
    //! The version of downloaded resources.
    std::string _version;
    
    std::string _packageUrl;
    
    std::string _downloadedVersion;
    
    CURL *_curl;
    Helper *_schedule;
    pthread_t *_tid;
    unsigned int _connectionTimeout;
    
    UpdateManagerDelegateProtocol *_delegate; // weak reference
};

class UpdateManagerDelegateProtocol
{
public:
    /* @brief Call back function for error
     @param errorCode Type of error
     */
    virtual void onError(UpdateManager::UMErrorCode errorCode) {};
    /** @brief Call back function for recording downloading percent
     @param percent How much percent downloaded
     @warn This call back function just for recording downloading percent.
     UpdateManager will do some other thing after downloading, you should
     write code in onSuccess() after downloading.
     */
    virtual void onProgress(int percent) {};
    /** @brief Call back function for success
     */
    virtual void onSuccess() {};
};

#endif /* defined(__IF__UpdateManager__) */
