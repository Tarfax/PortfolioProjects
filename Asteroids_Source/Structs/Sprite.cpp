#include "Sprite.h"
#include <TextureCoordinator/TextureCoordinator.h>
#include <iostream>

Sprite* Sprite::CreateSprite(std::string textureId) {
	return TextureCoordinator::GetSpriteFromId(textureId);
}

Sprite::Sprite(SDL_Texture* texture, Vector2 size, std::string textureId)
	: TextureId(textureId), Texture(texture), Size(size), Flip(SDL_FLIP_NONE), Rect({0, 0, (int)size.X, (int)size.Y}) {
}

void Sprite::Destroy() {
	TextureCoordinator::Delete(this);
}