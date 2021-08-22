#include "TextureCoordinator.h"
#include <stdio.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>

TextureCoordinator* TextureCoordinator::instance = nullptr;

TextureCoordinator::TextureCoordinator() { }

void TextureCoordinator::Init(SDL_Renderer* renderer) {
	TTF_Init();
	GetInstance()->renderer = renderer;
}

Sprite* TextureCoordinator::LoadTexture(std::string textureId) {

	SDL_Surface* surface = IMG_Load(textureId.c_str());
	if (surface == nullptr) {
		printf("ERROR: TextureCoordinator::LoadTexture - surface: %s", SDL_GetError());
		return nullptr;
	}

	Vector2 size = Vector2(surface->w, surface->h);

	SDL_Texture* texture = SDL_CreateTextureFromSurface(GetInstance()->renderer, surface);

	if (texture == nullptr) {
		printf("ERROR: TextureCoordinator::LoadTexture - texture: %s", SDL_GetError());
		return nullptr;
	}

	SDL_FreeSurface(surface);

	Sprite* sprite = new Sprite(texture, size, textureId);
	//Create()->textures[textureId] = texture;

	return sprite;
}

Sprite* TextureCoordinator::GetSpriteFromId(std::string textureId) {

	if (GetInstance()->sprites.count(textureId) == 1) {
		GetInstance()->refcount[GetInstance()->sprites[textureId]]++;
		return GetInstance()->sprites[textureId];
	}

	Sprite* sprite = LoadTexture(textureId);

	if (sprite == nullptr) {
		printf("ERROR: TextureCoordinator::GetSpriteFromId - sprite is nullptr");
		return nullptr;
	}

	GetInstance()->sprites[textureId] = sprite;
	GetInstance()->refcount[sprite]++;

	return sprite;
}

Sprite* TextureCoordinator::GetSpriteFromText(std::string text, std::string fontId, SDL_Color color, int size) {
	int hashCode = HashCode(text, fontId, size, color);
	if (GetInstance()->fonts.count(hashCode) == 1) {
		GetInstance()->refcount[GetInstance()->fonts[hashCode]]++;
		//std::cout << "TextureCoordinator::GetText reuse of "  << fontId << std::endl;

		return GetInstance()->fonts[hashCode];
	}

	TTF_Font* font = TTF_OpenFont(fontId.c_str(), size);
	SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(GetInstance()->renderer, surface);

	Sprite* sprite = new Sprite(texture, Vector2(surface->w, surface->h), fontId);
	sprite->IsText = true;
	GetInstance()->fonts[hashCode] = sprite;
	GetInstance()->fontHash[sprite] = hashCode;

	TTF_CloseFont(font);
	SDL_FreeSurface(surface);

	GetInstance()->refcount[sprite]++;

	return sprite;
}

void TextureCoordinator::Delete(Sprite* sprite) {
	if (GetInstance()->refcount.count(sprite) == 1) {
		GetInstance()->refcount[sprite]--;
	}
}

void TextureCoordinator::Destroy() {
	std::map<Sprite*, int>::iterator it;
	for (it = instance->refcount.begin(); it != instance->refcount.end(); ++it)	{
		SDL_DestroyTexture(it->first->Texture);
		delete it->first;
	}
	instance->sprites.clear();
	instance->fonts.clear();
	instance->fontHash.clear();
	instance->refcount.clear();
}

//Just a simple hash to store this specific text
//I know it can give same result if r g b are switched around. Not an issue for this small game though
int TextureCoordinator::HashCode(const std::string& msg, const std::string& id, int size, SDL_Color color) {
	int hash = 0;

	for (size_t i = 0; i < msg.size(); ++i) {
		hash = hash * 31 + static_cast<int>(msg[i]);
	}

	for (size_t i = 0; i < id.size(); ++i) {
		hash = hash * 31 + static_cast<int>(id[i]);
	}

	hash = hash + size + color.r + color.g + color.b + color.a;
	return hash;
}