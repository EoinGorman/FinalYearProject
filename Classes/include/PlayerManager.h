#pragma once
#include "Player.h"

class PlayerManager {
public:
	//Functions
	static PlayerManager* GetInstance();
	~PlayerManager()
	{
		instanceFlag = false;
	}

	void ResetPlayerManager();
	void SetNumberOfPlayers(int num);
	void AddPlayer(Player::Faction);
	Player* GetCurrentPlayer();
	void CycleToNextPlayer();
	int GetPlayerCount();
	Player* GetPlayerByID(int id);
	std::vector<Player*> GetPlayers();
	void DeleteMarkedPlayers();

private:
	int m_redFactionCount;
	int m_blueFactionCount;
	int m_greenFactionCount;
	int m_yellowFactionCount;

	int m_playerCount;
	int m_currentPlayerIndex;
	std::vector<Player*> m_players;

	//Functions
	PlayerManager()
	{
		//private constructor
		m_playerCount = 0;
		m_currentPlayerIndex = 0;

		m_redFactionCount = 0;
		m_blueFactionCount = 0;
		m_greenFactionCount = 0;
		m_yellowFactionCount = 0;
	}

	static bool instanceFlag;
	static PlayerManager *instance;
};