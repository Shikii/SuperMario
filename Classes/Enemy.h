#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "cocos2d.h"
#include "GameEnum.h"
using namespace cocos2d;

class CCEnemy : public CCNode
{
public:
	CCEnemy();
	virtual ~CCEnemy();

	void setTileCoord(CCPoint _tileCoord);
	CCPoint getTileCoord();
	void setEnemyPos(CCPoint _enemyPos);
	CCPoint getEnemyPos();
	void setBodySize(CCSize _size);
	CCSize getBodySize();

	void setMoveOffset(float _moveoffset);
	float getMoveOffset();

	void setccMoveOffset(float _ccmoveoffset);
	float getccMoveOffset();

	EnemyType getEnemyType();

	virtual CCRect getEnemyRect();

	// ר������������չʾ���ﶯ���Լ��ƶ�����ײ���ĺ���
	// ���麯������ΪCCEnemy�������ʾ��һ������������ϸ��Ҫ���ݾ����������
	virtual void launchEnemy() = 0;
	virtual void enemyCollistionH();
	virtual void enemyCollistionV();

	// �͹���״̬������ص�
	void setEnemyState(EnemyState _state);
	EnemyState getEnemyState();
	void checkState();
	virtual void stopEnemyUpdate();

	// �жϵ�ǰ���͵Ĺ����Ƿ�����������
	virtual EnemyVSHero checkCollisionWithHero();
	// �������Ƿ����ǲ�����Ģ���ڹ��ǿ��Ա���ô�����ģ�����ʳ�˻��Ͳ���
	//virtual bool checkKilled();
	// ���ﱻ���ǲ���ʱ���õĺ���
	virtual void forKilledByHero();
	void setNonVisibleForKilledByHero();

	// ���ӵ��������õĺ���
	virtual void forKilledByBullet();
	void setNonVisibleForKilledByBullet();

protected:
	EnemyType enemyType;  // ������ʾ���������
	CCSprite *enemyBody;  // չʾ����ľ���
	CCSize bodySize;      // ����ĳߴ�
	CCSpriteFrame *enemyLifeOver;  // ������ʾ����ҵ��ľ���֡
	CCSpriteFrame *overByArrow;    // ���������ľ���֡

	CCPoint tileCoord;   // ������ʾ���������ĵ�ͼ����ϵ�µ�
	CCPoint enemyPos;    // GL����ϵ�µģ�����ֱ�����ù����λ��

	marioDirection startFace;  // �տ�������״̬ʱ������ĳ���

	// ˮƽ�����ƶ����Ʊ���
	float moveOffset;
	float ccMoveOffset;
	// ��ֱ�������ƶ����Ʊ���
	float jumpOffset;
	float ccJumpOffset;

	// ��ʶ����ĵ�ǰ״̬
	// �涨����Ļ�еĹ������ڻ�Ծ״̬����Ļ֮ǰ��֮��ľ�Ϊ�ǻ�Ծ״̬���ٻ���������
	EnemyState enemyState;

};

// ����Ģ������ǰһֱ��Ϊ�ǰײˣ�
class CCEnemyMushroom : public CCEnemy
{
public:
	CCEnemyMushroom();
	~CCEnemyMushroom();

	// ʵ�ָ���Ĵ��麯��
	void launchEnemy();

	//void enemyCollistionH();
	//void enemyCollistionV();

	void update(float dt);
};

// ���˻����ӹܵ���ð�����ģ���Ҫ����ͼ��Ĺܵ�����ڵ���ϵʵ�ֳ��˻�������Ч��
class CCEnemyFlower : public CCEnemy
{
public:
	CCEnemyFlower();
	~CCEnemyFlower();

	// ʵ�ָ���Ĵ��麯��
	void launchEnemy();

	void update(float dt);
	// ��Ϊʳ�˻��������㷨��֮ǰ��Ģ���ڹ�Ȳ�ͬ����Ҫ��д����麯��
	EnemyVSHero checkCollisionWithHero();

	// ��д��ȡ���κ���
	CCRect getEnemyRect();

protected:
	CCPoint startPos;  // ��ʶʳ�˻��ճ���ʱ��λ��
};

// �ڹ꣬�����ߵ�����
class CCEnemyTortoise : public CCEnemy
{
public:
	CCEnemyTortoise(int _startface);
	~CCEnemyTortoise();

	// ʵ�ָ���Ĵ��麯��
	void launchEnemy();
	// ��дˮƽ������ײ��⣬��Ϊ�ڹ�����ˮƽ�����ϵĵ�ͷ
	void enemyCollistionH();
	void update(float dt);

private:
	CCSpriteFrame *leftFace;
	CCSpriteFrame *rightFace;
};

// �ڹ꣬�����ߵģ����������˶�����
class CCEnemyTortoiseRound : public CCEnemy
{
public:
	CCEnemyTortoiseRound(float dis);
	~CCEnemyTortoiseRound();

	void launchEnemy();

	// ��д�����ˮƽ������£����������ڹ�Ҳֻ��Ҫˮƽ����ĸ���
	//void enemyCollistionH();

	void update(float dt);

	void setRoundDis(float dis);
	float getRoundDis();
private:
	float roundDis;  // ˮƽ�����������ľ���

	void reRight();
	void reLeft();
};

// ��ɵ��ڹ�
class CCEnemyTortoiseFly : public CCEnemy
{
public:
	CCEnemyTortoiseFly(float dis);
	~CCEnemyTortoiseFly();

	void launchEnemy();

	void update(float dt);

	void setFlyDis(float dis);
	float getFlyDis();

private:
	float flyDis;
};

// ��
class CCEnemyFireString : public CCEnemy
{
public:
	CCEnemyFireString(float _begAngle, float _time);
	~CCEnemyFireString();

	// �����𴮺������ת��û��״̬��⣬��������������ǻ�Ծ״̬�����ҹ����в���
	void launchEnemy();

	// ��д�����ֹͣ������£���Ϊ������������
	void stopEnemyUpdate();

	// �������ǵ���ײ����кܴ�ͬ
	EnemyVSHero checkCollisionWithHero();

	// ��������д������Ϊ��
	void forKilledByBullet();
	void forKilledByHero();	

	void update(float dt);

private:
	CCSprite *enemyBody2;  // ��������������
	CCSprite *enemyBody3;

	CCSize fireSize;  // ��������ĳߴ磬�������ǵ���ײ��������������Ҫ������õ�
	CCArray *pArrayFire;  // �������������

	float begAngle;  // ������תǰ�������ĽǶ�
	float time;      // ����תһ������Ҫ��ʱ��

	double angle;  // ����ʱ�̣�������ʱ�뷽��ĽǶȣ�����ֵ
	double PI;  // ��ת���õ�Բ����

	void launchFireString();
};

// ����ͨͨ�ķ���
class CCEnemyFlyFish : public CCEnemy
{
public:
	CCEnemyFlyFish(float _offsetH, float _offsetV, float _duration);
	~CCEnemyFlyFish();

	void launchEnemy();
	void update(float dt);
	void checkState();

	// ������·������������ڱ����ǲ���������ֻҪ���������ཻ��Ϊ��������
	EnemyVSHero checkCollisionWithHero();

	// ���汻���ǲ����ĺ���Ϊ�գ����Ǳ��ӵ����е���������û����
	void forKilledByHero();

private:
	float offsetH;  // ����ˮƽ������ƫ��
	float offsetV;  // ��ֱ������ƫ��
	float offsetDuration;  // ƫ�Ƴ���ʱ��
	bool isFlying;    // �Ƿ��Ƿ���״̬����ֹһ���ط�����ֹ�ķ���

	void flyInSky();
	void reSetNotInSky();
};

// Boss
class CCEnemyBoss : public CCEnemy
{
public:
	CCEnemyBoss();
	~CCEnemyBoss();

	void launchEnemy();

	EnemyVSHero checkCollisionWithHero();

	void forKilledByBullet();
	void forKilledByHero();

	void enemyCollistionH();

	void update(float dt);

private:

	float leftSide;
	float rightSide;

	void moveLeft();
	void moveRight();
};

// Boss�������ӵ���Ҳ�����һ�ֹ���
class CCEnemyBossFire : public CCEnemy
{
public:
	CCEnemyBossFire();
	~CCEnemyBossFire();

	void launchEnemy();

	EnemyVSHero checkCollisionWithHero();

	void forKilledByBullet();
	void forKilledByHero();

	void update(float dt);
private:

};


// ���ڹܵ����������ϰ������ģ�����ð��һ������Ģ���Ĺ���
class CCEnemyAddMushroom : public CCEnemy
{
public:
	CCEnemyAddMushroom(int _addnum);
	~CCEnemyAddMushroom();

	void launchEnemy();

	EnemyVSHero checkCollisionWithHero();

	void forKilledByHero();

	void update(float dt);
private:
	int addNums;  // һ��Ҫ����Ģ���ĸ���
	bool isAddable;  // ÿ��һ��ʱ������һ��������ڿ��ƹ��������ٶ�

	void addMushroom();
	void reSetNonAddable();  // ������Ģ������������Ϊ��������
};


// �궷����̨
class CCEnemyBattery : public CCEnemy
{
public:
	CCEnemyBattery(float delay);
	~CCEnemyBattery();

	void launchEnemy();

	EnemyVSHero checkCollisionWithHero();

	void forKilledByBullet();  // ������ֻ̨�ܱ������
	void forKilledByHero();  // ��̨���ܱ����ǲ���

	void update(float dt);

private:

	bool isFireable;  // ���ڿ��Ʒ����ӵ���Ƶ��
	float fireDelay;  // �����ӵ�֮���ʱ����

	void addBatteryBullet();
	void reSetNonFireable();  // ��������Ϊ���ɷ����ӵ�

	void stopAndClear();

	CCPoint firePos;   // ����������ӵ��ĳ�ʼλ��
};


// ��̨�������ӵ���С�׵�
class CCEnemyBatteryBullet : public CCEnemy
{
public:
	CCEnemyBatteryBullet();
	~CCEnemyBatteryBullet();

	void launchEnemy();

	EnemyVSHero checkCollisionWithHero();

	void forKilledByBullet();
	void forKilledByHero();

	void update(float dt);
private:

};


// ���ƹ�����������ǿ�������ʱ��������
class CCEnemyDarkCloud : public CCEnemy
{
public:
	CCEnemyDarkCloud(float _delay, int _type);
	~CCEnemyDarkCloud();

	void launchEnemy();

	void update(float dt);

	void forKilledByBullet();  // ���ϵ����Ʋ��ᱻ�ӵ�����
	void forKilledByHero();    // Ҳ���ᱻ���ǲ���������������Ϊ��

private:
	float dropRegion;  // ������������� һ��̶�
	float leftSide;    // �������߽�
	float rightSide;   // �ұ߽�

	bool isDropable;   // �����Ƿ���Ե������Ƶı���

	float delay;       // ��������֮���ʱ����  ��Ҫ�ڶ�����趨

	void addLighting();
	void reSetDropable();

	int type;   // ���ƵĴ�С�ͺ� 0 1 2�ֱ����С�д�

	CCSpriteFrame *dark;   // ���Ƶ��������һ˲չʾ��
	CCSpriteFrame *normal; // �����İ���
	void reSetNormal();
};


// ����
class CCEnemyLighting : public CCEnemy
{
public:
	CCEnemyLighting();
	~CCEnemyLighting();

	EnemyVSHero checkCollisionWithHero();

	void launchEnemy();

	void checkState();

	void update(float dt);

	void forKilledByHero();
	void forKilledByBullet();

private:

};



#endif