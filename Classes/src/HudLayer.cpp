#include "HudLayer.h"
#include "GameScene.h"
USING_NS_CC;

bool HudLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	//Create UI
	// creating a menu with a single item

	// create a menu item
    auto endTurnItem = MenuItemImage::create("MainMenuScene/playButtonDefault.png", "MainMenuScene/playButtonClicked.png",
            CC_CALLBACK_1(HudLayer::EndTurnPressed, this));

	auto menu = Menu::create(endTurnItem, NULL);
	this->addChild(menu, 1);
	return true;
}

void HudLayer::EndTurnPressed(Ref *pSender)
{
	auto scene = getParent();
	Game* game = (Game*)scene->getChildByName("GameLayer");
	game->EndTurn();
}