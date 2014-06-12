#ifndef _GAMELAYER_H_
#define _GAMELAYER_H_

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "touch_dispatcher\CCTouch.h"
using namespace cocos2d;

// ���������Ϸ����ͼ�㣬���е���Ϸ���ݶ�����һ��ͼ���н���
// �������Ҫ����һ�ŵ�ͼ������һ������
// �����ͼ��֮�ϸ�����ײ���
// ���۶���ͼ��֮�ϸ���ʵʱ����
// ����һЩ������λ�ñ仯��صı�������Щ��������ģ�������Ч��

class CCGameMap;
class CCHero;
class CCGameLayer : public CCLayer
{
public:

	
	CCLayer *mainLayer;  // ����Ϸͼ��
	CCGameMap *mainMap;  // ����Ϸ��ͼ
	CCHero *hero;        // ��Ϸ�е�����
	CCPoint birthPoint;  // ���ǵĳ�����
	CCSize heroSize;     // ���ǵĳߴ�
	CCSize mapSize;      // ��ͼ�ĳߴ�
	CCPoint heroAnchor;  // �����ߵ���Ļ�ĸõ���ͼ�Ϳ�ʼ����
	static float mapMaxH;       // ˮƽ��������Ϸ����ͼmainMap������X����������ֹ��ͼ���˳���Ļ
	CCPoint mapBeginPos; // ��ͼ��λ������Ϸͼ���е�λ��

	// ģ������Ч����ر���
	float ccMoveOffset;  // ÿ֡Ҫ�ƶ��ľ���
	float ccMoveDelta;   // Ϊ��ģ������ٶ�Ч�����ƶ�����
	float ccJumpOffset;  // 
	float moveDelta;     
	float moveOffset;  // ˮƽ�ƶ�ƫ����
	float JumpOffset;  // ��ֱ��Ծ��ƫ����

	CCPoint currentPos;  // ���ǵ�ǰ��λ��
	bool isKeyDownA;  // ��ʶA���Ƿ񱻰���
	bool isKeyDownD;  // ��ʶD���Ƿ񱻰���
	bool isKeyDownJump;  // ��ʶ��Ծ���Ƿ񱻰���
	bool isSky;        // ��ʶ�����Ƿ��ڿ���״̬

	//CCMultiTouchMenu     *pMenu;  // ��Ϸ�����ϵ�UI���Զ�����
	CCMenu *pMenu;
	CCMenuItemImage      *pLeftKey;
	CCMenuItemImage      *pRightKey;
	CCMenuItemImage      *pJump;
	CCMenuItemImage      *pFire;
	CCMenuItemImage      *pMSet;

	CCSprite *pGameOverBack;  // ������������ʾ�ĵı���
	CCSprite *pPassSuccess;
	CCSprite *pPassFailure;
	CCMenuItemImage *pRetry;  // ���Ե�ǰ��
	CCMenuItemImage *pNext;   // ��һ��
	CCMenuItemImage *pBackToMenu;  // ���ص����˵��İ�ť

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
	CCPoint MSetKeyPos;  // ���ü�

	void pauseGameLayer();  // ��ͣ��Ϸ
	void resumeGameLayer();

	void menuMSet(CCObject *pSender);
	void menuCallBackLeftKey(CCObject *pSender);
	void menuCallBackRightKey(CCObject *pSender);
	void menuCallBackJumpKey(CCObject *pSender);
	void menuCallBackFireKey(CCObject *pSender);
	void menuCallBackBackToMenu(CCObject *pSender);
	void menuRetry(CCObject *pSender);  // ���Ա���
	void menuNext(CCObject *pSender);   // ��һ��
	
	void showPassAll();  // ͨȫ��֮��Ķ���

	void initSetMenu();  // ��ʼ�����ý���
	CCLayerColor *pColor;
	CCSprite *pSetMenu;  // ���ý���ı���
	CCMenuItemImage *pResume;  //  ������Ϸ
	CCMenuItemImage *pReStart;  // ���¿�ʼ
	CCMenuItemImage *pSelectMenu;  // ѡ�ؽ���
	void menuResume(CCObject *pSender);
	void menuReStart(CCObject *pSender);
	void menuSelectMenu(CCObject *pSender);

	CCSprite *pBulletBorderFireBall;          // �����߿�
	CCSprite *pBulletBorderArrow;
	CCMenuItemSprite *pMenuFireBall;  // ����ѡ��ť
	CCMenuItemSprite *pMenuArrow;     // ����ѡ��ť

	CCPoint FireBallPos;
	CCPoint arrowPos;

	void menuCallBackFireBall(CCObject *pSender);
	void menuCallBackArrow(CCObject *pSender);

	void stopForPassFailure();
	void reShowPassFailure();
	
	void stopForPassSuccess();
	void reShowPassSuccess();
	void showHeroJump();  // չʾͨ�ض����е��������Ծ

	void reSetKeyNo();

	bool isPass;

	bool heroFireable;  // Ϊ�˿����ӵ������Ƶ�ȣ�����һ�����䰲ȫ��
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
	void collistionV();  // ˮƽ�������ײ���
	void collistionH();  // ��ֱ�������ײ���
	void setSceneScrollPosition();  // �������ǵĵ�ǰλ���ƶ����������ʵ�λ��

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