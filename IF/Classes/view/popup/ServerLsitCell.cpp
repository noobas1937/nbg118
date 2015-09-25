//
//  ServerLsitCell.cpp
//  IF
//
//  Created by wangzhenlei on 13-10-18.
//
//

#include "ServerLsitCell.h"
#include "NetController.h"
#include "YesNoDialog.h"

ServerListCell::~ServerListCell()
{
    
}

ServerListCell::ServerListCell()
{

}

ServerListCell* ServerListCell::create(int idx,ServerInfo serverInfo)
{
    ServerListCell *pRet = new ServerListCell();
    if (pRet && pRet->init(idx, serverInfo))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}

bool ServerListCell::init(int idx,ServerInfo serverInfo){
    CCBLoadFile("servercell.ccbi", this, this);
    setInfo(idx, serverInfo);
    return true;
}

bool ServerListCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_ipText", CCLabelIF*, this->m_ipText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_levelText", CCLabelIF*, this->m_levelText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_redBtn", CCControlButton*, this->m_redBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_greenBtn", CCControlButton*, this->m_greenBtn);
    return true;
}

SEL_CCControlHandler ServerListCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBtnClick", ServerListCell::onBtnClick);
    return NULL;
}
void ServerListCell::onBtnClick(CCObject *pSender, CCControlEvent event){
    string ip = NetController::shared()->getIp();
    if(ip!=m_serverInfo.ip){
        NetController::shared()->setIp(m_serverInfo.ip);
        NetController::shared()->setPort(m_serverInfo.port);
        NetController::shared()->setZone(m_serverInfo.zone);
        CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_IP, m_serverInfo.ip);
        CCUserDefault::sharedUserDefault()->setStringForKey(ACCOUNT_ZONE, m_serverInfo.zone);
        CCUserDefault::sharedUserDefault()->setIntegerForKey(ACCOUNT_PORT, m_serverInfo.port);
        CCUserDefault::sharedUserDefault()->setStringForKey(GAME_UID, m_serverInfo.gameUid);
        CCUserDefault::sharedUserDefault()->flush();
        YesNoDialog::showVariableTitle(_lang("dialog_message_exit_confirm").c_str(),CCCallFuncO::create(GameController::getInstance(), callfuncO_selector(GameController::quitGame), NULL),"OK",true);

//        LogoutCommand* cmd=new LogoutCommand();
//        cmd->sendAndRelease();
    }
}


void ServerListCell::setInfo(int idx,ServerInfo serverInfo)
{
    m_index = idx;
    m_serverInfo = serverInfo;
    string ip = NetController::shared()->getIp();
    if(m_serverInfo.ip==ip){

    }else{
    
    }
    this->m_ipText->setString(m_serverInfo.name.c_str());
    if(m_serverInfo.level<0){
        this->m_levelText->setString("");
    }else{
        string lv = CC_ITOA(m_serverInfo.level);
        lv = _lang("102203")+lv;
        this->m_levelText->setString(lv.c_str());
    }
}