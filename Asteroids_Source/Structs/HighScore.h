#include <vector>

class HighScore {

public:
	static void SetScore(int newScore);
	static void LoadScore();
	static void SaveScore();
	static std::vector<int> GetScores() { return scores; }

	static int scoresToSave;
	static std::vector<int> scores;
};