#include "../dodf/MemoryPool.h"
#include "../dodf/Assert.h"

#include "TextureMgr.h"

#include <iostream>
#include <fstream>      // std::ofstream

#include "ScoreMgr.h"

using namespace dodf;

void ScoreMgr::addScore(ScoreID scoreID)
{
	m_currentScore += m_scores[scoreID];
}

void ScoreMgr::init()
{
	m_scores[ScoreID::ENEMY] = 10;
	m_scores[ScoreID::OBSTACLE] = 50;
	m_filename = "highscore.bin";
	loadHighScore();
}

void ScoreMgr::setHighScore(uint32_t highScore)
{
	m_highScore = highScore;
}

void ScoreMgr::saveHighScore()
{
	if (m_currentScore > m_highScore) {
		std::ofstream file(m_filename, std::ios::out | std::ios::binary);
		if (file.is_open()) {
			file.write(reinterpret_cast<const char *>(&m_currentScore), sizeof(m_currentScore));
			file.close();
		}
	}
}

void ScoreMgr::loadHighScore()
{
	std::ifstream file(m_filename, std::ios::in | std::ios::binary);
	if (file.is_open()) {
		file.read(reinterpret_cast<char *>(&m_highScore), sizeof(m_highScore));
		file.close();
	}
}
