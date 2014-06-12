#include "Global.h"

CCGlobal* CCGlobal::_globalInstance;
CCGlobal* CCGlobal::getGlobalInstance()
{
	if (_globalInstance == 0)
	{
		_globalInstance = new CCGlobal();
	}
	return _globalInstance;
}

CCGlobal::CCGlobal()
{
	currentLevel = 1;

	totalLevels = 8;

	//whyToMainMenu = efor_StartGame;

	currentHeroType = eBody_Small;

	currentBulletType = eBullet_common;

	lifeNum = 3;
}

CCGlobal::~CCGlobal()
{

}

void CCGlobal::currentLevelPlusOne()
{
	++currentLevel;
}

void CCGlobal::reSetLevel()
{
	currentLevel = 1;
}

int CCGlobal::getCurrentLevel()
{
	return currentLevel;
}

int CCGlobal::getTotalLevels()
{
	return totalLevels;
}

//void CCGlobal::setWhyToMainMenu(ToMainMenuFor why)
//{
//	whyToMainMenu = why;
//}
//
//ToMainMenuFor CCGlobal::getForToMainMenu()
//{
//	return whyToMainMenu;
//}

void CCGlobal::setCurrentHeroType(BodyType _type)
{
	currentHeroType = _type;
}

BodyType CCGlobal::getCurrentHeroType()
{
	return currentHeroType;
}

void CCGlobal::setCurrentBulletType(BulletType _type)
{
	currentBulletType = _type;
}

BulletType CCGlobal::getCurrentBulletType()
{
	return currentBulletType;
}

void CCGlobal::lifeNumPlusOne()
{
	++lifeNum;
}

void CCGlobal::lifeNumCutOne()
{
	--lifeNum;
}

void CCGlobal::setCurrentLifeNum(int num)
{
	lifeNum = num;
}

int CCGlobal::getCurrentLifeNum()
{
	return lifeNum;
}

void CCGlobal::setCurrentLevel( int level )
{
	currentLevel = level;
}
