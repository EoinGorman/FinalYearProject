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

	int m_currentPlayerID;
	enum TurnStage
	{
		ChoosingFactions,
		Waiting,
		ChoosingSpawn,
		ChoosingMove,
		Moving,
		ChoosingAttack
	};
	bool m_paused;
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
	void SetVisibleTiles();
	std::vector<LevelTile*> GetAllFOWVisibleTiles();
	std::vector<LevelTile*> GetAllTilesInSightRange(LevelTile* currentTile, Unit* unit);
	void SpawnUnit(LevelTile* tile);
	void EndTurn();
	void TogglePauseMenu();
	void ToggleBuildMenu();
	void SetUnitTypeSelected(Unit::Type);
	

	void Game::ToggleUnitMenu();
	void Game::BeginUnitMove();
	void Game::BeginUnitAttack();

	CREATE_FUNC(Game);

private:
	void SetNextPlayer();
	Unit* m_unitSelected;
	Unit::Type m_unitTypeSelected;
	LevelTile* m_levelTileSelected;
	std::vector<LevelTile*> m_path;
};