#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <regex>
#include <stack>

const std::regex C_REGEX_REARRANGEMENT("^move (\\d+) from (\\d+) to (\\d+)$");

class AoC2022_day05 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::string get_top_crates(const bool part2);
	std::vector<COORD3D> rearrangements_;
	std::vector<std::stack<char>> stacks_;
};

bool AoC2022_day05::init(const std::vector<std::string> lines) {
	std::smatch sm;
	std::vector<size_t> stacks_idxs = {};
	std::stack<std::string> crates = {};
	size_t stack_idx, crates_max;
	bool crates_done = false;
	std::string tmp = {};

	stacks_.clear();
	rearrangements_.clear();

	for (size_t i = 0; i < lines.size(); i++) {
		if (!crates_done) {
			if (lines[i].empty()) {
				if (i == 0) {
					std::cout << "Incomplette crates" << std::endl;
					return false;
				}

				stack_idx = i - 1;
				if (((lines[stack_idx].size() + 1) % 3) != 0) {
					std::cout << "Invalid stack numbers size at line " << stack_idx + 1 << std::endl;
					return false;
				}
				crates_max = (lines[stack_idx].size() + 1) / 4;

				for (size_t j = 0; j < crates_max; j++) {
					if (j != 0) {
						tmp += ' ';
					}

					tmp += ' ';
					stacks_idxs.push_back(tmp.size());
					tmp += '1' + j;
					tmp += ' ';
				}

				if (tmp != lines[stack_idx]) {
					std::cout << "Invalid stack numbers format at line " << stack_idx + 1 << std::endl;
					return false;
				}

				for (size_t c = 0; c < stacks_idxs.size(); c++) {
					stacks_.push_back({});
					for (int16_t j = stack_idx - 1; j >= 0; j--) {
						char ch = lines[j][stacks_idxs[c]];
						if (ch == ' ') {
							break;
						} else {
							stacks_[c].push(ch);
						}
					}
				}

				crates_done = true;
			}
		} else {
			if (std::regex_match(lines[i], sm, C_REGEX_REARRANGEMENT)) {
				coord_3d_str rearrangement;

				rearrangement.x = static_cast<int32_t>(std::stoi(sm.str(1)));
				rearrangement.y = static_cast<int32_t>(std::stoi(sm.str(2)));
				rearrangement.z = static_cast<int32_t>(std::stoi(sm.str(3)));

				if ((rearrangement.y > static_cast<int32_t>(stacks_.size())) || (rearrangement.z > static_cast<int32_t>(stacks_.size()))) {
					std::cout << "Invalid stack number in rearrangement at line " << i + 1 << std::endl;
					return false;
				} else {
					rearrangements_.push_back(rearrangement);
				}
			}
		}
	}

	return true;
}

std::string AoC2022_day05::get_top_crates(const bool part2) {
	std::string result = {};
	char tmp;
	std::stack<char> cache;
	std::vector<std::stack<char>> stacks = stacks_;

	if (part2) {
		for (size_t i = 0; i < rearrangements_.size(); i++) {
			cache = {};
			for (int32_t j = 0; j < rearrangements_[i].x; j++) {
				tmp = stacks[rearrangements_[i].y - 1].top();
				stacks[rearrangements_[i].y - 1].pop();
				cache.push(tmp);
			}
			for (int32_t j = 0; j < rearrangements_[i].x; j++) {
				tmp = cache.top();
				cache.pop();
				stacks[rearrangements_[i].z - 1].push(tmp);
			}
		}
	} else {
		for (size_t i = 0; i < rearrangements_.size(); i++) {
			for (int32_t j = 0; j < rearrangements_[i].x; j++) {
				tmp = stacks[rearrangements_[i].y - 1].top();
				stacks[rearrangements_[i].y - 1].pop();
				stacks[rearrangements_[i].z - 1].push(tmp);
			}
		}
	}

	for (size_t i = 0; i < stacks.size(); i++) {
		result += stacks[i].top();
	}

	return result;
}

int32_t AoC2022_day05::get_aoc_day() {
	return 5;
}

int32_t AoC2022_day05::get_aoc_year() {
	return 2022;
}

void AoC2022_day05::tests() {
	std::string res;

	if (init({"    [D]    ", "[N] [C]    ", "[Z] [M] [P]", " 1   2   3 ", "", "move 1 from 2 to 1", "move 3 from 1 to 3", "move 2 from 2 to 1",
			  "move 1 from 1 to 2"})) {
		res = get_top_crates(false); // "CMZ"
		res = get_top_crates(true);	 // "MCD"
	}
}

bool AoC2022_day05::part1() {

	result1_ = get_top_crates(false);

	return true;
}

bool AoC2022_day05::part2() {

	result2_ = get_top_crates(true);

	return true;
}

int main(void) {
	AoC2022_day05 day05;

	return day05.main_execution();
}
