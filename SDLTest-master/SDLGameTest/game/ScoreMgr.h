#pragma once

#include "Vec.h"

#include <stdint.h>
#include <array>

using namespace std;

class ScoreMgr  
{

public:
	enum ScoreID: uint32_t {
		ENEMY,
		OBSTACLE,
		SIZE
	};
	
	inline uint32_t getHighScore() const { return m_highScore; }
	inline uint32_t getCurrentScore() const { return m_currentScore; }	

	void addScore(ScoreID scoreID);
	void init();
	void setHighScore(uint32_t highScore);
	void saveHighScore();
private:

	uint32_t m_highScore = 0;
	uint32_t m_currentScore = 0;
	std::array<uint32_t, ScoreID::SIZE> m_scores;
	std::string m_filename;

	void loadHighScore();
};

