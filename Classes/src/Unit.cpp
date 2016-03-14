#include "Unit.h"
#include "Player.h"

#define MovedTint 0.4

Unit::Unit()
{
	m_healthLabel = cocos2d::LabelTTF::create("10", "fonts/Akashi.ttf", 18, cocos2d::Size(25, 25));

	std::shared_ptr<GameData> ptr = GameData::sharedGameData();
	m_moved = false;
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

	m_healthLabel = cocos2d::LabelTTF::create("10", "fonts/Akashi.ttf", 18, cocos2d::Size(25, 25));

	m_owner = owner;
	std::shared_ptr<GameData> ptr = GameData::sharedGameData();
	m_moved = false;
	m_tile = tile;
	m_type = type;
	m_position = cocos2d::Vec2(m_tile.x * ptr->m_tileSize, m_tile.y * ptr->m_tileSize);
	SetUnitStats();
	m_sprite->setPosition(m_position);
	m_sprite->setColor(m_owner->GetColour());
}

bool Unit::MoveToward(cocos2d::Vec2 target)
{
	cocos2d::Vec2 direction = target - m_position;
	direction.normalize();

	m_position += direction * 5;
	m_sprite->setPosition(m_position);

	if (m_position.distance(target) <= direction.length() * 5)
	{
		m_position = target;
		m_sprite->setPosition(m_position);
		return true;
	}
	return false;
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

	m_sprite->addChild(m_healthLabel);
	m_healthLabel->setPosition(cocos2d::Vec2(ptr->m_tileSize * 0.85f, ptr->m_tileSize * 0.15f));
	m_baseDefence = 1.0f;
	m_attackPower = 4.0f;
	m_attackRange = 1.0f;
	m_moveRange = 4.0f;
}

void Unit::AddSpriteToScene(cocos2d::Layer* layer)
{
	layer->addChild(m_sprite, -1);
	//layer->addChild(m_healthLabel, -1);
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

void Unit::SetTileIndex(cocos2d::Vec2 index)
{
	m_tile = index;
}

cocos2d::Vec2 Unit::GetTileIndex()
{
	return m_tile;
}

void Unit::SetInSight(bool value)
{
	m_sprite->setVisible(value);
}

void Unit::StartTurn(cocos2d::Color3B colour)
{
	m_moved = false;
	SetColour(colour);
}

void Unit::EndTurn()
{
	SetColour(cocos2d::Color3B(50, 50, 50));
}

bool Unit::GetMoved()
{
	return m_moved;
}

void Unit::SetMoved(bool value)
{
	m_moved = value;
	if (value)
	{
		//If moved set tint so Unit is visibly unselectable
		cocos2d::Color3B newColour = m_owner->GetColour();

		newColour.r *= MovedTint;
		newColour.g *= MovedTint;
		newColour.b *= MovedTint;
		/*
		//Alter R value
		if (newColour.r >= MovedTint)
		{
			newColour.r -= MovedTint;
		}
		else
		{
			newColour.r = 0;
		}

		//Alter G value
		if (newColour.g >= MovedTint)
		{
			newColour.g -= MovedTint;
		}
		else
		{
			newColour.g = 0;
		}

		//Alter B value
		if (newColour.b >= MovedTint)
		{
			newColour.b -= MovedTint;
		}
		else
		{
			newColour.b = 0;
		}
		*/

		SetColour(newColour);
	}
}