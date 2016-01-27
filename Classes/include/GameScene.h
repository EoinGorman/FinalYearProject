#pragma once

#include "cocos2d.h"
#include "PauseScene.h"
#include "HudLayer.h"
#include "Level.h"

class Game : public cocos2d::Layer
{
public:
	cocos2d::Vec2 cameraDirection;
	//HudLayer* hud;

	//Game logic variables
	int ScreenWidth;
	int ScreenHeight;

	int m_currentPlayer;
	enum TurnStage
	{
		Waiting,
		ChoosingSpawn,
		ChoosingMove,
		ChoosingAttack
	};
	TurnStage m_currentStage;
	std::list<LevelTile*> m_selectableTiles;
	static cocos2d::Scene* createScene();
	virtual bool init();

	// Called when user pauses gameplay.
	void activatePauseScene(Ref *pSender);
	void update(float) override;
	virtual bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchEnded(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchMoved(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchCancelled(cocos2d::Touch*, cocos2d::Event*);

	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);

	virtual void onMouseMove(cocos2d::Event *event);

	void SetSelectableTilesForSpawning(LevelTile* currentTile, Unit::Type unitType);
	void SetSelectableTilesForMoving(LevelTile* currentTile, Unit* unit);
	void SpawnUnit(LevelTile* tile);
	void EndTurn();
	
	CREATE_FUNC(Game);
};