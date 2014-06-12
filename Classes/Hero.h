#ifndef _HERO_H_
#define _HERO_H_

#include "cocos2d.h"
#include "GameEnum.h"
using namespace cocos2d;

class CCHero : public CCNode
{
public:
	CCSprite *mainBody;  // ��Ҫ������¾���
	CCSprite *mainTemp;  // ����µ�������Ҫ���ڱ���С�Ķ����ڼ�
	CCSize norBodySize;  // ��������µ������С(������)
	CCSize smallSize;    // С�����
	CCSize currentSize;  // ��ǰ�����
	BodyType bodyType;   // ��ǰ������״̬

	void reSetForSuccess();  // ��С���������ͬʱ�������Ҫִ�еĻ�״̬Ȼ����Ծ�������ĵ�

	CCSpriteFrame *_normalRight;    // ����״̬�������������
	CCSpriteFrame *_normalLeft;     // ��������������������
	CCSpriteFrame *_jumpLeft;  // �����ڿ���ͣ��״̬�£����Ƕ�Ӧ������
	CCSpriteFrame *_jumpRight; // ����ͬ��

	CCSpriteFrame *_smallRight;  // ��Ӧ��С��״̬�µ�
	CCSpriteFrame *_smallLeft;
	CCSpriteFrame *_smallJumpLeft;
	CCSpriteFrame *_smallJumpRight;

	CCSpriteFrame *_normalRightFire;  // �����еĴ����ӵ�״̬����
	CCSpriteFrame *_normalLeftFire;   // �����д����ӵ�����
	CCSpriteFrame *_jumpRightFire;
	CCSpriteFrame *_jumpLeftFire;

	CCSpriteFrame *_normalRightArrow;  // ���乭����Ƥ��
	CCSpriteFrame *_normalLeftArrow;
	CCSpriteFrame *_jumpRightArrow;
	CCSpriteFrame *_jumpLeftArrow;

	CCSpriteFrame *_lifeOverSmall;  // С�����������״̬
	CCSpriteFrame *_lifeOverNormal;
	CCSpriteFrame *_lifeOverFire;

	bool isDied;   // �Ƿ��Ѿ�����
	void setHeroDie(bool _die);
	bool isHeroDied();

	//void dieForEnemy();  // ���������������ִ�еĺ��������ֻ�ǻ�����״�ľ���֡
	void dieForTrap();   // ����µ���������Ҫִ��һ�����ƺ����ƵĶ�������ʧ��
	void reSetNonVisible();

	marioDirection state;  // ��ʾ��ǰ���ǵ�״̬
	marioDirection statePre;  // ��ʶ���ǵ���һ��״̬
	marioDirection face;   // ����µĳ���
	bool isFlying;   // ������Ƿ��ڿ��з���״̬

	bool bulletable;  // �����Ƿ���Է����ӵ������������������е�һ������
	bool isBulletable();

	BulletType currentBulletType;  // ��ǰ��������ʹ�õ��ӵ�����
	void setBulletType(BulletType _type);
	BulletType getBulletType();

	// �Ƿ�վ�����ӵ�С�����
	bool gadgetable;
	void setGadgetable(bool _gadgetable);
	bool isGadgetable();

	void fireAction();
	void reSetStateForFired();

	// ������ʾ����һ���˵�UP����
	CCLabelTTF *pLabelUp;

	static CCHero* create();

	void setHeroState(marioDirection state);
	marioDirection getHeroState();	

	CCHero();
	~CCHero();

	void onEnter();
	void onExit();

	void changeForGotAddLifeMushroom();
	void changeForGotMushroom();
	void changeForGotEnemy();

	void setBodyType(BodyType _bodytype);
	CCSize getCurrentSize();
	BodyType getCurrentBodyType();

	void setSafeTime(bool _issafe);
	bool getIsSafeTime();

	static CCHero *_heroInstance;
	static CCHero* getHeroInstance();
private:
	bool heroInit();
	void clearLabelUp();
	void setHeroTypeForSmall();
	void setHeroTypeForNormal();

	bool isSafeTime;  // ��ʶ�����Ƿ��ڰ�ȫ�ڣ����˱�С��5�����3���ڲ��ܹ�����ײ���
	void reSetSafeTime();
};

#endif