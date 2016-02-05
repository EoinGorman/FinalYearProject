#include "Player.h"

Player::Player(int id, Player::Faction faction, Player::Colour colour)
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
		m_colour = cocos2d::Color3B::GREEN;
		m_logoSpriteName = "greenLogo.png";
		break;
	case Faction::yellow:
		m_colour = cocos2d::Color3B::YELLOW;
		m_logoSpriteName = "yellowLogo.png";
		break;
	}

	SelectAltColour(colour);
}

void Player::SelectAltColour(Colour col)
{
	switch (col)
	{
	case Player::alt1:
		break;
	case Player::alt2:
		break;
	case Player::alt3:
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