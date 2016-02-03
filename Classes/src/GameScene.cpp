#include "GameScene.h"
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

	Level::GetInstance()->Load("level1", this);
	this->scheduleUpdate();
	m_currentPlayer = 1;
	m_currentStage = TurnStage::Waiting;

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
		if (m_currentPlayer == 1)
		{

		}
		//Player 2s turn
		else if (m_currentPlayer == 2)
		{

		}
	}
}

//KEY LISTENERS
void Game::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
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
		else
		{
			TogglePauseMenu();
		}
	}

	//Temp end turn key
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
					if (tile->HasUnit() && tile->GetOccupyingUnit()->GetOwner() == m_currentPlayer)
					{
						SetSelectableTilesForMoving(tile, tile->GetOccupyingUnit());
						m_currentStage = ChoosingMove;
					}

					//If player has clicked a tile with building on it
					else if (tile->HasObject() && tile->GetOccupyingObject()->GetOwner() == m_currentPlayer)
					{
						//Select unit to build ---
						m_levelTileSelected = tile;
						ToggleBuildMenu();
						/*
						Unit::Type unitType = Unit::Type::smallTank;
						//
						SetSelectableTilesForSpawning(tile, unitType);
						m_currentStage = ChoosingSpawn;
						CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("GameScene/selectBuildingSound.wav", false, 1.0f, 1.0f, 1.0f);
						*/
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
						m_levelTileSelected = NULL;
						break;
					}
				}
			}

			if (clickCanceled)
			{
				//Cancel spawn
				for each (LevelTile* tile in m_selectableTiles)
				{
					tile->GetSprite()->setColor(cocos2d::Color3B(255, 255, 255));
				}
				m_selectableTiles.clear();
				m_currentStage = Waiting;
				m_levelTileSelected = NULL;
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
					if (!tile->HasUnit())
					{
						clickCanceled = false;
						m_levelTileSelected = NULL;
						//Move selected unit to here
						break;
					}
				}
			}

			if (clickCanceled)
			{
				//Cancel spawn
				for each (LevelTile* tile in m_selectableTiles)
				{
					tile->GetSprite()->setColor(cocos2d::Color3B(255, 255, 255));
				}
				m_selectableTiles.clear();
				m_currentStage = Waiting;
				m_levelTileSelected = NULL;
				break;
			}
			break;
		case ChoosingAttack:
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
	/*
	if (!m_paused)
	{
		EventMouse* e = (EventMouse*)event;
		Vec2 touchLocation = e->getLocationInView();//event->getLocation();
		Vec2 center = Vec2(ScreenWidth / 2, -ScreenHeight / 2);

		center.subtract(touchLocation);

		if (ScreenHeight >= ScreenWidth)
		{
			if (center.length() > (ScreenWidth / 10 * 9) / 2)
			{
				cameraDirection = center.getNormalized();
				return;
			}
		}

		else
		{
			if (center.length() > (ScreenHeight / 10 * 9) / 2)
			{
				cameraDirection = center.getNormalized();
				return;
			}
		}

		cameraDirection = Vec2(0, 0);

		if (touchLocation.x < ScreenWidth / 10)
		{
		cameraDirection.x = 1;
		}
		else if (touchLocation.x > (ScreenWidth / 10) * 9)
		{
		cameraDirection.x = -1;
		}
		else
		{
		cameraDirection.x = 0;
		}

		if (touchLocation.y > -(ScreenHeight / 10))
		{
		cameraDirection.y = -1;
		}
		else if (touchLocation.y < -(ScreenHeight / 10 * 9))
		{
		cameraDirection.y = 1;
		}
		else
		{
		cameraDirection.y = 0;
		}
	}
		*/
}


void Game::SetSelectableTilesForSpawning(LevelTile* currentTile, Unit::Type unitType)
{
	//Select all tiles within distance of currentTile
	int distance = 4;

	for each (LevelTile* tile in Level::GetInstance()->GetTiles())
	{
		if (Level::GetInstance()->GetGameDistanceBetweenTiles(currentTile, tile) <= distance && !tile->HasUnit())	//If within range and has no unit
		{
			if (Level::GetInstance()->IsMoveableTile(unitType, tile->GetType()))
			{
				tile->GetSprite()->setColor(cocos2d::Color3B(100, 100, 255));
				m_selectableTiles.push_back(tile);
			}
		}
	}
}

void Game::SetSelectableTilesForMoving(LevelTile* currentTile, Unit* unit)
{
	//Select all tiles within distance of currentTile
	int distance = unit->m_moveRange;

	for each (LevelTile* tile in Level::GetInstance()->GetTiles())
	{
		if (Level::GetInstance()->GetGameDistanceBetweenTiles(currentTile, tile) <= distance && !tile->HasUnit())	//If within range and has no unit
		{
			if (Level::GetInstance()->IsMoveableTile(unit->GetType(), tile->GetType()))
			{
				tile->GetSprite()->setColor(cocos2d::Color3B(100, 100, 255));
				m_selectableTiles.push_back(tile);
			}
		}
	}
}

void Game::SpawnUnit(LevelTile* tile)
{
	for each (LevelTile* tile in m_selectableTiles)
	{
		tile->GetSprite()->setColor(cocos2d::Color3B(255, 255, 255));
	}
	tile->SetOccupyingUnit(new Unit(m_unitTypeSelected, Level::GetInstance()->GetTileIndexPosition(tile), m_currentPlayer), this);	//WHEN DONE, CREATE A NEW UNIT AND PASS IN HERE
	m_selectableTiles.clear();
	m_currentStage = Waiting;
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("GameScene/placeUnitSound.wav", false, 1.0f, 1.0f, 1.0f);
}

void Game::EndTurn()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("buttonClickSound.wav", false, 1.0f, 1.0f, 1.0f);
	if (m_currentPlayer == 1)
	{
		m_currentPlayer = 2;
	}

	else if (m_currentPlayer == 2)
	{
		m_currentPlayer = 1;
	}

	//Cancel
	for each (LevelTile* tile in m_selectableTiles)
	{
		tile->GetSprite()->setColor(cocos2d::Color3B(255, 255, 255));
	}
	m_selectableTiles.clear();
	m_currentStage = Waiting;
	m_levelTileSelected = NULL;
	TogglePauseMenu();
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
	ToggleBuildMenu();
	m_unitTypeSelected = type;
	SetSelectableTilesForSpawning(m_levelTileSelected, m_unitTypeSelected);
	m_currentStage = ChoosingSpawn;
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("GameScene/selectBuildingSound.wav", false, 1.0f, 1.0f, 1.0f);
}