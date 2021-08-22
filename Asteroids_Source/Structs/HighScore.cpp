#include "HighScore.h"
#include "Core/SaveLoadGame.h"

int HighScore::scoresToSave = 5;
std::vector<int> HighScore::scores;

void HighScore::SetScore(int newScore) {
	if (scores.size() > 0) {
		int lowestScore = 99999;
		int index;
		bool foundLowerScore = false;
		for (int i = 0; i < scoresToSave; i++) {
			if (scores[i] < lowestScore) {
				lowestScore = scores[i];
				index = i;
				foundLowerScore = true;
			}
		}

		if (foundLowerScore == true) {
			scores[index] = newScore;
		}
	}
}

void HighScore::LoadScore() { 
	LoadGame loadGame;
	scores = loadGame.scores;
	for (int i = scores.size(); i < scoresToSave; i++)	{
		scores.push_back(0);
	}
}

void HighScore::SaveScore() {
	SaveGame saveGame(scores);
}