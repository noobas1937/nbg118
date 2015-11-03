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
        //begin a by ljf
        
        //场景层，在3d物体下面，通过自定义相机实现，设置depth值控制显示层级关系
        auto s = Director::getInstance()->getWinSize();
        /*
        auto under3DCamera = Camera::createOrthographic(s.width, s.height, -1024000, 1024000);
        under3DCamera->setPosition3D(Vec3(0.0f, 0.0f, 0.0f));
        under3DCamera->setRotation3D(Vec3(0.f, 0.f, 0.f));
        */
        auto under3DCamera = Camera::create();
        under3DCamera->setCameraFlag(CameraFlag::USER4);
        
        under3DCamera->setDepth(-10);
        //under3DCamera->setVisible(false);
        this->addChild(under3DCamera);
        
        //3d物体层，通过自定义相机显示，设置depth值控制显示层级关系
        
        //auto just3DCamera = Camera::createPerspective(30, (GLfloat)s.width/s.height, 10, 2000);
        //auto just3DCamera = Camera::create();
        
        float zeye = Director::getInstance()->getZEye();
        
        //auto just3DCamera = Camera::createPerspective(60, (GLfloat)s.width / s.height, 10, zeye + s.height / 2.0f);
        
        //Vec3 eye(s.width/2.0f , s.height/2.0f , zeye), center(s.width/2 , s.height/2 , 0.0f), up(0.0f, 1.0f, 0.0f);
        //Vec3 eye(zeye * cos(138 / 180.0f * 3.14159265) , 400.0f , zeye * sin(138 / 180.0f * 3.14159265));
        //Vec3 eye(0.0f, 0.0f , 657);
        Vec3 eye(0.0f, 0.0f, zeye);
        Vec3 center(0.0f , 0.0f , 0.0f);
        Vec3 up(0.0f, 1.0f, 0.0f);
        /*
        auto just3DCamera = Camera::createOrthographic(s.width, s.height, -1024000, 1024000);
        //just3DCamera->setPosition3D(eye);
        //just3DCamera->lookAt(center, up);
        just3DCamera->setPosition3D(Vec3(0.0f, 0.0f, 0.0f));
        just3DCamera->setRotation3D(Vec3(0.f, 0.f, 0.f));
        */
        //just3DCamera->setRotation3D(Vec3(32, 39, -24));
        
        /*
        auto just3DCamera = Camera::createOrthographic(s.width, s.height, -1024000, 1024000);
        just3DCamera->setPosition3D(Vec3(0.0f, 0.0f, 0.0f));
        just3DCamera->setRotation3D(Vec3(0.f, 0.f, 0.f));
        */
        auto just3DCamera = Camera::create();
        just3DCamera->setCameraFlag(CameraFlag::USER2);
        just3DCamera->setDepth(-9);
        this->addChild(just3DCamera);
        //just3DCamera->setPosition3D(Vec3(0,100,100));
        //just3DCamera->lookAt(Vec3(0,0,0));
        
        //UI层，通过默认相机显示
        //end a by ljf
        
        for (int i=0;i < LEVEL_MAX; i++) {
            auto node = Node::create();
            node->setAnchorPoint(Vec2(0,0));
//            Layer *node = Layer::create();
            node->setContentSize(this->getContentSize());
            node->setTag(i);
            this->addChild(node,i*500);
            //node->setPositionZ(i*500);
            //node->setGlobalZOrder(i);
            SceneController::getInstance()->m_sceneContainer = this;
            _keyboardListener = nullptr;
            _keyboardEnabled = false;
            
        }
        
        Texture2D::setSmallFlag(USE_SMALL_IMAGE);
        CCTexture2D::setDownloadFlag(IS_DOWNLOAD_FLAG);
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