#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <map>
#include <set>

struct zone_str {
	coord_str zone_direction;
	std::vector<coord_str> zone_steps;
};

class AoC2022_day23 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::set<coord_str> elfs_;
	std::vector<zone_str> zones_;
	coord_str max_, min_;

	void zones_update();
	void one_round();
	void print_elfs();
	void limits_update(const coord_str elf);

	int64_t p1();
};

bool AoC2022_day23::init(const std::vector<std::string> lines) {
	coord_str coord;

	elfs_.clear();
	zones_.clear();

	min_ = {INT32_MAX, INT32_MAX};
	max_ = {INT32_MIN, INT32_MIN};

	for (int32_t y = 0; y < static_cast<int32_t>(lines.size()); y++) {
		for (int32_t x = 0; x < static_cast<int32_t>(lines[y].size()); x++) {
			if (lines[y][x] == '#') {
				coord = coord_str(x, y);
				elfs_.insert(coord);
				limits_update(coord);
			}
		}
	}

	zones_.push_back({coord_step_north, {coord_step_north, coord_step_north_east, coord_step_north_west}});
	zones_.push_back({coord_step_south, {coord_step_south, coord_step_south_east, coord_step_south_west}});
	zones_.push_back({coord_step_west, {coord_step_west, coord_step_north_west, coord_step_south_west}});
	zones_.push_back({coord_step_east, {coord_step_east, coord_step_north_east, coord_step_south_east}});

	return true;
}

int32_t AoC2022_day23::get_aoc_day() {
	return 23;
}

int32_t AoC2022_day23::get_aoc_year() {
	return 2022;
}

void AoC2022_day23::zones_update() {
	zones_.push_back(zones_.front());
	zones_.erase(zones_.begin());
}

void AoC2022_day23::one_round() {
	std::set<coord_str> new_elfs;
	std::map<coord_str, std::set<coord_str>> next_positions;
	static std::vector<coord_str> around = {coord_step_north,	   coord_step_south,	  coord_step_west,		 coord_step_east,
											coord_step_north_east, coord_step_north_west, coord_step_south_east, coord_step_south_west};

	new_elfs.clear();
	next_positions.clear();

	min_ = {INT32_MAX, INT32_MAX};
	max_ = {INT32_MIN, INT32_MIN};

	for (const auto& elf : elfs_) {
		bool skip = true;
		bool elf_in_zone = false;

		for (const auto& step : around) {
			if (elfs_.count(elf + step)) {
				skip = false;
				break;
			}
		}

		if (skip) {
			new_elfs.insert(elf);
			limits_update(elf);
			continue;
		}

		for (const auto& zone : zones_) {
			for (const auto& step : zone.zone_steps) {
				if (elfs_.count(elf + step)) {
					elf_in_zone = true;
					break;
				}
			}

			if (!elf_in_zone) {
				next_positions[elf + zone.zone_direction].insert(elf);
				break;
			}
		}

		if (elf_in_zone) {
			new_elfs.insert(elf);
			limits_update(elf);
		}
	}

 	for (const auto& elf : next_positions) {
		if (elf.second.size() == 1) {
			new_elfs.insert(elf.first);
			limits_update(elf.first);
		} else {
			for (const auto& elf_in_conflict : elf.second) {
				new_elfs.insert(elf_in_conflict);
				limits_update(elf_in_conflict);
			}
		}
	}

	elfs_.swap(new_elfs);
	zones_update();
}

void AoC2022_day23::print_elfs() {
	for (int32_t y = 0; y <= max_.y; y++) {
		for (int32_t x = 0; x <= max_.x; x++) {
			if (elfs_.count(coord_str(x, y))) {
				std::cout << '#';
			} else {
				std::cout << '.';
			}
		}
		std::cout << '\n';
	}
	std::cout << '\n';
}

void AoC2022_day23::limits_update(const coord_str elf) {
	if (elf.x < min_.x) {
		min_.x = elf.x;
	}
	if (elf.x > max_.x) {
		max_.x = elf.x;
	}
	if (elf.y < min_.y) {
		min_.y = elf.y;
	}
	if (elf.y > max_.y) {
		max_.y = elf.y;
	}
}

int64_t AoC2022_day23::p1() {
	int64_t result = 0;

	print_elfs();

	for (int32_t round = 0; round < 10; round++) {
		one_round();
		print_elfs();
	}

	result = (static_cast<int64_t>(max_.x - min_.x + 1) * static_cast<int64_t>(max_.y - min_.y + 1)) - static_cast<int64_t>(elfs_.size());

	return result;
}

void AoC2022_day23::tests() {
	int64_t result;

	/*
	if (init({".....", "..##.", "..#..", ".....", "..##.", "....."})) {
		result = p1(); // 110
	}
	*/

	if (init({"..............", "..............", ".......#......", ".....###.#....", "...#...#.#....", "....#...##....", "...#.###......", "...##.#.##....",
			  "....#..#......", "..............", "..............", ".............."})) {
		result = p1(); // 110
	}
}

bool AoC2022_day23::part1() {
	int64_t result;

	result = p1();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2022_day23::part2() {
	int64_t result;

	result = p1();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2022_day23 day23;

	return day23.main_execution();
}
