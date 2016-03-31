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
	bool MoveToward(cocos2d::Vec2 target);

	void AddSpriteToScene(cocos2d::Layer* layer);
	Type GetType();
	cocos2d::Vec2 GetPosition();
	void SetUnitStats();
	void SetColour(cocos2d::Color3B newColour);
	void SetTileIndex(cocos2d::Vec2);
	cocos2d::Vec2 GetTileIndex();
	void SetInSight(bool value);
	Player* GetOwner();
	void StartTurn(cocos2d::Color3B colour);
	void EndTurn();
	bool GetMoved();
	void SetMoved(bool value);
	bool GetUsed();
	void SetUsed(bool value);
	float GetHealth();
	void AlterHealth(float value);
	void RemoveFromLayer();

	float m_health;	//All units have 10 health - displayed as an int in game
	float m_defence;	//Defence of unit and tile combined
	float m_attackPower;
	float m_attackRange;
	float m_moveRange;
	float m_sightRange;

private:
	Player* m_owner;
	MovementType m_movementType;
	Type m_type;

	cocos2d::LabelTTF* m_healthLabel;
	cocos2d::Sprite* m_sprite;
	cocos2d::Vec2 m_tile;
	cocos2d::Vec2 m_position;
	bool m_moved;
	bool m_used;
};