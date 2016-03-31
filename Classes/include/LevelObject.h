#pragma once

#include "cocos2d.h"
////////////////////////////////////////////////////////////
/// \brief A class that stores gameplay data
/// 
////////////////////////////////////////////////////////////

class Player;

class LevelObject {
public:
	enum Type
	{
		HQ = 1,
		City = 2,
	};

	//Constructors
	LevelObject();
	LevelObject(LevelObject::Type type, cocos2d::Vec2 tile, Player* owner);

	void AddSpriteToScene(cocos2d::Layer* layer);
	Type GetType();
	cocos2d::Vec2 GetPosition();
	cocos2d::Vec2 GetTileIndex();

	void SetColour(cocos2d::Color3B);
	void SetOwner(Player* owner);
	Player* GetOwner();
	void SetHealth(float newHealth);
	void AlterHealth(float value);
	int GetBuildRange();
	float m_health;	//All buildings have 10 health - displayed as an int in game
	float m_defence;	//Defence of unit and tile combined

private:
	int m_buildRange;
	Player* m_owner;
	cocos2d::LabelTTF* m_healthLabel;
	cocos2d::Sprite* m_sprite;
	cocos2d::Vec2 m_tile;
	cocos2d::Vec2 m_position;
	Type m_type;
};