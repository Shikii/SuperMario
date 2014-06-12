#include "Enemy.h"
#include "Hero.h"
#include "AnimationManager.h"
#include "GameMap.h"
#include "GameLayer.h"
#include "Hero.h"
#include <math.h>
#include "Global.h"

// ******************CCEnemy******************** //
CCEnemy::CCEnemy()
{
	// 正常情况下，怪物出生后会朝着左方向移动来对主角产生威胁
	// 在特殊情况下可以通过地图上的怪物对象字典来携带朝向信息
	startFace = eLeft;
	moveOffset = 0.0f;
	ccMoveOffset = 0.6f;

	jumpOffset = 0.0f;
	ccJumpOffset = 0.3f;

	enemyState = eEnemyState_nonactive;
}

CCEnemy::~CCEnemy()
{
	//this->unscheduleAllSelectors();
}

void CCEnemy::setTileCoord(CCPoint _tileCoord)
{
	tileCoord = _tileCoord;
}
CCPoint CCEnemy::getTileCoord()
{
	return tileCoord;
}

void CCEnemy::setEnemyPos(CCPoint _enemyPos)
{
	enemyPos = _enemyPos;
}
CCPoint CCEnemy::getEnemyPos()
{
	return enemyPos;
}

void CCEnemy::setBodySize(CCSize _size)
{
	bodySize = _size;
}
CCSize CCEnemy::getBodySize()
{
	return bodySize;
}

void CCEnemy::setMoveOffset(float _moveoffset)
{
	moveOffset = _moveoffset;
}
float CCEnemy::getMoveOffset()
{
	return moveOffset;
}

void CCEnemy::setccMoveOffset(float _ccmoveoffset)
{
	ccMoveOffset = _ccmoveoffset;
}
float CCEnemy::getccMoveOffset()
{
	return ccMoveOffset;
}

EnemyType CCEnemy::getEnemyType()
{
	return enemyType;
}

CCRect CCEnemy::getEnemyRect()
{
	CCPoint pos = this->getPosition();
	return CCRectMake(pos.x - bodySize.width/2 + 2, pos.y + 2, bodySize.width - 4, bodySize.height - 4);
}

void CCEnemy::setEnemyState(EnemyState _state)
{
	enemyState = _state;
	switch (enemyState)
	{
	case eEnemyState_over:
		{
			this->enemyBody->stopAllActions();
			this->unscheduleUpdate();
			this->setVisible(false);
			break;
		}
	default:
		break;
	}
}
EnemyState CCEnemy::getEnemyState()
{
	return enemyState;
}
void CCEnemy::checkState()
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	float tempMaxH = CCGameLayer::getMapMaxH();
	CCPoint pos = this->getPosition();

	if ( (pos.x + bodySize.width/2 - tempMaxH >= 0) &&
		(pos.x - bodySize.width/2 - tempMaxH) <= winSize.width )
	{
		enemyState = eEnemyState_active;
	}
	else
	{
		if (pos.x + bodySize.width/2 - tempMaxH < 0)
		{
			this->setEnemyState(eEnemyState_over);
		}
		else
		{
			enemyState = eEnemyState_nonactive;
		}
	}
}

void CCEnemy::stopEnemyUpdate()
{
	enemyBody->stopAllActions();
}

// 普通怪物的水平方向碰撞检测
void CCEnemy::enemyCollistionH()
{
	CCPoint currentPos = this->getPosition();
	CCSize enemySize = this->getContentSize();
	CCPoint leftCollistion = ccp(currentPos.x - enemySize.width/2, currentPos.y);
	CCPoint leftTilecoord = CCGameMap::getGameMap()->positionToTileCoord(leftCollistion);
	CCPoint leftPos = CCGameMap::getGameMap()->tilecoordToPosition(leftTilecoord);
	leftPos = ccp(leftPos.x + bodySize.width/2 + CCGameMap::getGameMap()->tileSize.width, currentPos.y);

	TileType tileType;
	// 左侧检测
	tileType = CCGameMap::getGameMap()->tileTypeforPos(leftTilecoord);
	switch (tileType)
	{
	case eTile_Pipe:
	case eTile_Block:
		this->setPosition(leftPos);
		moveOffset *= -1;		
		break;
	default:
		break;
	}
	// 右侧检测
	CCPoint rightCollistion = ccp(currentPos.x + bodySize.width/2, currentPos.y);
	CCPoint rightTilecoord = CCGameMap::getGameMap()->positionToTileCoord(rightCollistion);
	CCPoint rightPos = CCGameMap::getGameMap()->tilecoordToPosition(rightTilecoord);
	rightPos = ccp(rightPos.x - bodySize.width/2, currentPos.y);

	tileType = CCGameMap::getGameMap()->tileTypeforPos(rightTilecoord);
	switch (tileType)
	{
	case eTile_Pipe:
	case eTile_Block:
		this->setPosition(rightPos);
		moveOffset *= -1;
		break;
	default:
		break;
	}
}

// 普通怪物的竖直方向碰撞检测
void CCEnemy::enemyCollistionV()
{
	CCPoint currentPos = this->getPosition();
	CCPoint downCollision = currentPos;
	CCPoint downTilecoord = CCGameMap::getGameMap()->positionToTileCoord(downCollision);
	downTilecoord.y += 1;

	CCPoint downPos = CCGameMap::getGameMap()->tilecoordToPosition(downTilecoord);
	downPos = ccp(currentPos.x, downPos.y + CCGameMap::getGameMap()->getTileSize().height);

	TileType tileType = CCGameMap::getGameMap()->tileTypeforPos(downTilecoord);
	bool downFlag = false;
	switch (tileType)
	{
	case eTile_Land:
	case eTile_Pipe:
	case eTile_Block:
		{
			downFlag = true;
			jumpOffset = 0.0f;
			this->setPosition(downPos);
			break;
		}
	case eTile_Trap:
		{
			this->setEnemyState(eEnemyState_over);
			break;
		}
	}

	if (downFlag)
	{
		return ;
	}

	jumpOffset -= ccJumpOffset;
}

EnemyVSHero CCEnemy::checkCollisionWithHero()
{
	EnemyVSHero ret = eVS_nonKilled;

	CCPoint heroPos = CCHero::getHeroInstance()->getPosition();
	CCSize heroSize = CCHero::getHeroInstance()->getContentSize();
	CCRect heroRect = CCRectMake(heroPos.x - heroSize.width/2 + 2, heroPos.y + 3, 
		heroSize.width - 4, heroSize.height - 4);

	CCRect heroRectVS = CCRectMake(heroPos.x - heroSize.width/2 - 3, heroPos.y, 
		heroSize.width - 6, 2);

	CCPoint enemyPos = this->getPosition();
	CCRect enemyRect = CCRectMake(enemyPos.x - bodySize.width/2 + 1, enemyPos.y, 
		bodySize.width - 2, bodySize.height - 4);

	CCRect enemyRectVS = CCRectMake(enemyPos.x - bodySize.width/2 - 2, enemyPos.y + bodySize.height - 4, 
		bodySize.width - 4, 4);

	if (heroRectVS.intersectsRect(enemyRectVS))
	{
		ret = eVS_enemyKilled;
		return ret;
	}

	if (heroRect.intersectsRect(enemyRect))
	{
		ret = eVS_heroKilled;
		return ret;
	}

	return ret;
}

void CCEnemy::forKilledByHero()
{
	enemyState = eEnemyState_over;
	enemyBody->stopAllActions();
	this->stopAllActions();
	this->unscheduleUpdate();
	enemyBody->setDisplayFrame(enemyLifeOver);
	CCActionInterval *pDelay = CCDelayTime::create(1.0f);
	this->runAction(CCSequence::create(pDelay, 
		CCCallFunc::create(this, callfunc_selector(CCEnemy::setNonVisibleForKilledByHero)), NULL));
}

void CCEnemy::setNonVisibleForKilledByHero()
{
	this->setVisible(false);
}

void CCEnemy::forKilledByBullet()
{
	enemyState = eEnemyState_over;
	enemyBody->stopAllActions();
	this->unscheduleUpdate();

	CCMoveBy *pMoveBy = NULL;
	CCJumpBy *pJumpBy = NULL;

	switch (CCGlobal::getGlobalInstance()->getCurrentBulletType())
	{
	case eBullet_common:
		{
			if (enemyType == eEnemy_mushroom || enemyType == eEnemy_AddMushroom)
			{
				enemyBody->setDisplayFrame(overByArrow);
			}
			else
			{
				enemyBody->setDisplayFrame(enemyLifeOver);
			}

			switch (CCHero::getHeroInstance()->face)
			{
			case eRight:
				pJumpBy = CCJumpBy::create(0.3f, ccp(bodySize.width*2, 0), bodySize.height, 1);
				break;
			case eLeft:
				pJumpBy = CCJumpBy::create(0.3f, ccp(-bodySize.width*2, 0), bodySize.height, 1);
				break;
			default:
				break;
			}

			break;
		}
	case eBullet_arrow:
		{
			enemyBody->setDisplayFrame(overByArrow);
			CCSprite *arrow = CCSprite::create("arrow.png");
			arrow->setPosition(ccp(bodySize.width/2, bodySize.height/2));
			this->addChild(arrow);

			switch (CCHero::getHeroInstance()->face)
			{
			case eRight:
				pMoveBy = CCMoveBy::create(0.1f, ccp(2*bodySize.width, 0));
				break;
			case eLeft:
				pMoveBy = CCMoveBy::create(0.1f, ccp(-2*bodySize.width, 0));
				arrow->runAction(CCFlipX::create(true));
				break;
			default:
				break;
			}

			break;
			break;
		}
	default:
		break;
	}

	// 先判断如果是食人花的话，就是原地停留一段时间然后消失掉
	if (enemyType == eEnemy_flower)
	{
		CCDelayTime *pDelay = CCDelayTime::create(0.2f);
		this->runAction(CCSequence::create(pDelay,
			CCCallFunc::create(this, callfunc_selector(CCEnemy::setNonVisibleForKilledByBullet)), NULL));
		return ;
	}
	
	if (pJumpBy)
	{
		this->runAction(CCSequence::create(pJumpBy, 
			CCCallFunc::create(this, callfunc_selector(CCEnemy::setNonVisibleForKilledByBullet)), NULL));
	}
	else
	{
		this->runAction(CCSequence::create(pMoveBy, 
			CCCallFunc::create(this, callfunc_selector(CCEnemy::setNonVisibleForKilledByBullet)), NULL));
	}
}

void CCEnemy::setNonVisibleForKilledByBullet()
{
	enemyState = eEnemyState_over;
	this->setVisible(false);
}


// ******************** CCEnemyMushroom ***************** //
CCEnemyMushroom::CCEnemyMushroom()
{
	enemyType = eEnemy_mushroom;
	bodySize = CCSizeMake(16.0f, 16.0f);
	enemyBody = CCSprite::create("Mushroom0.png", CCRectMake(0, 0, 16, 16));
	enemyBody->setAnchorPoint(ccp(0, 0));
	this->setContentSize(bodySize);
	this->addChild(enemyBody);
	this->setAnchorPoint(ccp(0.5f, 0));

	enemyLifeOver = CCSpriteFrame::create("Mushroom0.png", CCRectMake(32, 0, 16, 16));
	enemyLifeOver->retain();

	overByArrow = CCSpriteFrame::create("Mushroom0.png", CCRectMake(48, 0, 16, 16));
	overByArrow->retain();

	moveOffset = -ccMoveOffset;
}

CCEnemyMushroom::~CCEnemyMushroom()
{
	this->unscheduleAllSelectors();
}

void CCEnemyMushroom::launchEnemy()
{
	enemyBody->runAction(CCRepeatForever::create(sAnimationMgr->createAnimate(eAniMushroom)));
	this->scheduleUpdate();
}



void CCEnemyMushroom::update(float dt)
{
	this->checkState();

	if (enemyState == eEnemyState_active)
	{
		CCPoint currentPos = this->getPosition();
		currentPos.x += moveOffset;
		currentPos.y += jumpOffset;
		this->setPosition(currentPos);

		this->enemyCollistionH();
		this->enemyCollistionV();
	}
}


// ********************** CCEnemyFlower ****************** //
CCEnemyFlower::CCEnemyFlower()
{
	enemyType = eEnemy_flower;
	bodySize = CCSizeMake(16, 24);
	enemyBody = CCSprite::create("flower0.png", CCRectMake(0, 0, 16, 24));
	enemyBody->setAnchorPoint(ccp(0, 0));
	this->setContentSize(bodySize);
	this->addChild(enemyBody);
	this->setAnchorPoint(ccp(0.5f, 0));

	// 吃人花虽然不会被踩死，但是会被子弹打死
	enemyLifeOver = CCSpriteFrame::create("flower0.png", CCRectMake(0, 0, 16, 24));
	enemyLifeOver->retain();

	overByArrow = enemyLifeOver;
}

CCEnemyFlower::~CCEnemyFlower()
{
	this->unscheduleAllSelectors();
}

void CCEnemyFlower::launchEnemy()
{
	enemyBody->runAction(CCRepeatForever::create(sAnimationMgr->createAnimate(eAniflower)));
	CCPoint pos = this->getPosition();	
	pos.y -= bodySize.height;
	startPos = pos;
	this->runAction(CCPlace::create(pos));

	CCActionInterval *pMoveBy = CCMoveBy::create(1.0f, ccp(0.0f, bodySize.height));
	CCActionInterval *pDelay = CCDelayTime::create(1.0f);
	CCActionInterval *pMoveByBack = pMoveBy->reverse();
	CCActionInterval *pDelay2 = CCDelayTime::create(2.0f);
	this->runAction(CCRepeatForever::create(
		(CCActionInterval*)CCSequence::create(pMoveBy, pDelay, pMoveByBack, pDelay2, NULL)));

	this->scheduleUpdate();
}

void CCEnemyFlower::update(float dt)
{
	// 食人花的帧更新函数中不做位置控制类代码
//	if (! isLaunch)
//	{
//		if (enemyState == eEnemyState_active)
//		{
//			isLaunch = true;
//			//startPos.y += bodySize.height;
//			//CCActionInterval *pMoveBy = CCMoveBy::create(2.0f, ccp(0.0f, bodySize.height));
//			//CCActionInterval *pDelay = CCDelayTime::create(1.0f);
//			//CCActionInterval *pMoveByBack = pMoveBy->reverse();
//			//this->runAction(CCRepeatForever::create(
//			//	(CCActionInterval*)CCSequence::create(pMoveBy, pDelay, pMoveByBack, NULL)));
//		}
//	}
	this->checkState();
}

EnemyVSHero CCEnemyFlower::checkCollisionWithHero()
{
	EnemyVSHero ret = eVS_nonKilled;

	CCPoint heroPos = CCHero::getHeroInstance()->getPosition();
	CCSize heroSize = CCHero::getHeroInstance()->currentSize;
	CCRect heroRect = CCRectMake(heroPos.x - heroSize.width/2 + 2, heroPos.y + 2, 
		heroSize.width - 4, heroSize.height - 4);

	CCPoint enemyPos = this->getPosition();
	CCRect enemyRect = CCRectMake(enemyPos.x - bodySize.width/2 + 2, enemyPos.y + bodySize.height - (enemyPos.y-startPos.y), 
		bodySize.width - 4, enemyPos.y - startPos.y);


	if (heroRect.intersectsRect(enemyRect))
	{
		ret = eVS_heroKilled;
	}

	return ret;
}

CCRect CCEnemyFlower::getEnemyRect()
{
	CCPoint enemyPos = this->getPosition();
	CCRect enemyRect = CCRectMake(enemyPos.x - bodySize.width/2 + 2, enemyPos.y + bodySize.height - (enemyPos.y-startPos.y), 
		bodySize.width - 4, enemyPos.y - startPos.y);
	return enemyRect;
}

// ********************** CCEnemyTortoise ****************** //
CCEnemyTortoise::CCEnemyTortoise(int _startface)
{
	// 地上行走的乌龟在初始化时，要先判断起始朝向
	// 0：左             1：右
	switch (_startface)
	{
	case 0:
		startFace = eLeft;
		enemyBody = CCSprite::create("tortoise0.png", CCRectMake(18*2, 0, 18, 24));
		leftFace = CCSpriteFrame::create("tortoise0.png", CCRectMake(18*2, 0, 18, 24));
		leftFace->retain();
		moveOffset = -ccMoveOffset;
		break;
	case 1:
		startFace = eRight;
		enemyBody = CCSprite::create("tortoise0.png", CCRectMake(18*5, 0, 18, 24));
		rightFace = CCSpriteFrame::create("tortoise0.png", CCRectMake(18*5, 0, 18, 24));
		rightFace->retain();
		moveOffset = ccMoveOffset;
		break;
	default:
		break;
	}

	enemyType = eEnemy_tortoise;
	bodySize = CCSizeMake(18.0f, 24.0f);	
	enemyBody->setAnchorPoint(ccp(0, 0));
	this->setContentSize(bodySize);
	this->addChild(enemyBody);
	this->setAnchorPoint(ccp(0.5f, 0.0f));

	enemyLifeOver = CCSpriteFrame::create("tortoise0.png", CCRectMake(18*9, 0, 18, 24));
	enemyLifeOver->retain();

	overByArrow = CCSpriteFrame::create("tortoise0.png", CCRectMake(18*8, 0, 18, 24));
	overByArrow->retain();
}

CCEnemyTortoise::~CCEnemyTortoise()
{
	this->unscheduleAllSelectors();
}

void CCEnemyTortoise::launchEnemy()
{
	switch (startFace)
	{
	case eLeft:
		enemyBody->runAction(CCRepeatForever::create(sAnimationMgr->createAnimate(eAniTortoiseLeft)));
		break;
	case eRight:
		enemyBody->runAction(CCRepeatForever::create(sAnimationMgr->createAnimate(eAniTortoiseRight)));
		break;
	default:
		break;
	}
	this->scheduleUpdate();
}

void CCEnemyTortoise::enemyCollistionH()
{
	CCPoint currentPos = this->getPosition();
	CCSize enemySize = this->getContentSize();
	CCPoint leftCollistion = ccp(currentPos.x - enemySize.width/2, currentPos.y);
	CCPoint leftTilecoord = CCGameMap::getGameMap()->positionToTileCoord(leftCollistion);
	CCPoint leftPos = CCGameMap::getGameMap()->tilecoordToPosition(leftTilecoord);
	leftPos = ccp(leftPos.x + bodySize.width/2 + CCGameMap::getGameMap()->tileSize.width, currentPos.y);

	TileType tileType;
	// 左侧检测
	tileType = CCGameMap::getGameMap()->tileTypeforPos(leftTilecoord);
	switch (tileType)
	{
	case eTile_Pipe:
	case eTile_Block:
		this->setPosition(leftPos);
		moveOffset *= -1;
		//enemyBody->setDisplayFrame(rightFace);
		enemyBody->stopAllActions();
		enemyBody->runAction(CCRepeatForever::create(sAnimationMgr->createAnimate(eAniTortoiseRight)));
		break;
	default:
		break;
	}
	// 右侧检测
	CCPoint rightCollistion = ccp(currentPos.x + bodySize.width/2, currentPos.y);
	CCPoint rightTilecoord = CCGameMap::getGameMap()->positionToTileCoord(rightCollistion);
	CCPoint rightPos = CCGameMap::getGameMap()->tilecoordToPosition(rightTilecoord);
	rightPos = ccp(rightPos.x - bodySize.width/2, currentPos.y);

	tileType = CCGameMap::getGameMap()->tileTypeforPos(rightTilecoord);
	switch (tileType)
	{
	case eTile_Pipe:
	case eTile_Block:
		this->setPosition(rightPos);
		moveOffset *= -1;
		
		//enemyBody->setDisplayFrame(leftFace);
		enemyBody->stopAllActions();
		enemyBody->runAction(CCRepeatForever::create(sAnimationMgr->createAnimate(eAniTortoiseLeft)));
		break;
	default:
		break;
	}
}

void CCEnemyTortoise::update(float dt)
{
	this->checkState();

	if (enemyState == eEnemyState_active)
	{
		CCPoint currentPos = this->getPosition();
		currentPos.x += moveOffset;
		currentPos.y += jumpOffset;
		this->setPosition(currentPos);

		this->enemyCollistionH();
		this->enemyCollistionV();
	}
}


// ********************** CCEnemyTortoiseRound ****************** //
CCEnemyTortoiseRound::CCEnemyTortoiseRound(float dis)
{
	enemyType = eEnemy_tortoiseRound;
	bodySize = CCSizeMake(18.0f, 24.0f);
	enemyBody = CCSprite::create("tortoise0.png", CCRectMake(18*2, 0, 18, 24));
	enemyBody->setAnchorPoint(ccp(0, 0));
	this->setContentSize(bodySize);
	this->addChild(enemyBody);
	this->setAnchorPoint(ccp(0.5f, 0.0f));

	enemyLifeOver = CCSpriteFrame::create("tortoise0.png", CCRectMake(18*9, 0, 18, 24));
	enemyLifeOver->retain();

	overByArrow = CCSpriteFrame::create("tortoise0.png", CCRectMake(18*8, 0, 18, 24));
	overByArrow->retain();

	roundDis = dis;
}
CCEnemyTortoiseRound::~CCEnemyTortoiseRound()
{
	this->unscheduleAllSelectors();
}

void CCEnemyTortoiseRound::launchEnemy()
{
	enemyState = eEnemyState_active;
	enemyBody->runAction(CCRepeatForever::create(sAnimationMgr->createAnimate(eAniTortoiseLeft)));
	CCActionInterval *pMoveLeft = CCMoveBy::create(2.0f, ccp(-roundDis, 0.0f));
	CCActionInterval *pMoveRight = CCMoveBy::create(2.0f, ccp(roundDis, 0.0f));
	CCDelayTime *pDelay = CCDelayTime::create(0.2f);

	this->runAction(CCRepeatForever::create((CCActionInterval*)CCSequence::create(pMoveLeft, 
		CCCallFunc::create(this, callfunc_selector(CCEnemyTortoiseRound::reRight)), 
		pMoveRight,
		CCCallFunc::create(this, callfunc_selector(CCEnemyTortoiseRound::reLeft)),
		NULL)));
}

void CCEnemyTortoiseRound::update(float dt)
{
	this->checkState();
}

void CCEnemyTortoiseRound::reRight()
{
	enemyBody->stopAllActions();
	enemyBody->runAction(CCRepeatForever::create(sAnimationMgr->createAnimate(eAniTortoiseRight)));
}

void CCEnemyTortoiseRound::reLeft()
{
	enemyBody->stopAllActions();
	enemyBody->runAction(CCRepeatForever::create(sAnimationMgr->createAnimate(eAniTortoiseLeft)));
}


// ********************** CCEnemyTortoiseFly ****************** //

CCEnemyTortoiseFly::CCEnemyTortoiseFly(float dis)
{
	enemyType = eEnemy_tortoiseFly;
	bodySize = CCSizeMake(18.0f, 24.0f);
	enemyBody = CCSprite::create("tortoise0.png", CCRectMake(0, 0, 18, 24));
	enemyBody->setAnchorPoint(ccp(0, 0));
	this->setContentSize(bodySize);
	this->addChild(enemyBody);
	this->setAnchorPoint(ccp(0.5f, 0.0f));

	enemyLifeOver = CCSpriteFrame::create("tortoise0.png", CCRectMake(18*9, 0, 18, 24));
	enemyLifeOver->retain();

	overByArrow = CCSpriteFrame::create("tortoise0.png", CCRectMake(18*8, 0, 18, 24));
	overByArrow->retain();

	flyDis = dis;
}

CCEnemyTortoiseFly::~CCEnemyTortoiseFly()
{
	this->unscheduleAllSelectors();
}

void CCEnemyTortoiseFly::launchEnemy()
{
	enemyState = eEnemyState_active;
	enemyBody->runAction(CCRepeatForever::create(sAnimationMgr->createAnimate(eAniTortoiseFly)));
	CCActionInterval *pMoveDown = CCMoveBy::create(2.0f, ccp(0, -flyDis));
	CCActionInterval *pMoveUp = CCMoveBy::create(2.0f, ccp(0, flyDis));

	this->runAction(CCRepeatForever::create(
		(CCActionInterval*)CCSequence::create(pMoveDown, pMoveUp, NULL)));
}

void CCEnemyTortoiseFly::update(float dt)
{
	this->checkState();
}

void CCEnemyTortoiseFly::setFlyDis(float dis)
{
	flyDis = dis;
}

float CCEnemyTortoiseFly::getFlyDis()
{
	return flyDis;
}

// ********************** CCEnemyFireString ****************** //
CCEnemyFireString::CCEnemyFireString(float _begAngle, float _time)
{
	enemyType = eEnemy_fireString;

	pArrayFire = CCArray::createWithCapacity(3);
	pArrayFire->retain();

	enemyBody = CCSprite::create("fireBall.png");
	pArrayFire->addObject(enemyBody);
	enemyBody2 = CCSprite::create("fireBall.png");
	pArrayFire->addObject(enemyBody2);
	enemyBody3 = CCSprite::create("fireBall.png");
	pArrayFire->addObject(enemyBody3);
	fireSize = CCSizeMake(8.0f, 8.0f);

	enemyBody->setPosition(ccp(8, 8));
	this->addChild(enemyBody);
	enemyBody2->setPosition(ccp(24, 8));
	this->addChild(enemyBody2);
	enemyBody3->setPosition(ccp(40, 8));
	this->addChild(enemyBody3);

	bodySize = CCSizeMake(48, 16);
	this->setContentSize(bodySize);
	this->setAnchorPoint(ccp(0.0f, 0.5f));

	begAngle = _begAngle;
	time = _time;

	angle = begAngle;
	PI = 3.1415926;

	// 因为不存在火串被踩死或是被子弹打死，所以用不到这两个帧
	enemyLifeOver = NULL;
	overByArrow = NULL;
}

CCEnemyFireString::~CCEnemyFireString()
{
	this->unscheduleAllSelectors();
}

void CCEnemyFireString::launchEnemy()
{
	enemyState = eEnemyState_active;
	enemyBody->runAction(CCRepeatForever::create(sAnimationMgr->createAnimate(eAniRotatedFireBall)));
	enemyBody2->runAction(CCRepeatForever::create(sAnimationMgr->createAnimate(eAniRotatedFireBall)));
	enemyBody3->runAction(CCRepeatForever::create(sAnimationMgr->createAnimate(eAniRotatedFireBall)));

	this->runAction(CCSequence::create(CCRotateBy::create(1.0f, -begAngle),
		CCCallFunc::create(this, callfunc_selector(CCEnemyFireString::launchFireString)), NULL));
}

void CCEnemyFireString::stopEnemyUpdate()
{
	enemyBody->stopAllActions();
	enemyBody2->stopAllActions();
	enemyBody3->stopAllActions();
}

EnemyVSHero CCEnemyFireString::checkCollisionWithHero()
{
	EnemyVSHero ret = eVS_nonKilled;
	
	CCPoint heroPos = CCHero::getHeroInstance()->getPosition();
	CCSize heroSize = CCHero::getHeroInstance()->getContentSize();
	CCRect heroRect = CCRectMake(heroPos.x - heroSize.width/2 + 2, heroPos.y + 2,
		heroSize.width - 4, heroSize.height - 4);

	CCPoint thisPos = this->getPosition();

	double angleTemp = 2*PI*angle/360.0f;
	for (int i = 0; i < 3; ++i)
	{
		CCPoint firePos = ccp(thisPos.x + (2*i*8+8)*cos(angleTemp), thisPos.y + (2*i*8+8)*sin(angleTemp));
		CCRect fireRect = CCRectMake(firePos.x - fireSize.width/2, firePos.y - fireSize.height/2,
			fireSize.width, fireSize.height);
		if (heroRect.intersectsRect(fireRect))
		{
			ret = eVS_heroKilled;
			break;
		}
	}

	return ret;
}

void CCEnemyFireString::forKilledByBullet()
{

}

void CCEnemyFireString::forKilledByHero()
{

}

void CCEnemyFireString::launchFireString()
{
	this->runAction(CCRepeatForever::create(
		CCRotateBy::create(time, -360.0f)));
	this->scheduleUpdate();
}

void CCEnemyFireString::update(float dt)
{
	angle += (6.0/time);
	if (angle >= 360)
	{
		angle -= 360.0f;
	}
}


// ********************** CCEnemyFlyFish ****************** //
CCEnemyFlyFish::CCEnemyFlyFish(float _offsetH, float _offsetV, float _duration)
{
	enemyType = eEnemy_flyFish;
	enemyState = eEnemyState_active;
	enemyBody = CCSprite::create("flyFishRight.png", CCRectMake(16*4, 0, 16, 16));
	bodySize = CCSizeMake(16, 16);
	enemyBody->setAnchorPoint(ccp(0, 0));
	this->setContentSize(bodySize);
	this->addChild(enemyBody);
	this->setAnchorPoint(ccp(0.5f, 0.0f));

	enemyLifeOver = CCSpriteFrame::create("flyFishRight.png", CCRectMake(16*4, 0, 16, 16));
	enemyLifeOver->retain();
	overByArrow = enemyLifeOver;

	offsetH = _offsetH;
	offsetV = _offsetV;
	offsetDuration = _duration;
	isFlying = false;
}

CCEnemyFlyFish::~CCEnemyFlyFish()
{
	this->unscheduleAllSelectors();
}

void CCEnemyFlyFish::launchEnemy()
{
	this->setVisible(false);
	enemyBody->runAction(CCRepeatForever::create(sAnimationMgr->createAnimate(eAniFlyFishR)));
	this->scheduleUpdate();
}

EnemyVSHero CCEnemyFlyFish::checkCollisionWithHero()
{
	EnemyVSHero ret = eVS_nonKilled;

	CCPoint heroPos = CCHero::getHeroInstance()->getPosition();
	CCSize heroSize = CCHero::getHeroInstance()->getContentSize();
	CCRect heroRect = CCRectMake(heroPos.x - heroSize.width/2 + 2, heroPos.y + 2, 
		heroSize.width - 4, heroSize.height - 4);

	CCPoint enemyPos = this->getPosition();
	CCRect enemyRect = CCRectMake(enemyPos.x - bodySize.width/2 + 2, enemyPos.y + 2, 
		bodySize.width - 4, bodySize.height - 4);

	if (heroRect.intersectsRect(enemyRect))
	{
		ret = eVS_heroKilled;
	}

	return ret;
}

void CCEnemyFlyFish::checkState()
{
	//CCPoint heroPos = CCHero::getHeroInstance()->getPosition();
	//if (fabs(heroPos.x - enemyPos.x) < bodySize.width)
	//{
	//	enemyState = eEnemyState_active;
	//}
}

void CCEnemyFlyFish::update(float dt)
{
	if (!isFlying)
	{
		CCPoint heroPos = CCHero::getHeroInstance()->getPosition();
		if (fabs(heroPos.x - enemyPos.x) < bodySize.width)
		{
			this->flyInSky();
		}
	}
}

void CCEnemyFlyFish::flyInSky()
{
	isFlying = true;
	this->setVisible(true);
	CCActionInterval *pMoveBy = CCMoveBy::create(offsetDuration, ccp(offsetH, offsetV));
	this->runAction(CCSequence::create(pMoveBy,
		CCCallFunc::create(this, callfunc_selector(CCEnemyFlyFish::reSetNotInSky)), NULL));
}

void CCEnemyFlyFish::reSetNotInSky()
{
	this->setVisible(false);
	this->runAction(CCPlace::create(enemyPos));
	isFlying = false;
}

void CCEnemyFlyFish::forKilledByHero()
{

}


// ********************** CCEnemyBoss ****************** //
CCEnemyBoss::CCEnemyBoss()
{
	enemyType = eEnemy_Boss;
	enemyState = eEnemyState_nonactive;

	enemyBody = CCSprite::create("boss.png", CCRectMake(0, 0, 32, 32));
	enemyBody->setAnchorPoint(ccp(0, 0));
	bodySize = CCSizeMake(32, 32);
	this->setContentSize(bodySize);
	this->addChild(enemyBody);
	this->setAnchorPoint(ccp(0.5f, 0.0f));
	
	enemyLifeOver = CCSpriteFrame::create("boss.png", CCRectMake(0, 0, 32, 32));
	enemyLifeOver->retain();
	overByArrow = enemyLifeOver;
	overByArrow->retain();

	ccMoveOffset = 0.5f;
	moveOffset = -ccMoveOffset;
}

CCEnemyBoss::~CCEnemyBoss()
{
	this->unscheduleAllSelectors();
}

void CCEnemyBoss::launchEnemy()
{
	leftSide = enemyPos.x - 32;
	rightSide = enemyPos.x + 32;

	this->scheduleUpdate();
}

EnemyVSHero CCEnemyBoss::checkCollisionWithHero()
{
	EnemyVSHero ret = eVS_nonKilled;

	CCPoint heroPos = CCHero::getHeroInstance()->getPosition();
	CCSize heroSize = CCHero::getHeroInstance()->getContentSize();
	CCRect heroRect = CCRectMake(heroPos.x - heroSize.width/2 + 2, heroPos.y + 2, 
		heroSize.width - 4, heroSize.height - 4);

	CCPoint enemyPos = this->getPosition();
	CCRect enemyRect = CCRectMake(enemyPos.x - bodySize.width/2 + 2, enemyPos.y + 2, 
		bodySize.width - 4, bodySize.height - 4);

	if (heroRect.intersectsRect(enemyRect))
	{
		ret = eVS_heroKilled;
	}

	return ret;
}

void CCEnemyBoss::forKilledByBullet()
{
	if (CCGlobal::getGlobalInstance()->getCurrentBulletType() == eBullet_arrow)
	{
		this->runAction(CCSequence::create(CCMoveBy::create(0.1f, ccp(8, 0)),
			CCMoveBy::create(0.1f, ccp(-8, 0)), NULL));

		static int num = 0;
		++num;
		if (num == 5)
		{
			enemyState = eEnemyState_over;
			enemyBody->stopAllActions();
			this->stopAllActions();
			this->unscheduleUpdate();
			this->setVisible(false);
		}		
	}
}

void CCEnemyBoss::forKilledByHero()
{

}

void CCEnemyBoss::moveLeft()
{
	enemyBody->stopAllActions();
	enemyBody->runAction(CCRepeatForever::create(sAnimationMgr->createAnimate(eAniBossMoveLeft)));
}

void CCEnemyBoss::moveRight()
{
	enemyBody->stopAllActions();
	enemyBody->runAction(CCRepeatForever::create(sAnimationMgr->createAnimate(eAniBossMoveRight)));
}

void CCEnemyBoss::update(float dt)
{
	this->checkState();

	if (enemyState == eEnemyState_active)
	{
		CCPoint currentPos = this->getPosition();
		currentPos.x += moveOffset;
		this->setPosition(currentPos);

		this->enemyCollistionH();
	}
}

void CCEnemyBoss::enemyCollistionH()
{
	CCPoint pos = this->getPosition();

	float leftCheck = pos.x - bodySize.width/2;

	if (leftCheck - leftSide <= 0.5f)
	{
		if (enemyState == eEnemyState_active)
		{
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("BossHuoQiu.ogg");
			CCPoint tempPos = ccp(pos.x - bodySize.width/4, pos.y + 3*(bodySize.height)/4);
			CCGameMap::getGameMap()->createNewBulletForBoss(tempPos, eEnemy_BossFire);
		}
	}

	if (leftCheck <= leftSide)
	{
		moveOffset *= -1;
		this->moveRight();
		return ;
	}

	float rightCheck = pos.x + bodySize.width/2;
	if (rightCheck >= rightSide)
	{
		moveOffset *= -1;
		this->moveLeft();
	}
}


// ********************** CCEnemyBossFire ****************** //
CCEnemyBossFire::CCEnemyBossFire()
{
	enemyState = eEnemyState_active;
	enemyType = eEnemy_BossFire;

	enemyBody = CCSprite::create("bossBullet.png", CCRectMake(0, 0, 24, 8));
	enemyBody->setAnchorPoint(ccp(0, 0));
	bodySize = CCSizeMake(24, 8);
	this->setContentSize(bodySize);
	this->addChild(enemyBody);
	this->setAnchorPoint(ccp(0.5f, 0.0f));

	enemyLifeOver = CCSpriteFrame::create("bossBullet.png", CCRectMake(0, 0, 24, 8));
	enemyLifeOver->retain();
	overByArrow = enemyLifeOver;
	overByArrow->retain();

	moveOffset = -3.0f;

}

CCEnemyBossFire::~CCEnemyBossFire()
{
	this->unscheduleAllSelectors();
}

void CCEnemyBossFire::launchEnemy()
{
	enemyBody->runAction(CCRepeatForever::create(sAnimationMgr->createAnimate(eAniBossFireLeft)));
	this->scheduleUpdate();
}

EnemyVSHero CCEnemyBossFire::checkCollisionWithHero()
{
	EnemyVSHero ret = eVS_nonKilled;

	CCPoint heroPos = CCHero::getHeroInstance()->getPosition();
	CCSize heroSize = CCHero::getHeroInstance()->getContentSize();
	CCRect heroRect = CCRectMake(heroPos.x - heroSize.width/2 + 2, heroPos.y + 2, 
		heroSize.width - 4, heroSize.height - 4);

	CCPoint enemyPos = this->getPosition();
	CCRect enemyRect = CCRectMake(enemyPos.x - bodySize.width/2, enemyPos.y, 
		bodySize.width, bodySize.height);

	if (heroRect.intersectsRect(enemyRect))
	{
		ret = eVS_heroKilled;
	}

	return ret;
}

void CCEnemyBossFire::forKilledByBullet()
{

}

void CCEnemyBossFire::forKilledByHero()
{

}

void CCEnemyBossFire::update(float dt)
{
	this->checkState();

	if (enemyState == eEnemyState_active)
	{
		CCPoint currentPos = this->getPosition();
		currentPos.x += moveOffset;
		this->setPosition(currentPos);
	}
}


// ********************** CCEnemyAddMushroom ****************** //
CCEnemyAddMushroom::CCEnemyAddMushroom(int _addnum)
{
	enemyType = eEnemy_AddMushroom;
	enemyState = eEnemyState_nonactive;

	enemyBody = CCSprite::create("Mushroom0.png", CCRectMake(0, 0, 16, 16));
	enemyBody->setAnchorPoint(ccp(0.0f, 0.0f));
	bodySize = CCSizeMake(16, 16);
	this->setContentSize(bodySize);
	this->addChild(enemyBody);
	this->setAnchorPoint(ccp(0.5f, 0.0f));

	enemyLifeOver = CCSpriteFrame::create("Mushroom0.png", CCRectMake(0, 0, 16, 16));
	enemyLifeOver->retain();
	overByArrow = CCSpriteFrame::create("Mushroom0.png", CCRectMake(16*3, 0, 16, 16));
	overByArrow->retain();

	addNums = _addnum;
	isAddable = true;
}

CCEnemyAddMushroom::~CCEnemyAddMushroom()
{
	this->unscheduleAllSelectors();
}

void CCEnemyAddMushroom::launchEnemy()
{
	this->scheduleUpdate();
}

EnemyVSHero CCEnemyAddMushroom::checkCollisionWithHero()
{
	EnemyVSHero ret = eVS_nonKilled;

	CCPoint heroPos = CCHero::getHeroInstance()->getPosition();
	CCSize heroSize = CCHero::getHeroInstance()->getContentSize();
	CCRect heroRect = CCRectMake(heroPos.x - heroSize.width/2 + 2, heroPos.y + 2, 
		heroSize.width - 4, heroSize.height - 4);

	CCPoint enemyPos = this->getPosition();
	CCRect enemyRect = CCRectMake(enemyPos.x - bodySize.width/2 + 2, enemyPos.y + 2, 
		bodySize.width - 4, bodySize.height - 4);

	if (heroRect.intersectsRect(enemyRect))
	{
		ret = eVS_heroKilled;
	}

	return ret;
}

void CCEnemyAddMushroom::forKilledByHero()
{

}

void CCEnemyAddMushroom::update(float dt)
{
	this->checkState();

	if (enemyState == eEnemyState_active)
	{
		if (addNums)
		{
			if (isAddable)
			{
				isAddable = false;
				this->runAction(CCSequence::create(CCMoveBy::create(0.5f, ccp(0, 16)),
					CCCallFunc::create(this, callfunc_selector(CCEnemyAddMushroom::addMushroom)), NULL));

				this->runAction(CCSequence::create(CCDelayTime::create(2.0f),
					CCCallFunc::create(this, callfunc_selector(CCEnemyAddMushroom::reSetNonAddable)), NULL));

			}
		}
		else 
		{
			enemyState = eEnemyState_over;
			enemyBody->stopAllActions();
			this->stopAllActions();
			this->unscheduleUpdate();
			this->setVisible(false);
		}
	}
}

void CCEnemyAddMushroom::addMushroom()
{
	--addNums;
	CCGameMap::getGameMap()->createNewBulletForBoss(this->getPosition(), eEnemy_mushroom);
	this->runAction(CCMoveBy::create(0.5f, ccp(0, -16)));
}

void CCEnemyAddMushroom::reSetNonAddable()
{
	isAddable = true;
}


// ********************** CCEnemyBattery ****************** //
CCEnemyBattery::CCEnemyBattery(float delay)
{
	enemyType = eEnemy_Battery;
	enemyState = eEnemyState_nonactive;

	enemyBody = CCSprite::create("battery.png");
	enemyBody->setAnchorPoint(ccp(0, 0));
	bodySize = CCSizeMake(32, 32);
	this->setContentSize(bodySize);
	this->addChild(enemyBody);
	this->setAnchorPoint(ccp(0.5f, 0.0f));

	enemyLifeOver = NULL;
	overByArrow = NULL;

	isFireable = true;

	fireDelay = delay;
}

CCEnemyBattery::~CCEnemyBattery()
{
	this->unscheduleAllSelectors();
}

void CCEnemyBattery::launchEnemy()
{
	firePos = ccp(enemyPos.x - bodySize.width/2, enemyPos.y + bodySize.height/2);

	this->scheduleUpdate();
}

void CCEnemyBattery::update(float dt)
{
	this->checkState();

	if (enemyState == eEnemyState_active)
	{
		if (isFireable)
		{
			isFireable = false;

			this->addBatteryBullet();
			
			this->runAction(CCSequence::create(CCDelayTime::create(fireDelay),
				CCCallFunc::create(this, callfunc_selector(CCEnemyBattery::reSetNonFireable)), NULL));
		}
	}
}

EnemyVSHero CCEnemyBattery::checkCollisionWithHero()
{
	EnemyVSHero ret = eVS_nonKilled;

	CCPoint heroPos = CCHero::getHeroInstance()->getPosition();
	CCSize heroSize = CCHero::getHeroInstance()->getContentSize();
	CCRect heroRect = CCRectMake(heroPos.x - heroSize.width/2 + 2, heroPos.y + 2, 
		heroSize.width - 4, heroSize.height - 4);

	CCPoint enemyPos = this->getPosition();
	CCRect enemyRect = CCRectMake(enemyPos.x - bodySize.width/2 + 2, enemyPos.y + 2, 
		bodySize.width - 4, bodySize.height - 4);

	if (heroRect.intersectsRect(enemyRect))
	{
		ret = eVS_heroKilled;
	}

	return ret;
}

void CCEnemyBattery::forKilledByBullet()
{
	// 炮台只会被火球子弹打爆
	switch (CCGlobal::getGlobalInstance()->getCurrentBulletType())
	{
	case eBullet_common:
		{
			enemyBody->runAction(CCSequence::create(sAnimationMgr->createAnimate(eAniBatteryBoom),
				CCCallFunc::create(this, callfunc_selector(CCEnemyBattery::stopAndClear)), NULL));

			break;
		}
	default:
		break;
	}
}

void CCEnemyBattery::stopAndClear()
{
	enemyState = eEnemyState_over;

	enemyBody->stopAllActions();
	this->stopAllActions();
	this->unscheduleUpdate();
	this->setVisible(false);
}

void CCEnemyBattery::forKilledByHero()
{
	// 炮台不会被主角踩死
}

void CCEnemyBattery::addBatteryBullet()
{
	CCGameMap::getGameMap()->createNewBulletForBoss(firePos, eEnemy_BatteryBullet);
}

void CCEnemyBattery::reSetNonFireable()
{
	isFireable = true;
}


// ********************** CCEnemyBatteryBullet ****************** //
CCEnemyBatteryBullet::CCEnemyBatteryBullet()
{
	enemyType = eEnemy_BatteryBullet;
	enemyState = eEnemyState_active;

	enemyBody = CCSprite::create("batteryBullet.png");
	enemyBody->setAnchorPoint(ccp(0, 0));
	bodySize = CCSizeMake(4, 4);
	this->setContentSize(bodySize);
	this->addChild(enemyBody);
	this->setAnchorPoint(ccp(0.5f, 0.5f));

	ccMoveOffset = -1.5f;
	moveOffset = ccMoveOffset;
}

CCEnemyBatteryBullet::~CCEnemyBatteryBullet()
{
	this->unscheduleAllSelectors();
}

void CCEnemyBatteryBullet::launchEnemy()
{
	this->scheduleUpdate();
}

EnemyVSHero CCEnemyBatteryBullet::checkCollisionWithHero()
{
	EnemyVSHero ret = eVS_nonKilled;

	CCPoint heroPos = CCHero::getHeroInstance()->getPosition();
	CCSize heroSize = CCHero::getHeroInstance()->getContentSize();
	CCRect heroRect = CCRectMake(heroPos.x - heroSize.width/2 + 2, heroPos.y + 2, 
		heroSize.width - 4, heroSize.height - 4);

	CCPoint enemyPos = this->getPosition();

	// 炮台发出的子弹由于太小了，就直接用主角矩形是否包含这个小白点子弹的中心为准
	if (heroRect.containsPoint(enemyPos))
	{
		ret = eVS_heroKilled;
	}

	return ret;
}

void CCEnemyBatteryBullet::forKilledByBullet()
{

}

void CCEnemyBatteryBullet::forKilledByHero()
{

}

void CCEnemyBatteryBullet::update(float dt)
{
	this->checkState();

	if (enemyState == eEnemyState_active)
	{
		CCPoint currentPos = this->getPosition();
		currentPos.x += moveOffset;
		this->setPosition(currentPos);
	}
}



// ********************** CCEnemyDarkCloud ****************** //
CCEnemyDarkCloud::CCEnemyDarkCloud(float _delay, int _type)
{
	enemyType = eEnemy_DarkCloud;
	enemyState = eEnemyState_nonactive;

	type = _type;
	switch (type)
	{
	case 0:
		enemyBody = CCSprite::create("cloud.png", CCRectMake(114, 0, 32, 24));
		normal = CCSpriteFrame::create("cloud.png", CCRectMake(114, 0, 32, 24));
		bodySize = CCSizeMake(32, 32);
		dark = CCSpriteFrame::create("darkCloud.png", CCRectMake(114, 0, 32, 24));
		break;
	case 1:
		enemyBody = CCSprite::create("cloud.png", CCRectMake(0, 0, 48, 24));
		normal = CCSpriteFrame::create("cloud.png", CCRectMake(0, 0, 48, 24));
		bodySize = CCSizeMake(64, 32);
		dark = CCSpriteFrame::create("darkCloud.png", CCRectMake(0, 0, 48, 24));
		break;
	case 2:
		enemyBody = CCSprite::create("cloud.png", CCRectMake(49, 0, 64, 24));
		normal = CCSpriteFrame::create("cloud.png", CCRectMake(49, 0, 64, 24));
		bodySize = CCSizeMake(32, 32);
		dark = CCSpriteFrame::create("darkCloud.png", CCRectMake(49, 0, 64, 24));
		break;
	default:
		break;
	}

	dark->retain();
	normal->retain();

	enemyBody->setAnchorPoint(ccp(0, 0));
	this->setContentSize(bodySize);
	this->addChild(enemyBody);
	this->setAnchorPoint(ccp(0.5f, 0.0f));

	enemyLifeOver = NULL;
	overByArrow = NULL;

	delay = _delay;
	dropRegion = 64.0f;

	isDropable = true;
}

CCEnemyDarkCloud::~CCEnemyDarkCloud()
{
	this->unscheduleAllSelectors();
}

void CCEnemyDarkCloud::launchEnemy()
{
	leftSide = enemyPos.x - dropRegion;
	rightSide = enemyPos.x + dropRegion;

	this->scheduleUpdate();
}

void CCEnemyDarkCloud::update(float dt)
{
	this->checkState();

	CCPoint heroPos = CCHero::getHeroInstance()->getPosition();

	if (enemyState == eEnemyState_active)
	{
		if (leftSide <= heroPos.x && heroPos.x <= rightSide)
		{
			if (isDropable)
			{
				isDropable = false;

				this->addLighting();

				this->runAction(CCSequence::create(CCDelayTime::create(delay),
					CCCallFunc::create(this, callfunc_selector(CCEnemyDarkCloud::reSetDropable)), NULL));
			}
		}
	}
}

void CCEnemyDarkCloud::addLighting()
{
	enemyBody->setDisplayFrame(dark);
	CCGameMap::getGameMap()->createNewBulletForBoss(enemyPos, eEnemy_Lighting);
	this->runAction(CCSequence::create(CCDelayTime::create(0.3f),
		CCCallFunc::create(this, callfunc_selector(CCEnemyDarkCloud::reSetNormal)), NULL));
}

void CCEnemyDarkCloud::reSetNormal()
{
	enemyBody->setDisplayFrame(normal);
}

void CCEnemyDarkCloud::reSetDropable()
{
	isDropable = true;
}

void CCEnemyDarkCloud::forKilledByHero()
{
	// 乌云是不能被主角和任何子弹打死的
}

void CCEnemyDarkCloud::forKilledByBullet()
{

}


// ********************** CCEnemyLighting ****************** //
CCEnemyLighting::CCEnemyLighting()
{
	enemyType = eEnemy_Lighting;
	enemyState = eEnemyState_active;

	enemyBody = CCSprite::create("lighting.png", CCRectMake(0, 0, 16, 32));
	enemyBody->setAnchorPoint(ccp(0, 0));
	bodySize = CCSizeMake(16, 32);
	this->setContentSize(bodySize);
	this->addChild(enemyBody);
	this->setAnchorPoint(ccp(0.5f, 0.0f));

	enemyLifeOver = NULL;
	overByArrow = NULL;

	ccJumpOffset = 2.0f;
	jumpOffset = -ccJumpOffset;
}

CCEnemyLighting::~CCEnemyLighting()
{
	this->unscheduleAllSelectors();
}

void CCEnemyLighting::launchEnemy()
{
	enemyBody->runAction(CCRepeatForever::create(sAnimationMgr->createAnimate(eAniLighting)));

	this->scheduleUpdate();
}

void CCEnemyLighting::update(float dt)
{
	this->checkState();

	if (enemyState == eEnemyState_active)
	{
		CCPoint currentPos = this->getPosition();
		currentPos.y += jumpOffset;
		this->setPosition(currentPos);
	}
}

void CCEnemyLighting::checkState()
{
	CCPoint pos = this->getPosition();

	// 闪电的轨迹是从天上掉下来开始，一落到底，直到地板下面消失掉
	if (pos.y <= 2)
	{
		enemyState = eEnemyState_over;
		enemyBody->stopAllActions();
		this->stopAllActions();
		this->unscheduleUpdate();
		this->setVisible(false);
	}
}

EnemyVSHero CCEnemyLighting::checkCollisionWithHero()
{
	EnemyVSHero ret = eVS_nonKilled;

	CCPoint heroPos = CCHero::getHeroInstance()->getPosition();
	CCSize heroSize = CCHero::getHeroInstance()->getContentSize();
	CCRect heroRect = CCRectMake(heroPos.x - heroSize.width/2 + 2, heroPos.y + 2, 
		heroSize.width - 4, heroSize.height - 4);

	CCPoint enemyPos = this->getPosition();
	CCRect enemyRect = CCRectMake(enemyPos.x - bodySize.width/2 + 2, enemyPos.y + 2, 
		bodySize.width - 4, bodySize.height - 4);

	if (heroRect.intersectsRect(enemyRect))
	{
		ret = eVS_heroKilled;
	}

	return ret;
}

void CCEnemyLighting::forKilledByBullet()
{
	// 闪电这种东西是不能被任何自担打掉的
}

void CCEnemyLighting::forKilledByHero()
{

}





