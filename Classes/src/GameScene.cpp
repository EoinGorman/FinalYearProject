#include "GameScene.h"
#include "PlayerManager.h"
#include <iostream>

#define CameraSpeed 350
USING_NS_CC;

Scene* Game::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();
	auto layer = Game::create();
	auto hud = HudLayer::create();
	layer->setName("GameLayer");
	hud->setName("HudLayer");
	scene->addChild(layer);
	scene->addChild(hud);

	return scene;
}

bool Game::init()
{
	if (!Layer::init())
	{
		return false;
	}

	ScreenWidth = Director::getInstance()->getWinSizeInPixels().width;
	ScreenHeight = Director::getInstance()->getWinSizeInPixels().height;

	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("GameScene/MenuTheme1.wav", true);

	//Set up Key Listeners
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(Game::onKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(Game::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//Set up Click Listeners
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(Game::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(Game::onTouchEnded, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(Game::onTouchMoved, this);
	touchListener->onTouchCancelled = CC_CALLBACK_2(Game::onTouchCancelled, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	//Set up Mouse Listeners
	auto _mouseListener = EventListenerMouse::create();
	_mouseListener->onMouseMove = CC_CALLBACK_1(Game::onMouseMove, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);
	
	cameraDirection = Vec2(0, 0);
	this->setPosition(ScreenWidth/2, ScreenHeight/2);

	Level::GetInstance()->SetLevelToLoad("Level1");
	Level::GetInstance()->Load(this);

	this->scheduleUpdate();
	m_currentPlayerID = 0;
	m_currentStage = TurnStage::Waiting;

	for (int i = 0; i < PlayerManager::GetInstance()->GetPlayers().size(); i++)
	{
		PlayerManager::GetInstance()->GetPlayers()[i]->EndTurn(this);
		PlayerManager::GetInstance()->GetPlayers()[i]->SetStartingCameraPos();
	}
	
	PlayerManager::GetInstance()->GetPlayerByID(m_currentPlayerID)->StartTurn();
	SetVisibleTiles();

	m_path = std::vector<LevelTile*>();
	m_paused = false;
	return true;
}

void Game::activatePauseScene(Ref *pSender)
{
	auto scene = Pause::createScene();
	Director::getInstance()->pushScene(scene);
}

void Game::update(float delta)
{
	//Update Loop
	if (!m_paused)
	{
		if (cameraDirection.length() > 0)
		{
			Vec2 newPos = this->getPosition();
			newPos += cameraDirection.getNormalized() * delta * CameraSpeed;
			this->setPosition(newPos);
		}

		//Player 1s turn
		if (m_currentPlayerID == 1)
		{

		}
		//Player 2s turn
		else if (m_currentPlayerID == 2)
		{

		}
		switch (m_currentStage)
		{
		case Waiting:
			break;
		case Moving:
			if (m_unitSelected->MoveToward(m_path[0]->GetPosition()))
			{
				if (m_path.size() == 1)
				{
					m_path[0]->SetOccupyingUnit(m_unitSelected, this);
					
					//If nothing in attack range after move...
					if (GetSelectableTilesForAttacking(m_path[0], m_unitSelected).size() <= 0)
					{
						m_unitSelected->SetUsed(true);
					}
					m_path.erase(m_path.begin());
					m_currentStage = Waiting;
					m_unitSelected->SetMoved(true);
					m_unitSelected = NULL;
					SetVisibleTiles();

					break;
				}
				m_path.erase(m_path.begin());
			}
			break;
		}
	}
}

//KEY LISTENERS
void Game::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
	if (m_currentStage != Moving)
	{
		//Escape Key will Pause Game
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE)
		{
			auto scene = getParent();
			HudLayer* hud = (HudLayer*)scene->getChildByName("HudLayer");

			if (hud->IsBuildMenuVisible())
			{
				ToggleBuildMenu();
			}
			else if (hud->IsUnitMenuVisible())
			{
				ToggleUnitMenu();
			}
			else
			{
				TogglePauseMenu();
			}
		}
	}

		//Camera movement
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_W)
		{
			cameraDirection.y -= 1;
		}
		else if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_S)
		{
			cameraDirection.y += 1;
		}
		if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_A)
		{

			cameraDirection.x += 1;
		}
		else if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_D)
		{
			cameraDirection.x -= 1;
		}
}

void Game::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
	if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_W)
	{
		cameraDirection.y += 1;
	}
	else if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_S)
	{
		cameraDirection.y -= 1;
	}
	if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_A)
	{

		cameraDirection.x -= 1;
	}
	else if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_D)
	{
		cameraDirection.x += 1;
	}
}


//TOUCH LISTENERS
bool Game::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	return true;
}
void Game::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	if (!m_paused)
	{
		std::shared_ptr<GameData> ptr = GameData::sharedGameData();
		CCRect rect = CCRectMake(0, 0, ptr->m_tileSize, ptr->m_tileSize);
		bool clickCanceled = true;

		switch (m_currentStage)
		{
		case Waiting:
			//See if player clicks a tile
			for each (LevelTile* tile in Level::GetInstance()->GetTiles())
			{
				rect.setRect(
					(tile->GetPosition().x - rect.size.width / 2) + this->getPositionX(),
					(tile->GetPosition().y - rect.size.height / 2) + this->getPositionY(),
					rect.size.width,
					rect.size.height);

				if (rect.containsPoint(touch->getLocation()))
				{
					std::cout << "Tile Clicked: " << tile->GetType();

					//If player has clicked a tile with unit on it
					if (tile->HasUnit() && tile->GetOccupyingUnit()->GetOwner()->GetId() == m_currentPlayerID)
					{
						if (!tile->GetOccupyingUnit()->GetUsed())
						{
							m_unitSelected = tile->GetOccupyingUnit();
							ToggleUnitMenu();
						}
					}

					//If player has clicked a tile with building on it
					else if (tile->HasObject() && tile->GetOccupyingObject()->GetOwner() != NULL)
					{
						if (tile->GetOccupyingObject()->GetOwner()->GetId() == m_currentPlayerID)
						{
							if (PlayerManager::GetInstance()->GetPlayerByID(m_currentPlayerID)->GetTurnsTillReinforcements() <= 0)
							{
								//Select unit to build ---
								m_levelTileSelected = tile;
								ToggleBuildMenu();
							}
							else
							{
								//Play failed sound
							}
						}
					}
				}
			}

			break;
		case ChoosingSpawn:
			//Wait for player to click an appropriate tile
			for each (LevelTile* tile in m_selectableTiles)
			{
				rect.setRect(
					(tile->GetPosition().x - rect.size.width / 2) + this->getPositionX(),
					(tile->GetPosition().y - rect.size.height / 2) + this->getPositionY(),
					rect.size.width,
					rect.size.height);
				if (rect.containsPoint(touch->getLocation()))
				{
					std::cout << "Selectable Tile Clicked: " << tile->GetType();

					clickCanceled = false;
					//If player has clicked a tile with no other unit on it
					if (!tile->HasUnit())
					{
						SpawnUnit(tile);
						m_levelTileSelected = tile;
						break;
					}
				}
			}

			if (clickCanceled)
			{
				//Cancel spawn
				for each (LevelTile* tile in m_selectableTiles)
				{
					tile->ActivateAltSprite("", false);
				}
				m_selectableTiles.clear();
				m_currentStage = Waiting;
				m_levelTileSelected = NULL;
				m_unitSelected = NULL;
				break;

			}

			break;
		case ChoosingMove:
			//Wait for player to click an appropriate tile
			for each (LevelTile* tile in m_selectableTiles)
			{
				rect.setRect(
					(tile->GetPosition().x - rect.size.width / 2) + this->getPositionX(),
					(tile->GetPosition().y - rect.size.height / 2) + this->getPositionY(),
					rect.size.width,
					rect.size.height);
				if (rect.containsPoint(touch->getLocation()))
				{
					std::cout << "Selectable Tile Clicked: " << tile->GetType();

					//If player has clicked a tile with no other unit on it
					if (!tile->HasUnit() && !tile->HasObject())
					{
						clickCanceled = false;
						m_levelTileSelected->RemoveOccupyingUnit();
						m_levelTileSelected = tile;

						//Move selected unit to here
						for each (LevelTile* tile in m_path)
						{
							tile->ActivateAltSprite("", false);
							tile->SetInPath(false);
						}

						//Reset to waiting
						for each (LevelTile* tile in m_selectableTiles)
						{
							tile->ActivateAltSprite("", false);
						}

						m_selectableTiles.clear();
						m_currentStage = Moving;
						break;
					}
				}
			}

			if (clickCanceled)
			{
				//Cancel spawn
				for each (LevelTile* tile in m_path)
				{
					tile->ActivateAltSprite("", false);
					tile->SetInPath(false);
				}
				for each (LevelTile* tile in m_selectableTiles)
				{
					tile->ActivateAltSprite("", false);
				}
				m_selectableTiles.clear();
				m_path.clear();
				m_currentStage = Waiting;
				m_levelTileSelected = NULL;
				m_unitSelected = NULL;
				break;
			}
			break;
		case ChoosingAttack:

			//Wait for player to click an appropriate tile
			for each (LevelTile* tile in m_selectableTiles)
			{
				rect.setRect(
					(tile->GetPosition().x - rect.size.width / 2) + this->getPositionX(),
					(tile->GetPosition().y - rect.size.height / 2) + this->getPositionY(),
					rect.size.width,
					rect.size.height);
				if (rect.containsPoint(touch->getLocation()))
				{
					std::cout << "Selectable Tile Clicked: " << tile->GetType();

					//If player has clicked a tile with anoother unit on it
					if (tile->HasUnit())
					{
						clickCanceled = false;
						m_levelTileSelected = tile;

						//ATTACK OTHER UNIT
						UnitAttack(m_unitSelected, tile->GetOccupyingUnit());

						//Reset to waiting
						for each (LevelTile* tile in m_selectableTiles)
						{
							tile->ActivateAltSprite("", false);
						}

						m_selectableTiles.clear();
						m_currentStage = Waiting;
						break;
					}
					else if (tile->HasObject())
					{
						clickCanceled = false;
						m_levelTileSelected = tile;

						//ATTACK BUILDING
						UnitAttack(m_unitSelected, tile->GetOccupyingObject());

						//Reset to waiting
						for each (LevelTile* tile in m_selectableTiles)
						{
							tile->ActivateAltSprite("", false);
						}

						m_selectableTiles.clear();
						m_currentStage = Waiting;
						break;
					}
				}
			}

			if (clickCanceled)
			{
				//Cancel spawn
				for each (LevelTile* tile in m_path)
				{
					tile->ActivateAltSprite("", false);
					tile->SetInPath(false);
				}
				for each (LevelTile* tile in m_selectableTiles)
				{
					tile->ActivateAltSprite("", false);
				}
				m_selectableTiles.clear();
				m_path.clear();
				m_currentStage = Waiting;
				m_levelTileSelected = NULL;
				m_unitSelected = NULL;
				break;
			}

			break;
		}
	}
}
void Game::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{

}
void Game::onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event)
{

}

void Game::onMouseMove(Event *event)
{
	EventMouse* e = (EventMouse*)event;
	Vec2 mousePos = e->getLocationInView();
	mousePos.y += ScreenHeight;

	if (!m_paused)
	{
		std::shared_ptr<GameData> ptr = GameData::sharedGameData();
		CCRect rect = CCRectMake(0, 0, ptr->m_tileSize, ptr->m_tileSize);

		switch (m_currentStage)
		{
		case Waiting:
			break;
		case ChoosingSpawn:
			break;
		case ChoosingMove:
			for each (LevelTile* tile in m_selectableTiles)
			{
				rect.setRect(
					(tile->GetSprite()->getPosition().x - rect.size.width / 2) + this->getPositionX(),
					(tile->GetSprite()->getPosition().y - rect.size.height / 2) + this->getPositionY(),
					rect.size.width,
					rect.size.height);
				if (rect.containsPoint(mousePos) && !tile->GetInPath())
				{
					for each (LevelTile* t in m_path)
					{
						t->SetInPath(false);

						if (!t->HasUnit() || !tile->HasObject())
						{
							t->ActivateAltSprite("Moving", true);
						}
						else
						{
							t->ActivateAltSprite("", false);
						}
					}

					m_path = Level::GetInstance()->GetPath(m_levelTileSelected, tile, m_selectableTiles);
					tile->SetInPath(true);

					for each (LevelTile* t in m_path)
					{
						t->ActivateAltSprite("", true);
					}
					if (tile->HasUnit() || tile->HasObject())
					{
						tile->ActivateAltSprite("Attacking", true);
					}
					break;
				}
				else
				{
					tile->SetInPath(false);
				}
			}
			break;
		case ChoosingAttack:
			break;
		}
	}
}


void Game::SetSelectableTilesForSpawning(LevelTile* currentTile, Unit::Type unitType)
{
	//Select all tiles within distance of currentTile
	int distance = currentTile->GetOccupyingObject()->GetBuildRange();

	for each (LevelTile* tile in Level::GetInstance()->GetTiles())
	{
		if (Level::GetInstance()->GetGameDistanceBetweenTiles(currentTile, tile) <= distance && !tile->HasUnit() && !tile->HasObject())	//If within range and has no unit
		{
			if (Level::GetInstance()->IsMoveableTile(unitType, tile->GetType()))
			{
				tile->ActivateAltSprite("Spawning", true);
				m_selectableTiles.push_back(tile);
			}
		}
	}
}

void Game::SetSelectableTilesForMoving(LevelTile* currentTile, Unit* unit)
{
	//Select all tiles within distance of currentTile
	int checkCount = 0;
	int distance = unit->m_moveRange;
	std::vector<LevelTile*> tilesToCheck;
	std::vector<LevelTile*> tilesToReset;

	tilesToCheck.push_back(currentTile); 
	//currentTile->GetSprite()->setColor(cocos2d::Color3B(25, 25, 0));	//Dont think I want to set colour here...
	currentTile->SetChecked(true);
	//m_selectableTiles.push_back(currentTile);	//Place here because this tile is good to move to
	tilesToReset.push_back(currentTile);

	while (checkCount < distance)
	{
		std::vector<LevelTile*> validTiles;
		for (int i = 0; i < tilesToCheck.size(); i++)
		{
			std::vector<LevelTile*> neighbourTiles = Level::GetInstance()->GetNeighbourTiles(tilesToCheck[i]);
			for (int j = 0; j < neighbourTiles.size(); j++)
			{
				bool tileHasFriendlyUnit = neighbourTiles[j]->HasUnit() && neighbourTiles[j]->GetOccupyingUnit()->GetOwner() == PlayerManager::GetInstance()->GetPlayerByID(m_currentPlayerID);
				bool tileHasFriendlyBuilding = neighbourTiles[j]->HasObject() && neighbourTiles[j]->GetOccupyingObject()->GetOwner() == PlayerManager::GetInstance()->GetPlayerByID(m_currentPlayerID);

				if (!neighbourTiles[j]->GetChecked() && (!neighbourTiles[j]->HasUnit() || tileHasFriendlyUnit) && (!neighbourTiles[j]->HasObject() ||tileHasFriendlyBuilding))
				{
					if (Level::GetInstance()->IsMoveableTile(unit->GetType(), neighbourTiles[j]->GetType()))
					{
						neighbourTiles[j]->SetChecked(true);
						if (!tileHasFriendlyUnit && !tileHasFriendlyBuilding)
						{
							neighbourTiles[j]->ActivateAltSprite("Moving", true);
						}
						m_selectableTiles.push_back(neighbourTiles[j]);	//Place here because this tile is good to move to
						validTiles.push_back(neighbourTiles[j]);	//Place here so this tiles neighbours get checked
						tilesToReset.push_back(neighbourTiles[j]);	//Place here so we keep a list of all tiles that need to have values reset after search
					}
				}
			}
		}
		tilesToCheck.clear();
		tilesToCheck = validTiles;
		checkCount++;
	}

	for each (LevelTile* tile in Level::GetInstance()->GetTiles())
	{
		tile->SetChecked(false);
	}
	tilesToReset.clear();
}

void Game::SetSelectableTilesForAttacking(LevelTile* currentTile, Unit* unit)
{
	//Select all tiles within distance of currentTile
	int checkCount = 0;
	int distance = unit->m_attackRange;
	std::vector<LevelTile*> tilesToCheck;
	std::vector<LevelTile*> tilesToReset;

	tilesToCheck.push_back(currentTile);
	currentTile->SetChecked(true);
	tilesToReset.push_back(currentTile);

	while (checkCount < distance)
	{
		std::vector<LevelTile*> validTiles;
		for (int i = 0; i < tilesToCheck.size(); i++)
		{
			std::vector<LevelTile*> neighbourTiles = Level::GetInstance()->GetNeighbourTiles(tilesToCheck[i]);
			for (int j = 0; j < neighbourTiles.size(); j++)
			{
				if (!neighbourTiles[j]->GetChecked())
				{
					neighbourTiles[j]->SetChecked(true);

					bool tileHasEnemyUnit = neighbourTiles[j]->HasUnit() && neighbourTiles[j]->GetOccupyingUnit()->GetOwner() != PlayerManager::GetInstance()->GetPlayerByID(m_currentPlayerID);
					bool tileHasNonFriendlyBuilding = neighbourTiles[j]->HasObject() && neighbourTiles[j]->GetOccupyingObject()->GetOwner() != PlayerManager::GetInstance()->GetPlayerByID(m_currentPlayerID);
					if (tileHasEnemyUnit || tileHasNonFriendlyBuilding)
					{
						neighbourTiles[j]->ActivateAltSprite("Attacking", true);
						m_selectableTiles.push_back(neighbourTiles[j]);	//Place here because this tile is good to move to
					}
					validTiles.push_back(neighbourTiles[j]);	//Place here so this tiles neighbours get checked
					tilesToReset.push_back(neighbourTiles[j]);	//Place here so we keep a list of all tiles that need to have values reset after search
				}
			}
		}
		tilesToCheck.clear();
		tilesToCheck = validTiles;
		checkCount++;
	}

	for each (LevelTile* tile in Level::GetInstance()->GetTiles())
	{
		tile->SetChecked(false);
	}
	tilesToReset.clear();
}

std::vector<LevelTile*> Game::GetSelectableTilesForAttacking(LevelTile* currentTile, Unit* unit)
{
	//Select all tiles within distance of currentTile
	int checkCount = 0;
	int distance = unit->m_attackRange;
	std::vector<LevelTile*> tilesToCheck;
	std::vector<LevelTile*> tilesToReset;
	std::vector<LevelTile*> attackableTiles;

	tilesToCheck.push_back(currentTile);
	currentTile->SetChecked(true);
	tilesToReset.push_back(currentTile);

	while (checkCount < distance)
	{
		std::vector<LevelTile*> validTiles;
		for (int i = 0; i < tilesToCheck.size(); i++)
		{
			std::vector<LevelTile*> neighbourTiles = Level::GetInstance()->GetNeighbourTiles(tilesToCheck[i]);
			for (int j = 0; j < neighbourTiles.size(); j++)
			{
				if (!neighbourTiles[j]->GetChecked())
				{
					neighbourTiles[j]->SetChecked(true);

					bool tileHasEnemyUnit = neighbourTiles[j]->HasUnit() && neighbourTiles[j]->GetOccupyingUnit()->GetOwner() != PlayerManager::GetInstance()->GetPlayerByID(m_currentPlayerID);
					bool tileHasNonFriendlyBuilding = neighbourTiles[j]->HasObject() && neighbourTiles[j]->GetOccupyingObject()->GetOwner() != PlayerManager::GetInstance()->GetPlayerByID(m_currentPlayerID);
					if (tileHasEnemyUnit || tileHasNonFriendlyBuilding)
					{
						attackableTiles.push_back(neighbourTiles[j]);	//Place here because this tile is good to move to
					}
					validTiles.push_back(neighbourTiles[j]);	//Place here so this tiles neighbours get checked
					tilesToReset.push_back(neighbourTiles[j]);	//Place here so we keep a list of all tiles that need to have values reset after search
				}
			}
		}
		tilesToCheck.clear();
		tilesToCheck = validTiles;
		checkCount++;
	}

	for each (LevelTile* tile in Level::GetInstance()->GetTiles())
	{
		tile->SetChecked(false);
	}
	tilesToReset.clear();
	return attackableTiles;
}

void Game::SpawnUnit(LevelTile* tile)
{
	for each (LevelTile* tile in m_selectableTiles)
	{
		tile->ActivateAltSprite("", false);
	}
	Unit* newUnit = new Unit(m_unitTypeSelected, Level::GetInstance()->GetTileIndexPosition(tile), PlayerManager::GetInstance()->GetPlayerByID(m_currentPlayerID));
	tile->BuildUnitHere(newUnit, this);	//WHEN DONE, CREATE A NEW UNIT AND PASS IN HERE
	PlayerManager::GetInstance()->GetPlayerByID(m_currentPlayerID)->AddUnit(newUnit);	//Add to players unit list also
	m_selectableTiles.clear();
	m_currentStage = Waiting;
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("GameScene/placeUnitSound.wav", false, 1.0f, 1.0f, 1.0f);

	auto scene = this->getParent();
	HudLayer* hud = (HudLayer*)scene->getChildByName("HudLayer");
	hud->UpdateLabels(m_currentPlayerID);

	SetVisibleTiles();
}

void Game::EndTurn()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("buttonClickSound.wav", false, 1.0f, 1.0f, 1.0f);
	PlayerManager::GetInstance()->GetPlayerByID(m_currentPlayerID)->EndTurn(this);
	SetNextPlayer();
	PlayerManager::GetInstance()->GetPlayerByID(m_currentPlayerID)->StartTurn();

	auto scene = this->getParent();
	HudLayer* hud = (HudLayer*)scene->getChildByName("HudLayer");
	hud->UpdateLabels(m_currentPlayerID);

	//Move camera
	Vec2 lastPos = PlayerManager::GetInstance()->GetPlayerByID(m_currentPlayerID)->GetLastCameraPos();
	this->setPosition(lastPos);

	//Cancel
	for each (LevelTile* tile in m_selectableTiles)
	{
		tile->ActivateAltSprite("", false);
	}
	m_path.clear();
	m_selectableTiles.clear();
	m_currentStage = Waiting;
	m_levelTileSelected = NULL;
	m_unitSelected = NULL;
	TogglePauseMenu();

	SetVisibleTiles();
}

void Game::TogglePauseMenu()
{
	m_paused = !m_paused;
	auto scene = getParent();
	HudLayer* hud = (HudLayer*)scene->getChildByName("HudLayer");
	hud->TogglePauseMenu();
}

void Game::ToggleBuildMenu()
{
	m_paused = !m_paused;
	auto scene = getParent();
	HudLayer* hud = (HudLayer*)scene->getChildByName("HudLayer");
	hud->ToggleBuildMenu();
}

void Game::SetUnitTypeSelected(Unit::Type type)
{
	int ticketsRemaining = PlayerManager::GetInstance()->GetPlayerByID(m_currentPlayerID)->GetTicketsRemaining();
	int cost = INT32_MAX;

	switch (type)
	{
	case Unit::Type::soldier:
		cost = 10;
		break;
	case Unit::Type::mortarSquad:
		cost = 15;
		break;
	case Unit::Type::smallTank:
		cost = 25;
		break;
	case Unit::Type::tBoat:
		cost = 20;
		break;
	case Unit::Type::tCopter:
		cost = 25;
		break;
	case Unit::Type::soldier2:
		cost = 15;
		break;
	case Unit::Type::artillery:
		cost = 30;
		break;
	case Unit::Type::largeTank:
		cost = 35;
		break;
	case Unit::Type::attackBoat:
		cost = 30;
		break;
	case Unit::Type::attackCopter:
		cost = 35;
		break;
	}

	if (ticketsRemaining >= cost)
	{
		ToggleBuildMenu();
		m_unitTypeSelected = type;
		SetSelectableTilesForSpawning(m_levelTileSelected, m_unitTypeSelected);
		m_currentStage = ChoosingSpawn;
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("GameScene/selectBuildingSound.wav", false, 1.0f, 1.0f, 1.0f);
	}
	else
	{
		//Play failed sound
	}
}

void Game::SetNextPlayer()
{
	if (m_currentPlayerID >= PlayerManager::GetInstance()->GetPlayerCount() -1)	//If player count is 2 then the second players ID will be 1 so need to subtract here
	{
		m_currentPlayerID = 0;
	}
	else
	{
		m_currentPlayerID++;
	}
}

void Game::SetVisibleTiles()
{
	for each (LevelTile* tile in Level::GetInstance()->GetTiles())
	{
		tile->SetInSight(false);
	}

	for each (LevelTile* tile in GetAllFOWVisibleTiles())
	{
		tile->SetInSight(true);
	}
}

std::vector<LevelTile*> Game::GetAllFOWVisibleTiles()
{
	std::vector<LevelTile*> visibleTiles;
	Player* currentPlayer = PlayerManager::GetInstance()->GetPlayerByID(m_currentPlayerID);

	for each (Unit* unit in currentPlayer->GetUnits())
	{
		std::vector<LevelTile*> tilesInSight = GetAllTilesInSightRange(Level::GetInstance()->GetTileAtIndex(unit->GetTileIndex()), unit);
		for each (LevelTile* tile in tilesInSight)
		{
			visibleTiles.push_back(tile);
		}
	}

	for each (LevelObject* building in currentPlayer->GetBuildings())
	{
		visibleTiles.push_back(Level::GetInstance()->GetTileAtIndex(building->GetTileIndex()));
	}
	return visibleTiles;
}

std::vector<LevelTile*> Game::GetAllTilesInSightRange(LevelTile* currentTile, Unit* unit)
{
	//Select all tiles within distance of currentTile
	int checkCount = 0;
	int distance = unit->m_sightRange;
	std::vector<LevelTile*> tilesToCheck;
	std::vector<LevelTile*> tilesInSight;


	std::vector<LevelTile*> td = Level::GetInstance()->GetTiles();

	currentTile->SetChecked(true);	
	tilesToCheck.push_back(currentTile);
	tilesInSight.push_back(currentTile);

	while (checkCount < distance)
	{
		std::vector<LevelTile*> validTiles;
		for (int i = 0; i < tilesToCheck.size(); i++)
		{
			std::vector<LevelTile*> neighbourTiles = Level::GetInstance()->GetNeighbourTiles(tilesToCheck[i]);
			for (int j = 0; j < neighbourTiles.size(); j++)
			{
				if (!neighbourTiles[j]->GetChecked())
				{
					neighbourTiles[j]->SetChecked(true);
					validTiles.push_back(neighbourTiles[j]);	//Place here so this tiles neighbours get checked
					tilesInSight.push_back(neighbourTiles[j]);	//Place here so we keep a list of all tiles that are in sight
				}
			}
		}
		tilesToCheck.clear();
		tilesToCheck = validTiles;
		checkCount++;
	}

	for each (LevelTile* tile in Level::GetInstance()->GetTiles())
	{
		tile->SetChecked(false);
	}
	return tilesInSight;
}

void Game::ToggleUnitMenu()
{
	m_paused = !m_paused;
	auto scene = getParent();
	HudLayer* hud = (HudLayer*)scene->getChildByName("HudLayer");
	hud->ToggleUnitMenu(m_unitSelected);
}

void Game::BeginUnitMove()
{
	m_levelTileSelected = Level::GetInstance()->GetTileAtIndex(m_unitSelected->GetTileIndex());
	SetSelectableTilesForMoving(m_levelTileSelected, m_levelTileSelected->GetOccupyingUnit());
	m_currentStage = ChoosingMove;
	ToggleUnitMenu();
}

void Game::BeginUnitAttack()
{
	m_levelTileSelected = Level::GetInstance()->GetTileAtIndex(m_unitSelected->GetTileIndex());
	SetSelectableTilesForAttacking(m_levelTileSelected, m_levelTileSelected->GetOccupyingUnit());
	if (m_selectableTiles.size() == 0)
	{
		m_currentStage = Waiting;
		m_levelTileSelected = NULL;
		m_unitSelected = NULL;
	}
	else
	{
		m_currentStage = ChoosingAttack;
	}
	ToggleUnitMenu();
}

void Game::UnitAttack(Unit* attackingUnit, Unit* otherUnit)
{
	//Calculate attack based off health as well... LATER
	float attackingPower = attackingUnit->m_attackPower;
	attackingPower *= (attackingUnit->GetHealth() / 10.0f);

	float defence = otherUnit->m_defence;
	defence += m_levelTileSelected->m_defenceBonus;
	defence /= 10.0f;

	otherUnit->AlterHealth(-(attackingPower - (attackingPower * defence)));
	if (otherUnit->GetHealth() <= 0)
	{
		m_levelTileSelected->RemoveOccupyingUnit();
		otherUnit->RemoveFromLayer();
		otherUnit->GetOwner()->RemoveUnit(otherUnit);
		delete otherUnit;
	}
	attackingUnit->SetUsed(true);
}

void Game::UnitAttack(Unit* attackingUnit, LevelObject* building)
{
	//Calculate attack based off health as well... LATER
	float attackingPower = attackingUnit->m_attackPower;
	attackingPower *= (attackingUnit->GetHealth() / 10.0f);

	float defence = building->m_defence;
	defence += m_levelTileSelected->m_defenceBonus;
	defence /= 10.0f;

	building->AlterHealth(-(attackingPower - (attackingPower * defence)));
	if (building->m_health <= 0)
	{
		//Restore health
		building->SetHealth(10.0f);

		//Change Ownership
		if (building->GetOwner())
		{
			building->GetOwner()->RemoveBuilding(building);
		}
		building->SetOwner(PlayerManager::GetInstance()->GetPlayerByID(m_currentPlayerID));
	}

	attackingUnit->SetUsed(true);
}