#include "HudLayer.h"
#include "GameScene.h"
#include "PlayerManager.h"

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
		auto testButton = ui::Button::create();
		testButton->setTouchEnabled(true);
		testButton->setPosition(Vec2(m_scrollView->getPosition().x, m_scrollView->getBottomBoundary() + (i * 100)));
		std::string defaultTex = "Hud/UnitNames/UnitNamesDefault" + std::to_string(count) + ".png";
		std::string clickedTex = "Hud/UnitNames/UnitNamesClicked" + std::to_string(count) + ".png";
		testButton->loadTextures(defaultTex, clickedTex);

		testButton->addClickEventListener(CC_CALLBACK_0(HudLayer::BuildUnit, this, Unit::Type(count)));
		m_scrollView->addChild(testButton);
		m_scrollView->setVisible(false);
		count--;
	}

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

	m_ticketFrame = cocos2d::Sprite::create("Hud/ticketHolder.png");
	m_ticketFrame->setPosition(cocos2d::Point(visibleSize.width / 2,
		-m_ticketFrame->getContentSize().height / 2 + visibleSize.height + offsetFromCorner.y));	//Set pos to top left corner
	this->addChild(m_ticketFrame);
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


bool HudLayer::IsPauseMenuVisible()
{
	return m_pauseBackground->isVisible();
}

bool HudLayer::IsBuildMenuVisible()
{
	return m_buildBackground->isVisible();
}


void HudLayer::ToggleUnitMenu()
{

}

bool HudLayer::IsUnitMenuVisible()
{
	return false;
}
