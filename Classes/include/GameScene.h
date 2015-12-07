#pragma once

#include "cocos2d.h"
#include "PauseScene.h"
#include "Level.h"

class Game : public cocos2d::Layer
{
public:
	//Game logic variables
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

	void Game::SetSelectableTilesForSpawning(LevelTile* currentTile, Unit::Type unitType);
	void Game::SetSelectableTilesForMoving(LevelTile* currentTile, Unit* unit);
	void Game::SpawnUnit(LevelTile* tile);

	CREATE_FUNC(Game);
};