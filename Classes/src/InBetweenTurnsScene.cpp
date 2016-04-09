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

	Game* game = (Game*)previousScene->getChildByName("GameLayer");
	HudLayer* hud = (HudLayer*)previousScene->getChildByName("HudLayer");

	m_nextTeamLogo = cocos2d::Sprite::createWithTexture(hud->GetLogo(game->m_currentPlayerID)->getTexture());
	m_nextTeamLogo->setScaleX(2.0f);
	m_nextTeamLogo->setScaleY(2.0f);

	m_label = cocos2d::LabelTTF::create("NEXT PLAYERS TURN", "fonts/Akashi.ttf", 32, cocos2d::Size(250, 100), cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
	auto label = MenuItemLabel::create(m_label);
	//label->setDisabledColor(cocos2d::Color3B(68, 67, 72));
	label->setEnabled(false);
	auto playItem =
		MenuItemImage::create("Hud/playButtonPauseDefault.png",
		"MainMenuScene/playButtonClicked.png",
		CC_CALLBACK_1(InBetweenTurnsScene::resume, this));

	auto menu = Menu::create(playItem, label, NULL);

	// Returns visible size of OpenGL window in points.
	Size visibleSize = Director::getInstance()->getVisibleSize(); 
	menu->alignItemsVerticallyWithPadding(visibleSize.height / 5);

	m_nextTeamLogo->setPosition(visibleSize.width/2, visibleSize.height/2);

	//Add Background
	auto backgroundSprite = cocos2d::Sprite::create("Hud/blankBackground.png");
	//auto backgroundSprite = cocos2d::Sprite::create("MainMenuScene\frame_0_delay.png");
	backgroundSprite->setPosition(cocos2d::Point((visibleSize.width / 2), (visibleSize.height / 2)));
	//Set scale of background to match height of screen
	backgroundSprite->setScaleX(visibleSize.width / backgroundSprite->getContentSize().width);
	backgroundSprite->setScaleY(visibleSize.height / backgroundSprite->getContentSize().height);

	this->addChild(backgroundSprite);
	this->addChild(m_nextTeamLogo);
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