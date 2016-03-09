#include "Player.h"
#include "Unit.h"
#include "LevelObject.h"

Player::Player(int id, Player::Faction faction)
{
	m_ID = id;
	m_faction = faction;
	m_turnsTillNextBuild = 0;
	m_reinforcementTickets = 100;

	switch (m_faction)
	{
	case Faction::red:
		m_colour = cocos2d::Color3B::RED;
		m_logoSpriteName = "redLogo.png";
		break;
	case Faction::blue:
		m_colour = cocos2d::Color3B::BLUE;
		m_logoSpriteName = "blueLogo.png";
		break;
	case Faction::green:
		m_colour = cocos2d::Color3B(0, 128, 0);
		m_logoSpriteName = "greenLogo.png";
		break;
	case Faction::yellow:
		m_colour = cocos2d::Color3B(255, 232, 0);
		m_logoSpriteName = "yellowLogo.png";
		break;
	}
}

std::string Player::GetLogoName()
{
	return m_logoSpriteName;
}

int Player::GetId()
{
	return m_ID;
}

cocos2d::Color3B Player::GetColour()
{
	return m_colour;
}

void Player::AddUnit(Unit* unit)
{
	m_units.push_back(unit);
}

void Player::AddBuilding(LevelObject* building)
{
	m_buildings.push_back(building);
}

void Player::StartTurn()
{
	for (int i = 0; i < m_units.size(); i++)
	{
		m_units[i]->StartTurn(m_colour);
	}

	for (int i = 0; i < m_buildings.size(); i++)
	{
		m_buildings[i]->SetColour(m_colour);
	}
}

void Player::EndTurn()
{
	for (int i = 0; i < m_units.size(); i++)
	{
		m_units[i]->EndTurn();
	}

	for (int i = 0; i < m_buildings.size(); i++)
	{
		m_buildings[i]->SetColour(cocos2d::Color3B(50, 50, 50));
	}
}


std::vector<Unit*> Player::GetUnits()
{
	return m_units;
}

std::vector<LevelObject*> Player::GetBuildings()
{
	return m_buildings;
}