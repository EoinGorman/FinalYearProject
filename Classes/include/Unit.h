#pragma once

#include "cocos2d.h"
#include "GameData.h"

class Player;

class Unit {
public:
	enum MovementType
	{
		foot = 0,
		landVehicle = 1,
		seaVehicle = 2,
		airVehicle = 3
	};

	enum Type
	{
		soldier = 0,
		mortarSquad = 1,
		smallTank = 2,
		tBoat = 3,
		tCopter = 4,

		soldier2 = 5,
		artillery = 6,
		largeTank = 7,
		attackBoat = 8,
		attackCopter = 9,
	};

	//Constructors
	Unit();
	Unit(Unit::Type type, cocos2d::Vec2 tile, Player* owner);

	void AddSpriteToScene(cocos2d::Layer* layer);
	Type GetType();
	cocos2d::Vec2 GetPosition();
	void SetUnitStats();
	void SetColour(cocos2d::Color3B newColour);
	cocos2d::Vec2 GetTileIndex();
	void SetInSight(bool value);
	Player* GetOwner();

	float m_health;	//All units have 10 health - displayed as an int in game
	float m_baseDefence;	//Defence of unit type
	float m_defence;	//Defence of unit and tile combined
	float m_attackPower;
	float m_attackRange;
	float m_moveRange;
	float m_sightRange;
	bool moved;

private:
	Player* m_owner;
	MovementType m_movementType;
	Type m_type;

	cocos2d::Sprite* m_sprite;
	cocos2d::Vec2 m_tile;
	cocos2d::Vec2 m_position;
};