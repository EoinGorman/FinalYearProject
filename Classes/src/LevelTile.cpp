#include "LevelTile.h"

LevelTile::LevelTile()
{
	m_checked = false;
	m_hasObject = false;
	m_object = NULL;
	m_hasUnit = false;
	m_unit = NULL;
	std::shared_ptr<GameData> ptr = GameData::sharedGameData();

	m_type = Type::Grass;
	m_position = cocos2d::Vec2(0,0);
	m_sprite = cocos2d::Sprite::create("tiles.png", cocos2d::Rect(m_type * ptr->m_tileSize, 0, ptr->m_tileSize, ptr->m_tileSize));
	m_sprite->setPosition(m_position);
}

LevelTile::LevelTile(Type type, cocos2d::Vec2 position, cocos2d::Vec2 index)
{
	m_checked = false;
	m_hasObject = false;
	m_object = NULL;
	m_hasUnit = false;
	m_unit = NULL;
	std::shared_ptr<GameData> ptr = GameData::sharedGameData();
	m_type = type;
	m_position = position;
	m_index = index;
	int xOffset = m_type * ptr->m_tileSize;

	//m_terrainTexture = cocos2d::Sprite::create("tiles.png", cocos2d::Rect(xOffset, 0, ptr->m_tileSize, ptr->m_tileSize))->getTexture();
	//m_altTexture = cocos2d::Sprite::create("altTile.png")->getTexture();

	m_sprite = cocos2d::Sprite::create("tiles.png", cocos2d::Rect(xOffset, 0, ptr->m_tileSize, ptr->m_tileSize));
	m_sprite->setContentSize(cocos2d::Size(ptr->m_tileSize, ptr->m_tileSize));	//Not sure what this function does yet, not really needed anyway.. GOOGLE IT LATER ME!!!
	m_sprite->setPosition(m_position);

	m_altSprite = cocos2d::Sprite::create("altTile.png");
	m_altSprite->setPosition(m_position);
	m_altSprite->setContentSize(cocos2d::Size(ptr->m_tileSize, ptr->m_tileSize));
	//m_altSprite->setColor(cocos2d::Color4B(255, 255, 255, 100));
}


LevelTile::~LevelTile()
{
	if (m_unit != NULL)
	delete m_unit;
}

void LevelTile::AddSpritesToScene(cocos2d::Layer* layer)
{
	layer->addChild(m_sprite, -1);
	layer->addChild(m_altSprite, -1);
	m_altSprite->setVisible(false);
}

LevelTile::Type LevelTile::GetType()
{
	return m_type;
}

cocos2d::Sprite* LevelTile::GetSprite()
{
	return m_sprite;
}

cocos2d::Vec2 LevelTile::GetPosition()
{
	return m_position;
}

cocos2d::Vec2 LevelTile::GetIndex()
{
	return m_index;
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

void LevelTile::SetChecked(bool value)
{
	m_checked = value;
}

bool LevelTile::GetChecked()
{
	return m_checked;
}

void LevelTile::SetInSight(bool value)
{
	if (value)
	{
		m_sprite->setColor(cocos2d::Color3B::WHITE);
	}
	else
	{
		m_sprite->setColor(cocos2d::Color3B(85, 85, 85));
	}
	if (m_hasUnit)
	{
		m_unit->SetInSight(value);
	}
}

void LevelTile::ActivateAltSprite(std::string reason, bool value)
{
	//m_sprite->setVisible(!value);
	m_altSprite->setVisible(value);
	if (value)
	{
		if (reason == "Attacking")
		{
			m_altSprite->setColor(cocos2d::Color3B::RED);
		}
		else if (reason == "Moving" || reason == "Spawning")
		{
			m_altSprite->setColor(cocos2d::Color3B::BLUE);
		}
		else
		{
			m_altSprite->setColor(cocos2d::Color3B::WHITE);
		}
	}
	else
	{
		m_altSprite->setColor(cocos2d::Color3B::WHITE);
	}
}