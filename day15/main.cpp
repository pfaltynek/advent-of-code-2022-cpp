#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <regex>
#include <set>

typedef std::pair<coord_str, coord_str> sensor_report_t;

const std::regex C_SENSOR_REPORT_REGEX("^Sensor at x=(-?\\d+), y=(-?\\d+): closest beacon is at x=(-?\\d+), y=(-?\\d+)$");
const int32_t C_PART1_ROW = 2000000;
const int32_t C_PART1_TEST_ROW = 10;
const int32_t C_PART2_COORD_MIN = 0;
const int32_t C_PART2_COORD_MAX = 4000000;
const int32_t C_PART2_TEST_COORD_MAX = 20;
const uint64_t C_FREQ_MULTIPLIER = 4000000;

static bool const compare(const COORD2D& l, const COORD2D& r) {
	return (l.x < r.x || (l.x == r.x && l.y < r.y));
}

class AoC2022_day15 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	void get_row_covering(const int32_t row, std::vector<coord_str>& ranges, std::set<int32_t>& beacons);
	uint64_t get_positions_count_without_beacon(const int32_t row);
	uint64_t get_distress_tunning_frequency(const int32_t min_coord, const int32_t max_coord);
	std::vector<sensor_report_t> sensor_reports_;
};

bool AoC2022_day15::init(const std::vector<std::string> lines) {
	std::smatch sm;
	coord_str sensor, beacon;
	sensor_reports_.clear();

	for (size_t i = 0; i < lines.size(); i++) {
		if (std::regex_match(lines[i], sm, C_SENSOR_REPORT_REGEX)) {
			sensor = {static_cast<int32_t>(std::stoi(sm.str(1))), static_cast<int32_t>(std::stoi(sm.str(2)))};
			beacon = {static_cast<int32_t>(std::stoi(sm.str(3))), static_cast<int32_t>(std::stoi(sm.str(4)))};
			sensor_reports_.push_back(std::make_pair(sensor, beacon));
		} else {
			std::cout << "Invalid sensor report at line " << i + 1 << std::endl;
			return false;
		}
	}

	return true;
}

void AoC2022_day15::get_row_covering(const int32_t row, std::vector<coord_str>& ranges, std::set<int32_t>& beacons) {
	size_t distance;
	int32_t x_dist;
	bool united;

	ranges.clear();
	beacons.clear();

	for (size_t i = 0; i < sensor_reports_.size(); i++) {
		distance = (sensor_reports_[i].first - sensor_reports_[i].second).size();
		if (abs(sensor_reports_[i].first.y - row) <= distance) {
			x_dist = static_cast<int32_t>(distance - abs(sensor_reports_[i].first.y - row));
			ranges.push_back({sensor_reports_[i].first.x - x_dist, sensor_reports_[i].first.x + x_dist});
		}

		if (sensor_reports_[i].second.y == row) {
			beacons.emplace(sensor_reports_[i].second.x);
		}
	}

	std::sort(ranges.begin(), ranges.end(), compare);

	united = false;
	while (!united) {
		united = true;

		for (size_t i = 0; i < ranges.size() - 1; i++) {
			if (ranges[i].y + 1 >= ranges[i + 1].x) {
				ranges[i].y = std::max(ranges[i].y, ranges[i + 1].y);
				ranges.erase(ranges.begin() + i + 1);
				united = false;
				break;
			}
		}
	}
}

uint64_t AoC2022_day15::get_positions_count_without_beacon(const int32_t row) {
	uint64_t result = 0;
	std::vector<coord_str> ranges;
	std::set<int32_t> beacons;

	get_row_covering(row, ranges, beacons);

	for (auto& range : ranges) {
		result += range.y - range.x + 1;

		for (auto& beacon : beacons) {
			if ((beacon <= range.y) && (beacon >= range.x)) {
				result--;
			}
		}
	}

	return result;
}

uint64_t AoC2022_day15::get_distress_tunning_frequency(const int32_t min_coord, const int32_t max_coord) {
	std::vector<coord_str> ranges;
	std::set<int32_t> beacons;
	uint64_t result = 0, row;

	for (int32_t i = max_coord; i >= min_coord; i--) {
		get_row_covering(i, ranges, beacons);

		if (ranges.size() == 2) {
			if (ranges[1].x - ranges[0].y == 2) {
				result = ranges[1].x - 1;
				row = i;
				break;
			}
		}
	}

	result *= C_FREQ_MULTIPLIER;
	result += row;

	return result;
}

int32_t AoC2022_day15::get_aoc_day() {
	return 15;
}

int32_t AoC2022_day15::get_aoc_year() {
	return 2022;
}

void AoC2022_day15::tests() {
	int64_t result;

	if (init({"Sensor at x=2, y=18: closest beacon is at x=-2, y=15", "Sensor at x=9, y=16: closest beacon is at x=10, y=16",
			  "Sensor at x=13, y=2: closest beacon is at x=15, y=3", "Sensor at x=12, y=14: closest beacon is at x=10, y=16",
			  "Sensor at x=10, y=20: closest beacon is at x=10, y=16", "Sensor at x=14, y=17: closest beacon is at x=10, y=16",
			  "Sensor at x=8, y=7: closest beacon is at x=2, y=10", "Sensor at x=2, y=0: closest beacon is at x=2, y=10",
			  "Sensor at x=0, y=11: closest beacon is at x=2, y=10", "Sensor at x=20, y=14: closest beacon is at x=25, y=17",
			  "Sensor at x=17, y=20: closest beacon is at x=21, y=22", "Sensor at x=16, y=7: closest beacon is at x=15, y=3",
			  "Sensor at x=14, y=3: closest beacon is at x=15, y=3", "Sensor at x=20, y=1: closest beacon is at x=15, y=3"})) {
		result = get_positions_count_without_beacon(C_PART1_TEST_ROW);						// 26
		result = get_distress_tunning_frequency(C_PART2_COORD_MIN, C_PART2_TEST_COORD_MAX); // 56000011
	}
}

bool AoC2022_day15::part1() {
	int64_t result;

	result = get_positions_count_without_beacon(C_PART1_ROW);

	result1_ = std::to_string(result);

	return true;
}

bool AoC2022_day15::part2() {
	int64_t result;

	result = get_distress_tunning_frequency(C_PART2_COORD_MIN, C_PART2_COORD_MAX);

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2022_day15 day15;

	return day15.main_execution();
}
