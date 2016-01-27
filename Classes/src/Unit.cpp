#include "Unit.h"
#include "Leveltile.h"

Unit::Unit()
{
	std::shared_ptr<GameData> ptr = GameData::sharedGameData();
	moved = true;
	m_tile = cocos2d::Vec2(0,0);
	m_type = soldier;
	m_position = cocos2d::Vec2(m_tile.x * ptr->m_tileSize, m_tile.y * ptr->m_tileSize);
	SetUnitStats();
	m_sprite->setPosition(m_position);
	m_owner = 0;
}

Unit::Unit(Type type, cocos2d::Vec2 tile, int player)
{
	m_owner = player;
	std::shared_ptr<GameData> ptr = GameData::sharedGameData();
	moved = true;
	m_tile = tile;
	m_type = type;
	m_position = cocos2d::Vec2(m_tile.x * ptr->m_tileSize, m_tile.y * ptr->m_tileSize);
	SetUnitStats();
	m_sprite->setPosition(m_position);

	if (m_owner == 1)
	{
		m_sprite->setColor(cocos2d::Color3B(255, 0, 0));
	}

	else if (m_owner == 2)
	{
		m_sprite->setColor(cocos2d::Color3B(0, 0, 255));
	}
}
 

void Unit::SetUnitStats()
{
	std::shared_ptr<GameData> ptr = GameData::sharedGameData();
	m_health = 10.0f;

	//MOVE TO LEVEL CLASS 
	switch (m_type)
	{
	case soldier:
		m_movementType = foot;
		m_sprite = cocos2d::Sprite::create("unitsBW.png", cocos2d::Rect((0)* ptr->m_tileSize, 0, ptr->m_tileSize, ptr->m_tileSize));
		break;
	case mortarSquad:
		m_movementType = foot;
		m_sprite = cocos2d::Sprite::create("unitsBW.png", cocos2d::Rect((1)* ptr->m_tileSize, 0, ptr->m_tileSize, ptr->m_tileSize));
		break;
	case smallTank:
		m_movementType = landVehicle;
		m_sprite = cocos2d::Sprite::create("unitsBW.png", cocos2d::Rect((2)* ptr->m_tileSize, 0, ptr->m_tileSize, ptr->m_tileSize));
		break;
	case tBoat:
		m_movementType = seaVehicle;
		m_sprite = cocos2d::Sprite::create("unitsBW.png", cocos2d::Rect((3)* ptr->m_tileSize, 0, ptr->m_tileSize, ptr->m_tileSize));
		break;
	case tCopter:
		m_movementType = airVehicle;
		m_sprite = cocos2d::Sprite::create("unitsBW.png", cocos2d::Rect((4)* ptr->m_tileSize, 0, ptr->m_tileSize, ptr->m_tileSize));
		break;
	case soldier2:
		m_movementType = foot;
		m_sprite = cocos2d::Sprite::create("unitsBW.png", cocos2d::Rect((0)* ptr->m_tileSize, (1) * ptr->m_tileSize, ptr->m_tileSize, ptr->m_tileSize));
		break;
	case artillery:
		m_movementType = landVehicle;
		m_sprite = cocos2d::Sprite::create("unitsBW.png", cocos2d::Rect((1)* ptr->m_tileSize, (1) * ptr->m_tileSize, ptr->m_tileSize, ptr->m_tileSize));
		break;
	case largeTank:
		m_movementType = landVehicle;
		m_sprite = cocos2d::Sprite::create("unitsBW.png", cocos2d::Rect((2)* ptr->m_tileSize, (1) * ptr->m_tileSize, ptr->m_tileSize, ptr->m_tileSize));
		break;
	case attackBoat:
		m_movementType = seaVehicle;
		m_sprite = cocos2d::Sprite::create("unitsBW.png", cocos2d::Rect((3)* ptr->m_tileSize, (1) * ptr->m_tileSize, ptr->m_tileSize, ptr->m_tileSize));
		break;
	case attackCopter:
		m_movementType = airVehicle;
		m_sprite = cocos2d::Sprite::create("unitsBW.png", cocos2d::Rect((4)* ptr->m_tileSize, (1) * ptr->m_tileSize, ptr->m_tileSize, ptr->m_tileSize));
		break;
	}

	m_baseDefence = 1.0f;
	m_attackPower = 4.0f;
	m_attackRange = 1.0f;
	m_moveRange = 4.0f;
	m_sightRange = 3.0f;
}

void Unit::AddSpriteToScene(cocos2d::Layer* layer)
{
	layer->addChild(m_sprite, -1);
}

Unit::Type Unit::GetType()
{
	return m_type;
}

cocos2d::Vec2 Unit::GetPosition()
{
	return m_position;
}

int Unit::GetOwner()
{
	return m_owner;
}