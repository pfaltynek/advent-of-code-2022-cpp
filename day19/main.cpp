#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include "./../common/string-utils.hpp"

#include <sstream>

enum resource_types_t { E_ORE, E_CLAY, E_OBSIDIAN, E_GEODE, E_RES_TYPES_COUNT };
const uint32_t C_TIME_LIMIT_PART1 = 24;
const uint32_t C_TIME_LIMIT_PART2 = 32;

class common_t {
  private:
	uint32_t ore;
	uint32_t clay;
	uint32_t obsidian;
	uint32_t geode;

  public:
	uint32_t get(const resource_types_t type) {
		switch (type) {
			case E_ORE:
				return ore;
			case E_CLAY:
				return clay;
			case E_OBSIDIAN:
				return obsidian;
			case E_GEODE:
				return geode;
			default:
				assert(false);
				return E_ORE;
				break;
		}
	}

	void set(const resource_types_t res_type, const uint32_t value) {
		switch (res_type) {
			case E_ORE:
				ore = value;
				break;
			case E_CLAY:
				clay = value;
				break;
			case E_OBSIDIAN:
				obsidian = value;
				break;
			case E_GEODE:
				geode = value;
				break;
			default:
				assert(false);
				break;
		}
	}

	void inc(const resource_types_t res_type) {
		switch (res_type) {
			case E_ORE:
				ore++;
				break;
			case E_CLAY:
				clay++;
				break;
			case E_OBSIDIAN:
				obsidian++;
				break;
			case E_GEODE:
				geode++;
				break;
			default:
				assert(false);
				break;
		}
	}

	void sub(const common_t& other) {
		ore -= other.ore;
		clay -= other.clay;
		obsidian -= other.obsidian;
		geode -= other.geode;
	}

	void add(const common_t& other) {
		ore += other.ore;
		clay += other.clay;
		obsidian += other.obsidian;
		geode += other.geode;
	}

	void add_mult(const common_t& other, const uint32_t multiply) {
		ore += other.ore * multiply;
		clay += other.clay * multiply;
		obsidian += other.obsidian * multiply;
		geode += other.geode * multiply;
	}

	bool have_robots_required_for_robot(const common_t& resources_required) {
		if (resources_required.ore > 0) {
			if (ore == 0) {
				return false;
			}
		}
		if (resources_required.clay > 0) {
			if (clay == 0) {
				return false;
			}
		}
		if (resources_required.obsidian > 0) {
			if (obsidian == 0) {
				return false;
			}
		}
		if (resources_required.geode > 0) {
			if (geode == 0) {
				return false;
			}
		}

		return true;
	}

	bool have_sufficient_resources(const common_t& resources_required) {
		if (ore < resources_required.ore) {
			return false;
		}
		if (clay < resources_required.clay) {
			return false;
		}
		if (obsidian < resources_required.obsidian) {
			return false;
		}
		if (geode < resources_required.geode) {
			return false;
		}

		return true;
	}

	void calculate_missing_resources(const common_t& required_resources) {
		if (required_resources.ore > ore) {
			ore = required_resources.ore - ore;
		} else {
			ore = 0;
		}
		if (required_resources.clay > clay) {
			clay = required_resources.clay - clay;
		} else {
			clay = 0;
		}
		if (required_resources.obsidian > obsidian) {
			obsidian = required_resources.obsidian - obsidian;
		} else {
			obsidian = 0;
		}
		if (required_resources.geode > geode) {
			geode = required_resources.geode - geode;
		} else {
			geode = 0;
		}
	}

	uint32_t get_time_required_to_fill_missing_resources() {
		uint32_t result = ore;

		if (result < clay) {
			result = clay;
		}
		if (result < obsidian) {
			result = obsidian;
		}
		if (result < geode) {
			result = geode;
		}

		return result;
	}
};

struct blueprint_str {
	uint32_t number;
	common_t ore_cost;
	common_t clay_cost;
	common_t obsidian_cost;
	common_t geode_cost;

	common_t get_cost_by_type(resource_types_t res_type) const {
		switch (res_type) {
			case E_ORE:
				return ore_cost;
			case E_CLAY:
				return clay_cost;
			case E_OBSIDIAN:
				return obsidian_cost;
			case E_GEODE:
				return geode_cost;
			default:
				assert(false);
				return ore_cost;
				break;
		}
	}

	common_t get_max_robots() const {
		common_t result = {};

		for (size_t i = 0; i < E_RES_TYPES_COUNT; i++) {
			common_t cost = get_cost_by_type(static_cast<resource_types_t>(i));

			uint32_t value = cost.get(E_ORE);
			if (value > result.get(E_ORE)) {
				result.set(E_ORE, value);
			}

			value = cost.get(E_CLAY);
			if (value > result.get(E_CLAY)) {
				result.set(E_CLAY, value);
			}

			value = cost.get(E_OBSIDIAN);
			if (value > result.get(E_OBSIDIAN)) {
				result.set(E_OBSIDIAN, value);
			}
		}

		result.set(E_GEODE, UINT32_MAX);

		return result;
	}

	void clear() {
		number = 0;
		ore_cost = {};
		clay_cost = {};
		obsidian_cost = {};
		geode_cost = {};
	}
};

class AoC2022_day19 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<blueprint_str> blueprints_;
	void get_max_geodes(const blueprint_str& blueprint, uint32_t time, common_t robots, common_t resources, const uint32_t time_limit, uint64_t& geodes_max);
	int64_t get_quality_levels(const bool part2);
};

bool AoC2022_day19::init(const std::vector<std::string> lines) {
	std::stringstream ss;
	uint32_t value;
	blueprints_.clear();

	for (size_t i = 0; i < lines.size(); i++) {
		blueprint_str bp;

		bp.clear();

		ss.str(lines[i]);

		ss.ignore(10); //"Blueprint "
		ss >> bp.number;
		ss.ignore(23); // ": Each ore robot costs "
		ss >> value;
		bp.ore_cost.set(E_ORE, value);
		ss.ignore(28); //" ore. Each clay robot costs "
		ss >> value;
		bp.clay_cost.set(E_ORE, value);
		ss.ignore(32); //" ore. Each obsidian robot costs "
		ss >> value;
		bp.obsidian_cost.set(E_ORE, value);
		ss.ignore(9); //" ore and "
		ss >> value;
		bp.obsidian_cost.set(E_CLAY, value);
		ss.ignore(30); //" clay. Each geode robot costs "
		ss >> value;
		bp.geode_cost.set(E_ORE, value);
		ss.ignore(9); //" ore and "
		ss >> value;
		bp.geode_cost.set(E_OBSIDIAN, value);
		//" obsidian."

		blueprints_.push_back(bp);
	}

	return true;
}

int32_t AoC2022_day19::get_aoc_day() {
	return 19;
}

int32_t AoC2022_day19::get_aoc_year() {
	return 2022;
}

void AoC2022_day19::tests() {
	int64_t result;

	if (init({"Blueprint 1: Each ore robot costs 4 ore. Each clay robot costs 2 ore. Each obsidian robot costs 3 ore and 14 clay. Each geode robot costs 2 ore "
			  "and 7 obsidian.",
			  "Blueprint 2: Each ore robot costs 2 ore. Each clay robot costs 3 ore. Each obsidian robot costs 3 ore and 8 clay. Each geode robot costs 3 ore "
			  "and 12 obsidian."})) {
		result = get_quality_levels(false); // 33
		//result = get_quality_levels(true);	// 3472 disabled - takes too long
	}
}

void AoC2022_day19::get_max_geodes(const blueprint_str& blueprint, uint32_t time, common_t robots, common_t resources, const uint32_t time_limit,
								   uint64_t& geodes_max) {
	common_t cost, resources_new, robots_new, robots_max;
	uint32_t time_new;

	robots_max = blueprint.get_max_robots();

	for (int32_t i = E_GEODE; i >= E_ORE; i--) {
		resource_types_t robot_next = static_cast<resource_types_t>(i);

		cost = blueprint.get_cost_by_type(robot_next);

		if (!robots.have_robots_required_for_robot(cost)) {
			continue;
		}

		if (robots.get(robot_next) >= robots_max.get(robot_next)) {
			continue;
		}

		resources_new = resources;
		robots_new = robots;
		time_new = time;

		while ((!resources_new.have_sufficient_resources(cost)) && (time_new < time_limit)) {
			resources_new.add(robots_new);
			time_new++;
		}

		if (time_new == time_limit) {
			uint32_t geodes = resources_new.get(E_GEODE);

			if (geodes > geodes_max) {
				geodes_max = geodes;
			}
		} else if (time_new > time_limit) {
			assert(false);
		} else if (resources_new.have_sufficient_resources(cost)) {
			resources_new.add(robots_new);
			time_new++;

			robots_new.inc(robot_next);
			resources_new.sub(cost);

			get_max_geodes(blueprint, time_new, robots_new, resources_new, time_limit, geodes_max);
		}
	}
}

int64_t AoC2022_day19::get_quality_levels(const bool part2) {
	uint64_t geodes, max = 0;
	common_t robots, resources;

	if (part2) {
		max = 1;
		uint32_t cnt = (blueprints_.size() < 3) ? blueprints_.size() : 3;

		for (size_t i = 0; i < cnt; i++) {
			robots = {};
			resources = {};
			geodes = 0;
			robots.inc(E_ORE);

			get_max_geodes(blueprints_[i], 0, robots, resources, C_TIME_LIMIT_PART2, geodes);

			max *= geodes;
		}

	} else {
		for (auto& blueprint : blueprints_) {
			robots = {};
			resources = {};
			geodes = 0;
			robots.inc(E_ORE);

			get_max_geodes(blueprint, 0, robots, resources, C_TIME_LIMIT_PART1, geodes);

			max += geodes * blueprint.number;
		}
	}

	return max;
}

bool AoC2022_day19::part1() {
	int64_t result;

	result = get_quality_levels(false);

	result1_ = std::to_string(result);

	return true;
}

bool AoC2022_day19::part2() {
	int64_t result;

	result = get_quality_levels(true);

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2022_day19 day19;

	return day19.main_execution();
}
