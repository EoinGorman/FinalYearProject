#include "HudLayer.h"
#include "GameScene.h"
#include "PlayerManager.h"
#include <string>

USING_NS_CC;

void HudLayer::CreatePauseMenu()
{
	// creating pause menu

	// create a menu item
	auto continueItem = MenuItemImage::create("Hud/playButtonPauseDefault.png", "MainMenuScene/playButtonClicked.png",
		CC_CALLBACK_0(HudLayer::PlayPressed, this));

	auto endTurnItem = MenuItemImage::create("Hud/EndTurn.png", "PauseMenu/EndTurnClicked.png",
		CC_CALLBACK_1(HudLayer::EndTurnPressed, this));

	auto quitItem = MenuItemImage::create("Hud/quitButtonPauseDefault.png", "MainMenuScene/quitButtonClicked.png",
		CC_CALLBACK_0(HudLayer::QuitGame, this));

	auto menu = Menu::create(continueItem, endTurnItem, quitItem, NULL);
	menu->setName("PauseMenu");
	menu->setVisible(false);

	// Returns visible size of OpenGL window in points.
	Size visibleSize = Director::getInstance()->getVisibleSize();
	menu->alignItemsVerticallyWithPadding(visibleSize.height / 6);

	//Add Background
	m_pauseBackground = cocos2d::Sprite::create("Hud/pauseBackgroundPanel.png");
	m_pauseBackground->setPosition(cocos2d::Point((visibleSize.width / 2), (visibleSize.height / 2)));
	//Set scale of background to match height of screen
	m_pauseBackground->setScaleX(visibleSize.height / m_pauseBackground->getContentSize().height);
	m_pauseBackground->setScaleY(visibleSize.height / m_pauseBackground->getContentSize().height);
	m_pauseBackground->setVisible(false);

	this->addChild(m_pauseBackground);

	this->addChild(menu, 1);
}

void HudLayer::CreateBuildMenu()
{
	// Returns visible size of OpenGL window in points.
	Size visibleSize = Director::getInstance()->getVisibleSize();
	
	//Add Background
	m_buildBackground = cocos2d::Sprite::create("Hud/buildUnitsBackground.png");
	//Set scale of background to match height of screen
	m_buildBackground->setScaleX(visibleSize.height / m_buildBackground->getContentSize().height);
	m_buildBackground->setScaleY(visibleSize.height / m_buildBackground->getContentSize().height);
	m_buildBackground->setPosition(cocos2d::Point((m_buildBackground->getContentSize().width / 2) * m_buildBackground->getScaleX(), (visibleSize.height / 2)));
	m_buildBackground->setVisible(false);

	//Create scroll view
	m_scrollView = ui::ScrollView::create();
	m_scrollView->setAnchorPoint(Vec2(0.5, 0.5));
	m_scrollView->setContentSize(cocos2d::Size((m_buildBackground->getContentSize().width) * m_buildBackground->getScaleX(),
		(m_buildBackground->getContentSize().height) * m_buildBackground->getScaleY() * 0.8));
	m_scrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
	m_scrollView->setBounceEnabled(true);
	m_scrollView->setTouchEnabled(true);
	m_scrollView->setPosition(cocos2d::Point((m_buildBackground->getContentSize().width / 2) * m_buildBackground->getScaleX(), (visibleSize.height / 2)));	//same pos as background panel
	m_scrollView->setInnerContainerSize(m_scrollView->getSize() * 2.25);
	m_scrollView->setName("ScrollView");

	this->addChild(m_buildBackground);
	this->addChild(m_scrollView);

	int count = 9;
	// make buttons
	for (int i = 0; i < 10; i++)
	{
		Unit::Type unitType = Unit::Type(count);
		auto testButton = ui::Button::create();
		testButton->setTouchEnabled(true);
		testButton->setPosition(Vec2(m_scrollView->getPosition().x, m_scrollView->getBottomBoundary() + (i * 100)));
		std::string defaultTex = "Hud/UnitNames/UnitNamesDefault" + std::to_string(count) + ".png";
		std::string clickedTex = "Hud/UnitNames/UnitNamesClicked" + std::to_string(count) + ".png";
		testButton->loadTextures(defaultTex, clickedTex);

		testButton->addClickEventListener(CC_CALLBACK_0(HudLayer::BuildUnit, this, unitType));

		LabelTTF* priceLabel;
		LabelTTF* currencyLabel;
		switch (unitType)
		{
		case Unit::Type::soldier:
			priceLabel = cocos2d::LabelTTF::create("10", "fonts/Akashi.ttf", 32, cocos2d::Size(50, 50), cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
			break;
		case Unit::Type::mortarSquad:
			priceLabel = cocos2d::LabelTTF::create("15", "fonts/Akashi.ttf", 32, cocos2d::Size(50, 50), cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
			break;
		case Unit::Type::smallTank:
			priceLabel = cocos2d::LabelTTF::create("25", "fonts/Akashi.ttf", 32, cocos2d::Size(50, 50), cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
			break;
		case Unit::Type::tBoat:
			priceLabel = cocos2d::LabelTTF::create("20", "fonts/Akashi.ttf", 32, cocos2d::Size(50, 50), cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
			break;
		case Unit::Type::tCopter:
			priceLabel = cocos2d::LabelTTF::create("25", "fonts/Akashi.ttf", 32, cocos2d::Size(50, 50), cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
			break;
		case Unit::Type::soldier2:
			priceLabel = cocos2d::LabelTTF::create("15", "fonts/Akashi.ttf", 32, cocos2d::Size(50, 50), cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
			break;
		case Unit::Type::artillery:
			priceLabel = cocos2d::LabelTTF::create("30", "fonts/Akashi.ttf", 32, cocos2d::Size(50, 50), cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
			break;
		case Unit::Type::largeTank:
			priceLabel = cocos2d::LabelTTF::create("35", "fonts/Akashi.ttf", 32, cocos2d::Size(50, 50), cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
			break;
		case Unit::Type::attackBoat:
			priceLabel = cocos2d::LabelTTF::create("30", "fonts/Akashi.ttf", 32, cocos2d::Size(50, 50), cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
			break;
		case Unit::Type::attackCopter:
			priceLabel = cocos2d::LabelTTF::create("35", "fonts/Akashi.ttf", 32, cocos2d::Size(50, 50), cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
			break;
		}
		currencyLabel = cocos2d::LabelTTF::create("$", "fonts/Akashi.ttf", 16, cocos2d::Size(32, 32), cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::TOP);
		testButton->addChild(currencyLabel);

		testButton->addChild(priceLabel);
		priceLabel->setPosition(testButton->getSize().width + priceLabel->getContentSize().width/2, (testButton->getSize().height * testButton->getScaleY()) /2);
		priceLabel->setColor(cocos2d::Color3B(68, 67, 72));

		currencyLabel->setPosition(priceLabel->getPositionX(), priceLabel->getPositionY() + (priceLabel->getContentSize().height /3));
		currencyLabel->setColor(cocos2d::Color3B(68, 67, 72));

		m_scrollView->addChild(testButton);
		m_scrollView->setVisible(false);
		count--;
	}

}

void HudLayer::CreateUnitMenu()
{

	// Returns visible size of OpenGL window in points.
	Size visibleSize = Director::getInstance()->getVisibleSize();

	// create a menu item
	auto moveItem = MenuItemImage::create("Hud/moveButtonDefault.png", "Hud/moveButtonClicked.png", "Hud/moveButtonDisabled.png",
		CC_CALLBACK_0(HudLayer::MovePressed, this));
	moveItem->setName("MoveButton");

	auto attackItem = MenuItemImage::create("Hud/attackButtonDefault.png", "Hud/attackButtonClicked.png",
		CC_CALLBACK_0(HudLayer::AttackPressed, this));

	auto menu = Menu::create(moveItem, attackItem, NULL);
	menu->setName("UnitMenu");
	menu->setVisible(false);
	menu->alignItemsVerticallyWithPadding(10);

	//Add Background
	m_unitBackground = cocos2d::Sprite::create("Hud/unitPopUp.png");
	m_unitBackground->setVisible(false);

	this->addChild(m_unitBackground);
	this->addChild(menu, 1);
}

void HudLayer::CreateHud()
{
	// Returns visible size of OpenGL window in points.
	Size visibleSize = Director::getInstance()->getVisibleSize();
	
	cocos2d::Point offsetFromCorner(25, -25);

	for each(Player* player in PlayerManager::GetInstance()->GetPlayers())
	{
		std::string logoName = "Hud/" + player->GetLogoName();
		m_factionLogos.push_back(cocos2d::Sprite::create(logoName));
	}

	for (int i = 0; i < m_factionLogos.size(); i++)
	{
		m_factionLogos[i]->setPosition(cocos2d::Point(m_factionLogos[i]->getContentSize().width / 2, -m_factionLogos[i]->getContentSize().height / 2 + visibleSize.height) + offsetFromCorner);	//Set pos to top left corner
		m_factionLogos[i]->setVisible(false);
		this->addChild(m_factionLogos[i]);
	}
	m_factionLogos[0]->setVisible(true);

	m_reinforcementTicketsLabel = cocos2d::LabelTTF::create("$: 50", "fonts/Akashi.ttf", 32, cocos2d::Size(300, 32), cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::BOTTOM);
	this->addChild(m_reinforcementTicketsLabel);
	m_reinforcementTicketsLabel->setPosition(cocos2d::Vec2(175, visibleSize.height - 115));

	m_turnsUntilReinforcementsLabel = cocos2d::LabelTTF::create("BUILD NOW", "fonts/Akashi.ttf", 32, cocos2d::Size(300, 32), cocos2d::TextHAlignment::LEFT, cocos2d::TextVAlignment::BOTTOM);
	m_turnsUntilReinforcementsLabel->setPosition(cocos2d::Vec2(175, visibleSize.height - 145));
	m_turnsUntilReinforcementsLabel->setColor(cocos2d::Color3B::RED);
	this->addChild(m_turnsUntilReinforcementsLabel);
}

bool HudLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	//Create UI
	CreateHud();
	CreatePauseMenu();
	CreateBuildMenu();
	CreateUnitMenu();

	//Create Key Listener
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(HudLayer::onKeyPressed, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void HudLayer::update(float delta)
{
}

void HudLayer::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
	//Escape Key 
	if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE || keyCode == cocos2d::EventKeyboard::KeyCode::KEY_P)
	{

	}
}

void HudLayer::TogglePauseMenu()
{
	Menu* pauseMenu = (Menu*)this->getChildByName("PauseMenu");
	pauseMenu->setVisible(!pauseMenu->isVisible());
	m_pauseBackground->setVisible(!m_pauseBackground->isVisible());
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("buttonClickSound.wav", false, 1.0f, 1.0f, 1.0f);
}

void HudLayer::ToggleBuildMenu()
{
	m_scrollView->setVisible(!m_scrollView->isVisible());
	m_buildBackground->setVisible(!m_buildBackground->isVisible());
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("buttonClickSound.wav", false, 1.0f, 1.0f, 1.0f);
}

void HudLayer::PlayPressed()
{
	auto scene = getParent();
	Game* game = (Game*)scene->getChildByName("GameLayer");
	game->TogglePauseMenu();
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("buttonClickSound.wav", false, 1.0f, 1.0f, 1.0f);
}

void HudLayer::EndTurnPressed(Ref *pSender)
{
	auto scene = getParent();
	Game* game = (Game*)scene->getChildByName("GameLayer");
	m_factionLogos[game->m_currentPlayerID]->setVisible(false);
	game->EndTurn();
	m_factionLogos[game->m_currentPlayerID]->setVisible(true);

	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("buttonClickSound.wav", false, 1.0f, 1.0f, 1.0f);
}

void HudLayer::QuitGame()
{
	auto scene = MainMenu::createScene();
	delete Level::GetInstance();
	Director::getInstance()->replaceScene(scene);
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("buttonClickSound.wav", false, 1.0f, 1.0f, 1.0f);
}

void HudLayer::BuildUnit(Unit::Type type)
{
	auto scene = getParent();
	Game* game = (Game*)scene->getChildByName("GameLayer");
	game->SetUnitTypeSelected(type);
}


void HudLayer::MovePressed()
{
	auto scene = getParent();
	Game* game = (Game*)scene->getChildByName("GameLayer");
	game->BeginUnitMove();
}

void HudLayer::AttackPressed()
{
	auto scene = getParent();
	Game* game = (Game*)scene->getChildByName("GameLayer");
	game->BeginUnitAttack();
}


bool HudLayer::IsPauseMenuVisible()
{
	return m_pauseBackground->isVisible();
}

bool HudLayer::IsBuildMenuVisible()
{
	return m_buildBackground->isVisible();
}


void HudLayer::ToggleUnitMenu(Unit* unit)
{
	m_currentUnit = unit;
	Menu* unitMenu = (Menu*)this->getChildByName("UnitMenu");
	unitMenu->setVisible(!unitMenu->isVisible());
	m_unitBackground->setVisible(!m_unitBackground->isVisible());
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("buttonClickSound.wav", false, 1.0f, 1.0f, 1.0f);

	if (unit != NULL)
	{
		auto scene = getParent();
		Game* game = (Game*)scene->getChildByName("GameLayer");
		Vec2 unitPosInScreenSpace = unit->GetPosition() + game->getPosition();

		unitMenu->setPosition(unitPosInScreenSpace);
		m_unitBackground->setPosition(unitPosInScreenSpace);

		MenuItemImage* moveButton = (MenuItemImage*)unitMenu->getChildByName("MoveButton");
		if (unit->GetMoved())
		{
			moveButton->setEnabled(false);
			//->setColor(cocos2d::Color3B(50, 50, 50));
		}
		else
		{
			moveButton->setEnabled(true);
			//moveButton->setColor(cocos2d::Color3B::WHITE);
		}
	}
}

bool HudLayer::IsUnitMenuVisible()
{
	return m_unitBackground->isVisible();
}

void HudLayer::UpdateLabels(int currentPlayer)
{
	std::string ticketsRemaining = "$: " + std::to_string(PlayerManager::GetInstance()->GetPlayerByID(currentPlayer)->GetTicketsRemaining());
	std::string turnsRemaining = "BUILD IN: " + std::to_string(PlayerManager::GetInstance()->GetPlayerByID(currentPlayer)->GetTurnsTillReinforcements());

	m_reinforcementTicketsLabel->setString(ticketsRemaining);

	if (PlayerManager::GetInstance()->GetPlayerByID(currentPlayer)->GetTurnsTillReinforcements() == 0)
	{
		m_turnsUntilReinforcementsLabel->setString("BUILD NOW");
		m_turnsUntilReinforcementsLabel->setColor(cocos2d::Color3B::RED);
	}
	else
	{
		m_turnsUntilReinforcementsLabel->setString(turnsRemaining);
		m_turnsUntilReinforcementsLabel->setColor(cocos2d::Color3B::WHITE);
	}
}
