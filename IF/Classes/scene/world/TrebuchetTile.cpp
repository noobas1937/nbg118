//
//  TrebuchetTile.cpp
//  IF
//
//  Created by 李锐奇 on 14/12/4.
//
//

#include "TrebuchetTile.h"

bool TrebuchetTile::init() {
    NewBaseTileInfo::init();
    
    if(ThroneTile::init()){
        return true;
    }
    return false;
}

std::string TrebuchetTile::getShowText(){
    if(m_cityInfo.trebuchetInfo.allianceUid != ""){
        return m_cityInfo.trebuchetInfo.allianceUid;
    }else if(m_cityInfo.trebuchetInfo.uid != ""){
        return m_cityInfo.trebuchetInfo.uid;
    }
    return "";
}

int TrebuchetTile::getOwnerFlag(){
    int hasOwner = 0;
    if(m_cityInfo.trebuchetInfo.uid != "" || m_cityInfo.trebuchetInfo.allianceUid != ""){
        hasOwner = 1;
    }
    return hasOwner;
}

void TrebuchetTile::setPeaceTimeButtons(){
    setButtonCount(1);
    setButtonName(1,_lang("110076")); //
    setButtonState(1, ButtonInformation);
    setButtonCallback(1,cccontrol_selector(TrebuchetTile::onInfoClick));
}

void TrebuchetTile::setNotOpenTimeButtons(){
    ThroneTile::setNotOpenTimeButtons();
}

void TrebuchetTile::setDefenderWarTimeButtons(){
    ThroneTile::setDefenderWarTimeButtons();
}

void TrebuchetTile::setAttackerWarTimeButtons(){
    ThroneTile::setAttackerWarTimeButtons();
}

bool TrebuchetTile::isSelfBuild(){
    return WorldController::getInstance()->isSelfTrebuchet(m_cityInfo);
}