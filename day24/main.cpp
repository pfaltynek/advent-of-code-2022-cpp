#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <map>
#include <queue>
#include <set>

class AoC2022_day24 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::set<coord_str> walls_;
	std::map<coord_str, std::vector<coord_str>> blizzards_, blizzards_backup_;
	coord_str expedition_, start_, finish_, ul_, lr_;

	void print_valley();
	int64_t get_steps_count_to_finish();
	void blizzards_move();
	int64_t get_steps_count_to_return_with_snacks();
};

bool AoC2022_day24::init(const std::vector<std::string> lines) {
	bool start = false, finish = false;
	uint32_t width = static_cast<uint32_t>(lines[0].size());

	walls_.clear();
	blizzards_.clear();

	for (int32_t y = 0; y < static_cast<int32_t>(lines.size()); y++) {

		if (lines[y].size() != width) {
			std::cout << "Line " << y + 1 << " has incorrect width." << std::endl;
			return false;
		}
		for (int32_t x = 0; x < static_cast<int32_t>(lines[y].size()); x++) {
			switch (lines[y][x]) {
				case '#': {
					coord_str coord(x, y);
					walls_.insert(coord);
					break;
				}
				case '.': {
					if (y == 0) {
						start_ = coord_str(x, y);
						start = true;
					} else if (y == static_cast<int32_t>(lines.size()) - 1) {
						finish_ = coord_str(x, y);
						finish = true;
					}
					break;
				}
				case '^': {
					blizzards_[coord_str(x, y)].push_back(coord_step_north);
					break;
				}
				case 'v': {
					blizzards_[coord_str(x, y)].push_back(coord_step_south);
					break;
				}
				case '<': {
					blizzards_[coord_str(x, y)].push_back(coord_step_west);
					break;
				}
				case '>': {
					blizzards_[coord_str(x, y)].push_back(coord_step_east);
					break;
				}
			}
		}
	}

	ul_ = {0, 0};
	lr_ = {static_cast<int32_t>(width) - 1, static_cast<int32_t>(lines.size()) - 1};

	blizzards_backup_ = blizzards_;

	return start && finish;
}

int32_t AoC2022_day24::get_aoc_day() {
	return 24;
}

int32_t AoC2022_day24::get_aoc_year() {
	return 2022;
}

void AoC2022_day24::print_valley() {
	coord_str coord;

	for (int32_t y = ul_.y; y <= lr_.y; y++) {
		for (int32_t x = ul_.x; x <= lr_.x; x++) {
			coord = {x, y};
			if (walls_.count(coord)) {
				std::cout << '#';
			} else if (expedition_ == coord) {
				std::cout << 'E';
			} else if (start_ == coord) {
				std::cout << 'S';
			} else if (finish_ == coord) {
				std::cout << 'F';
			} else {
				switch (blizzards_[coord].size()) {
					case 0:
						std::cout << '.';
						break;
					case 1:
						if (blizzards_[coord][0] == coord_step_north) {
							std::cout << '^';
						} else if (blizzards_[coord][0] == coord_step_south) {
							std::cout << 'v';
						} else if (blizzards_[coord][0] == coord_step_west) {
							std::cout << '<';
						} else if (blizzards_[coord][0] == coord_step_east) {
							std::cout << '>';
						}
						break;
					default:
						std::cout << blizzards_[coord].size();
						break;
				}
			}
		}
		std::cout << '\n';
	}
	std::cout << '\n';
}

int64_t AoC2022_day24::get_steps_count_to_finish() {
	int64_t result = 0;
	std::set<coord_str> wave, next_wave;
	std::vector<coord_str> directions = {coord_step_north, coord_step_south, coord_step_west, coord_step_east};

	wave.clear();

	wave.insert(start_);

	while (wave.size()) {
		next_wave.clear();
		blizzards_move();
		result++;

		for (const auto& coord : wave) {

			expedition_ = coord;

			if (blizzards_[expedition_].size() == 0) {
				next_wave.insert(expedition_);
			}

			for (const auto& direction : directions) {
				expedition_ = {coord.x + direction.x, coord.y + direction.y};

				if (expedition_ == finish_) {
					return result;
				}

				if (!walls_.count(expedition_) && blizzards_[expedition_].size() == 0 && expedition_.y >= ul_.y && expedition_.y <= lr_.y &&
					expedition_.x >= ul_.x && expedition_.x <= lr_.x) {
					next_wave.insert(expedition_);
				}
			}
		}

		wave = std::move(next_wave);
	}

	return 0;
}

int64_t AoC2022_day24::get_steps_count_to_return_with_snacks() {
	int64_t result = 0;

	blizzards_ = blizzards_backup_;
	result += get_steps_count_to_finish();
	std::swap(start_, finish_);
	result += get_steps_count_to_finish();
	std::swap(start_, finish_);
	result += get_steps_count_to_finish();

	return result;
}

void AoC2022_day24::blizzards_move() {
	std::map<coord_str, std::vector<coord_str>> new_blizzards;

	new_blizzards.clear();

	for (const auto& [coord, directions] : blizzards_) {
		for (const auto& direction : directions) {
			coord_str new_coord = {coord.x + direction.x, coord.y + direction.y};
			if (new_coord.x <= ul_.x)
				new_coord.x = lr_.x - 1;
			if (new_coord.x >= lr_.x)
				new_coord.x = ul_.x + 1;
			if (new_coord.y <= ul_.y)
				new_coord.y = lr_.y - 1;
			if (new_coord.y >= lr_.y)
				new_coord.y = ul_.y + 1;
			new_blizzards[new_coord].push_back(direction);
		}
	}

	blizzards_ = std::move(new_blizzards);
}

void AoC2022_day24::tests() {
	int64_t result;

	if (init({"#.######", "#>>.<^<#", "#.<..<<#", "#>v.><>#", "#<^v^^>#", "######.#"})) {
		result = get_steps_count_to_finish();			  // 18
		result = get_steps_count_to_return_with_snacks(); // 54
	}
}

bool AoC2022_day24::part1() {
	int64_t result;

	result = get_steps_count_to_finish();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2022_day24::part2() {
	int64_t result;

	result = get_steps_count_to_return_with_snacks();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2022_day24 day24;

	return day24.main_execution();
}
