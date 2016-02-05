#include "PlayerManager.h"

bool PlayerManager::instanceFlag = false;
PlayerManager* PlayerManager::instance = NULL;

PlayerManager* PlayerManager::GetInstance()
{
	if (!instanceFlag)
	{
		instance = new PlayerManager();
		instanceFlag = true;
		return instance;
	}
	else
	{
		return instance;
	}
}

void PlayerManager::ResetPlayerManager()
{
	if (m_playerCount > 0)
	{
		m_playerCount = 0;

		m_redFactionCount = 0;
		m_blueFactionCount = 0;
		m_greenFactionCount = 0;
		m_yellowFactionCount = 0;

		for each (Player* player in m_players)
		{
			delete player;
		}
		m_players.clear();
	}
}

void PlayerManager::SetNumberOfPlayers(int num)
{
	ResetPlayerManager();
	m_playerCount = num;
}

void PlayerManager::AddPlayer(Player::Faction faction)
{
	int playerId = m_redFactionCount + m_blueFactionCount + m_greenFactionCount + m_yellowFactionCount;
	switch (faction)
	{
	case Player::Faction::red:
		m_players.push_back(new Player(playerId, faction, Player::Colour(m_redFactionCount)));
		m_redFactionCount++;
		break;
	case Player::Faction::blue:
		m_players.push_back(new Player(playerId, faction, Player::Colour(m_blueFactionCount)));
		m_blueFactionCount++;
		break;
	case Player::Faction::green:
		m_players.push_back(new Player(playerId, faction, Player::Colour(m_greenFactionCount)));
		m_greenFactionCount++;
		break;
	case Player::Faction::yellow:
		m_players.push_back(new Player(playerId, faction, Player::Colour(m_yellowFactionCount)));
		m_yellowFactionCount++;
		break;
	}
}

Player* PlayerManager::GetPlayerByID(int id)
{
	for each(Player* player in m_players)
	{
		if (player->GetId() == id)
		{
			return player;
		}
	}
	return NULL;
}

int PlayerManager::GetPlayerCount()
{
	return m_playerCount;
}

std::vector<Player*> PlayerManager::GetPlayers()
{
	return m_players;
}