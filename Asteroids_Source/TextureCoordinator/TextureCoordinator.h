#pragma once
#include <string>
#include <SDL.h>
#include <Structs/Vector2.h>
#include <Structs/Sprite.h>
#include <map>
#include <vector>

class TextureCoordinator {

public: 
	static void Init(SDL_Renderer* renderer);
	static Sprite* GetSpriteFromId(std::string textureId);
	static Sprite* GetSpriteFromText(std::string text, std::string fontId, SDL_Color color, int size);
	static void Delete(Sprite* texture);
	static void Destroy();

private:
	TextureCoordinator();
	static TextureCoordinator* instance;
	static TextureCoordinator* GetInstance() {
		return instance = (instance != nullptr) ? instance : new TextureCoordinator();
	}

	static Sprite* LoadTexture(std::string textureId);

	SDL_Renderer* renderer;

	std::map<std::string, Sprite*> sprites;

	std::map<int, Sprite*> fonts;
	std::map<Sprite*, int> fontHash;
	
	std::map<Sprite*, int> refcount;

	static int HashCode(const std::string& msg, const std::string& id, int size, SDL_Color color);

};