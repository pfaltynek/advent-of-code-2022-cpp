#include "./../common/aoc.hpp"
#include <map>
#include <regex>

enum operation_t { E_ADD, E_SUB, E_MUL, E_DIV };

const std::regex C_MONKEY_TEMPLATE_NUMBER("^([a-z]{4}): (\\d+)$");
const std::regex C_MONKEY_TEMPLATE_MATH("^([a-z]{4}): ([a-z]{4}) ([\\*\\/\\+\\-]) ([a-z]{4})$");
const std::string C_ROOT_NAME = "root";
const std::string C_INPUT_NAME = "humn";

struct monkey_str {
	std::string name;
	int64_t value;
	std::string monkey1;
	std::string monkey2;
	operation_t operation;
	bool yells_number;
};

class AoC2022_day21 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::map<std::string, monkey_str> monkeys_;
	int64_t what_number_root_monkey_yells();
	int64_t what_number_to_yell_for_root_equality();
	void solve_monkeys(std::map<std::string, monkey_str>& maths, std::map<std::string, int64_t>& yells);
	int64_t solve_math(const int64_t monkey1, const int64_t monkey2, const operation_t op);
	int64_t solve_reverse_math(const int64_t monkey, const int64_t result, const operation_t op, const bool calculate_monkey1);
	void sort_monkeys(std::map<std::string, monkey_str>& maths, std::map<std::string, int64_t>& yells);
};

bool AoC2022_day21::init(const std::vector<std::string> lines) {
	monkeys_.clear();
	monkey_str monkey;
	std::smatch sm;

	for (size_t i = 0; i < lines.size(); i++) {
		monkey = {};

		if (std::regex_match(lines[i], sm, C_MONKEY_TEMPLATE_NUMBER)) {
			monkey.name = sm.str(1);
			monkey.yells_number = true;
			monkey.value = std::stoi(sm.str(2));
		} else if (std::regex_match(lines[i], sm, C_MONKEY_TEMPLATE_MATH)) {
			monkey.name = sm.str(1);
			monkey.yells_number = false;
			monkey.monkey1 = sm.str(2);
			monkey.monkey2 = sm.str(4);
			switch (sm.str(3)[0]) {
				case '*':
					monkey.operation = E_MUL;
					break;
				case '/':
					monkey.operation = E_DIV;
					break;
				case '+':
					monkey.operation = E_ADD;
					break;
				case '-':
					monkey.operation = E_SUB;
					break;
				default:
					std::cout << "Unknown math operation at line " << i + 1 << std::endl;
					return false;
			}
		} else {
			std::cout << "Unknown monkey data format at line " << i + 1 << std::endl;
			return false;
		}

		monkeys_[monkey.name] = monkey;
	}

	if (!monkeys_.count(C_ROOT_NAME)) {
		std::cout << "root monkey not found" << std::endl;
		return false;
	}

	return true;
}

int32_t AoC2022_day21::get_aoc_day() {
	return 21;
}

int32_t AoC2022_day21::get_aoc_year() {
	return 2022;
}

void AoC2022_day21::tests() {
	int64_t result;

	if (init({"root: pppw + sjmn", "dbpl: 5", "cczh: sllz + lgvd", "zczc: 2", "ptdq: humn - dvpt", "dvpt: 3", "lfqf: 4", "humn: 5", "ljgn: 2",
			  "sjmn: drzm * dbpl", "sllz: 4", "pppw: cczh / lfqf", "lgvd: ljgn * ptdq", "drzm: hmdt - zczc", "hmdt: 32"})) {
		result = what_number_root_monkey_yells();		  // 152
		result = what_number_to_yell_for_root_equality(); // 301
	}
}

int64_t AoC2022_day21::solve_reverse_math(const int64_t monkey, const int64_t result, const operation_t op, const bool calculate_monkey1) {
	int64_t rev_result;
	switch (op) {
		case E_ADD:
			rev_result = result - monkey;
			break;
		case E_SUB:
			if (calculate_monkey1) {
				rev_result = result + monkey;
			} else {
				rev_result = monkey - result;
			}
			break;
		case E_MUL:
			rev_result = result / monkey;
			break;
		case E_DIV:
			if (calculate_monkey1) {
				rev_result = result * monkey;
			} else {
				rev_result = monkey / result;
			}
			break;
		default:
			assert(false);
			rev_result = -1;
			break;
	}

	return rev_result;
}

int64_t AoC2022_day21::solve_math(const int64_t monkey1, const int64_t monkey2, const operation_t op) {
	switch (op) {
		case E_ADD:
			return monkey1 + monkey2;
		case E_SUB:
			return monkey1 - monkey2;
		case E_MUL:
			return monkey1 * monkey2;
		case E_DIV:
			return monkey1 / monkey2;
		default:
			assert(false);
			return -1;
			break;
	}
}

void AoC2022_day21::sort_monkeys(std::map<std::string, monkey_str>& maths, std::map<std::string, int64_t>& yells) {
	maths.clear();
	yells.clear();

	for (auto& pair : monkeys_) {
		if (pair.second.yells_number) {
			yells[pair.first] = pair.second.value;
		} else {
			maths[pair.first] = pair.second;
		}
	}
}

void AoC2022_day21::solve_monkeys(std::map<std::string, monkey_str>& maths, std::map<std::string, int64_t>& yells) {
	bool found;

	do {
		found = false;
		for (auto& pair : maths) {
			if (yells.count(pair.second.monkey1) && yells.count(pair.second.monkey2)) {
				yells[pair.first] = solve_math(yells[pair.second.monkey1], yells[pair.second.monkey2], pair.second.operation);
				yells.erase(pair.second.monkey1);
				yells.erase(pair.second.monkey2);
				maths.erase(pair.first);
				found = true;
				break;
			}
		}
	} while (found);
}

int64_t AoC2022_day21::what_number_root_monkey_yells() {
	std::map<std::string, monkey_str> maths = {};
	std::map<std::string, int64_t> yells = {};

	sort_monkeys(maths, yells);

	solve_monkeys(maths, yells);

	return yells[C_ROOT_NAME];
}

int64_t AoC2022_day21::what_number_to_yell_for_root_equality() {
	std::map<std::string, monkey_str> maths = {};
	std::map<std::string, int64_t> yells = {};
	monkey_str parent;
	int64_t monkey, result;
	std::string tmp;
	bool calculate_monkey1;

	sort_monkeys(maths, yells);

	yells.erase(C_INPUT_NAME);

	solve_monkeys(maths, yells);

	std::string name = C_ROOT_NAME;
	parent = maths[name];

	if (yells.count(parent.monkey1) > 0) {
		result = yells[parent.monkey1];
		yells.erase(parent.monkey1);
		name = parent.monkey2;
	} else if (yells.count(parent.monkey2) > 0) {
		result = yells[parent.monkey2];
		yells.erase(parent.monkey2);
		name = parent.monkey1;
	} else {
		assert(false);
		return -1;
	}

	maths.erase(parent.name);

	while (maths.size() > 0) {
		if (maths.count(name)) {
			parent = maths[name];

			if (yells.count(parent.monkey1) > 0) {
				tmp = parent.monkey1;
				name = parent.monkey2;
				calculate_monkey1 = false;
			} else if (yells.count(parent.monkey2) > 0) {
				tmp = parent.monkey2;
				name = parent.monkey1;
				calculate_monkey1 = true;
			} else {
				break;
			}

			monkey = yells[tmp];
			yells.erase(tmp);
			result = solve_reverse_math(monkey, result, parent.operation, calculate_monkey1);
			maths.erase(parent.name);

			if (name == C_INPUT_NAME) {
				return result;
			}

		} else {
			break;
		}
	}
	return -1;
}

bool AoC2022_day21::part1() {
	int64_t result;

	result = what_number_root_monkey_yells();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2022_day21::part2() {
	int64_t result;

	result = what_number_to_yell_for_root_equality();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2022_day21 day21;

	return day21.main_execution();
}
