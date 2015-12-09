#include "MainMenuScene.h"

USING_NS_CC;

Scene* MainMenu::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();
	auto layer = MainMenu::create();

	scene->addChild(layer);

	return scene;
}

bool MainMenu::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto playItem =
		MenuItemImage::create("MainMenuScene/playButtonDefault.png",
		"MainMenuScene/playButtonClicked.png",
		CC_CALLBACK_1(MainMenu::activateGameScene, this));


	auto quitItem =
		MenuItemImage::create("MainMenuScene/quitButtonDefault.png",
		"MainMenuScene/quitButtonClicked.png",
		CC_CALLBACK_1(MainMenu::quitGame, this));

	auto menu = Menu::create(playItem, quitItem, NULL);

	// Returns visible size of OpenGL window in points.
	Size visibleSize = Director::getInstance()->getVisibleSize(); 
	menu->alignItemsVerticallyWithPadding(visibleSize.height / 3);


	//Add Background
	auto backgroundSprite = cocos2d::Sprite::create("MainMenuScene/gameTitle.png");
	backgroundSprite->setPosition(cocos2d::Point((visibleSize.width / 2), (visibleSize.height / 2)));
	//Set scale of background to match height of screen
	backgroundSprite->setScaleX(visibleSize.height / backgroundSprite->getContentSize().height);
	backgroundSprite->setScaleY(visibleSize.height / backgroundSprite->getContentSize().height);

	this->addChild(backgroundSprite);
	this->addChild(menu);
	 
	return true;
}

void MainMenu::activateGameScene(Ref *pSender)
{
	auto scene = Game::createScene(); 
	Director::getInstance()->replaceScene(TransitionFlipAngular::create(1.0, scene));
}

void MainMenu::quitGame(Ref* pSender)
{
	Director::getInstance()->end();
}