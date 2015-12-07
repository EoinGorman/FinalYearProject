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

	auto label = Label::createWithSystemFont("Pause Scene", "Arial", 96);
	label->setAnchorPoint(cocos2d::Vec2(0.5, 0.5));
	Vec2 pos = label->getPosition();
	label->setPosition(100, 1);
	this->addChild(label, 1);

	return true;
}

void Pause::resume(Ref *pSender)
{
	Director::getInstance()->popScene();
}

void Pause::activateMainMenuScene(Ref *pSender)
{
	auto scene = MainMenu::createScene();
	Director::getInstance()->popScene();
	Director::getInstance()->replaceScene(scene);
}

void Pause::retry(Ref *pSender)
{
	auto scene = Game::createScene();
	Director::getInstance()->popScene();
	Director::getInstance()->replaceScene(scene);
}