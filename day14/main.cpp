#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include "./../common/string-utils.hpp"
#include <map>

const coord_str C_SAND_SOURCE = {500, 0};

class AoC2022_day14 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	uint64_t get_sand_units_count(const bool part2);
	std::map<coord_str, char> map_;
	coord_str min_, max_;
};

bool AoC2022_day14::init(const std::vector<std::string> lines) {
	std::vector<std::string> line, coords;
	std::vector<coord_str> line_coords;
	int32_t from, to, constant;

	map_.clear();
	min_ = {INT32_MAX, INT32_MAX};

	for (size_t i = 0; i < lines.size(); i++) {
		line = split(lines[i], " -> ");

		line_coords.clear();
		for (size_t j = 0; j < line.size(); j++) {
			coords = split(line[j], ",");

			if (coords.size() == 2) {
				line_coords.push_back({static_cast<int32_t>(std::stoi(coords[0])), static_cast<int32_t>(std::stoi(coords[1]))});
			} else {
				std::cout << "Invalid coordinate at line " << i + 1 << " coordinate at position " << j + 1 << std::endl;
				return false;
			}
		}

		for (size_t j = 1; j < line_coords.size(); j++) {
			if (line_coords[j - 1].x == line_coords[j].x) {
				constant = line_coords[j - 1].x;
				from = std::min(line_coords[j - 1].y, line_coords[j].y);
				to = std::max(line_coords[j - 1].y, line_coords[j].y);

				for (int32_t k = from; k <= to; k++) {
					map_[{constant, k}] = '#';
				}

				min_.y = std::min(from, min_.y);
				max_.y = std::max(to, max_.y);
			} else if (line_coords[j - 1].y == line_coords[j].y) {
				constant = line_coords[j - 1].y;
				from = std::min(line_coords[j - 1].x, line_coords[j].x);
				to = std::max(line_coords[j - 1].x, line_coords[j].x);

				for (int32_t k = from; k <= to; k++) {
					map_[{k, constant}] = '#';
				}

				min_.x = std::min(from, min_.x);
				max_.x = std::max(to, max_.x);
			} else {
				std::cout << "Invalid coordinate definitions at line " << i + 1 << std::endl;
				return false;
			}
		}
	}

	return true;
}

uint64_t AoC2022_day14::get_sand_units_count(const bool part2) {
	uint64_t result = 0;
	bool finished = false, fall = true;
	coord_str sand;
	std::map<coord_str, char> map(map_);

	if (part2) {
		for (int32_t k = min_.x - 1000; k <= max_.x + 1000; k++) {
			map[{k, max_.y + 2}] = '#';
		}
	}

	while (!finished) {
		sand = C_SAND_SOURCE;
		fall = true;

		while (fall) {
			if (map.count({sand.x, sand.y + 1}) == 0) {
				sand.y++;
			} else if (map.count({sand.x - 1, sand.y + 1}) == 0) {
				sand.x--;
				sand.y++;
			} else if (map.count({sand.x + 1, sand.y + 1}) == 0) {
				sand.x++;
				sand.y++;
			} else {
				map[sand] = 'o';
				fall = false;
				result++;

				if ((part2) && (sand == C_SAND_SOURCE)) {
					finished = true;
					break;
				}
			}

			if (!part2 && (sand.y > max_.y)) {
				finished = true;
				break;
			}
		}
	}

	return result;
}

int32_t AoC2022_day14::get_aoc_day() {
	return 14;
}

int32_t AoC2022_day14::get_aoc_year() {
	return 2022;
}

void AoC2022_day14::tests() {
	int64_t result;

	if (init({"498,4 -> 498,6 -> 496,6", "503,4 -> 502,4 -> 502,9 -> 494,9"})) {
		result = get_sand_units_count(false); // 24
		result = get_sand_units_count(true);  // 93
	}
}

bool AoC2022_day14::part1() {
	int64_t result;

	result = get_sand_units_count(false);

	result1_ = std::to_string(result);

	return true;
}

bool AoC2022_day14::part2() {
	int64_t result;

	result = get_sand_units_count(true);

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2022_day14 day14;

	return day14.main_execution();
}
