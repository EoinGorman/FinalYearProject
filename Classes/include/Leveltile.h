#pragma once

#include "cocos2d.h"
#include "GameData.h"
#include "LevelObject.h"
#include "Unit.h"

class LevelTile {
public:
	enum Type
	{
		Grass = 0,
		Road = 1,
		River = 2,
		Mountain = 3,
		Sea = 4,
		Beach = 5,
		Forest = 6,
		Jungle = 7
	};

	//Constructors
	LevelTile();
	LevelTile(LevelTile::Type type, cocos2d::Vec2 position);
	~LevelTile();

	void AddSpriteToScene(cocos2d::Layer* layer);
	Type GetType();
	cocos2d::Vec2 GetPosition();
	cocos2d::Sprite* GetSprite();

	//Buildings
	bool HasObject();
	LevelObject* GetOccupyingObject();
	void SetOccupyingObject(LevelObject* levelObject);

	//Units
	bool HasUnit();
	Unit* GetOccupyingUnit();
	void SetOccupyingUnit(Unit* unit, cocos2d::Layer* layer);

private:
	cocos2d::Sprite* m_sprite;
	cocos2d::Vec2 m_position;
	Type m_type;
	bool m_hasObject;
	LevelObject* m_object;

	bool m_hasUnit;
	Unit* m_unit;
};
