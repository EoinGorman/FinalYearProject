#pragma once

#include "cocos2d.h"
#include "GameScene.h"

class MainMenu : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	void activateGameScene(Ref * pSender);
	void quitGame(Ref* sender);

	CREATE_FUNC(MainMenu);
};