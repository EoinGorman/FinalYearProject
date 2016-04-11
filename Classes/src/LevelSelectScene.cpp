#include "LevelSelectScene.h"

USING_NS_CC;

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

	m_levelItem = CreateLevelItem(0);

	auto* sceneTitle = cocos2d::LabelTTF::create("-SELECT LEVEL-", "fonts/Akashi.ttf", 64, cocos2d::Size(visibleSize.width, 64), cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::TOP);
	sceneTitle->setColor(cocos2d::Color3B(68, 67, 72));
	sceneTitle->setPositionX(visibleSize.width / 2);
	sceneTitle->setPositionY(visibleSize.height - sceneTitle->getBoundingBox().size.height / 2);

	//Add Background
	auto backgroundSprite = cocos2d::Sprite::create("Hud/blankBackground.png");
	//auto backgroundSprite = cocos2d::Sprite::create("MainMenuScene\frame_0_delay.png");
	backgroundSprite->setPosition(cocos2d::Point((visibleSize.width / 2), (visibleSize.height / 2)));
	//Set scale of background to match height of screen
	backgroundSprite->setScaleX(visibleSize.width / backgroundSprite->getContentSize().width);
	backgroundSprite->setScaleY(visibleSize.height / backgroundSprite->getContentSize().height);

	this->addChild(backgroundSprite);
	this->addChild(sceneTitle);

	this->addChild(m_levelItem);

	CreateConfirmationPopUp();
	 
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
	std::string levelName = "Level" + std::to_string(m_levelNumber);
	Level::GetInstance()->SetLevelToLoad(levelName);

	auto scene = Game::createScene();
	Director::getInstance()->replaceScene(TransitionFadeBL::create(1.0f, scene));
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("buttonClickSound.wav", false, 1.0f, 1.0f, 1.0f);
}

//Private Functions
void LevelSelectScene::LeftArrowClicked()
{

}

void LevelSelectScene::RightArrowClicked()
{

}

void LevelSelectScene::ToggleConfirmPopUp(Ref *pSender)
{
	m_levelItem->setEnabled(m_confirmationPopUp->isVisible());
	m_confirmationPopUp->setVisible(!m_confirmationPopUp->isVisible());
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

	this->addChild(m_confirmationPopUp);
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
		m_levelPreviewClickedPaths[0],
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
	//....
	//....
	//....
	//etc.
}

void LevelSelectScene::SetLevelNames()
{
	m_levelNames = std::vector<std::string>();

	m_levelNames.push_back("Test Level");
	//....
	//....
	//....
	//etc.
}

void LevelSelectScene::SetLevelDescriptions()
{
	m_levelDescriptions = std::vector<std::string>();

	m_levelDescriptions.push_back("- Two Player Map.\n- Size = Small.\n");
	//....
	//....
	//....
	//etc.
}

void LevelSelectScene::onEnterTransitionDidFinish()
{

}