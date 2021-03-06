#pragma once

#include <string>
#include <list>
#include <stdlib.h> 
#include "Leveltile.h"
#include "LevelLoader.h"
#include "Player.h"

//Will be a singleton, reload level to play a different one
class Level
{
public:
	static Level* GetInstance();
	~Level()
	{
		instanceFlag = false;
		for each (LevelTile* tile in m_levelTerrain)
		{
			delete tile;
		}
		for each (LevelObject* object in m_levelObjects)
		{
			delete object;
		}
	}

	void Load(cocos2d::Layer* layer);
	std::vector<LevelTile*> GetTiles();
	cocos2d::Vec2 GetTileIndexPosition(LevelTile* tile);
	int Level::GetGameDistanceBetweenTiles(LevelTile* tile, LevelTile* otherTile);
	bool IsMoveableTile(Unit::Type unitType, LevelTile::Type tyleType);
	bool IsAttackableUnit(Unit::Type unitType, Unit::MovementType otherUnitType);
	void SetLevelToLoad(std::string levelName);
	void SetFactionsChosen(std::vector<Player::Faction>);
	void SetNumberOfHQs(int amount);
	int GetNumberOfHQs();
	std::vector<LevelTile*> GetNeighbourTiles(LevelTile*);
	LevelTile* GetTileAtIndex(cocos2d::Vec2 index);

	std::vector<LevelTile*> GetPath(LevelTile* start, LevelTile* goal, std::list<LevelTile*> availableTiles);
private:
	Level();
	static bool instanceFlag;
	static Level *instance;

	int m_numberOfHQs;
	std::string m_levelToLoadName;
	std::vector<Player::Faction> m_factionsChosen;
	std::vector<LevelTile*> m_levelTerrain;
	std::vector<LevelObject*> m_levelObjects;
	cocos2d::Sprite* m_background;

	void AddToPath(std::vector<LevelTile*> path);
	void RemoveFromPath(std::vector<LevelTile*> path);
	float GetManhattanDistance(LevelTile* start, LevelTile* goal);
	float GetDistanceSoFar(LevelTile*);
};