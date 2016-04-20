#include "CharacterSelectScene.h"

USING_NS_CC;

#define ScrollSpeed 750.0f
#define FadeOutSpeed 450.0f

Scene* CharacterSelectScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();
	auto layer = CharacterSelectScene::create();

	scene->addChild(layer);

	return scene;
}

bool CharacterSelectScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	m_arrowClicked = false;
	m_chosenCount = 0;

	//Set up Key Listeners
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(CharacterSelectScene::onKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(CharacterSelectScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	// Returns visible size of OpenGL window in points.
	Size visibleSize = Director::getInstance()->getVisibleSize();

	//Set all data needed to create UI for each level
	SetCharacterLogoPaths();
	SetCharacterNames();
	SetCharacterDescriptions(); 
	
	m_characterItem = CreateCharacterItem(0);

	auto* sceneTitle = cocos2d::LabelTTF::create("-SELECT FACTION-", "fonts/Akashi.ttf", 64, cocos2d::Size(visibleSize.width, 64), cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::TOP);
	sceneTitle->setColor(cocos2d::Color3B(68, 67, 72));
	sceneTitle->setPositionX(visibleSize.width / 2);
	sceneTitle->setPositionY(visibleSize.height - sceneTitle->getBoundingBox().size.height / 2);

	m_currentPlayerChoosing = cocos2d::LabelTTF::create("PLAYER 1", "fonts/Akashi.ttf", 48, cocos2d::Size(visibleSize.width, 48), cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::TOP);
	m_currentPlayerChoosing->setColor(cocos2d::Color3B(68, 67, 72));
	m_currentPlayerChoosing->setPositionX(visibleSize.width / 2);
	m_currentPlayerChoosing->setPositionY(visibleSize.height - (sceneTitle->getBoundingBox().size.height / 2) - (m_currentPlayerChoosing->getBoundingBox().size.height));

	//Add arrow buttons
	auto rightArrow =
		MenuItemImage::create("Hud/rightArrowDefault.png",
		"Hud/rightArrowCLicked.png",
		CC_CALLBACK_0(CharacterSelectScene::RightArrowClicked, this));


	auto leftArrow =
		MenuItemImage::create("Hud/leftArrowDefault.png",
		"Hud/leftArrowCLicked.png",
		CC_CALLBACK_0(CharacterSelectScene::LeftArrowClicked, this));

	auto arrowButtons = Menu::create(leftArrow, rightArrow, NULL);
	arrowButtons->alignItemsHorizontallyWithPadding(m_characterItem->getBoundingBox().size.width / 2);

	//Add Background
	auto backgroundSprite = cocos2d::Sprite::create("Hud/blankBackground.png");
	//auto backgroundSprite = cocos2d::Sprite::create("MainMenuScene\frame_0_delay.png");
	backgroundSprite->setPosition(cocos2d::Point((visibleSize.width / 2), (visibleSize.height / 2)));
	//Set scale of background to match height of screen
	backgroundSprite->setScaleX(visibleSize.width / backgroundSprite->getContentSize().width);
	backgroundSprite->setScaleY(visibleSize.height / backgroundSprite->getContentSize().height);

	this->addChild(backgroundSprite, 0);
	this->addChild(sceneTitle, 0);
	this->addChild(m_currentPlayerChoosing, 0);

	this->addChild(m_characterItem, 0);

	this->addChild(arrowButtons, 1);

	CreateConfirmationPopUp();

	this->scheduleUpdate();

	return true;
}

/*
void LevelSelectScene::resume(Ref *pSender)
{
	Game* game = (Game*)previousScene->getChildByName("GameLayer");
	game->StartTurn();
	Director::getInstance()->replaceScene(TransitionTurnOffTiles::create(0.5f, previousScene));
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("buttonClickSound.wav", false, 1.0f, 1.0f, 1.0f);
}
*/

void CharacterSelectScene::update(float delta)
{
	if (m_arrowClicked)
	{
		//check if offscreen
		if (m_characterItem->getOpacity() - (FadeOutSpeed * delta) <= 0)
		{
			m_arrowClicked = false;

			m_characterIndex += m_itemMoveDirection;
			if (m_characterIndex < 0)
			{
				m_characterIndex = m_characterNames.size() - 1;
			}
			else if (m_characterIndex >= m_characterNames.size())
			{
				m_characterIndex = 0;
			}

			this->removeChild(m_characterItem);

			m_characterItem = CreateCharacterItem(m_characterIndex);

			this->addChild(m_characterItem, 0);
		}
		else
		{
			//move levelItem
			m_characterItem->setPositionX(m_characterItem->getPosition().x + ((ScrollSpeed * delta) * m_itemMoveDirection));
			m_characterItem->setOpacity(m_characterItem->getOpacity() - (FadeOutSpeed * delta));
		}
	}
}

void CharacterSelectScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
	if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		backToMain(this);
	}
}

void CharacterSelectScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{

}

//Public functions
void CharacterSelectScene::backToMain(Ref *pSender)
{
	auto scene = MainMenu::createScene();
	Director::getInstance()->replaceScene(TransitionFadeTR::create(1.0f, scene));
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("buttonClickSound.wav", false, 1.0f, 1.0f, 1.0f);
}

void CharacterSelectScene::allCharactersChosen(Ref *pSender)
{
	//Set the factions that have been chosen
	Level::GetInstance()->SetFactionsChosen(m_factionsChosen);

	auto scene = Game::createScene();
	Director::getInstance()->replaceScene(TransitionFadeBL::create(1.0f, scene));
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("buttonClickSound.wav", false, 1.0f, 1.0f, 1.0f);
}

//Private Functions
void CharacterSelectScene::LeftArrowClicked()
{
	if (!m_arrowClicked)
	{
		m_arrowClicked = true;
		m_itemMoveDirection = -1;
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("buttonClickSound.wav", false, 1.0f, 1.0f, 1.0f);
	}
	else
	{
		//play negative sound effect
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("wrongSelectionBeep.wav", false, 1.0f, 1.0f, 1.0f);
	}
}

void CharacterSelectScene::RightArrowClicked()
{
	if (!m_arrowClicked)
	{
		m_arrowClicked = true;
		m_itemMoveDirection = 1;
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("buttonClickSound.wav", false, 1.0f, 1.0f, 1.0f);
	}
	else
	{
		//play negative sound effect
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("wrongSelectionBeep.wav", false, 1.0f, 1.0f, 1.0f);
	}
}

void CharacterSelectScene::ToggleConfirmPopUp(Ref *pSender)
{
	if (!m_arrowClicked)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("buttonClickSound.wav", false, 1.0f, 1.0f, 1.0f);
		m_characterItem->setEnabled(m_confirmationPopUp->isVisible());
		m_confirmationPopUp->setVisible(!m_confirmationPopUp->isVisible());
	}
	else
	{
		//play negative sound effect
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("wrongSelectionBeep.wav", false, 1.0f, 1.0f, 1.0f);
	}
}

void CharacterSelectScene::CreateConfirmationPopUp()
{
	// Returns visible size of OpenGL window in points.
	Size visibleSize = Director::getInstance()->getVisibleSize();

	//Add Background
	m_confirmationPopUp = cocos2d::Sprite::create("Hud/confirmationPopUp.png");
	m_confirmationPopUp->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	m_confirmationPopUp->setVisible(false);

	// create a menu item
	auto confirmItem = MenuItemImage::create("Hud/tickIcon.png", "Hud/tickIconClicked.png",
		CC_CALLBACK_1(CharacterSelectScene::CharacterConfirmed, this));

	auto cancelItem = MenuItemImage::create("Hud/xIcon.png", "Hud/xIconClicked.png",
		CC_CALLBACK_1(CharacterSelectScene::ToggleConfirmPopUp, this));

	auto menu = Menu::create(confirmItem, cancelItem, NULL);
	menu->setVisible(true);
	menu->alignItemsHorizontallyWithPadding(35);
	menu->setPosition(m_confirmationPopUp->getBoundingBox().size.width / 2, 75);

	m_confirmationPopUp->addChild(menu);

	this->addChild(m_confirmationPopUp, 1);
}

void CharacterSelectScene::CharacterConfirmed(Ref* pSender)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("buttonClickSound.wav", false, 1.0f, 1.0f, 1.0f);
	//select character
	if (m_characterNames[m_characterIndex] == "Red")
	{
		m_factionsChosen.push_back(Player::Faction::red);
	}
	else if (m_characterNames[m_characterIndex] == "Blue")
	{
		m_factionsChosen.push_back(Player::Faction::blue);
	}
	else if (m_characterNames[m_characterIndex] == "Green")
	{
		m_factionsChosen.push_back(Player::Faction::green);
	}
	else
	{
		m_factionsChosen.push_back(Player::Faction::yellow);
	}

	m_chosenCount++;

	if (m_chosenCount >= Level::GetInstance()->GetNumberOfHQs())
	{
		allCharactersChosen(this);
	}
	else
	{
		//close pop up
		ToggleConfirmPopUp(this);

		//Remove chosen character from list
		RemoveCharacterFromLists(m_characterIndex);

		//Set selection wheel to beggining
		m_characterIndex = 0;
		this->removeChild(m_characterItem);
		m_characterItem = CreateCharacterItem(m_characterIndex);
		this->addChild(m_characterItem, 0);
		m_currentPlayerChoosing->setString("Player " + std::to_string(m_chosenCount + 1));
	}
}

cocos2d::Menu* CharacterSelectScene::CreateCharacterItem(int index)
{
	m_characterIndex = index;
	auto* label = cocos2d::LabelTTF::create(m_characterNames[index], "fonts/Akashi.ttf", 32, cocos2d::Size(250, 64), cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::TOP);
	auto levelname = MenuItemLabel::create(label);
	levelname->setDisabledColor(cocos2d::Color3B(68, 67, 72));
	levelname->setEnabled(false);

	label = cocos2d::LabelTTF::create(m_characterDescriptions[index], "fonts/Akashi.ttf", 32, cocos2d::Size(500, 100), cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::TOP);
	auto levelDescription = MenuItemLabel::create(label);
	levelDescription->setDisabledColor(cocos2d::Color3B(68, 67, 72));
	levelDescription->setEnabled(false);

	auto levelPreview =
		MenuItemImage::create(m_characterLogoPaths[index],
		m_characterLogoClickedPaths[index],
		CC_CALLBACK_1(CharacterSelectScene::ToggleConfirmPopUp, this));

	auto levelitem = Menu::create(levelPreview, levelname, levelDescription, NULL);

	// Returns visible size of OpenGL window in points.
	Size visibleSize = Director::getInstance()->getVisibleSize();
	levelitem->alignItemsVerticallyWithPadding(0);
	levelitem->setPositionY(225);

	return levelitem;
}


void CharacterSelectScene::SetCharacterLogoPaths()
{
	m_characterLogoPaths = std::vector<std::string>();
	m_characterLogoClickedPaths = std::vector<std::string>();
	
	//Red
	m_characterLogoPaths.push_back("FactionPreviews/FactionPreview0.png");
	m_characterLogoClickedPaths.push_back("FactionPreviews/FactionPreviewClicked0.png");

	//Blue
	m_characterLogoPaths.push_back("FactionPreviews/FactionPreview1.png");
	m_characterLogoClickedPaths.push_back("FactionPreviews/FactionPreviewClicked1.png");

	//Green
	m_characterLogoPaths.push_back("FactionPreviews/FactionPreview2.png");
	m_characterLogoClickedPaths.push_back("FactionPreviews/FactionPreviewClicked2.png");

	//Yellow
	m_characterLogoPaths.push_back("FactionPreviews/FactionPreview3.png");
	m_characterLogoClickedPaths.push_back("FactionPreviews/FactionPreviewClicked3.png");
}

void CharacterSelectScene::SetCharacterNames()
{
	m_characterNames = std::vector<std::string>();

	m_characterNames.push_back("Red");
	m_characterNames.push_back("Blue");
	m_characterNames.push_back("Green");
	m_characterNames.push_back("Yellow");
}

void CharacterSelectScene::SetCharacterDescriptions()
{
	m_characterDescriptions = std::vector<std::string>();

	m_characterDescriptions.push_back("- Red Faction\nThey Are Red. :(");
	m_characterDescriptions.push_back("- Blue Faction\nThey Are Blue. :)");
	m_characterDescriptions.push_back("- Green Faction\nThey Are Green. ;)");
	m_characterDescriptions.push_back("- Yellow Faction\nThey Are Yellow. -_-");
}

void CharacterSelectScene::RemoveCharacterFromLists(int index)
{
	//m_units.erase(std::remove(m_units.begin(), m_units.end(), unit));
	m_characterLogoPaths.erase(std::remove(m_characterLogoPaths.begin(), m_characterLogoPaths.end(), m_characterLogoPaths[index]));
	m_characterLogoClickedPaths.erase(std::remove(m_characterLogoClickedPaths.begin(), m_characterLogoClickedPaths.end(), m_characterLogoClickedPaths[index]));
	m_characterNames.erase(std::remove(m_characterNames.begin(), m_characterNames.end(), m_characterNames[index]));
	m_characterDescriptions.erase(std::remove(m_characterDescriptions.begin(), m_characterDescriptions.end(), m_characterDescriptions[index]));
}

void CharacterSelectScene::onEnterTransitionDidFinish()
{

}