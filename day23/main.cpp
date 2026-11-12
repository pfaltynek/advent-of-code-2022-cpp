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

	void zones_reset();
	void zones_update();
	bool one_round();
	void print_elfs();
	void limits_update();
	bool elf_has_neighbor(const coord_str elf);
	bool elf_in_zone(const coord_str elf, const zone_str zone);

	int64_t simulate_10_rounds();
	int64_t simulate_until_no_move();
};

bool AoC2022_day23::init(const std::vector<std::string> lines) {
	coord_str coord;

	elfs_.clear();

	min_ = {INT32_MAX, INT32_MAX};
	max_ = {INT32_MIN, INT32_MIN};

	for (int32_t y = 0; y < static_cast<int32_t>(lines.size()); y++) {
		for (int32_t x = 0; x < static_cast<int32_t>(lines[y].size()); x++) {
			if (lines[y][x] == '#') {
				coord = coord_str(x, y);
				elfs_.insert(coord);
			}
		}
	}

	limits_update();

	return true;
}

int32_t AoC2022_day23::get_aoc_day() {
	return 23;
}

int32_t AoC2022_day23::get_aoc_year() {
	return 2022;
}

void AoC2022_day23::zones_reset() {
	zones_.clear();

	zones_.push_back({coord_step_north, {coord_step_north, coord_step_north_east, coord_step_north_west}});
	zones_.push_back({coord_step_south, {coord_step_south, coord_step_south_east, coord_step_south_west}});
	zones_.push_back({coord_step_west, {coord_step_west, coord_step_north_west, coord_step_south_west}});
	zones_.push_back({coord_step_east, {coord_step_east, coord_step_north_east, coord_step_south_east}});
}

void AoC2022_day23::zones_update() {
	zones_.push_back(zones_.front());
	zones_.erase(zones_.begin());
}

bool AoC2022_day23::one_round() {
	std::set<coord_str> next_elfs, to_move_elfs;
	std::map<coord_str, std::set<coord_str>> next_positions;

	next_elfs.clear();
	to_move_elfs.clear();
	next_positions.clear();

	for (const auto& elf : elfs_) {
		if (elf_has_neighbor(elf)) {
			to_move_elfs.insert(elf);
		} else {
			next_elfs.insert(elf);
		}
	}

	if (next_elfs.size() == elfs_.size()) {
		return false;
	}

	for (const auto& elf : to_move_elfs) {
		bool is_elf_in_zone;

		for (const auto& zone : zones_) {

			is_elf_in_zone = elf_in_zone(elf, zone);

			if (!is_elf_in_zone) {
				next_positions[elf + zone.zone_direction].insert(elf);
				break;
			}
		}

		if (is_elf_in_zone) {
			next_elfs.insert(elf);
		}
	}

	for (const auto& elf : next_positions) {
		if (elf.second.size() == 1) {
			next_elfs.insert(elf.first);
		} else {
			for (const auto& elf_in_conflict : elf.second) {
				next_elfs.insert(elf_in_conflict);
			}
		}
	}

	elfs_.swap(next_elfs);
	limits_update();
	zones_update();

	return true;
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

void AoC2022_day23::limits_update() {

	min_ = {INT32_MAX, INT32_MAX};
	max_ = {INT32_MIN, INT32_MIN};

	for (const auto& elf : elfs_) {
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
}

bool AoC2022_day23::elf_has_neighbor(const coord_str elf) {
	static std::vector<coord_str> around = {coord_step_north,	   coord_step_south,	  coord_step_west,		 coord_step_east,
											coord_step_north_east, coord_step_north_west, coord_step_south_east, coord_step_south_west};

	for (const auto& step : around) {
		if (elfs_.count(elf + step)) {
			return true;
		}
	}

	return false;
}

bool AoC2022_day23::elf_in_zone(const coord_str elf, const zone_str zone) {

	for (const auto& step : zone.zone_steps) {
		if (elfs_.count(elf + step)) {
			return true;
		}
	}

	return false;
}

int64_t AoC2022_day23::simulate_10_rounds() {
	int64_t result = 0;
	std::set<coord_str> elfs_backup;
	elfs_backup = elfs_;

	zones_reset();

	//print_elfs();

	for (int32_t round = 0; round < 10; round++) {
		if (!one_round()) {
			// print_elfs();
			break;
		}
		//print_elfs();
	}

	elfs_ = elfs_backup;

	result = (static_cast<int64_t>(max_.x - min_.x + 1) * static_cast<int64_t>(max_.y - min_.y + 1)) - static_cast<int64_t>(elfs_.size());

	return result;
}

int64_t AoC2022_day23::simulate_until_no_move() {
	int64_t result = 0;

	zones_reset();

	while (true) {
		result++;
		if (!one_round()) {
			break;
		}
	}

	// print_elfs();

	return result;
}

void AoC2022_day23::tests() {
	int64_t result;

	if (init({".....", "..##.", "..#..", ".....", "..##.", "....."})) {
		result = simulate_10_rounds();	   // 25
		result = simulate_until_no_move(); // 4
	}

	if (init({"..............", "..............", ".......#......", ".....###.#....", "...#...#.#....", "....#...##....", "...#.###......", "...##.#.##....",
			  "....#..#......", "..............", "..............", ".............."})) {
		result = simulate_10_rounds();	   // 110
		result = simulate_until_no_move(); // 20
	}
}

bool AoC2022_day23::part1() {
	int64_t result;

	result = simulate_10_rounds();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2022_day23::part2() {
	int64_t result;

	result = simulate_until_no_move();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2022_day23 day23;

	return day23.main_execution();
}
