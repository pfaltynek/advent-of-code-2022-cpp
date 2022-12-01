#include "./../common/aoc.hpp"
#include <algorithm>
#include <numeric>

class AoC2022_day01 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	uint64_t get_max_calories_held();
	uint64_t get_top3_max_calories_held();
	std::vector<std::vector<uint64_t>> elfs_invertory_;
};

bool AoC2022_day01::init(const std::vector<std::string> lines) {
	std::vector<uint64_t> elf_inv = {};

	elfs_invertory_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {
		if (lines[i].empty()) {
			if (!elf_inv.empty()) {
				elfs_invertory_.push_back(elf_inv);
				elf_inv.clear();
			}
		} else {
			elf_inv.push_back(static_cast<uint64_t>(std::stoi(lines[i])));
		}
	}

	if (!elf_inv.empty()) {
		elfs_invertory_.push_back(elf_inv);
	}

	return true;
}

uint64_t AoC2022_day01::get_max_calories_held() {
	uint64_t result = 0;

	for (size_t i = 1; i < elfs_invertory_.size(); i++) {
		uint64_t elf_cals = 0;

		elf_cals = std::accumulate(elfs_invertory_[i].begin(), elfs_invertory_[i].end(), static_cast<uint64_t>(0));

		if (elf_cals > result) {
			result = elf_cals;
		}
	}

	return result;
}

uint64_t AoC2022_day01::get_top3_max_calories_held() {
	std::vector<uint64_t> sums = {};
	uint64_t result = 0;

	for (size_t i = 1; i < elfs_invertory_.size(); i++) {
		uint64_t elf_cals = 0;

		elf_cals = std::accumulate(elfs_invertory_[i].begin(), elfs_invertory_[i].end(), static_cast<uint64_t>(0));

		sums.push_back(elf_cals);
	}

	std::sort(sums.begin(), sums.end(), std::greater<>());

	result = std::accumulate(sums.begin(), sums.begin() + 3, static_cast<uint64_t>(0));

	return result;
}

int32_t AoC2022_day01::get_aoc_day() {
	return 1;
}

int32_t AoC2022_day01::get_aoc_year() {
	return 2022;
}

void AoC2022_day01::tests() {
	uint64_t result;

	if (init({"1000", "2000", "3000", "", "4000", "", "5000", "6000", "", "7000", "8000", "9000", "", "10000"})) {
		result = get_max_calories_held();	   // 24000
		result = get_top3_max_calories_held(); // 45000
	}
}

bool AoC2022_day01::part1() {
	int64_t result = 0;

	result = get_max_calories_held();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2022_day01::part2() {
	int64_t result = 0;

	result = get_top3_max_calories_held();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2022_day01 day01;

	return day01.main_execution();
}
