//
//  SceneController.h
//  IF
//
//  Created by 邹 程 on 13-8-29.
//
//

#ifndef __IF__SceneController__
#define __IF__SceneController__

#include "CommonInclude.h"
#include "WorldController.h"

#define LEVEL_SCENE      0
#define LEVEL_MINIMAP      1
#define LEVEL_POPUP_IN   2
#define LEVEL_GUI        3
#define LEVEL_POPUP      4
#define LEVEL_TIP        5
#define LEVEL_CONTROL    6
#define LEVEL_WORLD_UI   7
#define LEVEL_GUIDE      8
#define LEVEL_POP_TOUCH      9
#define LEVEL_SCENE_TRANSITION 10
#define LEVEL_MAX        11
#define SCENE_ID_LOADING       0
#define SCENE_ID_MAIN          1
#define SCENE_ID_BATTLE        2
#define SCENE_ID_CROP          3
#define SCENE_ID_WOOD          4
#define SCENE_ID_PVE           5
#define SCENE_ID_GUI           6
#define SCENE_ID_IRON          7
#define SCENE_ID_BARRACKS      8
#define SCENE_ID_MARKET        9
#define SCENE_ID_IMPERIAL      10
#define SCENE_ID_WORLD         11
#define SCENE_ID_BATTLE2         12
#define SCENE_ID_DRAGON          13

#define MSG_SCENE_CHANGE_FINISH "msgSceneChangeFinish"

class SceneContainer;
class TransitionCallback;
class SceneTransition;
class PopupViewController;
class SceneController :
public CCObject
{
public:
    int currentSceneId;
    static SceneController* getInstance();

    void gotoScene(int sceneId,bool shouldSave = false,bool shouldCleanUp = true,int subSceneId = -1,MapType mapType = DEFAULT_MAP);
    void addGUI();
    void removeGUI();
    void gotoLoading(bool clearFlag = false);
    
    CCLayer* getCurrentLayerByLevel(int level);
    
    int getSceneIdByTag(int tag);
    bool shouldSaveSceneByTag(int tag);
    bool shouldCleanUpSceneByTag(int tag);
    void setLayerTag(CCLayer* layer,int sceneId,bool shouldSave,bool shouldCleanUp);
    
    int curSceneIsCropScene();
    void updateSubSceneBuild(int itemId);
    void updateSubSceneBuildUI(int itemId, double saveTime=0);
    void updateSubSceneBuildPic(int buildId, int type);
    void upAutoBtnState();
    void upBuildingFlyBall(int buildId, int qid, bool toScene=false);
    void updateBuildings();
    void clearLayerByLevelAndSceneId(int level,int sceneId);
    void loadSceneResource(int sceneId,bool shouldSave,bool shouldCleanUp,bool fisrtInit = true,int subSceneId = -1,MapType mapType = DEFAULT_MAP);
    void loadSceneResourceBack(CCObject *obj);
    void onFlyOutPut(int itemId, int output, int forceResType=-1);
    friend class SceneContainer;
    friend class TransitionCallback;
    friend class SceneTransition;
    friend class PopupViewController;
    CCNode *showBG;
    ~SceneController();
private:
    void init();
    SceneController() = default;
    SceneController& operator=(const SceneController&) = default;
    SceneController(const SceneController&) = default;

    CCLayer* getLayerByLevelAndSceneId(int level,int sceneId);
    void setLayerByLevelAndSceneId(CCLayer *layer,int level,int sceneId,bool shouldSave,bool shouldCleanUp,bool specialTransition = false);

    
    // test if goto self
    bool isGotoSelf(int sceneId, MapType type);
    
    // this method can not use in other place
    CCNode* getLevelNode(int level);
    
    void doLastSceneClear(int nextSceneId);
    void doSceneInit(int sceneId,bool shouldSave,bool shouldCleanUp,bool fisrtInit = true,int subSceneId = -1,MapType mapType = DEFAULT_MAP);
    
    SceneContainer *m_sceneContainer;
    map<int, map<int, CCLayer*> > m_levelMap;
    
    int m_sceneId;
    bool m_shouldSave;
    bool m_shouldCleanUp;
    bool m_fisrtInit;
    int m_subSceneId;
    MapType m_mapType;
        //主城换皮
    CC_SYNTHESIZE(bool, m_maincityCustom, MaincityCustom);

};
#endif /* defined(__IF__SceneController__) */
