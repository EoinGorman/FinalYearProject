#pragma once

#include "cocos2d.h"
#include "GameData.h"
////////////////////////////////////////////////////////////
/// \brief A class that stores gameplay data
/// 
////////////////////////////////////////////////////////////


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

	void SetOwner(Player* owner);
	Player* GetOwner();

private:
	Player* m_owner;
	cocos2d::Sprite* m_sprite;
	cocos2d::Vec2 m_tile;
	cocos2d::Vec2 m_position;
	Type m_type;
};