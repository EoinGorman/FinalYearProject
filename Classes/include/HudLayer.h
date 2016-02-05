#pragma once

#include "cocos2d.h"
#include "ui\CocosGUI.h"
#include "Unit.h"

//#include "MainMenuScene.h"

class HudLayer : public cocos2d::Layer
{
public:
	virtual bool init();
	void update(float) override;
	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);

	void HudLayer::PlayPressed();
	void HudLayer::EndTurnPressed(Ref *pSender);
	void HudLayer::QuitGame();

	void HudLayer::TogglePauseMenu();
	bool HudLayer::IsPauseMenuVisible();

	void HudLayer::ToggleBuildMenu();
	bool HudLayer::IsBuildMenuVisible();

	CREATE_FUNC(HudLayer);

private:
	void HudLayer::CreatePauseMenu();
	void HudLayer::CreateBuildMenu();
	void HudLayer::CreateHud();
	void HudLayer::BuildUnit(Unit::Type);
	std::vector<cocos2d::Sprite*> m_factionLogos;
	cocos2d::Sprite* m_ticketFrame;

	cocos2d::Sprite* m_pauseBackground;
	cocos2d::Sprite* m_buildBackground;
	cocos2d::ui::ScrollView* m_scrollView;
};