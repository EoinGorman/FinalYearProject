#pragma once

#include "cocos2d.h"
//#include "MainMenuScene.h"

class HudLayer : public cocos2d::Layer
{
public:
    virtual bool init();
	void HudLayer::EndTurnPressed(Ref *pSender);

	CREATE_FUNC(HudLayer);
};