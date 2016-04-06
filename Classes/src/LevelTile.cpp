#include "LevelTile.h"

LevelTile::LevelTile()
{
	ResetSearchVariables();
	m_inPath = false;
	m_checked = false;
	m_hasObject = false;
	m_object = NULL;
	m_hasUnit = false;
	m_unit = NULL;
	std::shared_ptr<GameData> ptr = GameData::sharedGameData();

	m_type = Type::Grass;
	SetStats();
	m_movementCost = 50 * m_terrainCost;
	m_position = cocos2d::Vec2(0,0);
	m_sprite = cocos2d::Sprite::create("tiles.png", cocos2d::Rect(m_type * ptr->m_tileSize, 0, ptr->m_tileSize, ptr->m_tileSize));
	m_sprite->setPosition(m_position);
}

LevelTile::LevelTile(Type type, cocos2d::Vec2 position, cocos2d::Vec2 index)
{
	ResetSearchVariables();
	m_inPath = false;
	m_checked = false;
	m_hasObject = false;
	m_object = NULL;
	m_hasUnit = false;
	m_unit = NULL;

	std::shared_ptr<GameData> ptr = GameData::sharedGameData();
	m_type = type;
	SetStats();
	m_movementCost = 50 * m_terrainCost;
	m_position = position;
	m_index = index;
	int xOffset = m_type * ptr->m_tileSize;

	m_sprite = cocos2d::Sprite::create("tiles.png", cocos2d::Rect(xOffset, 0, ptr->m_tileSize, ptr->m_tileSize));
	m_sprite->setContentSize(cocos2d::Size(ptr->m_tileSize, ptr->m_tileSize));	//Not sure what this function does yet, not really needed anyway.. GOOGLE IT LATER ME!!!
	m_sprite->setPosition(m_position);

	m_altSprite = cocos2d::Sprite::create("altTile.png");
	m_altSprite->setPosition(m_position);
	m_altSprite->setContentSize(cocos2d::Size(ptr->m_tileSize, ptr->m_tileSize));
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

	m_defenceBonus++;

	if (m_object->GetType() == LevelObject::City)
	{
		m_defenceBonus++;
	}
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

void LevelTile::RemoveOccupyingUnit()
{
	m_hasUnit = false;
	m_unit = NULL;
}

void LevelTile::SetOccupyingUnit(Unit* unit, cocos2d::Layer* layer)
{
	m_hasUnit = true;
	m_unit = unit;
	m_unit->SetTileIndex(m_index);
}

void LevelTile::BuildUnitHere(Unit* unit, cocos2d::Layer* layer)
{
	m_hasUnit = true;
	m_unit = unit;
	m_unit->SetTileIndex(m_index);
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

void LevelTile::SetInPath(bool value)
{
	m_inPath = value;
}

bool LevelTile::GetInPath()
{
	return m_inPath;
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


void LevelTile::ResetSearchVariables()
{
	SetChecked(false);
	m_costToThis = 0;
	m_costToGoal = 0;
	m_totalCost = 0;
	m_parent = NULL;
}

void LevelTile::SetCostVariables(float costTo, float costFrom)
{
	SetChecked(true);
	m_costToThis = costTo;
	m_costToGoal = costFrom;
	m_totalCost = m_costToThis + m_costToGoal;
}

void LevelTile::SetParent(LevelTile* parent)
{
	m_parent = parent;
}

LevelTile* LevelTile::GetParent()
{
	return m_parent;
}

float LevelTile::GetTotalCost() const
{
	return m_totalCost;
}

float LevelTile::GetCostToThis()
{
	return m_costToThis;
}

float LevelTile::GetCostToGoal()
{
	return m_costToGoal;
}

float LevelTile::GetMovementCost()
{
	return m_movementCost;
}

void LevelTile::SetStats()
{
	switch (m_type)
	{
	case Road:
		m_terrainCost = 1.0f;
		m_defenceBonus = 1.0f;
		break;
	case River:
		m_terrainCost = 1.5f;
		m_defenceBonus = 1.5f;
		break;
	case Mountain:
		m_terrainCost = 2.5f;
		m_defenceBonus = 3.0f;
		break;
	case Beach:
		m_terrainCost = 1.5f;
		m_defenceBonus = 1.0f;
		break;
	case Forest:
		m_terrainCost = 2;
		m_defenceBonus = 2.0f;
		break;
	case Jungle:
		m_terrainCost = 2.5f;
		m_defenceBonus = 2.5f;
		break;
	case Grass:
		m_terrainCost = 1.25f;
		m_defenceBonus = 1.0f;
		break;
	case Sea:
		m_terrainCost = 1.0f;
		m_defenceBonus = 1.0f;
		break;
	}
}

float LevelTile::GetTerrainCost()
{
	return m_terrainCost;
}