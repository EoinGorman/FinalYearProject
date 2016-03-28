#include "Player.h"
#include "Unit.h"
#include "LevelObject.h"
#include "cocos2d.h"
#include "GameScene.h"

Player::Player(int id, Player::Faction faction)
{
	m_ID = id;
	m_faction = faction;
	m_turnsTillNextBuild = 0;
	m_reinforcementTickets = 100;
	m_base = NULL;
	m_lastCameraPos = cocos2d::Vec2(0, 0);

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

	if (building->GetType() == LevelObject::HQ && m_base == NULL)
	{
		m_base = building;
	}
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

void Player::EndTurn(Game* game)
{
	for (int i = 0; i < m_units.size(); i++)
	{
		m_units[i]->EndTurn();
	}

	for (int i = 0; i < m_buildings.size(); i++)
	{
		m_buildings[i]->SetColour(cocos2d::Color3B(50, 50, 50));
	}
	
	m_lastCameraPos = game->getPosition();
}

std::vector<Unit*> Player::GetUnits()
{
	return m_units;
}

LevelObject* Player::GetBase()
{
	return m_base;
}

std::vector<LevelObject*> Player::GetBuildings()
{
	return m_buildings;
}


cocos2d::Vec2 Player::GetLastCameraPos()
{
	return m_lastCameraPos;
}

void Player::RemoveUnit(Unit* unit)
{
	m_units.erase(std::remove(m_units.begin(), m_units.end(), unit));
}

void Player::SetStartingCameraPos()
{
	int ScreenWidth = cocos2d::Director::getInstance()->getWinSizeInPixels().width;
	int ScreenHeight = cocos2d::Director::getInstance()->getWinSizeInPixels().height;
	m_lastCameraPos = -m_base->GetPosition() + cocos2d::Vec2(ScreenWidth / 2, ScreenHeight / 2);
}