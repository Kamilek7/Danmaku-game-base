#ifndef GAME_CONSTANTS_H_
#define GAME_CONSTANTS_H_
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <string>

class AnimatedSprite
{
	const int framerate = 12;
	bool repeat;
	int frameNum;
	int currentFrame;
	float timer;
	float scale;
	float enTimer;
	std::string baseName;
	std::vector <sf::Texture> baseTextures;
public:
	sf::Sprite baseSprite;
	AnimatedSprite operator=(const AnimatedSprite& temp);
	AnimatedSprite(int _frameNum = 1, std::string _baseName = "anim", float _scale = 1.0, bool _repeat = true);
	void play(float dt);
	void playE(float enemTime);
	void reset() { this->currentFrame = 0; }
};
class Animation
{
	std::string entityName;
	float scale;
public:
	AnimatedSprite idle;
	AnimatedSprite left;
	AnimatedSprite right;
	Animation(std::string _entityName="reimu", float _scale=1.6);
	Animation operator=(const Animation& temp) { return Animation(temp.entityName, temp.scale); }
};
#endif