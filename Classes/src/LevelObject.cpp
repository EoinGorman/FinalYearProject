#include "LevelObject.h"
#include "GameData.h"
#include "Player.h"

LevelObject::LevelObject()
{
	m_healthLabel = cocos2d::LabelTTF::create("10", "fonts/Akashi.ttf", 18, cocos2d::Size(25, 25));
	m_health = 10.0f;
	m_defence = 0.0f;

	m_owner = 0;
	std::shared_ptr<GameData> ptr = GameData::sharedGameData();

	m_type = Type::City;
	m_buildRange = 4;
	m_position = cocos2d::Vec2(0,0);
	m_sprite = cocos2d::Sprite::create("objectsBW.png", cocos2d::Rect((m_type - 1) * ptr->m_tileSize, 0, ptr->m_tileSize, ptr->m_tileSize));
	m_sprite->setPosition(m_position);

	m_sprite->addChild(m_healthLabel);
	m_healthLabel->setPosition(cocos2d::Vec2(ptr->m_tileSize * 0.85f, ptr->m_tileSize * 0.15f));
}

LevelObject::LevelObject(Type type, cocos2d::Vec2 tile, Player* owner)
{
	m_healthLabel = cocos2d::LabelTTF::create("10", "fonts/Akashi.ttf", 18, cocos2d::Size(25, 25));
	m_health = 10.0f;
	m_defence = 0.0f;

	std::shared_ptr<GameData> ptr = GameData::sharedGameData();
	m_tile = tile;
	m_type = type;

	if (type == Type::HQ)
	{
		m_buildRange = 4;
	}
	else
	{
		m_buildRange = 1;
	}

	m_position = cocos2d::Vec2(m_tile.x * ptr->m_tileSize, m_tile.y * ptr->m_tileSize);
	m_sprite = cocos2d::Sprite::create("objectsBW.png", cocos2d::Rect(((int)m_type-1) * ptr->m_tileSize, 0, ptr->m_tileSize, ptr->m_tileSize));
	//m_sprite->setContentSize(cocos2d::Size(ptr->m_tileSize, ptr->m_tileSize));	//Not sure what this function does yet, not really needed anyway.. GOOGLE IT LATER ME!!!
	m_sprite->setPosition(m_position);

	m_sprite->addChild(m_healthLabel);
	m_healthLabel->setPosition(cocos2d::Vec2(ptr->m_tileSize * 0.85f, ptr->m_tileSize * 0.15f));

	SetOwner(owner);
}

void LevelObject::AddSpriteToScene(cocos2d::Layer* layer)
{
	layer->addChild(m_sprite, -1);
}

LevelObject::Type LevelObject::GetType()
{
	return m_type;
}

cocos2d::Vec2 LevelObject::GetPosition()
{
	return m_position;
}
void LevelObject::SetOwner(Player* owner)
{
	m_owner = owner;
	if (owner != NULL)
	{
		owner->AddBuilding(this);
	}
}

Player* LevelObject::GetOwner()
{
	return m_owner;
}

void LevelObject::SetColour(cocos2d::Color3B newColour)
{
	m_sprite->setColor(newColour);
}

cocos2d::Vec2 LevelObject::GetTileIndex()
{
	return m_tile;
}

int LevelObject::GetBuildRange()
{
	return m_buildRange;
}

void LevelObject::SetHealth(float newHealth)
{
	m_health = newHealth;
	int displayedHealth = (int)m_health;

	if (m_health > 0 && m_health < 1)
	{
		displayedHealth = 1;
	}
	m_healthLabel->setString(std::to_string(displayedHealth));
}

void LevelObject::AlterHealth(float value)
{
	m_health += value;
	int displayedHealth = (int)m_health;

	if (m_health > 0 && m_health < 1)
	{
		displayedHealth = 1;
	}
	m_healthLabel->setString(std::to_string(displayedHealth));
}