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

	auto menuTitle =
		MenuItemImage::create("MainMenuScene/Game_Title.png",
		"MainMenuScene/Game_Title.png");
	auto playItem =
		MenuItemImage::create("MainMenuScene/Play_Button.png",
		"MainMenuScene/Play_Button(Click).png",
		CC_CALLBACK_1(MainMenu::activateGameScene, this));
	auto menu = Menu::create(menuTitle, playItem, NULL);

	// Returns visible size of OpenGL window in points.
	Size visibleSize = Director::getInstance()->getVisibleSize(); 
	menu->alignItemsVerticallyWithPadding(visibleSize.height / 4);
	this->addChild(menu);

	return true;
}

void MainMenu::activateGameScene(Ref *pSender)
{
	auto scene = Game::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(1.0, scene));
}