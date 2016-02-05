#include "Level.h"
#include "PlayerManager.h"

bool Level::instanceFlag = false;
Level* Level::instance = NULL;

Level* Level::GetInstance()
{
	if (!instanceFlag)
	{
		instance = new Level();
		instanceFlag = true;
		return instance;
	}
	else
	{
		return instance;
	}
}

Level::Level()
{
	//Empty constructor
	m_levelToLoadName = "Level1";
}

void Level::Load(cocos2d::Layer* layer)
{
	//1.	SELECT LEVEL AND THEN...	FYI: Use SetLevelToLoad() function
	//Load the data for the current level
	LevelLoader loader;
	loader.LoadLevel(m_levelToLoadName); 

	//2.	SELECT CHARACTERS AND THEN...
	PlayerManager::GetInstance()->AddPlayer(Player::Faction::red);
	PlayerManager::GetInstance()->AddPlayer(Player::Faction::green);

	//3.	LOAD ACTUAL LEVEL
	cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	std::shared_ptr<GameData> ptr = GameData::sharedGameData();

	//Add Background
	auto backgroundSprite = cocos2d::Sprite::create(ptr->m_backgroundTextureFile);
	backgroundSprite->setPosition(cocos2d::Point((visibleSize.width / 2), (visibleSize.height / 2)));
	//Set scale of background to match width of screen
	backgroundSprite->setScaleX(visibleSize.width / backgroundSprite->getContentSize().width);
	backgroundSprite->setScaleY(visibleSize.width / backgroundSprite->getContentSize().width);
	//layer->addChild(backgroundSprite, -1);

	//Create and place tiles 
	std::vector<int> terrainTypes = ptr->m_levelTerrain;
	int count = 0; 
	for (int i = 0; i < ptr->m_height; i++)
	{
		for (int j = 0; j < ptr->m_width; j++)
		{
			m_levelTerrain.push_back(new LevelTile(LevelTile::Type(terrainTypes[count]), cocos2d::Vec2(ptr->m_tileSize * j, ptr->m_tileSize * i)));
			m_levelTerrain[count]->AddSpriteToScene(layer);
			count++;
		}
	}

	//Create and place objects (buildings for now I think?)
	int hqCount = 0;
	std::vector<int> objectTypes = ptr->m_levelObjects;
	count = 0;
	for (int i = 0; i < ptr->m_height; i++)
	{
		for (int j = 0; j < ptr->m_width; j++)
		{
			if (objectTypes[count] != 0)
			{
				auto type = objectTypes[count];
				if (type == LevelObject::Type::HQ)
				{
					m_levelObjects.push_back(new LevelObject(LevelObject::Type(objectTypes[count]), cocos2d::Vec2(j, i), PlayerManager::GetInstance()->GetPlayerByID(hqCount)));
					hqCount++;
				}
				else
				{
					m_levelObjects.push_back(new LevelObject(LevelObject::Type(objectTypes[count]), cocos2d::Vec2(j, i), NULL));
				}

				m_levelObjects[m_levelObjects.size()-1]->AddSpriteToScene(layer);
				m_levelTerrain[count]->SetOccupyingObject(m_levelObjects[m_levelObjects.size() - 1]);	//Give terrain tile a reference to the object on top of it
			}
			count++;
		}
	}
}


std::vector<LevelTile*> Level::GetTiles()
{
	return m_levelTerrain;
}

cocos2d::Vec2 Level::GetTileIndexPosition(LevelTile* tile)
{
	std::shared_ptr<GameData> ptr = GameData::sharedGameData();
	return cocos2d::Vec2(tile->GetPosition().x / ptr->m_tileSize, tile->GetPosition().y / ptr->m_tileSize);
}

int Level::GetGameDistanceBetweenTiles(LevelTile* tile, LevelTile* otherTile)
{
	int distance = 0;
	cocos2d::Vec2 pos1 = GetTileIndexPosition(tile);
	cocos2d::Vec2 pos2 = GetTileIndexPosition(otherTile);

	distance += abs(pos1.x - pos2.x);
	distance += abs(pos1.y - pos2.y);

	return distance;
}

bool Level::IsMoveableTile(Unit::Type unitType, LevelTile::Type tyleType)
{
	switch (unitType)
	{
	case Unit::Type::soldier:
		if (tyleType == LevelTile::Type::Sea)
			return false;
		else
			return true;
	case Unit::Type::mortarSquad:
		if (tyleType == LevelTile::Type::Sea)
			return false;
		else
			return true;
	case Unit::Type::smallTank:
		if (tyleType == LevelTile::Type::Sea || tyleType == LevelTile::Type::Mountain || tyleType == LevelTile::Type::Jungle)
			return false;
		else
			return true;
	case Unit::Type::tBoat:
		if (tyleType == LevelTile::Type::Grass || tyleType == LevelTile::Type::Road || tyleType == LevelTile::Type::Mountain || tyleType == LevelTile::Type::Forest || tyleType == LevelTile::Type::Jungle)
			return false;
		else
			return true;
	case Unit::Type::tCopter:
		return true;
	case Unit::Type::soldier2:
		if (tyleType == LevelTile::Type::Sea)
			return false;
		else
			return true;
	case Unit::Type::artillery:
		if (tyleType == LevelTile::Type::Sea || tyleType == LevelTile::Type::Mountain || tyleType == LevelTile::Type::Jungle || tyleType == LevelTile::Type::River)
			return false;
		else
			return true;
	case Unit::Type::largeTank:
		if (tyleType == LevelTile::Type::Sea || tyleType == LevelTile::Type::Mountain || tyleType == LevelTile::Type::Jungle)
			return false;
		else
			return true;
	case Unit::Type::attackBoat:
		if (tyleType == LevelTile::Type::Grass || tyleType == LevelTile::Type::Road || tyleType == LevelTile::Type::Mountain || tyleType == LevelTile::Type::Forest || tyleType == LevelTile::Type::Jungle)
			return false;
		else
			return true;
	case Unit::Type::attackCopter:
		return true;
	}
}

bool Level::IsAttackableUnit(Unit::Type unitType, Unit::MovementType otherUnitType)
{
	switch (unitType)
	{
	case Unit::Type::soldier:
		if (otherUnitType == Unit::MovementType::airVehicle || otherUnitType == Unit::MovementType::seaVehicle)
			return false;
		else
			return true;
	case Unit::Type::mortarSquad:
		if (otherUnitType == Unit::MovementType::airVehicle)
			return false;
		else
			return true;
	case Unit::Type::smallTank:
		return true;
	case Unit::Type::tBoat:
		return false;
	case Unit::Type::tCopter:
		return false;
	case Unit::Type::soldier2:
		if (otherUnitType == Unit::MovementType::airVehicle || otherUnitType == Unit::MovementType::seaVehicle)
			return false;
		else
			return true;
	case Unit::Type::artillery:
		if (otherUnitType == Unit::MovementType::airVehicle)
			return false;
		else
			return true;
	case Unit::Type::largeTank:
		return true;
	case Unit::Type::attackBoat:
		return true;
	case Unit::Type::attackCopter:
		return true;
	}
}

void Level::SetLevelToLoad(std::string levelName)
{
	m_levelToLoadName = levelName;
}