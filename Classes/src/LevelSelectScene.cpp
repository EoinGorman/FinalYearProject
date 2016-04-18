#include "LevelSelectScene.h"
#include "CharacterSelectScene.h"

USING_NS_CC;

#define ScrollSpeed 750.0f
#define FadeOutSpeed 450.0f

Scene* LevelSelectScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();
	auto layer = LevelSelectScene::create();

	scene->addChild(layer);

	return scene;
}

bool LevelSelectScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	m_arrowClicked = false;

	//Set up Key Listeners
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(LevelSelectScene::onKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(LevelSelectScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	// Returns visible size of OpenGL window in points.
	Size visibleSize = Director::getInstance()->getVisibleSize();

	//Set all data needed to create UI for each level
	SetLevelPreviewPaths();
	SetLevelNames();
	SetLevelDescriptions();
	SetLevelPlayerCount();

	m_levelItem = CreateLevelItem(0);

	auto* sceneTitle = cocos2d::LabelTTF::create("-SELECT LEVEL-", "fonts/Akashi.ttf", 64, cocos2d::Size(visibleSize.width, 64), cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::TOP);
	sceneTitle->setColor(cocos2d::Color3B(68, 67, 72));
	sceneTitle->setPositionX(visibleSize.width / 2);
	sceneTitle->setPositionY(visibleSize.height - sceneTitle->getBoundingBox().size.height / 2);

	//Add arrow buttons
	auto rightArrow =
		MenuItemImage::create("Hud/rightArrowDefault.png",
		"Hud/rightArrowCLicked.png",
		CC_CALLBACK_0(LevelSelectScene::RightArrowClicked, this));


	auto leftArrow =
		MenuItemImage::create("Hud/leftArrowDefault.png",
		"Hud/leftArrowCLicked.png",
		CC_CALLBACK_0(LevelSelectScene::LeftArrowClicked, this));

	auto arrowButtons = Menu::create(leftArrow, rightArrow, NULL);
	arrowButtons->alignItemsHorizontallyWithPadding(m_levelItem->getBoundingBox().size.width/2);

	//Add Background
	auto backgroundSprite = cocos2d::Sprite::create("Hud/blankBackground.png");
	//auto backgroundSprite = cocos2d::Sprite::create("MainMenuScene\frame_0_delay.png");
	backgroundSprite->setPosition(cocos2d::Point((visibleSize.width / 2), (visibleSize.height / 2)));
	//Set scale of background to match height of screen
	backgroundSprite->setScaleX(visibleSize.width / backgroundSprite->getContentSize().width);
	backgroundSprite->setScaleY(visibleSize.height / backgroundSprite->getContentSize().height);

	this->addChild(backgroundSprite, 0);
	this->addChild(sceneTitle, 0);

	this->addChild(m_levelItem, 0);

	this->addChild(arrowButtons, 1);

	CreateConfirmationPopUp();

	this->scheduleUpdate();

	return true;
}

/*
void LevelSelectScene::resume(Ref *pSender)
{
	Game* game = (Game*)previousScene->getChildByName("GameLayer");
	game->StartTurn();
	Director::getInstance()->replaceScene(TransitionTurnOffTiles::create(0.5f, previousScene));
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("buttonClickSound.wav", false, 1.0f, 1.0f, 1.0f);
}
*/

void LevelSelectScene::update(float delta)
{
	if (m_arrowClicked)
	{
		//check if offscreen
		if (m_levelItem->getOpacity() - (FadeOutSpeed * delta) <= 0)
		{
			m_arrowClicked = false;

			m_levelNumber += m_itemMoveDirection;
			if (m_levelNumber < 0)
			{
				m_levelNumber = m_levelNames.size() - 1;
			}
			else if (m_levelNumber >= m_levelNames.size())
			{
				m_levelNumber = 0;
			}

			this->removeChild(m_levelItem);

			m_levelItem = CreateLevelItem(m_levelNumber);

			this->addChild(m_levelItem, 0);
		}
		else
		{
			//move levelItem
			m_levelItem->setPositionX(m_levelItem->getPosition().x + ((ScrollSpeed * delta) * m_itemMoveDirection));
			m_levelItem->setOpacity(m_levelItem->getOpacity() - (FadeOutSpeed * delta));
		}
	}
}

void LevelSelectScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
	if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		backToMain(this);
	}
}

void LevelSelectScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{

}

//Public functions
void LevelSelectScene::backToMain(Ref *pSender)
{
	auto scene = MainMenu::createScene();
	Director::getInstance()->replaceScene(TransitionFadeTR::create(1.0f, scene));
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("buttonClickSound.wav", false, 1.0f, 1.0f, 1.0f);
}

void LevelSelectScene::levelChosen(Ref *pSender)
{
	//Sets level to be loaded
	std::string levelName = "Level" + std::to_string(m_levelNumber);
	Level::GetInstance()->SetLevelToLoad(levelName);
	Level::GetInstance()->SetNumberOfHQs(m_levelPlayerCount[m_levelNumber]);

	//Moves to Character Select Scene
	auto scene = CharacterSelectScene::createScene();
	Director::getInstance()->replaceScene(TransitionFadeBL::create(1.0f, scene));
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("buttonClickSound.wav", false, 1.0f, 1.0f, 1.0f);
}

//Private Functions
void LevelSelectScene::LeftArrowClicked()
{
	if (!m_arrowClicked)
	{
		m_arrowClicked = true;
		m_itemMoveDirection = -1;
	}
	else
	{
		//play negative sound effect
	}
}

void LevelSelectScene::RightArrowClicked()
{
	if (!m_arrowClicked)
	{
		m_arrowClicked = true;
		m_itemMoveDirection = 1;
	}
	else
	{
		//play negative sound effect
	}
}

void LevelSelectScene::ToggleConfirmPopUp(Ref *pSender)
{
	if (!m_arrowClicked)
	{
		m_levelItem->setEnabled(m_confirmationPopUp->isVisible());
		m_confirmationPopUp->setVisible(!m_confirmationPopUp->isVisible());
	}
}

void LevelSelectScene::CreateConfirmationPopUp()
{
	// Returns visible size of OpenGL window in points.
	Size visibleSize = Director::getInstance()->getVisibleSize();

	//Add Background
	m_confirmationPopUp = cocos2d::Sprite::create("Hud/confirmationPopUp.png");
	m_confirmationPopUp->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	m_confirmationPopUp->setVisible(false);

	// create a menu item
	auto confirmItem = MenuItemImage::create("Hud/tickIcon.png", "Hud/tickIconClicked.png",
		CC_CALLBACK_1(LevelSelectScene::LevelConfirmed, this));

	auto cancelItem = MenuItemImage::create("Hud/xIcon.png", "Hud/xIconClicked.png",
		CC_CALLBACK_1(LevelSelectScene::ToggleConfirmPopUp, this));

	auto menu = Menu::create(confirmItem, cancelItem, NULL);
	menu->setVisible(true);
	menu->alignItemsHorizontallyWithPadding(35);
	menu->setPosition(m_confirmationPopUp->getBoundingBox().size.width / 2, 75);

	m_confirmationPopUp->addChild(menu);

	this->addChild(m_confirmationPopUp, 1);
}

void LevelSelectScene::LevelConfirmed(Ref* pSender)
{
	levelChosen(this);
}

cocos2d::Menu* LevelSelectScene::CreateLevelItem(int index)
{
	m_levelNumber = index;
	auto* label = cocos2d::LabelTTF::create(m_levelNames[index], "fonts/Akashi.ttf", 32, cocos2d::Size(250, 64), cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::TOP);
	auto levelname = MenuItemLabel::create(label);
	levelname->setDisabledColor(cocos2d::Color3B(68, 67, 72));
	levelname->setEnabled(false);

	label = cocos2d::LabelTTF::create(m_levelDescriptions[index], "fonts/Akashi.ttf", 32, cocos2d::Size(500, 100), cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::TOP);
	auto levelDescription = MenuItemLabel::create(label);
	levelDescription->setDisabledColor(cocos2d::Color3B(68, 67, 72));
	levelDescription->setEnabled(false);

	auto levelPreview =
		MenuItemImage::create(m_levelPreviewPaths[index],
		m_levelPreviewClickedPaths[index],
		CC_CALLBACK_1(LevelSelectScene::ToggleConfirmPopUp, this));

	auto levelitem = Menu::create(levelPreview, levelname, levelDescription, NULL);

	// Returns visible size of OpenGL window in points.
	Size visibleSize = Director::getInstance()->getVisibleSize();
	levelitem->alignItemsVerticallyWithPadding(0);
	levelitem->setPositionY(225);

	return levelitem;
}


void LevelSelectScene::SetLevelPreviewPaths()
{
	m_levelPreviewPaths = std::vector<std::string>();
	m_levelPreviewClickedPaths = std::vector<std::string>();
	
	m_levelPreviewPaths.push_back("LevelPreviews/LevelPreview0.png");
	m_levelPreviewClickedPaths.push_back("LevelPreviews/LevelPreviewClicked0.png");

	m_levelPreviewPaths.push_back("LevelPreviews/LevelPreview1.png");
	m_levelPreviewClickedPaths.push_back("LevelPreviews/LevelPreviewClicked1.png");

	m_levelPreviewPaths.push_back("LevelPreviews/LevelPreview2.png");
	m_levelPreviewClickedPaths.push_back("LevelPreviews/LevelPreviewClicked2.png");
	//....
	//....
	//....
	//etc.
}

void LevelSelectScene::SetLevelNames()
{
	m_levelNames = std::vector<std::string>();

	m_levelNames.push_back("Bare Bones");

	m_levelNames.push_back("Triple Threat");

	m_levelNames.push_back("Fever Pitch");
	//....
	//....
	//....
	//etc.
}

void LevelSelectScene::SetLevelDescriptions()
{
	m_levelDescriptions = std::vector<std::string>();

	m_levelDescriptions.push_back("- Two Player Map.\n- Size = Small.\n");

	m_levelDescriptions.push_back("- Three Player Map.\n- Size = Large.\n");

	m_levelDescriptions.push_back("- Two Player Map.\n- Size = Large.\n");
	//....
	//....
	//....
	//etc.
}

void LevelSelectScene::SetLevelPlayerCount()
{
	m_levelPlayerCount = std::vector<int>();

	m_levelPlayerCount.push_back(2);

	m_levelPlayerCount.push_back(3);

	m_levelPlayerCount.push_back(2);
	//....
	//....
	//....
	//etc.
}

void LevelSelectScene::onEnterTransitionDidFinish()
{

}