#include "InBetweenTurnsScene.h"

USING_NS_CC;
Scene* previousScene;

Scene* InBetweenTurnsScene::createScene(Scene* fromScene)
{
	previousScene = fromScene;
	// 'scene' is an autorelease object
	auto scene = Scene::create();
	auto layer = InBetweenTurnsScene::create();

	scene->addChild(layer);

	return scene;
}

bool InBetweenTurnsScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	m_label = cocos2d::LabelTTF::create("NEXT PLAYERS TURN", "fonts/Akashi.ttf", 32, cocos2d::Size(250, 100), cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
	auto label = MenuItemLabel::create(m_label);
	label->setEnabled(false);
	auto playItem =
		MenuItemImage::create("MainMenuScene/playButtonDefault.png",
		"MainMenuScene/playButtonClicked.png",
		CC_CALLBACK_1(InBetweenTurnsScene::resume, this));

	auto menu = Menu::create(playItem, label, NULL);

	// Returns visible size of OpenGL window in points.
	Size visibleSize = Director::getInstance()->getVisibleSize(); 
	menu->alignItemsVerticallyWithPadding(visibleSize.height / 5);

	//Add Background
	auto backgroundSprite = cocos2d::Sprite::create("MainMenuScene/frame_0_delay.png");
	//auto backgroundSprite = cocos2d::Sprite::create("MainMenuScene\frame_0_delay.png");
	backgroundSprite->setPosition(cocos2d::Point((visibleSize.width / 2), (visibleSize.height / 2)));
	//Set scale of background to match height of screen
	backgroundSprite->setScaleX(visibleSize.height / backgroundSprite->getContentSize().height);
	backgroundSprite->setScaleY(visibleSize.height / backgroundSprite->getContentSize().height);

	this->addChild(backgroundSprite);
	this->addChild(menu);
	 
	return true;
}

void InBetweenTurnsScene::resume(Ref *pSender)
{
	Game* game = (Game*)previousScene->getChildByName("GameLayer");
	game->StartTurn();
	Director::getInstance()->replaceScene(TransitionTurnOffTiles::create(0.5f, previousScene));
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("buttonClickSound.wav", false, 1.0f, 1.0f, 1.0f);
}

void InBetweenTurnsScene::onEnterTransitionDidFinish()
{

}