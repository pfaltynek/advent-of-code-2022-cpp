#include "./../common/aoc.hpp"

class AoC2022_day03 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	uint64_t get_priorities_sum();
	uint64_t get_priorities_sum_by_badge();
	uint64_t get_priority(const char item);
	std::vector<std::string> contents_;
};

bool AoC2022_day03::init(const std::vector<std::string> lines) {
	contents_ = lines;

	return true;
}

uint64_t AoC2022_day03::get_priority(const char item) {
	if ((item <= 'z') && (item >= 'a')) {
		return item - 'a' + 1;
	} else if ((item <= 'Z') && (item >= 'A')) {
		return item - 'A' + 27;
	} else {
		return 0;
	}
}

uint64_t AoC2022_day03::get_priorities_sum() {
	std::string c1, c2;
	uint64_t result = 0, len;
	size_t pos;

	for (size_t i = 0; i < contents_.size(); i++) {
		len = contents_[i].size() / 2;
		c1 = contents_[i].substr(0, len);
		c2 = contents_[i].substr(len);

		pos = c1.find_first_of(c2);
		if (pos != std::string::npos) {
			result += get_priority(c1[pos]);
		} else {
			int u = 2;
		}
	}

	return result;
}

uint64_t AoC2022_day03::get_priorities_sum_by_badge() {
	uint64_t result = 0;

	for (size_t i = 0; i < contents_.size(); i += 3) {
		for (size_t j = 0; j < contents_[i].size(); j++) {
			if (contents_[i + 1].find(contents_[i][j]) != std::string::npos) {
				if (contents_[i + 2].find(contents_[i][j]) != std::string::npos) {
					result += get_priority(contents_[i][j]);
					break;
				}
			}
		}
	}

	return result;
}

int32_t AoC2022_day03::get_aoc_day() {
	return 3;
}

int32_t AoC2022_day03::get_aoc_year() {
	return 2022;
}

void AoC2022_day03::tests() {
	uint64_t result;

	if (init({"vJrwpWtwJgWrhcsFMMfFFhFp", "jqHRNqRjqzjGDLGLrsFMfFZSrLrFZsSL", "PmmdzqPrVvPwwTWBwg", "wMqvLMZHhHMvwLHjbvcjnnSBnvTQFn", "ttgJtRGJQctTZtZT",
			  "CrZsJsPPZsGzwwsLwLmpwMDw"})) {
		result = get_priorities_sum();			// 157
		result = get_priorities_sum_by_badge(); // 70
	}
}

bool AoC2022_day03::part1() {
	int64_t result = 0;

	result = get_priorities_sum();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2022_day03::part2() {
	int64_t result = 0;

	result = get_priorities_sum_by_badge();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2022_day03 day03;

	return day03.main_execution();
}
