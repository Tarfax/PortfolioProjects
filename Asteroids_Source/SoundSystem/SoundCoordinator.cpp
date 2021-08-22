#include "SoundCoordinator.h"

SoundCoordinator* SoundCoordinator::instance;

int SoundCoordinator::effectVolume = 30;
int SoundCoordinator::musicVolume = 30;

SoundCoordinator::SoundCoordinator() { }

void SoundCoordinator::Init() {
	GetInstance()->iInit();
}

void SoundCoordinator::iInit() {
	Mix_Init(MIX_INIT_MOD | MIX_INIT_OPUS);
	if (Mix_OpenAudio(Freq, MIX_DEFAULT_FORMAT, 2, ChunkSize) < 0) {
		std::cout << Mix_GetError() << std::endl;
	}
	Mix_AllocateChannels(10);
}

void SoundCoordinator::PlayMusic(std::string id) {
	GetInstance()->iPlayMusic(id);
}

void SoundCoordinator::iPlayMusic(std::string id) {
	if (Mix_PlayMusic(musicMap[id], -1) == -1) {
		std::cout << Mix_GetError() << ": " << id << std::endl;
	}
}

void SoundCoordinator::LoadMusic(std::string id) {
	Mix_Music* music = Mix_LoadMUS(id.c_str());
	if (music == nullptr) {
		std::cout << Mix_GetError() << ": " << id << std::endl;
		return;
	}
	GetInstance()->musicMap[id] = music;
}

void SoundCoordinator::PlayEffect(std::string id) {
	GetInstance()->iPlayEffect(id);
}

void SoundCoordinator::iPlayEffect(std::string id) {
	if (effectMap.count(id) != 1) {
		LoadEffect(id);
	}
	int channel;
	if (channel = Mix_PlayChannel(effectMapChannel[effectMap[id]], effectMap[id], 0) == -1) {
		std::cout << Mix_GetError() << ": " << id << std::endl;
	}
}

void SoundCoordinator::LoadEffect(std::string id) {
	Mix_Chunk* effect = Mix_LoadWAV(id.c_str());
	Mix_VolumeChunk(effect, effectVolume);
	if (effect == nullptr) {
		std::cout << Mix_GetError() << ": " << id << std::endl;
		return;
	}
	GetInstance()->effectMap[id] = effect;
	GetInstance()->effectMapChannel[effect] = GetInstance()->effectsLoaded++;
}

void SoundCoordinator::Destroy() {

	std::map<std::string, Mix_Music*>::iterator it;
	for (it = GetInstance()->musicMap.begin(); it != GetInstance()->musicMap.end(); it++) {
		Mix_FreeMusic(it->second);
	}

	std::map<std::string, Mix_Chunk*>::iterator it2;
	for (it2 = GetInstance()->effectMap.begin(); it2 != GetInstance()->effectMap.end(); it2++) {
		Mix_FreeChunk(it2->second);
	}
	GetInstance()->musicMap.clear();
	GetInstance()->effectMap.clear();
	GetInstance()->effectMapChannel.clear();
}

void SoundCoordinator::SetMusicVolume(int newVolume) {
	musicVolume = newVolume;
	Mix_VolumeMusic(newVolume);
}

void SoundCoordinator::SetEffectVolume(int newVolume) {
	effectVolume = newVolume;
	GetInstance()->iSetEffectVolume(newVolume);
}

void SoundCoordinator::iSetEffectVolume(int newVolume) {
	std::map<std::string, Mix_Chunk*>::iterator it2;
	for (it2 = GetInstance()->effectMap.begin(); it2 != GetInstance()->effectMap.end(); it2++) {
		Mix_VolumeChunk(it2->second, newVolume);
	}
}
