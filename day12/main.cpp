#include "./../common/aoc.hpp"
#include <queue>

struct climb_state_str {
	size_t position;
	uint64_t steps_a;
	uint64_t steps;
};

class AoC2022_day12 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	uint64_t get_shortest_path();
	uint64_t get_shortest_trail();
	std::vector<size_t> get_neighbors(size_t point);
	std::string heightmap_;
	size_t start_;
	size_t end_;
	size_t width_;
	size_t height_;
};

bool AoC2022_day12::init(const std::vector<std::string> lines) {

	heightmap_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {
		if (heightmap_.empty()) {
			width_ = lines[i].size();
		} else {
			if (width_ != lines[i].size()) {
				std::cout << "Invalid heightmap size at line " << i + 1 << std::endl;
				return false;
			}
		}

		if (lines[i].find_first_not_of("SEabcdefghijklmnopqrstuvwxyz") != std::string::npos) {
			std::cout << "Invalid heightmap content at line " << i + 1 << std::endl;
			return false;
		}

		heightmap_.append(lines[i]);
	}

	start_ = heightmap_.find('S');
	end_ = heightmap_.find('E');

	if (start_ == std::string::npos) {
		std::cout << "Start not found" << std::endl;
		return false;
	}

	if (end_ == std::string::npos) {
		std::cout << "End not found" << std::endl;
		return false;
	}

	heightmap_[start_] = 'a';
	heightmap_[end_] = 'z';

	height_ = lines.size();

	return true;
}

std::vector<size_t> AoC2022_day12::get_neighbors(size_t point) {
	std::vector<size_t> result = {};
	size_t x, y;

	x = point % width_;
	y = point / width_;

	if ((x) > 0) {
		result.push_back(point - 1);
	}

	if (x < (width_ - 1)) {
		result.push_back(point + 1);
	}

	if ((y) > 0) {
		result.push_back(point - width_);
	}

	if (y < (height_ - 1)) {
		result.push_back(point + width_);
	}

	return result;
}

uint64_t AoC2022_day12::get_shortest_path() {
	std::queue<climb_state_str> q = {};
	climb_state_str cs, next;
	std::vector<size_t> neighbors;
	char height;
	std::string tmp;
	std::string history = "";

	history = '|' + std::to_string(start_) + '|';
	cs.position = start_;
	cs.steps = 0;

	q.emplace(cs);

	while (!q.empty()) {
		cs = q.front();
		q.pop();

		neighbors = get_neighbors(cs.position);
		height = heightmap_[cs.position];

		if (cs.position == end_) {
			return cs.steps;
		}

		for (size_t& pos : neighbors) {

			tmp = '|' + std::to_string(pos) + '|';

			if (history.find(tmp) != std::string::npos) {
				continue;
			}

			if ((heightmap_[pos] - 1) <= height) {
				next = cs;
				next.steps++;
				next.position = pos;
				history.append(tmp);
				q.emplace(next);
			}
		}
	}

	return 0;
}

uint64_t AoC2022_day12::get_shortest_trail() {
	std::queue<climb_state_str> q = {};
	climb_state_str cs, next;
	std::vector<size_t> neighbors;
	char height;
	std::string tmp;
	std::string history = "";

	history = '|' + std::to_string(end_) + '|';
	cs.position = end_;
	cs.steps = 0;

	q.emplace(cs);

	while (!q.empty()) {
		cs = q.front();
		q.pop();

		neighbors = get_neighbors(cs.position);
		height = heightmap_[cs.position];

		if (height == 'a') {
			return cs.steps;
		}

		for (size_t& pos : neighbors) {

			tmp = '|' + std::to_string(pos) + '|';

			if (history.find(tmp) != std::string::npos) {
				continue;
			}

			if ((heightmap_[pos] + 1) >= height) {
				next = cs;
				next.steps++;
				next.position = pos;
				history.append(tmp);
				q.emplace(next);
			}
		}
	}

	return 0;
}

int32_t AoC2022_day12::get_aoc_day() {
	return 12;
}

int32_t AoC2022_day12::get_aoc_year() {
	return 2022;
}

void AoC2022_day12::tests() {
	int64_t result;

	if (init({"Sabqponm", "abcryxxl", "accszExk", "acctuvwj", "abdefghi"})) {
		result = get_shortest_path(); // 31
		result = get_shortest_trail(); // 29
	}
}

bool AoC2022_day12::part1() {
	int64_t result;

	result = get_shortest_path();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2022_day12::part2() {
	int64_t result;

	result = get_shortest_trail();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2022_day12 day12;

	return day12.main_execution();
}
