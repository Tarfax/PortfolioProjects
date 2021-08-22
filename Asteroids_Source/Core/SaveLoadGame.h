#pragma once
#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <vector>

class SaveGame {
public:
	SaveGame(std::vector<int> scores) {
		std::ofstream outputFile;

		outputFile.open("res/HighScore/highscore.txt");
		if (outputFile.fail() == true) {
			perror("res/HighScore/highscore.txt");
			openSuccess = false;
			return;
		}

		for (int i = 0; i < scores.size(); i++) {
			outputFile << scores[i] << std::endl;
		}

		outputFile.close();
	}

	bool openSuccess;
};

class LoadGame {
public:
	LoadGame() {
		std::ifstream inputFile;
		inputFile.open("res/HighScore/highscore.txt");

		if (inputFile.fail() == true) {
			perror("res/HighScore/highscore.txt");
			readSuccess = false;
			return;
		}

		std::string inputString;
		while (getline(inputFile, inputString)) {
			scores.push_back(std::stoi(inputString));
		}

		inputFile.close();
	}

	bool readSuccess;
	std::vector<int> scores;
};