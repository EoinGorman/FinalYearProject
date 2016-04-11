#pragma once

#include "cocos2d.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"  

class LevelSelectScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	// Called when resume is selected.
	void backToMain(Ref *pSender);
	void levelChosen(Ref *pSender);

	void update(float) override;

	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);

	CREATE_FUNC(LevelSelectScene);
private:
	std::vector<std::string> m_levelPreviewPaths;
	std::vector<std::string> m_levelPreviewClickedPaths;
	std::vector<std::string> m_levelNames;
	std::vector<std::string> m_levelDescriptions;

	void LevelSelectScene::SetLevelPreviewPaths();
	void LevelSelectScene::SetLevelNames();
	void LevelSelectScene::SetLevelDescriptions();

	void LevelSelectScene::LeftArrowClicked();
	void LevelSelectScene::RightArrowClicked();

	cocos2d::Sprite* m_confirmationPopUp;
	void LevelSelectScene::CreateConfirmationPopUp();
	void LevelSelectScene::ToggleConfirmPopUp(Ref *pSender);
	void LevelSelectScene::LevelConfirmed(Ref *pSender);

	int m_levelNumber;
	cocos2d::Menu* m_levelItem;
	cocos2d::Menu* LevelSelectScene::CreateLevelItem(int index);

	void onEnterTransitionDidFinish();
};