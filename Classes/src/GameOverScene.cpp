#include "GameOverScene.h"

USING_NS_CC;

Scene* GameOverScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();
	auto layer = GameOverScene::create();

	scene->addChild(layer);

	return scene;
}

bool GameOverScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	// Returns visible size of OpenGL window in points.
	Size visibleSize = Director::getInstance()->getVisibleSize();

	//Add Background
	auto backgroundSprite = cocos2d::Sprite::create("GameOverScene/frame_0_delay.png");
	//auto backgroundSprite = cocos2d::Sprite::create("MainMenuScene\frame_0_delay.png");
	backgroundSprite->setPosition(cocos2d::Point((visibleSize.width / 2), (visibleSize.height / 2)));
	//Set scale of background to match height of screen
	backgroundSprite->setScaleX(visibleSize.height / backgroundSprite->getContentSize().height);
	backgroundSprite->setScaleY(visibleSize.height / backgroundSprite->getContentSize().height);

	//Create and run animation
	Vector<SpriteFrame*> animFrames(24);
	char str[100] = { 0 };
	for (int i = 0; i < 24; i++)
	{
		sprintf(str, "GameOverScene/frame_%0d_delay.png", i);
		auto frame = SpriteFrame::create(str, Rect(0, 0, 800, 336)); //we assume that the sprites' dimentions are 40*40 rectangles.
		animFrames.pushBack(frame);
	}
	auto animation = Animation::createWithSpriteFrames(animFrames, 0.15f, INFINITE);
	auto animate = Animate::create(animation);

	backgroundSprite->runAction(animate);
	this->addChild(backgroundSprite);

	//Add title
	auto* sceneTitle = cocos2d::LabelTTF::create("VICTORY!!!", "fonts/Akashi.ttf", 128, cocos2d::Size(visibleSize.width, 128), cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::TOP);
	sceneTitle->setColor(cocos2d::Color3B(255, 255, 255));
	sceneTitle->setPositionX(visibleSize.width / 2);
	sceneTitle->setPositionY(visibleSize.height - sceneTitle->getBoundingBox().size.height / 2);

	//Add button
	auto backToMenu =
		MenuItemImage::create("GameOverScene/returnToMenuButtonDefault.png",
		"GameOverScene/returnToMenuButtonClicked.png",
		CC_CALLBACK_1(GameOverScene::activateMainMenuScene, this));

	auto menu = Menu::create(backToMenu, NULL);
	menu->alignItemsVerticallyWithPadding(0);
	//menu->setPositionX(visibleSize.width / 2);
	//menu->setPositionY((visibleSize.height / 2) - menu->getBoundingBox().size.height);

	this->addChild(sceneTitle);
	this->addChild(menu);

	return true;
}

void GameOverScene::activateMainMenuScene(Ref *pSender)
{
	auto scene = MainMenu::createScene();
	Director::getInstance()->popScene();
	delete Level::GetInstance();
	Director::getInstance()->replaceScene(TransitionFadeTR::create(1.0f, scene));
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("buttonClickSound.wav", false, 1.0f, 1.0f, 1.0f);
}