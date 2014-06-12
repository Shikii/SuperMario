#include "Bullet.h"
#include "AnimationManager.h"
#include "Hero.h"
#include "GameMap.h"
#include "GameLayer.h"

// ****************** CCBullet ******************** //
CCBullet::CCBullet():
bulletBody(NULL)
{
	CCPoint heroPos = CCHero::getHeroInstance()->getPosition();
	CCSize heroSize = CCHero::getHeroInstance()->getContentSize();
	startPos = ccp(heroPos.x, heroPos.y + heroSize.height/2);
}

CCBullet::~CCBullet()
{

}

void CCBullet::setBodySize(CCSize _size)
{
	bodySize = _size;
}
CCSize CCBullet::getBodySize()
{
	return bodySize;
}

void CCBullet::setStartPos(CCPoint pos)
{
	startPos = pos;
}
CCPoint CCBullet::getStartPos()
{
	return startPos;
}

BulletType CCBullet::getBulletType()
{
	return bulletType;
}

BulletState CCBullet::getBulletState()
{
	return bulletState;
}

void CCBullet::checkBulletState()
{

}

CCRect CCBullet::getBulletRect()
{
	CCPoint pos = this->getPosition();
	return CCRectMake(pos.x - bodySize.width/2, pos.y, bodySize.width, bodySize.height);
}

void CCBullet::forKilledEnemy()
{

}

// ****************** CCBulletCommon ******************** //
CCBulletCommon::CCBulletCommon()
{
	bulletType = eBullet_common;
	bulletState = eBulletState_active;
	isLand = false;

	bodySize = CCSizeMake(10, 10);
	bulletBody = CCSprite::create("fireRight.png", CCRectMake(0, 0, 10, 10));
	bulletBody->setAnchorPoint(ccp(0, 0));
	this->setContentSize(bodySize);
	this->addChild(bulletBody);
	this->setAnchorPoint(ccp(0.5f, 0.0f));

	moveOffset = 0.0f;
	ccMoveOffset = 5.0f;
	jumpOffset = 0.0f;
	ccJumpOffset = 0.3f;

	switch (CCHero::getHeroInstance()->face)
	{
	case eRight:
		moveOffset = ccMoveOffset;
		break;
	case eLeft:
		moveOffset = -ccMoveOffset;
		break;
	default:
		break;
	}
}

CCBulletCommon::~CCBulletCommon()
{
	this->unscheduleAllSelectors();
}

void CCBulletCommon::launchBullet()
{
	bulletBody->runAction(CCRepeatForever::create(sAnimationMgr->createAnimate(eAniRotatedFireBall)));
	
	this->scheduleUpdate();
}

// 子弹的两侧碰撞检测
// 如果两侧碰到了land，block，pipe等会自动爆炸掉
void CCBulletCommon::commonBulletCollisionH()
{
	CCPoint currentPos = this->getPosition();

	// 判断子弹是否会射出屏幕
	float leftSide = currentPos.x - bodySize.width/2;
	float rightSide = currentPos.x + bodySize.width/2;
	float mapMaxH = CCGameLayer::getMapMaxH();
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	if (fabs(leftSide - mapMaxH) <= 8)
	{
		this->showBoom();
		return ;
	}

	// 判断是否射出地图右侧
	float mapRightSide = CCGameMap::getGameMap()->mapSize.width*
		CCGameMap::getGameMap()->tileSize.width;
	if (fabs(rightSide - mapRightSide) <= 8)
	{
		this->showBoom();
		return ;
	}

	if (leftSide - mapMaxH >= winSize.width)
	{
		bulletBody->stopAllActions();
		this->unscheduleUpdate();
		this->autoClear();
	}

	// 右侧
	CCPoint rightCollision = ccp(currentPos.x + bodySize.width/2, currentPos.y + bodySize.height/2);
	CCPoint rightTilecoord = CCGameMap::getGameMap()->positionToTileCoord(rightCollision);
	TileType tileType = CCGameMap::getGameMap()->tileTypeforPos(rightTilecoord);
	switch (tileType)
	{
	case eTile_Land:
	case eTile_Pipe:
	case eTile_Block:
	case eTile_Flagpole:
		this->showBoom();
		return ;
		break;
	}

	// 左侧
	CCPoint leftCollision = ccp(currentPos.x - bodySize.width/2, currentPos.y + bodySize.height/2);
	CCPoint leftTilecoord = CCGameMap::getGameMap()->positionToTileCoord(leftCollision);
	tileType = CCGameMap::getGameMap()->tileTypeforPos(leftTilecoord);
	switch (tileType)
	{
	case eTile_Land:
	case eTile_Pipe:
	case eTile_Block:
	case eTile_Flagpole:
		this->showBoom();
		return ;
		break;
	}
}

// 因为子弹只会往下落，所以这里仅仅做脚底的判断
// 脚底如果接触到land，block，会自动反弹，但是脚底碰到pipe也会爆炸
void CCBulletCommon::commonBulletCollisionV()
{
	CCPoint currentPos = this->getPosition();

	CCPoint downCollision = currentPos;
	CCPoint downTilecoord = CCGameMap::getGameMap()->positionToTileCoord(downCollision);
	downTilecoord.y += 1;
	CCPoint downPos = CCGameMap::getGameMap()->tilecoordToPosition(downTilecoord);
	downPos = ccp(currentPos.x, downPos.y + CCGameMap::getGameMap()->getTileSize().height);

	TileType tileType = CCGameMap::getGameMap()->tileTypeforPos(downTilecoord);
	switch (tileType)
	{
	case eTile_Land:
		isLand = true;
		jumpOffset = 3.0f;
		this->setPosition(downPos);
		return ;
		break;
	case eTile_Pipe:
	case eTile_Block:
		this->showBoom();
		return ;
		break;
	case eTile_Trap:
		{
			bulletBody->stopAllActions();
			this->unscheduleUpdate();
			this->autoClear();
			return ;
			break;
		}
	default:
		break;
	}

	jumpOffset -= ccJumpOffset;
}

void CCBulletCommon::update(float dt)
{
	if (bulletState == eBulletState_active)
	{
		CCPoint currentPos = this->getPosition();
		currentPos.x += moveOffset;
		currentPos.y += jumpOffset;
		this->setPosition(currentPos);

		this->commonBulletCollisionH();
		this->commonBulletCollisionV();
	}
}

void CCBulletCommon::showBoom()
{	
	bulletBody->stopAllActions();
	this->stopAllActions();
	this->unscheduleUpdate();
	bulletBody->runAction(CCSequence::create(sAnimationMgr->createAnimate(eAniBoomedFireBall), 
		CCCallFunc::create(this, callfunc_selector(CCBulletCommon::autoClear)), NULL));
}

void CCBulletCommon::autoClear()
{	
	bulletState = eBulletState_nonactive;
	this->setVisible(false);
}

void CCBulletCommon::forKilledEnemy()
{
	this->showBoom();
}


// ****************** CCBulletArrow ******************** //
CCBulletArrow::CCBulletArrow()
{
	bulletType = eBullet_arrow;
	bulletState = eBulletState_active;

	bodySize = CCSizeMake(16, 16);
	bulletBody = CCSprite::create("arrow.png");
	bulletBody->setAnchorPoint(ccp(0, 0));
	this->setContentSize(bodySize);
	this->addChild(bulletBody);
	this->setAnchorPoint(ccp(0.5f, 0.5f));

	ccMoveOffset = 6.0f;
	CCActionInstant *flipX = CCFlipX::create(true);

	switch (CCHero::getHeroInstance()->face)
	{
	case eRight:
		moveOffset = ccMoveOffset;
		break;
	case eLeft:
		moveOffset = -ccMoveOffset;
		bulletBody->runAction(flipX);
		break;
	default:
		break;
	}

}

CCBulletArrow::~CCBulletArrow()
{
	this->unscheduleAllSelectors();
}

void CCBulletArrow::launchBullet()
{
	this->scheduleUpdate();
}

void CCBulletArrow::arrowBulletCollisionH()
{
	CCPoint currentPos = this->getPosition();
	
	// 判断子弹是否会射出屏幕
	float leftSide = currentPos.x - bodySize.width/2;
	float rightSide = currentPos.x + bodySize.width/2;
	float mapMaxH = CCGameLayer::getMapMaxH();
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	if (fabs(leftSide - mapMaxH) <= 8)
	{
		this->broken();
		return ;
	}
	// 判断是否射出地图右侧
	float mapRightSide = CCGameMap::getGameMap()->mapSize.width*
		CCGameMap::getGameMap()->tileSize.width;
	if (fabs(rightSide - mapRightSide) <= 8)
	{
		this->broken();
		return ;
	}
	
	if (leftSide - mapMaxH >= winSize.width)
	{
		bulletBody->stopAllActions();
		this->stopAllActions();
		this->unscheduleUpdate();
		this->autoClear();
	}

	// 右侧
	CCPoint rightCollision = ccp(currentPos.x + bodySize.width/2, currentPos.y);
	CCPoint rightTilecoord = CCGameMap::getGameMap()->positionToTileCoord(rightCollision);
	TileType tileType = CCGameMap::getGameMap()->tileTypeforPos(rightTilecoord);
	switch (tileType)
	{
	case eTile_Land:
	case eTile_Pipe:
	case eTile_Block:
	case eTile_Flagpole:
		this->broken();
		return ;
		break;
	}

	// 左侧
	CCPoint leftCollision = ccp(currentPos.x - bodySize.width/2, currentPos.y);
	CCPoint leftTilecoord = CCGameMap::getGameMap()->positionToTileCoord(leftCollision);
	tileType = CCGameMap::getGameMap()->tileTypeforPos(leftTilecoord);
	switch (tileType)
	{
	case eTile_Land:
	case eTile_Pipe:
	case eTile_Block:
	case eTile_Flagpole:
		this->broken();
		return ;
		break;
	}
}

void CCBulletArrow::forKilledEnemy()
{
	bulletState = eBulletState_nonactive;
	bulletBody->stopAllActions();
	this->stopAllActions();
	this->unscheduleUpdate();
	this->setVisible(false);
}

void CCBulletArrow::update(float dt)
{
	if (bulletState == eBulletState_active)
	{
		CCPoint currentPos = this->getPosition();
		currentPos.x += moveOffset;

		this->setPosition(currentPos);
		this->arrowBulletCollisionH();
	}
}

CCRect CCBulletArrow::getBulletRect()
{
	CCPoint pos = this->getPosition();
	return CCRectMake(pos.x - 6, pos.y - 5, 12, 10);
}

void CCBulletArrow::autoClear()
{
	bulletState = eBulletState_nonactive;
	this->setVisible(false);
}

void CCBulletArrow::broken()
{
	bulletBody->stopAllActions();
	this->stopAllActions();
	this->unscheduleUpdate();

	bulletBody->runAction(CCSequence::create(sAnimationMgr->createAnimate(eAniArrowBroken),
		CCCallFunc::create(this, callfunc_selector(CCBulletArrow::autoClear)), NULL));
}
