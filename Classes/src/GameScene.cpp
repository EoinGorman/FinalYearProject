#include "GameScene.h"
#include <iostream>

USING_NS_CC;

Scene* Game::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();
	auto layer = Game::create();

	scene->addChild(layer);

	return scene;
}

bool Game::init()
{
	if (!Layer::init())
	{
		return false;
	}

	//DO AUDIO YOU LAZY CUNT!! 
	//USE CCFOLLOW WITH MOUSE INPUT OR SOMETHING... :/
	//CCFollow

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

	auto label = Label::createWithSystemFont(".", "Arial", 96);
	label->setAnchorPoint(cocos2d::Vec2(0, 0));
	Vec2 pos = label->getPosition();
	label->setPosition(0, 0);
	this->addChild(label, 1);

	Level::GetInstance()->Load("level1", this);
	this->scheduleUpdate();
	m_currentPlayer = 1;
	m_currentStage = TurnStage::Waiting;
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
	//Player 1s turn
	if (m_currentPlayer == 1)
	{

	}
	//Player 2s turn
	else if (m_currentPlayer == 2)
	{

	}

	//TEMP move camera
	/*
	int SPEED = 10;
	this->setPosition(this->getPosition().x - SPEED * delta, this->getPosition().y - SPEED * delta);
	this->setScale(0.5f);
	*/

}

//KEY LISTENERS
void Game::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
	if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		auto scene = Pause::createScene();
		Director::getInstance()->pushScene(scene);
	}
}

void Game::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{

}


//TOUCH LISTENERS
bool Game::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	return true;
}
void Game::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	std::shared_ptr<GameData> ptr = GameData::sharedGameData();
	CCRect rect = CCRectMake(0, 0, ptr->m_tileSize, ptr->m_tileSize);

	switch (m_currentStage)
	{
	case Waiting:
		//See if player clicks a tile
		for each (LevelTile* tile in Level::GetInstance()->GetTiles())
		{
			rect.setRect(tile->GetPosition().x - rect.size.width / 2, tile->GetPosition().y - rect.size.height / 2, rect.size.width, rect.size.height);
			if (rect.containsPoint(touch->getLocation()))
			{
				this->setPosition(tile->GetPosition());
				std::cout << "Tile Clicked: " << tile->GetType();

				//If player has clicked a tile with unit on it
				if (tile->HasUnit())
				{

					SetSelectableTilesForMoving(tile, tile->GetOccupyingUnit());
					m_currentStage = ChoosingMove;
				}

				//If player has clicked a tile with building on it
				else if (tile->HasObject())
				{
					//Select unit to build ---
					Unit::Type unitType = Unit::Type::smallTank;
					//
					SetSelectableTilesForSpawning(tile, unitType);
					m_currentStage = ChoosingSpawn;
				}
			}
		}

		break;
	case ChoosingSpawn:
		//Wait for player to click an appropriate tile
		for each (LevelTile* tile in m_selectableTiles)
		{
			rect.setRect(tile->GetPosition().x - rect.size.width / 2, tile->GetPosition().y - rect.size.height / 2, rect.size.width, rect.size.height);
			if (rect.containsPoint(touch->getLocation()))
			{
				std::cout << "Selectable Tile Clicked: " << tile->GetType();

				//If player has clicked a tile with no other unit on it
				if (!tile->HasUnit())
				{
					SpawnUnit(tile);
					break;
				}
			}

			else
			{
				/*
				//Cancel spawn
				for each (LevelTile* tile in m_selectableTiles)
				{
					tile->GetSprite()->setColor(cocos2d::Color3B(255, 255, 255));
				}
				m_selectableTiles.clear();
				m_currentStage = Waiting;
				break;
				*/
			}
		}

		break;
	case ChoosingMove:
		//Wait for player to click an appropriate tile
		for each (LevelTile* tile in m_selectableTiles)
		{
			rect.setRect(tile->GetPosition().x - rect.size.width / 2, tile->GetPosition().y - rect.size.height / 2, rect.size.width, rect.size.height);
			if (rect.containsPoint(touch->getLocation()))
			{
				std::cout << "Selectable Tile Clicked: " << tile->GetType();

				//If player has clicked a tile with no other unit on it
				if (!tile->HasUnit())
				{
					//Move selected unit to here
					break;
				}
			}

			else
			{
				/*
				//Cancel spawn
				for each (LevelTile* tile in m_selectableTiles)
				{
				tile->GetSprite()->setColor(cocos2d::Color3B(255, 255, 255));
				}
				m_selectableTiles.clear();
				m_currentStage = Waiting;
				break;
				*/
			}
		}
		break;
	case ChoosingAttack:
		break;
	}
}
void Game::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{


}
void Game::onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event)
{

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
	tile->SetOccupyingUnit(new Unit(Unit::Type::smallTank, Level::GetInstance()->GetTileIndexPosition(tile)), this);	//WHEN DONE, CREATE A NEW UNIT AND PASS IN HERE
	m_selectableTiles.clear();
	m_currentStage = Waiting;
}