#ifndef _GAMELAYER_H_
#define _GAMELAYER_H_

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "touch_dispatcher\CCTouch.h"
using namespace cocos2d;

// 这个类是游戏内容图层，所有的游戏内容都在这一个图层中进行
// 这个类中要加载一张地图，加载一个主角
// 在这个图层之上负责碰撞检测
// 再折耳根图层之上负责实时更新
// 持有一些和主角位置变化相关的变量，这些变量用于模拟出物理效果

class CCGameMap;
class CCHero;
class CCGameLayer : public CCLayer
{
public:

	
	CCLayer *mainLayer;  // 主游戏图层
	CCGameMap *mainMap;  // 主游戏地图
	CCHero *hero;        // 游戏中的主角
	CCPoint birthPoint;  // 主角的出生点
	CCSize heroSize;     // 主角的尺寸
	CCSize mapSize;      // 地图的尺寸
	CCPoint heroAnchor;  // 主角走到屏幕的该点后地图就开始跟随
	static float mapMaxH;       // 水平方向上游戏主地图mainMap缩进的X最大分量，防止地图后退出屏幕
	CCPoint mapBeginPos; // 地图层位于主游戏图层中的位置

	// 模拟物理效果相关变量
	float ccMoveOffset;  // 每帧要移动的距离
	float ccMoveDelta;   // 为了模拟出加速度效果，移动增量
	float ccJumpOffset;  // 
	float moveDelta;     
	float moveOffset;  // 水平移动偏移量
	float JumpOffset;  // 垂直跳跃的偏移量

	CCPoint currentPos;  // 主角当前的位置
	bool isKeyDownA;  // 标识A键是否被按下
	bool isKeyDownD;  // 标识D键是否被按下
	bool isKeyDownJump;  // 标识跳跃键是否被按下
	bool isSky;        // 标识主角是否在空中状态

	//CCMultiTouchMenu     *pMenu;  // 游戏画面上的UI，自定义类
	CCMenu *pMenu;
	CCMenuItemImage      *pLeftKey;
	CCMenuItemImage      *pRightKey;
	CCMenuItemImage      *pJump;
	CCMenuItemImage      *pFire;
	CCMenuItemImage      *pMSet;

	CCSprite *pGameOverBack;  // 主角死掉后显示的的背景
	CCSprite *pPassSuccess;
	CCSprite *pPassFailure;
	CCMenuItemImage *pRetry;  // 重试当前关
	CCMenuItemImage *pNext;   // 下一关
	CCMenuItemImage *pBackToMenu;  // 返回到主菜单的按钮

	CCSprite *controlUI;

	CCSprite *pBackKeyImage;
	CCSpriteFrame *pBackKeyNormal;
	CCSpriteFrame *pBackKeyLeft;
	CCSpriteFrame *pBackKeyRight;
	CCSprite *pJumpImage;
	CCSprite *pFireImage;
	CCSpriteFrame *pAB_Normal;
	CCSpriteFrame *pAB_Selected;

	CCPoint backKeyPos;
	CCPoint leftKeyPos;
	CCPoint rightKeyPos;
	CCPoint jumpKeyPos;
	CCPoint fireKeyPos;
	CCPoint MSetKeyPos;  // 设置键

	void pauseGameLayer();  // 暂停游戏
	void resumeGameLayer();

	void menuMSet(CCObject *pSender);
	void menuCallBackLeftKey(CCObject *pSender);
	void menuCallBackRightKey(CCObject *pSender);
	void menuCallBackJumpKey(CCObject *pSender);
	void menuCallBackFireKey(CCObject *pSender);
	void menuCallBackBackToMenu(CCObject *pSender);
	void menuRetry(CCObject *pSender);  // 重试本关
	void menuNext(CCObject *pSender);   // 下一关
	
	void showPassAll();  // 通全关之后的动画

	void initSetMenu();  // 初始化设置界面
	CCLayerColor *pColor;
	CCSprite *pSetMenu;  // 设置界面的背景
	CCMenuItemImage *pResume;  //  继续游戏
	CCMenuItemImage *pReStart;  // 重新开始
	CCMenuItemImage *pSelectMenu;  // 选关界面
	void menuResume(CCObject *pSender);
	void menuReStart(CCObject *pSender);
	void menuSelectMenu(CCObject *pSender);

	CCSprite *pBulletBorderFireBall;          // 武器边框
	CCSprite *pBulletBorderArrow;
	CCMenuItemSprite *pMenuFireBall;  // 火球选择按钮
	CCMenuItemSprite *pMenuArrow;     // 弓箭选择按钮

	CCPoint FireBallPos;
	CCPoint arrowPos;

	void menuCallBackFireBall(CCObject *pSender);
	void menuCallBackArrow(CCObject *pSender);

	void stopForPassFailure();
	void reShowPassFailure();
	
	void stopForPassSuccess();
	void reShowPassSuccess();
	void showHeroJump();  // 展示通关动画中的马里奥跳跃

	void reSetKeyNo();

	bool isPass;

	bool heroFireable;  // 为了控制子弹发射的频度，设置一个发射安全期
	void reSetHeroFireable();

	CCGameLayer();
	~CCGameLayer();

	bool init();
	void initHeroAndMap();
	void initcontrolUI();
	void initBulletUI();


	void loadMap();

	void updateControl();
	bool isLeftKeyDown;
	bool isRightKeyDown;
	bool isJumpKeyDown;
	bool isFireKeyDown;

	CCRect leftKeyRect;
	CCRect rightKeyRect;
	CCRect jumpKeyRect;
	CCRect fireKeyRect;
	void initRect();

	void update(float dt);
	void updateContent(float dt);
	void updateDelta(float dt);
	void collistionV();  // 水平方向的碰撞检测
	void collistionH();  // 垂直方向的碰撞检测
	void setSceneScrollPosition();  // 根据主角的当前位置移动场景至合适的位置

	static float getMapMaxH();


	void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
	void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);

	void registerWithTouchDispatcher(void);

	//void processWin32KeyPress(UINT message, WPARAM wParam, LPARAM lParam);

	void menuCloseCallBack(CCObject *pSender);

	void toMainMenu();
	
	CREATE_FUNC(CCGameLayer);
};

#endif