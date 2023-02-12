#include "./../common/aoc.hpp"
#include "./../common/string-utils.hpp"
#include <queue>
#include <regex>
#include <unordered_map>
#include <unordered_set>

const std::regex C_VALVE_REGEX("^Valve ([A-Z][A-Z]) has flow rate=(\\d+); tunnels? leads? to valves? (.*)$");
const std::string C_START_VALVE = "AA";
const uint32_t C_TIMEOUT_MINUTES_PART1 = 30;
const uint32_t C_TIMEOUT_MINUTES_PART2 = 26;

struct shortest_path_str {
	uint32_t steps;
	std::string name;
};

/*
	Original solution redesigned to use bitmap od opened valves to be reusable for part II.
	Idea taken from reddit AoC solution thread including part II (mirror available valves by bit mask)
*/
class AoC2022_day16 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	uint64_t get_highest_pressure_release(const bool part2);
	bool find_shortest_path(const std::string from, const std::string to, uint32_t& steps);
	uint64_t find_highest_pressure_release_part1(const uint32_t current, const uint64_t time_spent, const uint64_t time_limit, const uint64_t pressure,
												 const uint32_t mask, const uint32_t open);
	uint64_t find_highest_pressure_release_part2(const std::string current1, const std::string current2, const std::string next1, const std::string next2,
												 const uint64_t time, const uint64_t pressure, std::unordered_set<std::string>& open);
	uint64_t get_actual_flow(const uint32_t open);
	std::unordered_map<uint32_t, std::unordered_map<uint32_t, uint32_t>> distances_;
	std::unordered_map<std::string, std::unordered_set<std::string>> neighbors_;
	std::unordered_map<uint32_t, uint32_t> valve_rates_;
	std::unordered_map<std::string, uint32_t> valve2index_;
	std::unordered_map<uint32_t, std::string> index2valve_;
	uint32_t full_mask_;
};

bool AoC2022_day16::init(const std::vector<std::string> lines) {
	std::smatch sm;
	std::vector<std::string> items;
	uint32_t rate, bit;
	std::string valve;

	neighbors_.clear();
	valve_rates_.clear();
	valve2index_.clear();
	index2valve_.clear();

	bit = 1;
	full_mask_ = 0;

	valve2index_[C_START_VALVE] = 0;
	index2valve_[0] = C_START_VALVE;

	for (size_t i = 0; i < lines.size(); i++) {
		if (std::regex_match(lines[i], sm, C_VALVE_REGEX)) {

			if (sm.str(1).size() != 2) {
				std::cout << "Invalid valve name size at line " << i + 1 << std::endl;
				return false;
			}

			valve = sm.str(1);
			rate = static_cast<uint32_t>(std::stoi(sm.str(2)));

			if (rate > 0) {
				valve2index_[valve] = bit;
				index2valve_[bit] = valve;
				valve_rates_[bit] = rate;
				full_mask_ |= bit;
				bit = bit << 1;
			}

			items = split(sm.str(3), ", ");

			for (auto& item : items) {
				if (item.size() != 2) {
					std::cout << "Invalid valve name size at line " << i + 1 << std::endl;
					return false;
				} else {
					neighbors_[valve].insert(item);
				}
			}
		} else {
			std::cout << "Invalid valve report at line " << i + 1 << std::endl;
			return false;
		}
	}

	return true;
}

bool AoC2022_day16::find_shortest_path(const std::string from, const std::string to, uint32_t& steps) {
	std::unordered_set<std::string> history = {};
	std::queue<shortest_path_str> q = {};
	shortest_path_str curr, next;

	curr.name = from;
	curr.steps = 0;

	q.push(curr);

	steps = 0;
	while (q.size()) {
		curr = q.front();
		q.pop();

		if (curr.name == to) {
			steps = curr.steps;
			return true;
		}

		if (history.count(curr.name) > 0) {
			continue;
		}

		next = curr;
		next.steps++;
		history.emplace(curr.name);
		for (auto& valve : neighbors_[curr.name]) {
			next.name = valve;
			q.push(next);
		}
	}

	return false;
}

uint64_t AoC2022_day16::get_actual_flow(const uint32_t open) {
	uint64_t result = 0;

	for (uint32_t i = 1; i < full_mask_; i = i << 1) {
		if ((open & i) != 0) {
			result += valve_rates_[i];
		}
	}

	return result;
}

uint64_t AoC2022_day16::find_highest_pressure_release_part1(const uint32_t current, const uint64_t time_spent, const uint64_t time_limit,
															const uint64_t pressure, const uint32_t mask, const uint32_t open) {
	uint64_t new_press, max;

	max = ((time_limit - time_spent) * get_actual_flow(open)) + pressure;

	for (uint32_t next = 1; next < mask; next = next << 1) {
		if (((next & mask) == 0) || ((open & next) != 0)) {
			continue;
		}

		uint32_t time_diff = distances_[current][next];
		if (time_diff == 0) {
			continue;
		} else {
			time_diff++;
		}

		if (time_spent + time_diff < time_limit) {
			new_press = pressure + get_actual_flow(open) * time_diff;

			uint64_t value = find_highest_pressure_release_part1(next, time_spent + time_diff, time_limit, new_press, mask, open | next);

			if (value > max) {
				max = value;
			}
		}
	}

	return max;
}

uint64_t AoC2022_day16::get_highest_pressure_release(const bool part2) {
	uint64_t result = 0, result_me, result_elephant;
	std::unordered_set<std::string> open;
	uint32_t elephant, from;

	if (!part2) {
		std::vector<std::string> valves = {};
		uint32_t steps;

		distances_.clear();
		for (auto it = valve2index_.begin(); it != valve2index_.end(); ++it) {
			valves.push_back(it->first);
		}

		for (size_t i = 0; i < valves.size(); i++) {
			for (size_t j = i + 1; j < valves.size(); j++) {
				if (find_shortest_path(valves[i], valves[j], steps)) {
					distances_[valve2index_[valves[i]]][valve2index_[valves[j]]] = steps;
					distances_[valve2index_[valves[j]]][valve2index_[valves[i]]] = steps;
				}
			}
		}

		result = find_highest_pressure_release_part1(valve2index_[C_START_VALVE], 0, C_TIMEOUT_MINUTES_PART1, 0, full_mask_, 0);
	} else {

		from = full_mask_ / 2; // this is optimized for speed, if not working use from = 1;

		for (uint32_t i = from; i <= full_mask_; i++) {

			result_me = find_highest_pressure_release_part1(valve2index_[C_START_VALVE], 0, C_TIMEOUT_MINUTES_PART2, 0, i, 0);

			elephant = ~i & full_mask_;

			result_elephant = find_highest_pressure_release_part1(valve2index_[C_START_VALVE], 0, C_TIMEOUT_MINUTES_PART2, 0, elephant, 0);

			if ((result_me + result_elephant) > result) {
				result = result_me + result_elephant;
			}
		}
	}

	return result;
}

int32_t AoC2022_day16::get_aoc_day() {
	return 16;
}

int32_t AoC2022_day16::get_aoc_year() {
	return 2022;
}

void AoC2022_day16::tests() {
	int64_t result;

	if (init({"Valve AA has flow rate=0; tunnels lead to valves DD, II, BB", "Valve BB has flow rate=13; tunnels lead to valves CC, AA",
			  "Valve CC has flow rate=2; tunnels lead to valves DD, BB", "Valve DD has flow rate=20; tunnels lead to valves CC, AA, EE",
			  "Valve EE has flow rate=3; tunnels lead to valves FF, DD", "Valve FF has flow rate=0; tunnels lead to valves EE, GG",
			  "Valve GG has flow rate=0; tunnels lead to valves FF, HH", "Valve HH has flow rate=22; tunnel leads to valve GG",
			  "Valve II has flow rate=0; tunnels lead to valves AA, JJ", "Valve JJ has flow rate=21; tunnel leads to valve II"})) {
		result = get_highest_pressure_release(false); // 1651
		result = get_highest_pressure_release(true);  // 1707
	}
}

bool AoC2022_day16::part1() {
	int64_t result;

	result = get_highest_pressure_release(false);

	result1_ = std::to_string(result);

	return true;
}

bool AoC2022_day16::part2() {
	int64_t result;

	result = get_highest_pressure_release(true);

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2022_day16 day16;

	return day16.main_execution();
}
