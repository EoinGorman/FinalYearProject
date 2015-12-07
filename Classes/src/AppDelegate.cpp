#include "AppDelegate.h"
#include "MainMenuScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {
}

AppDelegate::~AppDelegate() {
}

bool AppDelegate::applicationDidFinishLaunching() {
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if (!glview) {
		glview = GLViewImpl::create("Hello World");
		glview->setFrameSize(640, 480);
		director->setOpenGLView(glview);
	}

	auto fileUtils = FileUtils::getInstance();
	std::vector<std::string> searchPath;

	// Need to add our "data" path too.
	searchPath.push_back("data");
	fileUtils->setSearchPaths(searchPath);

	// Create the GameData singleton.
	m_ptr = GameData::sharedGameData();
	// Create the level loader object to load general game data.
	//LevelLoader loader("data/GeneralGameData.json");

	auto scene = MainMenu::createScene();
	director->runWithScene(scene);

	return true;
}

void AppDelegate::applicationDidEnterBackground() {
}

void AppDelegate::applicationWillEnterForeground() {
}
