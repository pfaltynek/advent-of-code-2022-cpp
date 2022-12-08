#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <map>

class AoC2022_day08 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	uint64_t get_visibles_trees_sum();
	uint64_t get_highest_scenic_score();
	uint64_t get_scenic_score(coord_str tree);
	bool is_tree_visible(coord_str tree);
	std::map<coord_str, uint8_t> trees_;
	uint64_t width_, height_;
};

bool AoC2022_day08::init(const std::vector<std::string> lines) {
	height_ = lines.size();

	trees_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {
		if (i == 0) {
			width_ = lines[i].size();
		} else {
			if (width_ != lines[i].size()) {
				std::cout << "Invalid data size at line " << i + 1 << std::endl;
				return false;
			}
		}

		if (lines[i].find_first_not_of("0123456789") != std::string::npos) {
			std::cout << "Invalid data content at line " << i + 1 << std::endl;
			return false;
		}
		for (uint32_t j = 0; j < lines[i].size(); j++) {
			trees_[coord_str(j, i)] = static_cast<uint8_t>(lines[i][j] - '0');
		}
	}

	return true;
}

uint64_t AoC2022_day08::get_visibles_trees_sum() {
	uint64_t result = 0;

	for (uint32_t i = 1; i < width_ - 1; i++) {
		for (uint32_t j = 1; j < height_ - 1; j++) {
			if (is_tree_visible(coord_str(i, j))) {
				result++;
			}
		}
	}

	result += (2 * (width_ + height_)) - 4;

	return result;
}

bool AoC2022_day08::is_tree_visible(coord_str tree) {
	uint8_t h = trees_[tree];
	bool hidden;

	hidden = false;
	for (int32_t i = tree.x - 1; i >= 0; i--) {
		if (trees_[{i, tree.y}] >= h) {
			hidden = true;
			break;
		}
	}

	if (!hidden) {
		return true;
	}

	hidden = false;
	for (int32_t i = tree.x + 1; i < static_cast<int32_t>(width_); i++) {
		if (trees_[{i, tree.y}] >= h) {
			hidden = true;
			break;
		}
	}

	if (!hidden) {
		return true;
	}

	hidden = false;
	for (int32_t i = tree.y - 1; i >= 0; i--) {
		if (trees_[{tree.x, i}] >= h) {
			hidden = true;
			break;
		}
	}

	if (!hidden) {
		return true;
	}

	hidden = false;
	for (int32_t i = tree.y + 1; i < static_cast<int32_t>(height_); i++) {
		if (trees_[{tree.x, i}] >= h) {
			hidden = true;
			break;
		}
	}

	if (!hidden) {
		return true;
	}

	return false;
}

uint64_t AoC2022_day08::get_highest_scenic_score() {
	uint64_t result = 0, tmp;

	for (uint32_t i = 1; i < width_ - 1; i++) {
		for (uint32_t j = 1; j < height_ - 1; j++) {
			tmp = get_scenic_score(coord_str(i, j));

			if (tmp > result) {
				result = tmp;
			}
		}
	}

	return result;
}

uint64_t AoC2022_day08::get_scenic_score(coord_str tree) {
	uint8_t h = trees_[tree];
	uint64_t result = 1, tmp;

	tmp = 0;
	for (int32_t i = tree.x - 1; i >= 0; i--) {
		tmp++;
		if (trees_[{i, tree.y}] >= h) {
			break;
		}
	}

	result *= tmp;
	tmp = 0;

	for (int32_t i = tree.x + 1; i < static_cast<int32_t>(width_); i++) {
		tmp++;
		if (trees_[{i, tree.y}] >= h) {
			break;
		}
	}

	result *= tmp;
	tmp = 0;

	for (int32_t i = tree.y - 1; i >= 0; i--) {
		tmp++;
		if (trees_[{tree.x, i}] >= h) {
			break;
		}
	}

	result *= tmp;
	tmp = 0;

	for (int32_t i = tree.y + 1; i < static_cast<int32_t>(height_); i++) {
		tmp++;
		if (trees_[{tree.x, i}] >= h) {
			break;
		}
	}

	result *= tmp;

	return result;
}

int32_t AoC2022_day08::get_aoc_day() {
	return 8;
}

int32_t AoC2022_day08::get_aoc_year() {
	return 2022;
}

void AoC2022_day08::tests() {
	int64_t result;

	if (init({"30373", "25512", "65332", "33549", "35390"})) {
		result = get_visibles_trees_sum(); // 21
		result = get_highest_scenic_score(); // 8
	}
}

bool AoC2022_day08::part1() {
	int64_t result;

	result = get_visibles_trees_sum();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2022_day08::part2() {
	int64_t result;

	result = get_highest_scenic_score();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2022_day08 day08;

	return day08.main_execution();
}
