#pragma once

#include "cocos2d.h"

class Unit;
class LevelObject;
class Game;

class Player {
public:
	enum Faction
	{
		red,
		blue,
		green,
		yellow
	};

	//Constructors
	bool m_markedForDeletion;

	Player(int, Faction);
	int GetId();
	cocos2d::Color3B GetColour();
	std::string GetLogoName();
	void AddUnit(Unit*);
	void AddBuilding(LevelObject*);

	std::vector<Unit*> GetUnits(); 
	LevelObject* GetBase();
	std::vector<LevelObject*> GetBuildings();
	void RemoveUnit(Unit* unit);
	void RemoveBuilding(LevelObject* building);
	void RemovePlayerFromGame();
	cocos2d::Vec2 GetLastCameraPos();
	void SetStartingCameraPos();

	void StartTurn();
	void EndTurn(Game*);

	int GetTicketsRemaining();
	int GetTurnsTillReinforcements();

private:
	int m_ID;
	int m_turnsTillNextBuild;
	int m_reinforcementTickets;
	cocos2d::Vec2 m_lastCameraPos;
	std::string m_logoSpriteName;
	cocos2d::Color3B m_colour;
	Faction m_faction;
	std::vector<Unit*> m_units;
	LevelObject* m_base;
	std::vector<LevelObject*> m_buildings;
};