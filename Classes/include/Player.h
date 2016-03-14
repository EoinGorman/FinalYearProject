#pragma once

#include "cocos2d.h"

class Unit;
class LevelObject;

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
	Player(int, Faction);
	int GetId();
	cocos2d::Color3B GetColour();
	std::string GetLogoName();
	void AddUnit(Unit*);
	void AddBuilding(LevelObject*);

	std::vector<Unit*> GetUnits(); 
	LevelObject* GetBase();
	std::vector<LevelObject*> GetBuildings();

	void StartTurn();
	void EndTurn();

private:
	int m_ID;
	int m_turnsTillNextBuild;
	int m_reinforcementTickets;
	std::string m_logoSpriteName;
	cocos2d::Color3B m_colour;
	Faction m_faction;
	std::vector<Unit*> m_units;
	LevelObject* m_base;
	std::vector<LevelObject*> m_buildings;
};