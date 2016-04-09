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
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("MainMenuScene/MenuTheme0.wav", true);

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
	auto backgroundSprite = cocos2d::Sprite::create("MainMenuScene/frame_0_delay.png");
	//auto backgroundSprite = cocos2d::Sprite::create("MainMenuScene\frame_0_delay.png");
	backgroundSprite->setPosition(cocos2d::Point((visibleSize.width / 2), (visibleSize.height / 2)));
	//Set scale of background to match height of screen
	backgroundSprite->setScaleX(visibleSize.height / backgroundSprite->getContentSize().height);
	backgroundSprite->setScaleY(visibleSize.height / backgroundSprite->getContentSize().height);

	//Create and run animation
	Vector<SpriteFrame*> animFrames(8);
	char str[100] = { 0 };
	for (int i = 0; i < 8; i++)
	{
		sprintf(str, "MainMenuScene/frame_%0d_delay.png", i);
		auto frame = SpriteFrame::create(str, Rect(0, 0, 800, 336)); //we assume that the sprites' dimentions are 40*40 rectangles.
		animFrames.pushBack(frame);
	}
	auto animation = Animation::createWithSpriteFrames(animFrames, 0.15f, INFINITE);
	auto animate = Animate::create(animation);

	backgroundSprite->runAction(animate);
	this->addChild(backgroundSprite);
	this->addChild(menu);
	 
	return true;
}

void MainMenu::activateGameScene(Ref *pSender)
{
	auto scene = LevelSelectScene::createScene(); 
	Director::getInstance()->replaceScene(TransitionFadeBL::create(1.0f, scene));
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("buttonClickSound.wav", false, 1.0f, 1.0f, 1.0f);
}

void MainMenu::quitGame(Ref* pSender)
{
	Director::getInstance()->end();
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("buttonClickSound.wav", false, 1.0f, 1.0f, 1.0f);
}