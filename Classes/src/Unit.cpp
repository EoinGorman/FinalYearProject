#include "Unit.h"
#include "Player.h"

Unit::Unit()
{
	std::shared_ptr<GameData> ptr = GameData::sharedGameData();
	moved = true;
	m_tile = cocos2d::Vec2(0,0);
	m_type = soldier;
	m_position = cocos2d::Vec2(m_tile.x * ptr->m_tileSize, m_tile.y * ptr->m_tileSize);
	SetUnitStats();
	m_sprite->setPosition(m_position);
	m_sprite->setColor(cocos2d::Color3B(50, 50, 50));
	m_owner = NULL;
}

Unit::Unit(Type type, cocos2d::Vec2 tile, Player* owner)
{
	m_owner = owner;
	std::shared_ptr<GameData> ptr = GameData::sharedGameData();
	moved = true;
	m_tile = tile;
	m_type = type;
	m_position = cocos2d::Vec2(m_tile.x * ptr->m_tileSize, m_tile.y * ptr->m_tileSize);
	SetUnitStats();
	m_sprite->setPosition(m_position);
	m_sprite->setColor(m_owner->GetColour());
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
		m_sightRange = 3.0f;
		break;
	case mortarSquad:
		m_movementType = foot;
		m_sprite = cocos2d::Sprite::create("unitsBW.png", cocos2d::Rect((1)* ptr->m_tileSize, 0, ptr->m_tileSize, ptr->m_tileSize));
		m_sightRange = 2.0f;
		break;
	case smallTank:
		m_movementType = landVehicle;
		m_sprite = cocos2d::Sprite::create("unitsBW.png", cocos2d::Rect((2)* ptr->m_tileSize, 0, ptr->m_tileSize, ptr->m_tileSize));
		m_sightRange = 3.0f;
		break;
	case tBoat:
		m_movementType = seaVehicle;
		m_sprite = cocos2d::Sprite::create("unitsBW.png", cocos2d::Rect((3)* ptr->m_tileSize, 0, ptr->m_tileSize, ptr->m_tileSize));
		m_sightRange = 4.0f;
		break;
	case tCopter:
		m_movementType = airVehicle;
		m_sprite = cocos2d::Sprite::create("unitsBW.png", cocos2d::Rect((4)* ptr->m_tileSize, 0, ptr->m_tileSize, ptr->m_tileSize));
		m_sightRange = 5.0f;
		break;
	case soldier2:
		m_movementType = foot;
		m_sprite = cocos2d::Sprite::create("unitsBW.png", cocos2d::Rect((0)* ptr->m_tileSize, (1) * ptr->m_tileSize, ptr->m_tileSize, ptr->m_tileSize));
		m_sightRange = 3.0f;
		break;
	case artillery:
		m_movementType = landVehicle;
		m_sprite = cocos2d::Sprite::create("unitsBW.png", cocos2d::Rect((1)* ptr->m_tileSize, (1) * ptr->m_tileSize, ptr->m_tileSize, ptr->m_tileSize));
		m_sightRange = 3.0f;
		break;
	case largeTank:
		m_movementType = landVehicle;
		m_sprite = cocos2d::Sprite::create("unitsBW.png", cocos2d::Rect((2)* ptr->m_tileSize, (1) * ptr->m_tileSize, ptr->m_tileSize, ptr->m_tileSize));
		m_sightRange = 3.0f;
		break;
	case attackBoat:
		m_movementType = seaVehicle;
		m_sprite = cocos2d::Sprite::create("unitsBW.png", cocos2d::Rect((3)* ptr->m_tileSize, (1) * ptr->m_tileSize, ptr->m_tileSize, ptr->m_tileSize));
		m_sightRange = 4.0f;
		break;
	case attackCopter:
		m_movementType = airVehicle;
		m_sprite = cocos2d::Sprite::create("unitsBW.png", cocos2d::Rect((4)* ptr->m_tileSize, (1) * ptr->m_tileSize, ptr->m_tileSize, ptr->m_tileSize));
		m_sightRange = 5.0f;
		break;
	}

	m_baseDefence = 1.0f;
	m_attackPower = 4.0f;
	m_attackRange = 1.0f;
	m_moveRange = 1.0f;
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

Player* Unit::GetOwner()
{
	return m_owner;
}

void Unit::SetColour(cocos2d::Color3B newColour)
{
	m_sprite->setColor(newColour);
}

cocos2d::Vec2 Unit::GetTileIndex()
{
	return m_tile;
}

void Unit::SetInSight(bool value)
{
	m_sprite->setVisible(value);
}