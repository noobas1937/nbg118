//
//  SceneContainer.cpp
//  IF
//
//  Created by 邹 程 on 13-8-29.
//
//

#include "SceneContainer.h"
#include "GameController.h"
#include "YesNoDialog.h"
#include "PopupviewController.h"
#include "WorldMapView.h"
#include "ChatView.h"
#include "MailDialogView.h"
#include "DragonScene.h"

bool SceneContainer::init() {
    bool ret = false;
    if (CCScene::init()) {
        for (int i=0;i < LEVEL_MAX; i++) {
            Sprite3D *node = Sprite3D::create();
            node->setAnchorPoint(Vec2(0,0));
//            Layer *node = Layer::create();
            node->setContentSize(this->getContentSize());
            node->setTag(i);
            this->addChild(node,i*500);
            node->setPositionZ(i*500);
//            node->setGlobalZOrder(i);
            SceneController::getInstance()->m_sceneContainer = this;
            _keyboardListener = nullptr;
            _keyboardEnabled = false;
        }
        
        bool isdown=true;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        isdown=true;
#else
#if COCOS2D_DEBUG == 1
        isdown=false;
#else
        isdown=true;
#endif
#endif
        CCTexture2D::setDownloadFlag(isdown);
        ret = true;
    }
    return ret;
}

void SceneContainer::keyBackClicked()
{
    int currentScene = SceneController::getInstance()->currentSceneId;
    auto popup = PopupViewController::getInstance()->getCurrentPopupView();
    switch (currentScene) {
        case SCENE_ID_BATTLE:{
//            if (!popup) {
//                auto battleSence = dynamic_cast<BattleScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
//                battleSence->goBackMainScene();
//                return;
//            }
        }
            break;
        case SCENE_ID_LOADING:{
//            GameController::getInstance()->quitGame();
            return;
        }
            break;
        case SCENE_ID_MAIN:{
            if (!popup) {
                //根据平台显示退出窗口
                if(GlobalData::shared()->needLogout()){
                    GameController::getInstance()->platformQuit();
                }else{
                    YesNoDialog::showQuitDialog(CCCallFuncO::create(GameController::getInstance(), callfuncO_selector(GameController::quitGame), NULL),true);
                }
                return;
            }
        }
            break;
        case SCENE_ID_PVE:
        case SCENE_ID_WOOD:
        case SCENE_ID_BARRACKS:
        case SCENE_ID_MARKET:
        case SCENE_ID_IRON:
        case SCENE_ID_CROP:
        {
            if (!popup) {
                SceneController::getInstance()->gotoScene(SCENE_ID_MAIN);
                return;
            }
        }
            break;
        case SCENE_ID_WORLD: {
            if (!popup) {
                auto instance = WorldMapView::instance();
                if (instance) {
                    WorldMapView::instance()->clearPopupView();
                }
            }
        }
            break;
        case SCENE_ID_DRAGON: {
            if (!popup) {
                auto instance = DragonScene::instance();
                if (instance) {
                    DragonScene::instance()->leaveDragonScene();
                }
            }
        }
            break;
        default:
            break;
    }
    
    if (popup) {
        auto test = dynamic_cast<YesNoDialog*>(popup);
        if (test) {
            test->keypressedBtnCancel(NULL, CCControlEventTouchUpInside);
        } else {
            auto chatview = dynamic_cast<ChatView*>(popup);
            auto mailDialogview = dynamic_cast<MailDialogView*>(popup);
            if (chatview||mailDialogview) {
                CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
                if (scene->getPositionY() != 0) {
                    scene->setPositionY(0);
                    return;
                }
                if(PopupViewController::getInstance()->getCurrViewCount() >0){
                    auto popupLayer = SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_POPUP_IN);
                    popupLayer->setPositionY(0);
                }
            }
            if (PopupViewController::getInstance()->getCurrViewCount() >1) {
                PopupViewController::getInstance()->removePopupView(popup);
            }
            else
            {
                PopupViewController::getInstance()->goBackPopupView();
            }
        }
    }
}

void SceneContainer::onEnter()
{
    CCScene::onEnter();
    setKeyboardEnabled(true);
//    CCDirector::sharedDirector()->getKeypadDispatcher()->addDelegate(this);
}

void SceneContainer::onExit()
{
    setKeyboardEnabled(false);
//    CCDirector::sharedDirector()->getKeypadDispatcher()->removeDelegate(this);
    CCScene::onExit();
}

void SceneContainer::setKeyboardEnabled(bool enabled)
{
    if (enabled != _keyboardEnabled)
    {
        _keyboardEnabled = enabled;
        
        _eventDispatcher->removeEventListener(_keyboardListener);
        _keyboardListener = nullptr;
        
        if (enabled)
        {
            auto listener = EventListenerKeyboard::create();
            listener->onKeyPressed = CC_CALLBACK_2(SceneContainer::onKeyPressed, this);
            listener->onKeyReleased = CC_CALLBACK_2(SceneContainer::onKeyReleased, this);
            
            _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
            _keyboardListener = listener;
        }
    }
}

void SceneContainer::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* unused_event)
{
    
}

void SceneContainer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* unused_event)
{
    if(keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
    {
        keyBackClicked();
    }
}