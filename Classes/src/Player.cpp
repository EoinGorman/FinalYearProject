#include "Player.h"
#include "Unit.h"
#include "LevelObject.h"
#include "cocos2d.h"
#include "GameScene.h"
#include "PlayerManager.h"

Player::Player(int id, Player::Faction faction)
{
	m_markedForDeletion = false;
	m_reinforcementTickets = 40;
	m_ID = id;
	m_faction = faction;
	m_turnsTillNextBuild = 1;
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

	//Subtract reinforcement tickets depending on unit type
	Unit::Type unitType = unit->GetType();
	switch (unitType)
	{
	case Unit::Type::soldier:
		m_reinforcementTickets -= 10;
		break;
	case Unit::Type::mortarSquad:
		m_reinforcementTickets -= 15;
		break;
	case Unit::Type::smallTank:
		m_reinforcementTickets -= 25;
		break;
	case Unit::Type::scoutBoat:
		m_reinforcementTickets -= 20;
		break;
	case Unit::Type::scoutCopter:
		m_reinforcementTickets -= 25;
		break;
	case Unit::Type::soldier2:
		m_reinforcementTickets -= 15;
		break;
	case Unit::Type::artillery:
		m_reinforcementTickets -= 30;
		break;
	case Unit::Type::largeTank:
		m_reinforcementTickets -= 35;
		break;
	case Unit::Type::attackBoat:
		m_reinforcementTickets -= 30;
		break;
	case Unit::Type::attackCopter:
		m_reinforcementTickets -= 35;
		break;
	}
}

void Player::AddBuilding(LevelObject* building)
{
	m_buildings.push_back(building);
	building->SetColour(m_colour);

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
		//m_buildings[i]->SetColour(m_colour);
		
		//for each building owned, add reinforcement tickets
		m_reinforcementTickets += 15;

		//If the building is a hq add an extra 10
		if (m_buildings[i]->GetType() == LevelObject::HQ)
		{
			m_reinforcementTickets += 10;
		}
	}

	if (m_turnsTillNextBuild <= 0)
	{
		for (int i = 0; i < m_buildings.size(); i++)
		{
			m_turnsTillNextBuild += i; //TEMP - Calculate based on number of cities owned
		}
	}
	else
	{
		m_turnsTillNextBuild--;
	}
}

void Player::EndTurn(Game* game)
{
	for (int i = 0; i < m_units.size(); i++)
	{
		m_units[i]->EndTurn(m_colour);
	}

	for (int i = 0; i < m_buildings.size(); i++)
	{
		//m_buildings[i]->SetColour(cocos2d::Color3B(50, 50, 50));
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

void Player::RemoveBuilding(LevelObject* building)
{
	if (building == m_base)
	{
		RemovePlayerFromGame();
	}
	else
	{
		m_buildings.erase(std::remove(m_buildings.begin(), m_buildings.end(), building));
	}
}

void Player::RemovePlayerFromGame()
{
	m_markedForDeletion = true;

	//destroy all units
	for (int i = 0; i < m_units.size(); i++)
	{
		Level::GetInstance()->GetTileAtIndex(m_units[i]->GetTileIndex())->RemoveOccupyingUnit();
		m_units[i]->RemoveFromLayer();
		delete m_units[i];
	}
	m_units.clear();

	//Set all Currently owned buildings to neutral
	for (int i = 0; i < m_buildings.size(); i++)
	{
		m_buildings[i]->SetColour(cocos2d::Color3B(255, 255, 255));
		m_buildings[i]->SetOwner(NULL);
	}
	m_buildings.clear();
	PlayerManager::GetInstance()->DeleteMarkedPlayers();
}

void Player::SetStartingCameraPos()
{
	int ScreenWidth = cocos2d::Director::getInstance()->getWinSizeInPixels().width;
	int ScreenHeight = cocos2d::Director::getInstance()->getWinSizeInPixels().height;
	m_lastCameraPos = -m_base->GetPosition() + cocos2d::Vec2(ScreenWidth / 2, ScreenHeight / 2);
}

int Player::GetTicketsRemaining()
{
	return m_reinforcementTickets;
}

int Player::GetTurnsTillReinforcements()
{
	return m_turnsTillNextBuild;
}