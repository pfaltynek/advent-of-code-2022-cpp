#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <regex>
#include <set>

const std::regex C_DIRECTION_REGEX("^([L|R|D|U]) (\\d+)$");

class AoC2022_day09 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	uint64_t get_tail_positions_count(const bool part2);
	std::vector<std::pair<char, uint32_t>> directions_;
};

bool AoC2022_day09::init(const std::vector<std::string> lines) {
	std::smatch sm;

	directions_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {
		if (std::regex_match(lines[i], sm, C_DIRECTION_REGEX)) {
			directions_.push_back({sm.str(1)[0], static_cast<uint32_t>(stoi(sm.str(2)))});
		} else {
			std::cout << "Invalid direction at line " << i + 1 << std::endl;
			return false;
		}
	}

	return true;
}

uint64_t AoC2022_day09::get_tail_positions_count(const bool part2) {
	std::set<coord_str> positions = {};
	coord_str dir, diff;
	uint32_t size, t_cnt;
	std::vector<coord_str> t = {};

	if (part2) {
		t_cnt = 10;
	} else {
		t_cnt = 2;
	}

	for (size_t i = 0; i < t_cnt; i++) {
		t.push_back({0, 0});
	}

	positions.emplace(t.back());

	for (size_t i = 0; i < directions_.size(); i++) {
		switch (directions_[i].first) {
			case 'D':
				dir = {0, -1};
				break;
			case 'U':
				dir = {0, 1};
				break;
			case 'L':
				dir = {-1, 0};
				break;
			case 'R':
				dir = {1, 0};
				break;
			default:
				return 0;
		}

		for (size_t j = 0; j < directions_[i].second; j++) {

			t[0] = t[0] + dir;

			for (size_t k = 1; k < t_cnt; k++) {

				diff = t[k - 1] - t[k];
				size = diff.size();

				switch (size) {
					case 0:
					case 1:
						break;
					case 2:
						if (diff.x == 0) {
							if (diff.y > 0) {
								t[k].y++;
							} else {
								t[k].y--;
							}
						} else if (diff.y == 0) {
							if (diff.x > 0) {
								t[k].x++;
							} else {
								t[k].x--;
							}
						}
						break;
					case 3:
						if ((diff.x != 0) && (diff.y != 0)) {
							if (abs(diff.x) > abs(diff.y)) {
								diff.x /= 2;
							} else {
								diff.y /= 2;
							}
							t[k] = t[k] + diff;
						} else {
							int u = 2;
						}
						break;
					case 4:
						if (abs(diff.x) == abs(diff.y)) {
							diff.x /= 2;
							diff.y /= 2;
							t[k] = t[k] + diff;
						} else {
							int o = 9;
						}
						break;
					default:
						int x = 1;
						break;
				}
			}

			positions.emplace(t.back());
		}
	}

	return static_cast<uint64_t>(positions.size());
}

int32_t AoC2022_day09::get_aoc_day() {
	return 9;
}

int32_t AoC2022_day09::get_aoc_year() {
	return 2022;
}

void AoC2022_day09::tests() {
	int64_t result;

	if (init({"R 4", "U 4", "L 3", "D 1", "R 4", "D 1", "L 5", "R 2"})) {
		result = get_tail_positions_count(false); // 13
		result = get_tail_positions_count(true);  // 1
	}
	if (init({"R 5", "U 8", "L 8", "D 3", "R 17", "D 10", "L 25", "U 20"})) {
		result = get_tail_positions_count(true); // 36
	}
}

bool AoC2022_day09::part1() {
	int64_t result;

	result = get_tail_positions_count(false);

	result1_ = std::to_string(result);

	return true;
}

bool AoC2022_day09::part2() {
	int64_t result;

	result = get_tail_positions_count(true);

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2022_day09 day09;

	return day09.main_execution();
}
