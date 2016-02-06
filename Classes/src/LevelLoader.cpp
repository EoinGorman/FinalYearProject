#include "LevelLoader.h"
#include "PlayerManager.h"

////////////////////////////////////////////////////////////
LevelLoader::LevelLoader() 
{

}

////////////////////////////////////////////////////////////
void LevelLoader::loadJSONData(std::string const & filename)
{
	m_JSONData.clear();

	auto fileUtils = cocos2d::FileUtils::getInstance();
	// Loads all the data in the file into a std::string.
	m_JSONData = fileUtils->getStringFromFile(filename);
}


////////////////////////////////////////////////////////////
void LevelLoader::loadGameData()
{

}


void LevelLoader::LoadLevel(std::string levelName)
{
	loadJSONData("data/" + levelName + ".json");
	m_document.Parse<0>(m_JSONData.c_str());
	
	//Get pointer to GameData singleton
	std::shared_ptr<GameData> ptr = GameData::sharedGameData();

	// Get value of "background_texture" 
	ptr->m_backgroundTextureFile = m_document["Background"].GetString();

	//Set the width and height of the level
	ptr->m_width = m_document["Width"].GetInt();
	ptr->m_height = m_document["Height"].GetInt();

	//Get size of tile
	ptr->m_tileSize = m_document["TileSize"].GetDouble();

	//Get the level Terrain
	const Value& terrainArray = m_document["Terrain"];
	ptr->m_levelTerrain.clear();
	ptr->m_levelTerrain = std::vector<int>(terrainArray.Size());
	// Iterate through the array.
	for (int i = 0; i < terrainArray.Size(); i++) {
		ptr->m_levelTerrain[i] = terrainArray[i].GetInt();
	}
	//Reverse vector because coordinate systems of level file and cocos are different
	//std::reverse(ptr->m_levelTerrain.begin(), ptr->m_levelTerrain.end());.

	//Get the level objects
	int hqCount = 0;
	const Value& objectsArray = m_document["Objects"];
	ptr->m_levelObjects.clear();
	ptr->m_levelObjects = std::vector<int>(objectsArray.Size());
	// Iterate through the array.
	for (int i = 0; i < objectsArray.Size(); i++) {
		ptr->m_levelObjects[i] = objectsArray[i].GetInt();
		if (objectsArray[i].GetInt() == 1)
		{
			hqCount++;
		}
	}

	//Setup number of players
	PlayerManager::GetInstance()->SetNumberOfPlayers(hqCount);

	//Reverse vector because coordinate systems of level file and cocos are different
	//std::reverse(ptr->m_levelObjects.begin(), ptr->m_levelObjects.end());
}
