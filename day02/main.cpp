#include "./../common/aoc.hpp"
#include <algorithm>
#include <map>
#include <numeric>

class AoC2022_day02 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	uint64_t get_total_score();
	uint64_t get_total_score_strategy();
	std::vector<uint8_t> rounds_;
	std::map<std::string, uint32_t> score_points_;
	std::map<char, uint32_t> choose_points_;
};

bool AoC2022_day02::init(const std::vector<std::string> lines) {
	rounds_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {
		if (lines[i].size() != 3) {
			std::cout << "Invalid game round size at line " << i + 1 << std::endl;
			return false;
		}
		if (((lines[i][0] == 'A') || (lines[i][0] == 'B') || (lines[i][0] == 'C')) && (lines[i][1] == ' ') &&
			((lines[i][2] == 'X') || (lines[i][2] == 'Y') || (lines[i][2] == 'Z'))) {
			rounds_.push_back(((lines[i][0] - 'A') * 10) + (lines[i][2] - 'X'));
		} else {
			std::cout << "Invalid game round content at line " << i + 1 << std::endl;
			return false;
		}
	}

	score_points_.clear();
	choose_points_.clear();

	// order is Oponent, Me
	score_points_["R R"] = 3; // Rock:Rock -> Draw
	score_points_["R P"] = 6; // Rock:Paper -> Win
	score_points_["R S"] = 0; // Rock:Scicssors -> Loss
	score_points_["P S"] = 6; // Paper:Scicssors -> Win
	score_points_["P R"] = 0; // Paper:Rock -> Loss
	score_points_["P P"] = 3; // Paper:Paper -> Draw
	score_points_["S P"] = 0; // Scicssors:Paper -> Loss
	score_points_["S R"] = 6; // Scicssors:Rock -> Win
	score_points_["S S"] = 3; // Scicssors:Scicssors -> Draw

	choose_points_['R'] = 1;
	choose_points_['P'] = 2;
	choose_points_['S'] = 3;

	return true;
}

uint64_t AoC2022_day02::get_total_score() {
	const std::string shapes = "RPS";
	std::string round;
	uint64_t result = 0;
	uint8_t m, o;

	for (size_t i = 0; i < rounds_.size(); i++) {
		round = "   ";

		m = rounds_[i] % 10;
		o = rounds_[i] / 10;
		round[0] = shapes[o];
		round[2] = shapes[m];

		result += score_points_[round];
		result += choose_points_[shapes[m]];
	}

	return result;
}

uint64_t AoC2022_day02::get_total_score_strategy() {
	const std::string shapes = "RPS";
	std::vector<uint8_t> strategy = {0, 3, 6};
	std::string round;
	uint64_t result = 0;
	uint8_t m, o, s;

	for (size_t i = 0; i < rounds_.size(); i++) {
		round = "   ";

		m = rounds_[i] % 10;
		o = rounds_[i] / 10;
		s = strategy[m];
		round[0] = shapes[o];

		for (size_t j = 0; j < shapes.size(); j++) {
			round[2] = shapes[j];
			if (s == score_points_[round]) {
				result += s;
				result += choose_points_[shapes[j]];
				break;
			}
		}
	}

	return result;
}

int32_t AoC2022_day02::get_aoc_day() {
	return 2;
}

int32_t AoC2022_day02::get_aoc_year() {
	return 2022;
}

void AoC2022_day02::tests() {
	uint64_t result;

	if (init({"A Y", "B X", "C Z"})) {
		result = get_total_score();			 // 15
		result = get_total_score_strategy(); // 12
	}
}

bool AoC2022_day02::part1() {
	int64_t result = 0;

	result = get_total_score();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2022_day02::part2() {
	int64_t result = 0;

	result = get_total_score_strategy();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2022_day02 day02;

	return day02.main_execution();
}
