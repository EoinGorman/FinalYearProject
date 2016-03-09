#pragma once

#include "cocos2d.h"
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
	LevelTile(LevelTile::Type type, cocos2d::Vec2 position, cocos2d::Vec2 index);
	~LevelTile();

	void AddSpritesToScene(cocos2d::Layer* layer);
	Type GetType();
	cocos2d::Sprite* GetSprite();
	cocos2d::Vec2 GetPosition();
	cocos2d::Vec2 GetIndex();

	//Buildings
	bool HasObject();
	LevelObject* GetOccupyingObject();
	void RemoveOccupyingUnit();
	void SetOccupyingObject(LevelObject* levelObject);

	//Units
	bool HasUnit();
	Unit* GetOccupyingUnit(); 
	void SetOccupyingUnit(Unit* unit, cocos2d::Layer* layer);
	void BuildUnitHere(Unit* unit, cocos2d::Layer* layer);

	void ActivateAltSprite(std::string reason, bool value);
	void SetInSight(bool value);
	void SetChecked(bool value);
	bool GetChecked();
	void SetInPath(bool value);
	bool GetInPath();

	void ResetSearchVariables();
	void SetCostVariables(float costTo, float costFrom);

	void SetParent(LevelTile* parent);
	LevelTile* GetParent();
	float GetTotalCost() const;
	float GetCostToThis();
	float GetCostToGoal();
	float GetMovementCost();

private:
	cocos2d::Sprite* m_sprite;
	cocos2d::Sprite* m_altSprite;
	cocos2d::Vec2 m_position;
	cocos2d::Vec2 m_index;
	Type m_type;
	float m_movementCost;
	bool m_hasObject;
	LevelObject* m_object;

	bool m_hasUnit;
	Unit* m_unit;

	bool m_checked;
	bool m_inPath;

	LevelTile* m_parent;
	float m_totalCost;	//"F" value
	float m_costToThis;	//"G" value
	float m_costToGoal;	//"H" value
};
