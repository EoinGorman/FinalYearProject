#pragma once

#include <string>
#include <list>
#include <stdlib.h> 
#include "Leveltile.h"
#include "LevelLoader.h"

//Will be a singleton, reload level to play a different one
class Level
{
public:
	static Level* GetInstance();
	~Level()
	{
		instanceFlag = false;

		delete instance;
		for each (LevelTile* tile in m_levelTerrain)
		{
			delete tile;
		}
		for each (LevelObject* object in m_levelObjects)
		{
			delete object;
		}
	}

	void Load(std::string levelName, cocos2d::Layer* layer);
	std::vector<LevelTile*> GetTiles();
	cocos2d::Vec2 GetTileIndexPosition(LevelTile* tile);
	int Level::GetGameDistanceBetweenTiles(LevelTile* tile, LevelTile* otherTile);
	bool IsMoveableTile(Unit::Type unitType, LevelTile::Type tyleType);
	bool IsAttackableUnit(Unit::Type unitType, Unit::MovementType otherUnitType);

private:
	Level();
	static bool instanceFlag;
	static Level *instance;

	std::vector<LevelTile*> m_levelTerrain;
	std::vector<LevelObject*> m_levelObjects;
	cocos2d::Sprite* m_background;
};