#pragma once

enum class JUDGE_TYPE;

class Score
{
public:
	Score();
	~Score();
	void Add(JUDGE_TYPE type);

private:
	static const int JUDGE_SCORE[3];
	int score;
};