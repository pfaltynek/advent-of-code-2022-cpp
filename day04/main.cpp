#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <regex>

const std::regex C_REGEX_RANGES("^(\\d+)-(\\d+),(\\d+)-(\\d+)$");

class AoC2022_day04 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	uint64_t get_encapsulated_ranges_count();
	uint64_t get_intersection_ranges_count();
	std::vector<COORD4D> ranges_;
};

bool AoC2022_day04::init(const std::vector<std::string> lines) {
	std::smatch sm;

	ranges_.clear();

	for (size_t i = 0; i < lines.size(); i++) {
		coord_4d_str ranges;

		if (std::regex_match(lines[i], sm, C_REGEX_RANGES)) {
			ranges.x = static_cast<int32_t>(std::stoi(sm.str(1)));
			ranges.y = static_cast<int32_t>(std::stoi(sm.str(2)));
			ranges.z = static_cast<int32_t>(std::stoi(sm.str(3)));
			ranges.w = static_cast<int32_t>(std::stoi(sm.str(4)));

			if ((ranges.x > ranges.y) || (ranges.z > ranges.w)) {
				std::cout << "Invalid ranges definition at line " << i + 1;
				return false;
			}

			ranges_.push_back(ranges);
		} else {
			std::cout << "Invalid ranges definition at line " << i + 1;
			return false;
		}
	}

	return true;
}

uint64_t AoC2022_day04::get_encapsulated_ranges_count() {
	uint64_t result = 0;

	for (size_t i = 0; i < ranges_.size(); i++) {
		if (((ranges_[i].x >= ranges_[i].z) && (ranges_[i].y <= ranges_[i].w)) || ((ranges_[i].x <= ranges_[i].z) && (ranges_[i].y >= ranges_[i].w))) {
			result++;
		}
	}

	return result;
}

uint64_t AoC2022_day04::get_intersection_ranges_count() {
	uint64_t result = 0;

	for (size_t i = 0; i < ranges_.size(); i++) {
		if (!(((ranges_[i].x > ranges_[i].z) && (ranges_[i].x > ranges_[i].w)) || ((ranges_[i].x < ranges_[i].z) && (ranges_[i].y < ranges_[i].z))))
			result++;
	}

	return result;
}

int32_t AoC2022_day04::get_aoc_day() {
	return 4;
}

int32_t AoC2022_day04::get_aoc_year() {
	return 2022;
}

void AoC2022_day04::tests() {
	uint64_t result;

	if (init({"2-4,6-8", "2-3,4-5", "5-7,7-9", "2-8,3-7", "6-6,4-6", "2-6,4-8"})) {
		result = get_encapsulated_ranges_count(); // 2
		result = get_intersection_ranges_count(); // 4
	}
}

bool AoC2022_day04::part1() {
	int64_t result = 0;

	result = get_encapsulated_ranges_count();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2022_day04::part2() {
	int64_t result = 0;

	result = get_intersection_ranges_count();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2022_day04 day04;

	return day04.main_execution();
}
