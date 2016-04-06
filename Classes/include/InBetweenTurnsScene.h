#pragma once

#include "cocos2d.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"  

class InBetweenTurnsScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene(cocos2d::Scene* fromScene);
	virtual bool init();

	// Called when resume is selected.
	void resume(Ref *pSender);

	CREATE_FUNC(InBetweenTurnsScene);
private:
	cocos2d::LabelTTF* m_label;
	void onEnterTransitionDidFinish();
};