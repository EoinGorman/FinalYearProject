#include "LevelTile.h"

LevelTile::LevelTile()
{
	m_hasObject = false;
	m_hasUnit = false; 
	std::shared_ptr<GameData> ptr = GameData::sharedGameData();

	m_type = Type::Grass;
	m_position = cocos2d::Vec2(0,0);
	m_sprite = cocos2d::Sprite::create("tiles.png", cocos2d::Rect(m_type * ptr->m_tileSize, 0, ptr->m_tileSize, ptr->m_tileSize));
	m_sprite->setPosition(m_position);
}

LevelTile::LevelTile(Type type, cocos2d::Vec2 position)
{
	m_hasObject = false;
	m_hasUnit = false;
	std::shared_ptr<GameData> ptr = GameData::sharedGameData();
	m_type = type;
	m_position = position;
	m_sprite = cocos2d::Sprite::create("tiles.png", cocos2d::Rect(m_type * ptr->m_tileSize, 0, ptr->m_tileSize, ptr->m_tileSize));
	//m_sprite->setContentSize(cocos2d::Size(ptr->m_tileSize, ptr->m_tileSize));	//Not sure what this function does yet, not really needed anyway.. GOOGLE IT LATER ME!!!
	m_sprite->setPosition(m_position);
}


LevelTile::~LevelTile()
{
	delete m_unit;
}

void LevelTile::AddSpriteToScene(cocos2d::Layer* layer)
{
	layer->addChild(m_sprite, -1);
}

LevelTile::Type LevelTile::GetType()
{
	return m_type;
}

cocos2d::Vec2 LevelTile::GetPosition()
{
	return m_position;
}

cocos2d::Sprite* LevelTile::GetSprite()
{
	return m_sprite;
}

//Buildings
bool LevelTile::HasObject()
{
	return m_hasObject;
}

LevelObject* LevelTile::GetOccupyingObject()
{
	if (m_hasObject)
	{
		return m_object;
	}
	return NULL;
}

void LevelTile::SetOccupyingObject(LevelObject* levelObject)
{
	m_hasObject = true;
	m_object = levelObject;
}

//Units
bool LevelTile::HasUnit()
{
	return m_hasUnit;
}

Unit* LevelTile::GetOccupyingUnit()
{
	if (m_hasUnit)
	{
		return m_unit;
	}
	return NULL;
}

void LevelTile::SetOccupyingUnit(Unit* unit, cocos2d::Layer* layer)
{
	m_hasUnit = true;
	m_unit = unit;
	m_unit->AddSpriteToScene(layer);
}
