#ifndef _BULLET_
#define _BULLET_

#include "cocos2d.h"
#include "GameEnum.h"
using namespace cocos2d;

class CCBullet : public CCNode
{
public:
	CCBullet();
	virtual ~CCBullet();

	void setBodySize(CCSize _size);
	CCSize getBodySize();

	void setStartPos(CCPoint pos);
	CCPoint getStartPos();

	BulletType getBulletType();

	BulletState getBulletState();

	virtual void checkBulletState();

	// ���麯��
	virtual void launchBullet() = 0;

	virtual void forKilledEnemy();

	virtual CCRect getBulletRect();

protected:
	BulletType bulletType;  // �ӵ�����������
	CCSprite *bulletBody;
	CCSize bodySize;

	CCPoint startPos;  // �ӵ�������

	BulletState bulletState; // �ӵ��Ļ״̬
};

// ��ͳ����ͨ�ӵ�
class CCBulletCommon : public CCBullet
{
public:
	CCBulletCommon();
	~CCBulletCommon();

	// ʵ�ָ���Ĵ��麯���������ӵ�
	void launchBullet();

	void commonBulletCollisionH();
	void commonBulletCollisionV();

	void update(float dt);

	void forKilledEnemy();
private:
	float moveOffset;
	float ccMoveOffset;

	float jumpOffset;
	float ccJumpOffset;

	bool isLand;  // �����ӵ��Ĺ켣����������ֱ��Ȼ���ǵ����ϵ�������

	void showBoom();
	void autoClear();
};

// ����
class CCBulletArrow : public CCBullet
{
public:
	CCBulletArrow();
	~CCBulletArrow();

	void launchBullet();

	void forKilledEnemy();

	void update(float dt);

	void arrowBulletCollisionH();

	CCRect getBulletRect();
private:
	// ����ֻ��ˮƽ������ƶ�
	float moveOffset;
	float ccMoveOffset;

	void autoClear();
	void broken();
};

#endif