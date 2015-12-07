#ifndef GAMEDATA_H
#define GAMEDATA_H

#include <string>
#include <memory>
#include <vector>
////////////////////////////////////////////////////////////
/// \brief A class that stores gameplay data
/// 
////////////////////////////////////////////////////////////


class GameData {
private:
	//Constructor
	GameData();

	static std::weak_ptr<GameData> m_instance;

	friend std::weak_ptr < GameData >;
	friend std::shared_ptr < GameData >;
public:
	//Get instance of singleton
	static std::shared_ptr<GameData> sharedGameData();

	//Level Related fields
	int m_width;
	int m_height;
	float m_tileSize;
	std::string m_backgroundTextureFile;
	std::vector<int> m_levelTerrain;
	std::vector<int> m_levelObjects;
};

#endif