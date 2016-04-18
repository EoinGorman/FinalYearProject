#pragma once

#include "cocos2d.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"  
#include "Player.h"

class CharacterSelectScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	// Called when resume is selected.
	void backToMain(Ref *pSender);
	void allCharactersChosen(Ref *pSender);

	void update(float) override;

	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);

	CREATE_FUNC(CharacterSelectScene);
private:
	std::vector<std::string> m_characterLogoPaths;
	std::vector<std::string> m_characterLogoClickedPaths;
	std::vector<std::string> m_characterNames;
	std::vector<std::string> m_characterDescriptions;
	std::vector<Player::Faction> m_factionsChosen;

	void CharacterSelectScene::SetCharacterLogoPaths();
	void CharacterSelectScene::SetCharacterNames();
	void CharacterSelectScene::SetCharacterDescriptions();

	void CharacterSelectScene::LeftArrowClicked();
	void CharacterSelectScene::RightArrowClicked();

	cocos2d::Sprite* m_confirmationPopUp;
	void CharacterSelectScene::CreateConfirmationPopUp();
	void CharacterSelectScene::ToggleConfirmPopUp(Ref *pSender);
	void CharacterSelectScene::CharacterConfirmed(Ref *pSender);

	bool m_arrowClicked;
	int m_itemMoveDirection;	//Either be 1 or -1 and just multiply this in the moving equation

	int m_characterIndex;	//the index of the currently displayed character
	int m_chosenCount;	//Keeps track of how many characters have been chosen so far
	cocos2d::Menu* m_characterItem;
	cocos2d::Menu* CharacterSelectScene::CreateCharacterItem(int index);

	cocos2d::LabelTTF* m_currentPlayerChoosing;

	void onEnterTransitionDidFinish();
};