#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include "./../common/string-utils.hpp"

#include <array>
#include <queue>
#include <set>

const std::array<coord_3d_str, 6> C_NEIGHBORS = {coord_step_front,		  coord_step_rear,		  coord_step_north_center,
												 coord_step_south_center, coord_step_west_center, coord_step_east_center};
class AoC2022_day18 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::set<coord_3d_str> cubes_;
	uint64_t count_surface_area(const bool part2);
	coord_3d_str max_, min_;
	void print(const std::set<coord_3d_str>& area, const coord_3d_str& min, const coord_3d_str& max);
};

bool AoC2022_day18::init(const std::vector<std::string> lines) {
	std::vector<std::string> coords;
	int32_t x, y, z;

	cubes_ = {};

	for (size_t i = 0; i < lines.size(); i++) {
		coords = split(lines[i], ",");

		if (coords.size() != 3) {
			std::cout << "Invalid cube coords on line " << i + 1 << std::endl;
			return false;
		}

		x = static_cast<int32_t>(std::stoi(coords[0]));
		y = static_cast<int32_t>(std::stoi(coords[1]));
		z = static_cast<int32_t>(std::stoi(coords[2]));

		if (i == 0) {
			max_ = {x, y, z};
			min_ = {x, y, z};
		} else {
			if (max_.x < x) {
				max_.x = x;
			}
			if (max_.y < y) {
				max_.y = y;
			}
			if (max_.z < z) {
				max_.z = z;
			}

			if (min_.x > x) {
				min_.x = x;
			}
			if (min_.y > y) {
				min_.y = y;
			}
			if (min_.z > z) {
				min_.z = z;
			}
		}

		cubes_.insert({x, y, z});
	}

	min_ = min_ + coord_3d_str(-1, -1, -1);
	max_ = max_ + coord_3d_str(1, 1, 1);

	// print(cubes_, min_, max_);

	return true;
}

void AoC2022_day18::print(const std::set<coord_3d_str>& area, const coord_3d_str& min, const coord_3d_str& max) {
	for (int32_t z = 0; z <= max_.z; z++) {
		std::cout << "z = " << z << std::endl;

		for (int32_t y = 0; y <= max_.y; y++) {
			for (int32_t x = 0; x <= max_.x; x++) {
				std::cout << (area.count({x, y, z}) > 0 ? "#" : ".");
			}
			std::cout << std::endl;
		}
	}
}

uint64_t AoC2022_day18::count_surface_area(const bool part2) {
	std::set<coord_3d_str> inverted = {};
	uint64_t result = 0;
	coord_3d_str coord;

	for (int32_t x = min_.x; x <= max_.x; x++) {
		for (int32_t y = min_.y; y <= max_.y; y++) {
			for (int32_t z = min_.z; z <= max_.z; z++) {
				coord = {x, y, z};

				if (cubes_.count(coord) > 0) {
					result += 6;

					for (uint8_t i = 0; i < C_NEIGHBORS.size(); i++) {
						if (cubes_.count(C_NEIGHBORS[i] + coord) > 0) {
							result--;
						}
					}
				} else if (part2) {
					inverted.emplace(coord);
				}
			}
		}
	}

	if (part2) {
		std::queue<coord_3d_str> q = {};
		coord_3d_str curr, next;

		// print(inverted, min_, max_);

		q.emplace(min_);
		q.emplace(max_);
		q.emplace(coord_3d_str(min_.x, min_.y, max_.z));
		q.emplace(coord_3d_str(min_.x, max_.y, max_.z));
		q.emplace(coord_3d_str(min_.x, max_.y, min_.z));
		q.emplace(coord_3d_str(max_.x, min_.y, min_.z));
		q.emplace(coord_3d_str(max_.x, max_.y, min_.z));
		q.emplace(coord_3d_str(max_.x, min_.y, max_.z));

		while (q.size()) {
			curr = q.front();
			q.pop();

			if (!curr.in_bounds(min_, max_)) {
				continue;
			}

			if (inverted.count(curr) == 0) {
				continue;
			}

			inverted.erase(curr);

			for (uint8_t i = 0; i < C_NEIGHBORS.size(); i++) {
				next = C_NEIGHBORS[i] + curr;
				q.emplace(next);
			}
		}

		for (auto& coord : inverted) {
			for (uint8_t i = 0; i < C_NEIGHBORS.size(); i++) {
				if (cubes_.count(C_NEIGHBORS[i] + coord) > 0) {
					result--;
				}
			}
		}

		// print(inverted, min_, max_);
	}

	return result;
}

int32_t AoC2022_day18::get_aoc_day() {
	return 18;
}

int32_t AoC2022_day18::get_aoc_year() {
	return 2022;
}

void AoC2022_day18::tests() {
	int64_t result;

	if (init({"2,2,2", "1,2,2", "3,2,2", "2,1,2", "2,3,2", "2,2,1", "2,2,3", "2,2,4", "2,2,6", "1,2,5", "3,2,5", "2,1,5", "2,3,5"})) {
		result = count_surface_area(false); // 64
		result = count_surface_area(true);	// 58
	}
}

bool AoC2022_day18::part1() {
	int64_t result;

	result = count_surface_area(false);

	result1_ = std::to_string(result);

	return true;
}

bool AoC2022_day18::part2() {
	int64_t result;

	result = count_surface_area(true);

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2022_day18 day18;

	return day18.main_execution();
}
