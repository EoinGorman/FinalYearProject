#include "PauseScene.h"

USING_NS_CC;

Scene* Pause::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();
	auto layer = Pause::create();

	scene->addChild(layer);

	return scene;
}

bool Pause::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto playItem =
		MenuItemImage::create("MainMenuScene/playButtonDefault.png",
		"MainMenuScene/playButtonClicked.png",
		CC_CALLBACK_1(Pause::resume, this));


	auto quitItem =
		MenuItemImage::create("MainMenuScene/quitButtonDefault.png",
		"MainMenuScene/quitButtonClicked.png",
		CC_CALLBACK_1(Pause::activateMainMenuScene, this));

	auto menu = Menu::create(playItem, quitItem, NULL);

	// Returns visible size of OpenGL window in points.
	Size visibleSize = Director::getInstance()->getVisibleSize();
	menu->alignItemsVerticallyWithPadding(visibleSize.height / 3);

	this->addChild(menu);
	return true;
}

void Pause::resume(Ref *pSender)
{
	Director::getInstance()->popScene();
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("buttonClickSound.wav", false, 1.0f, 1.0f, 1.0f);
}

void Pause::activateMainMenuScene(Ref *pSender)
{
	auto scene = MainMenu::createScene();
	Director::getInstance()->popScene();
	Director::getInstance()->replaceScene(scene);
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("buttonClickSound.wav", false, 1.0f, 1.0f, 1.0f);
}

void Pause::retry(Ref *pSender)
{
	auto scene = Game::createScene();
	Director::getInstance()->popScene();
	Director::getInstance()->replaceScene(scene);
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("buttonClickSound.wav", false, 1.0f, 1.0f, 1.0f);
}