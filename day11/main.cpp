#include "./../common/aoc.hpp"
#include "./../common/string-utils.hpp"
#include <map>
#include <regex>

const std::regex C_MONKEY_ID_REGEX("^Monkey (\\d+):$");
const std::regex C_MONKEY_ITEMS_REGEX("^  Starting items: (.+)$");
const std::regex C_MONKEY_OP_MULTIPLY_REGEX("^  Operation: new = old \\* (\\d+)$");
const std::regex C_MONKEY_OP_ADD_REGEX("^  Operation: new = old \\+ (\\d+)$");
const std::regex C_MONKEY_OP_QUAD_REGEX("^  Operation: new = old \\* old$");
const std::regex C_MONKEY_TEST_REGEX("^  Test: divisible by (\\d+)$");
const std::regex C_MONKEY_TRUE_REGEX("^    If true: throw to monkey (\\d+)$");
const std::regex C_MONKEY_FALSE_REGEX("^    If false: throw to monkey (\\d+)$");
const size_t C_ROUNDS_COUNT_PART1 = 20;
const size_t C_ROUNDS_COUNT_PART2 = 10000;

enum operation_t { E_MONKEY_ADD, E_MONKEY_MULTIPLY, E_MONKEY_QUAD };

struct monkey_str {
	uint8_t id;
	std::vector<uint64_t> items;
	operation_t operation;
	uint64_t value;
	uint64_t test;
	uint8_t throw_if_false, throw_if_true;
};

class AoC2022_day11 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	uint64_t get_monkey_business_level(const bool part2);
	std::map<uint8_t, monkey_str> monkeys_;
	std::map<uint8_t, uint64_t> stats_;
	std::vector<uint8_t> monkey_order_;
};

bool AoC2022_day11::init(const std::vector<std::string> lines) {
	std::smatch sm;
	uint8_t monkey_part = 0;
	monkey_str monkey;

	monkeys_.clear();
	monkey_order_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {
		if (lines[i].empty()) {
			continue;
		}

		switch (monkey_part) {
			case 0:
				monkey = {};

				if (std::regex_match(lines[i], sm, C_MONKEY_ID_REGEX)) {
					monkey.id = static_cast<uint8_t>(std::stoi(sm.str(1)));
					monkey_part++;
				} else {
					std::cout << "Monkey number not found on line " << i + 1 << std::endl;
					return false;
				}
				break;
			case 1:
				if (std::regex_match(lines[i], sm, C_MONKEY_ITEMS_REGEX)) {
					std::vector<std::string> items = split(sm.str(1), ", ");

					if (items.size() == 0) {
						std::cout << "Monkey items not recognized on line " << i + 1 << std::endl;
						return false;
					} else {
						for (size_t j = 0; j < items.size(); j++) {
							monkey.items.push_back(static_cast<size_t>(std::stoi(items[j])));
						}
					}
					monkey_part++;
				} else {
					std::cout << "Monkey items not found on line " << i + 1 << std::endl;
					return false;
				}
				break;
			case 2:
				if (std::regex_match(lines[i], sm, C_MONKEY_OP_MULTIPLY_REGEX)) {
					monkey.value = static_cast<uint64_t>(std::stoi(sm.str(1)));
					monkey.operation = E_MONKEY_MULTIPLY;
					monkey_part++;
				} else if (std::regex_match(lines[i], sm, C_MONKEY_OP_ADD_REGEX)) {
					monkey.value = static_cast<uint64_t>(std::stoi(sm.str(1)));
					monkey.operation = E_MONKEY_ADD;
					monkey_part++;
				} else if (std::regex_match(lines[i], sm, C_MONKEY_OP_QUAD_REGEX)) {
					monkey.value = 0;
					monkey.operation = E_MONKEY_QUAD;
					monkey_part++;
				} else {
					std::cout << "Monkey operation not found on line " << i + 1 << std::endl;
					return false;
				}
				break;
			case 3:
				if (std::regex_match(lines[i], sm, C_MONKEY_TEST_REGEX)) {
					monkey.test = static_cast<uint64_t>(std::stoi(sm.str(1)));
					monkey_part++;
				} else {
					std::cout << "Monkey test not found on line " << i + 1 << std::endl;
					return false;
				}
				break;
			case 4:
				if (std::regex_match(lines[i], sm, C_MONKEY_TRUE_REGEX)) {
					monkey.throw_if_true = static_cast<uint8_t>(std::stoi(sm.str(1)));
					monkey_part++;
				} else {
					std::cout << "Monkey if-true not found on line " << i + 1 << std::endl;
					return false;
				}
				break;
			case 5:
				if (std::regex_match(lines[i], sm, C_MONKEY_FALSE_REGEX)) {
					monkey.throw_if_false = static_cast<uint8_t>(std::stoi(sm.str(1)));
				} else {
					std::cout << "Monkey if-true not found on line " << i + 1 << std::endl;
					return false;
				}

				monkeys_[monkey.id] = monkey;
				monkey_order_.push_back(monkey.id);

				monkey_part = 0;
				break;
		}
	}

	return true;
}

uint64_t AoC2022_day11::get_monkey_business_level(const bool part2) {
	std::map<uint8_t, monkey_str> monkeys;
	uint64_t max1 = 0, max2 = 0;
	uint8_t id;
	uint64_t new_worry;
	size_t rounds;
	uint64_t chinese_remainder_modulo; // see https://en.wikipedia.org/wiki/Chinese_remainder_theorem

	monkeys = monkeys_;

	if (part2) {
		rounds = C_ROUNDS_COUNT_PART2;

		chinese_remainder_modulo = 1;

		for (uint8_t& id : monkey_order_) {
			chinese_remainder_modulo *= monkeys[id].test;
		}

	} else {
		rounds = C_ROUNDS_COUNT_PART1;
	}

	stats_.clear();
	for (size_t r = 0; r < rounds; r++) {
		for (uint8_t i = 0; i < monkey_order_.size(); i++) {
			id = monkey_order_[i];

			for (size_t j = 0; j < monkeys[id].items.size(); j++) {
				new_worry = monkeys[id].items[j];

				switch (monkeys[id].operation) {
					case E_MONKEY_ADD:
						new_worry += monkeys[id].value;
						break;
					case E_MONKEY_MULTIPLY:
						new_worry *= monkeys[id].value;
						break;
					case E_MONKEY_QUAD:
						new_worry *= new_worry;
						break;
					default:
						return 0;
						break;
				}

				if (!part2) {
					new_worry /= 3;
				} else {
					new_worry %= chinese_remainder_modulo;
				}

				if ((new_worry % monkeys[id].test) == 0) {
					monkeys[monkeys[id].throw_if_true].items.push_back(new_worry);
				} else {
					monkeys[monkeys[id].throw_if_false].items.push_back(new_worry);
				}

				stats_[id]++;
			}

			monkeys[id].items.clear();
		}
	}

	for (uint8_t i = 0; i < monkey_order_.size(); i++) {
		if (stats_[monkey_order_[i]] > max1) {
			max2 = max1;
			max1 = stats_[monkey_order_[i]];

		} else if (stats_[monkey_order_[i]] > max2) {
			max2 = stats_[monkey_order_[i]];
		}
	}

	return max1 * max2;
}

int32_t AoC2022_day11::get_aoc_day() {
	return 11;
}

int32_t AoC2022_day11::get_aoc_year() {
	return 2022;
}

void AoC2022_day11::tests() {
	int64_t result;

	if (init({"Monkey 0:",
			  "  Starting items: 79, 98",
			  "  Operation: new = old * 19",
			  "  Test: divisible by 23",
			  "    If true: throw to monkey 2",
			  "    If false: throw to monkey 3",
			  "",
			  "Monkey 1:",
			  "  Starting items: 54, 65, 75, 74",
			  "  Operation: new = old + 6",
			  "  Test: divisible by 19",
			  "    If true: throw to monkey 2",
			  "    If false: throw to monkey 0",
			  "",
			  "Monkey 2:",
			  "  Starting items: 79, 60, 97",
			  "  Operation: new = old * old",
			  "  Test: divisible by 13",
			  "    If true: throw to monkey 1",
			  "    If false: throw to monkey 3",
			  "",
			  "Monkey 3:",
			  "  Starting items: 74",
			  "  Operation: new = old + 3",
			  "  Test: divisible by 17",
			  "    If true: throw to monkey 0",
			  "    If false: throw to monkey 1"})) {
		result = get_monkey_business_level(false); // 10605
		result = get_monkey_business_level(true); // 2713310158
	}
}

bool AoC2022_day11::part1() {
	int64_t result;

	result = get_monkey_business_level(false);

	result1_ = std::to_string(result);

	return true;
}

bool AoC2022_day11::part2() {
	int64_t result;

	result = get_monkey_business_level(true);

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2022_day11 day11;

	return day11.main_execution();
}
