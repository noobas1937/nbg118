//
//  BranchController.h
//  IF
//
//  Created by liruiqi on 15/7/16.
//
//

#ifndef __IF__BranchController__
#define __IF__BranchController__

#include "CommonInclude.h"

USING_NS_CC;

class BranchController : public CCObject{
public:
    static BranchController* getInstance();
    void sendToBranch(std::string cmd, std::string value);
    void excute(std::string cmd);
    void addToBranchData(std::string cmd, std::string value);
    void addToBranchData(CCDictionary *dict);
    void redeemCredit(int creditNum);
    int getCredit();
private:
    std::string getBranchValue(std::string cmd);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    void doSendToBranchAndroid(std::string cmd, std::string value);
    void redeemCreditAndriod(int creditNum);
    int getCreditAndroid();
#else
    void doSendToBranchIOS(std::string cmd, std::string value);
    void redeemCreditIOS(int creditNum);
    int getCreditIOS();
#endif

    std::map<std::string, std::string> m_branchData;
};

#endif /* defined(__IF__BranchController__) */
