#pragma once

#include "cocos2d.h"

class Player {
public:
	enum Faction
	{
		red,
		blue,
		green,
		yellow
	};

	enum Colour
	{
		original,
		alt1,
		alt2,
		alt3
	};

	//Constructors
	Player(int, Faction, Colour);
	int GetId();
	cocos2d::Color3B GetColour();
	std::string GetLogoName();

private:
	void SelectAltColour(Colour col);
	int m_ID;
	int m_turnsTillNextBuild;
	int m_reinforcementTickets;
	std::string m_logoSpriteName;
	cocos2d::Color3B m_colour;
	Faction m_faction;
};