#include "LevelTile.h"
#include "Level.h"

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
	m_position = cocos2d::Vec2(0, 0);
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
		m_sprite->setColor(cocos2d::Color3B(175, 175, 175));
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

void LevelTile::ChooseCorrectSprite()
{
	std::shared_ptr<GameData> ptr = GameData::sharedGameData();
	int xOffset = 0;
	int yOffset = 0;

	std::vector<LevelTile*> neighbours = Level::GetInstance()->GetNeighbourTiles(this);
	bool roadToLeft = false;
	bool roadToRight = false;
	bool roadAbove = false;
	bool roadBelow = false;
	int surroundingRoad = 0;

	bool waterToLeft = false;
	bool waterToRight = false;
	bool waterAbove = false;
	bool waterBelow = false;
	int surroundingWater = 0;

	bool riverToLeft = false;
	bool riverToRight = false;
	bool riverAbove = false;
	bool riverBelow = false;
	int surroundingRiver = 0;

	std::string animationPath;
	cocos2d::Vector<cocos2d::SpriteFrame*> animFrames(6);
	cocos2d::Animation* animation;
	cocos2d::Animate* animate;
	int numberOfFrames = 6;

	switch (m_type)
	{
	case Road:
		xOffset = 0;
		yOffset = 0;

		//Check tile to left
		if (neighbours[0] != NULL)
		{
			if (neighbours[0]->GetType() == River || neighbours[0]->GetType() == Sea)
			{
				waterToLeft = true;
			}
			else if (neighbours[0]->GetType() == Road)
			{
				roadToLeft = true;
			}
		}

		//Check tile to right
		if (neighbours[1] != NULL)
		{
			if (neighbours[1]->GetType() == River || neighbours[1]->GetType() == Sea)
			{
				waterToRight = true;
			}
			else if (neighbours[1]->GetType() == Road)
			{
				roadToRight = true;
			}
		}

		//Check tile above
		if (neighbours[2] != NULL)
		{
			if (neighbours[2]->GetType() == River || neighbours[2]->GetType() == Sea)
			{
				waterAbove = true;
			}
			else if (neighbours[2]->GetType() == Road)
			{
				roadAbove = true;
			}
		}

		//Check tile below
		if (neighbours[3] != NULL)
		{
			if (neighbours[3]->GetType() == River || neighbours[3]->GetType() == Sea)
			{
				waterBelow = true;
			}
			else if (neighbours[3]->GetType() == Road)
			{
				roadBelow = true;
			}
		}

		//Check if it's a bridge
		if ((waterAbove && waterBelow) || (waterToLeft && waterToRight))
		{
			if (waterAbove && waterBelow)
			{
				//horizontal bridge
				xOffset = 0 * 50;
			}
			else
			{
				//Vertical bridge
				xOffset = 1 * 50;
			}
		}

		//if not
		else
		{
			surroundingRoad = (int)roadToLeft + (int)roadToRight + (int)roadAbove + (int)roadBelow;

			if (surroundingRoad == 4)
			{
				xOffset = 9 * 50;
			}
			else if (surroundingRoad == 3)
			{
				if (roadAbove && roadToLeft && roadToRight)
				{
					xOffset = 10 * 50;
				}
				else if (roadAbove && roadToRight && roadBelow)
				{
					xOffset = 4 * 50;
				}
				else if (roadAbove && roadToLeft && roadBelow)
				{
					xOffset = 5 * 50;
				}
				else if (roadBelow && roadToLeft && roadToRight)
				{
					xOffset = 11 * 50;
				}
			}
			else if (surroundingRoad == 2)
			{
				if (roadToLeft && roadAbove)
				{
					xOffset = 7 * 50;
				}
				else if (roadToLeft && roadBelow)
				{
					xOffset = 13 * 50;
				}
				else if (roadToRight && roadAbove)
				{
					xOffset = 6 * 50;
				}
				else if (roadToRight && roadBelow)
				{
					xOffset = 12 * 50;
				}

				else if (roadAbove || roadBelow)
				{
					xOffset = 2 * 50;
				}
				else
				{
					xOffset = 3 * 50;
				}

			}
			else if (surroundingRoad == 1)
			{
				if (roadAbove || roadBelow)
				{
					xOffset = 2 * 50;
				}
				else
				{
					xOffset = 3 * 50;
				}
			}
			else
			{
				xOffset = 9 * 50;
			}
		}

		m_sprite = cocos2d::Sprite::create("staticTiles.png", cocos2d::Rect(xOffset, yOffset, ptr->m_tileSize, ptr->m_tileSize));

		break;
	case River:
		xOffset = m_type * ptr->m_tileSize;
		m_sprite = cocos2d::Sprite::create("tiles.png", cocos2d::Rect(xOffset, 0, ptr->m_tileSize, ptr->m_tileSize));

		animationPath = "";

		//Check tile to left
		if (neighbours[0] != NULL)
		{
			if (neighbours[0]->GetType() == River || neighbours[0]->GetType() == Sea)
			{
				waterToLeft = true;
			}
			else if (neighbours[0]->GetType() == Road)
			{
				roadToLeft = true;
			}
		}

		//Check tile to right
		if (neighbours[1] != NULL)
		{
			if (neighbours[1]->GetType() == River)
			{
				waterToRight = true;
			}
		}

		//Check tile above
		if (neighbours[2] != NULL)
		{
			if (neighbours[2]->GetType() == River)
			{
				waterAbove = true;
			}
		}

		//Check tile below
		if (neighbours[3] != NULL)
		{
			if (neighbours[3]->GetType() == River)
			{
				waterBelow = true;
			}
		}

		surroundingWater = (int)waterToLeft + (int)waterToRight + (int)waterAbove + (int)waterBelow;
		
		if (surroundingWater == 3)
		{
			if (waterAbove && waterToLeft && waterToRight)
			{
				animationPath = "RiverAnimations/leftRightDownRiverSprites.png";
			}
			else if (waterAbove && waterToRight && waterBelow)
			{
				animationPath = "RiverAnimations/upRightDownRiverSprites.png";
			}
			else if (waterAbove && waterToLeft && waterBelow)
			{
				animationPath = "RiverAnimations/leftUpDownRiverSprites.png";
			}
			else if (waterBelow && waterToLeft && waterToRight)
			{
				animationPath = "RiverAnimations/leftUpRightRiverSprites.png";
			}
		}
		else if (surroundingWater == 2)
		{
			if (waterToLeft && waterAbove)
			{
				animationPath = "RiverAnimations/leftDownRiverSprites.png";
			}
			else if (waterToLeft && waterBelow)
			{
				animationPath = "RiverAnimations/leftUpRiverSprites.png";
			}
			else if (waterToRight && waterAbove)
			{
				animationPath = "RiverAnimations/rightDownRiverSprites.png";
			}
			else if (waterToRight && waterBelow)
			{
				animationPath = "RiverAnimations/upRightRiverSprites.png";
			}

			else if (waterAbove || waterBelow)
			{
				animationPath = "RiverAnimations/upDownRiverSprites.png";
			}
			else
			{
				animationPath = "RiverAnimations/leftRightRiverSprites.png";
			}

		}
		else if (surroundingWater == 1)
		{
			if (waterAbove || waterBelow)
			{
				animationPath = "RiverAnimations/upDownRiverSprites.png";
			}
			else
			{
				animationPath = "RiverAnimations/leftRightRiverSprites.png";
			}
		}
		else
		{
			animationPath = "RiverAnimations/upDownRiverSprites.png";
		}

		//Create and run animation
		for (int i = 0; i < 6; i++)
		{
			auto frame = cocos2d::SpriteFrame::create(animationPath, cocos2d::Rect(i * ptr->m_tileSize, 0, ptr->m_tileSize, ptr->m_tileSize));
			animFrames.pushBack(frame);
		}
		animation = cocos2d::Animation::createWithSpriteFrames(animFrames, 0.15f, INFINITE);
		animate = cocos2d::Animate::create(animation);
		m_sprite->runAction(animate);

		break;
	case Mountain:
		yOffset = 50;
		xOffset = 50;
		m_sprite = cocos2d::Sprite::create("staticTiles.png", cocos2d::Rect(xOffset, yOffset, ptr->m_tileSize, ptr->m_tileSize));
		break;
	case Beach:
		xOffset = m_type * ptr->m_tileSize;
		m_sprite = cocos2d::Sprite::create("tiles.png", cocos2d::Rect(xOffset, 0, ptr->m_tileSize, ptr->m_tileSize));
		break;
	case Forest:
		xOffset = 0;
		yOffset = 50;
		m_sprite = cocos2d::Sprite::create("staticTiles.png", cocos2d::Rect(xOffset, yOffset, ptr->m_tileSize, ptr->m_tileSize));
		break;
	case Jungle:
		xOffset = m_type * ptr->m_tileSize;
		m_sprite = cocos2d::Sprite::create("tiles.png", cocos2d::Rect(xOffset, 0, ptr->m_tileSize, ptr->m_tileSize));
		break;
	case Grass:
		xOffset = 100;
		yOffset = 50;
		m_sprite = cocos2d::Sprite::create("staticTiles.png", cocos2d::Rect(xOffset, yOffset, ptr->m_tileSize, ptr->m_tileSize));
		break;
	case Sea:
		xOffset = m_type * ptr->m_tileSize;
		m_sprite = cocos2d::Sprite::create("tiles.png", cocos2d::Rect(xOffset, 0, ptr->m_tileSize, ptr->m_tileSize));

		//Check tile to left
		if (neighbours[0] != NULL)
		{
			if (neighbours[0]->GetType() == River || neighbours[0]->GetType() == Sea)
			{
				waterToLeft = true;
				if (neighbours[0]->GetType() == River)
				{
					riverToLeft = true;
				}
			}
		}

		//Check tile to right
		if (neighbours[1] != NULL)
		{
			if (neighbours[1]->GetType() == River || neighbours[1]->GetType() == Sea)
			{
				waterToRight = true;
				if (neighbours[1]->GetType() == River)
				{
					riverToRight = true;
				}
			}
		}

		//Check tile above
		if (neighbours[2] != NULL)
		{
			if (neighbours[2]->GetType() == River || neighbours[2]->GetType() == Sea)
			{
				waterAbove = true;
				if (neighbours[2]->GetType() == River)
				{
					riverAbove = true;
				}
			}
		}

		//Check tile below
		if (neighbours[3] != NULL)
		{
			if (neighbours[3]->GetType() == River || neighbours[3]->GetType() == Sea)
			{
				waterBelow = true;
				if (neighbours[3]->GetType() == River)
				{
					riverBelow = true;
				}
			}
		}

		surroundingRiver = (int)riverToLeft + (int)riverAbove + (int)riverToRight + (int)riverBelow;
		if (surroundingRiver > 0)
		{
			numberOfFrames = 6;
			if (riverToLeft)
			{
				animationPath = "SeaAnimations/riverToLeft.png";
			}
			else if (riverAbove)
			{
				animationPath = "SeaAnimations/riverAbove.png";
			}
			else if (riverToRight)
			{
				animationPath = "SeaAnimations/riverToRight.png";
			}
			else
			{
				animationPath = "SeaAnimations/riverBelow.png";
			}
		}
		else
		{
			numberOfFrames = 4;
			//Check what type of sea tile it is
			surroundingWater = (int)waterToLeft + (int)waterToRight + (int)waterAbove + (int)waterBelow;

			if (surroundingWater == 3)
			{
				//check which side the land is on
				if (!waterToLeft)
				{
					animationPath = "SeaAnimations/landToLeft.png";
				}
				else if (!waterAbove)
				{
					animationPath = "SeaAnimations/landDown.png";
				}
				else if (!waterToRight)
				{
					animationPath = "SeaAnimations/landToRight.png";
				}
				else
				{
					animationPath = "SeaAnimations/landUp.png";
				}
				
			}
			else if (surroundingWater == 2)
			{
				//check which corner it's in
				if (waterToLeft && waterAbove)
				{
					animationPath = "SeaAnimations/waterLeftDown.png";
				}
				else if (waterAbove && waterToRight)
				{
					animationPath = "SeaAnimations/waterRightDown.png";
				}
				else if (waterToRight && waterBelow)
				{
					animationPath = "SeaAnimations/waterUpRight.png";
				}
				else if (waterBelow && waterToLeft)
				{
					animationPath = "SeaAnimations/waterUpLeft.png";
				}
			}
			else
			{
				animationPath = "SeaAnimations/surrounded.png";
			}
		}

		//Create and run animation
		for (int i = 0; i < numberOfFrames; i++)
		{
			auto frame = cocos2d::SpriteFrame::create(animationPath, cocos2d::Rect(i * ptr->m_tileSize, 0, ptr->m_tileSize, ptr->m_tileSize));
			animFrames.pushBack(frame);
		}
		animation = cocos2d::Animation::createWithSpriteFrames(animFrames, 0.15f, INFINITE);
		animate = cocos2d::Animate::create(animation);
		m_sprite->runAction(animate);
		break;
	}

	m_sprite->setContentSize(cocos2d::Size(ptr->m_tileSize, ptr->m_tileSize));	//Not sure what this function does yet, not really needed anyway.. GOOGLE IT LATER ME!!!
	m_sprite->setPosition(m_position);
}