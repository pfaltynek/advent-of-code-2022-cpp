#include "./../common/aoc.hpp"
#include <array>
#include <regex>

const std::regex C_INSTRUCTION_ADDX_REGEX("^addx (-?\\d+)$");
const std::string C_INSTRUCTION_NOOP("noop");
const std::array<uint16_t, 6> C_CHECKPOINTS({20, 60, 100, 140, 180, 220});
const size_t C_DISPLAY_SIZE = 240;
const size_t C_DISPLAY_WIDTH = 40;
const size_t C_DISPLAY_HEIGHT = 6;

class AoC2022_day10 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	uint64_t get_signal_strengths_sum(const bool part2);
	uint16_t get_cycles_by_instruction(const int32_t instruction);
	std::string print_display();
	std::vector<int32_t> instructions_;
	std::array<bool, C_DISPLAY_SIZE> display_;
};

bool AoC2022_day10::init(const std::vector<std::string> lines) {
	std::smatch sm;
	int32_t value;

	instructions_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {
		if (lines[i] == C_INSTRUCTION_NOOP) {
			instructions_.push_back(0);
		} else if (std::regex_match(lines[i], sm, C_INSTRUCTION_ADDX_REGEX)) {
			value = static_cast<int32_t>(stoi(sm.str(1)));
			if (value != 0) {
				instructions_.push_back(value);
			} else {
				std::cout << "Unsupported instruction 'addx 0' at line " << i + 1 << std::endl;
				return false;
			}
		} else {
			std::cout << "Invalid instruction at line " << i + 1 << std::endl;
			return false;
		}
	}

	return true;
}

uint16_t AoC2022_day10::get_cycles_by_instruction(const int32_t instruction) {
	uint16_t cycles = 1;

	if (instruction != 0) {
		cycles++;
	}

	return cycles;
}

uint64_t AoC2022_day10::get_signal_strengths_sum(const bool part2) {
	uint64_t result = 0;
	int64_t x = 1, tmp;
	uint64_t cycles = 0, countdown = 0, hpos;
	uint8_t checkpoint_idx = 0;
	int32_t value;
	size_t ip = 0;
	bool finished = false, pixel;

	if (instructions_.size() == 0) {
		return 0;
	}

	display_.fill(false);

	cycles = 0;
	value = instructions_[ip];
	ip++;
	countdown = get_cycles_by_instruction(value);

	while (!finished) {
		cycles++;
		countdown--;

		if (part2) {
			pixel = false;

			tmp = x - 1;
			hpos = (cycles - 1) % C_DISPLAY_WIDTH;

			for (size_t i = 0; i < 3; i++) {
				if ((tmp >= 0) && (tmp < static_cast<int64_t>(C_DISPLAY_WIDTH))) {
					if (tmp == static_cast<int64_t>(hpos)) {
						pixel = true;
						break;
					}
				}

				tmp++;
			}

			display_[cycles - 1] = pixel;
		} else {
			if (cycles == C_CHECKPOINTS[checkpoint_idx]) {
				result += x * cycles;

				checkpoint_idx++;
			}
		}

		if (countdown == 0) {
			if (value != 0) {
				x += value;
			}

			value = instructions_[ip];
			ip++;

			countdown = get_cycles_by_instruction(value);
		}

		if (part2) {
			finished = cycles >= C_DISPLAY_SIZE;
		} else {
			finished = checkpoint_idx >= C_CHECKPOINTS.size();
		}
	}

	return result;
}

std::string AoC2022_day10::print_display() {
	std::stringstream ss;

	ss.clear();

	for (size_t i = 0; i < C_DISPLAY_SIZE; i++) {
		if (i % C_DISPLAY_WIDTH == 0) {
			ss << std::endl;
		}
		ss << (display_[i] ? '#' : '.');
	}

	return ss.str();
}

int32_t AoC2022_day10::get_aoc_day() {
	return 10;
}

int32_t AoC2022_day10::get_aoc_year() {
	return 2022;
}

void AoC2022_day10::tests() {
	int64_t result;

	if (init({"addx 15",  "addx -11", "addx 6",	  "addx -3", "addx 5",	 "addx -1",	 "addx -8",	 "addx 13",	 "addx 4",	 "noop",	 "addx -1",	 "addx 5",
			  "addx -1",  "addx 5",	  "addx -1",  "addx 5",	 "addx -1",	 "addx 5",	 "addx -1",	 "addx -35", "addx 1",	 "addx 24",	 "addx -19", "addx 1",
			  "addx 16",  "addx -11", "noop",	  "noop",	 "addx 21",	 "addx -15", "noop",	 "noop",	 "addx -3",	 "addx 9",	 "addx 1",	 "addx -3",
			  "addx 8",	  "addx 1",	  "addx 5",	  "noop",	 "noop",	 "noop",	 "noop",	 "noop",	 "addx -36", "noop",	 "addx 1",	 "addx 7",
			  "noop",	  "noop",	  "noop",	  "addx 2",	 "addx 6",	 "noop",	 "noop",	 "noop",	 "noop",	 "noop",	 "addx 1",	 "noop",
			  "noop",	  "addx 7",	  "addx 1",	  "noop",	 "addx -13", "addx 13",	 "addx 7",	 "noop",	 "addx 1",	 "addx -33", "noop",	 "noop",
			  "noop",	  "addx 2",	  "noop",	  "noop",	 "noop",	 "addx 8",	 "noop",	 "addx -1",	 "addx 2",	 "addx 1",	 "noop",	 "addx 17",
			  "addx -9",  "addx 1",	  "addx 1",	  "addx -3", "addx 11",	 "noop",	 "noop",	 "addx 1",	 "noop",	 "addx 1",	 "noop",	 "noop",
			  "addx -13", "addx -19", "addx 1",	  "addx 3",	 "addx 26",	 "addx -30", "addx 12",	 "addx -1",	 "addx 3",	 "addx 1",	 "noop",	 "noop",
			  "noop",	  "addx -9",  "addx 18",  "addx 1",	 "addx 2",	 "noop",	 "noop",	 "addx 9",	 "noop",	 "noop",	 "noop",	 "addx -1",
			  "addx 2",	  "addx -37", "addx 1",	  "addx 3",	 "noop",	 "addx 15",	 "addx -21", "addx 22",	 "addx -6",	 "addx 1",	 "noop",	 "addx 2",
			  "addx 1",	  "noop",	  "addx -10", "noop",	 "noop",	 "addx 20",	 "addx 1",	 "addx 2",	 "addx 2",	 "addx -6",	 "addx -11", "noop",
			  "noop",	  "noop"})) {
		result = get_signal_strengths_sum(false); // 36
		result = get_signal_strengths_sum(true);
		std::string tmp = print_display();
		// ##..##..##..##..##..##..##..##..##..##..
		// ###...###...###...###...###...###...###.
		// ####....####....####....####....####....
		// #####.....#####.....#####.....#####.....
		// ######......######......######......####
		// #######.......#######.......#######.....
	}
}

bool AoC2022_day10::part1() {
	int64_t result;

	result = get_signal_strengths_sum(false);

	result1_ = std::to_string(result);

	return true;
}

bool AoC2022_day10::part2() {

	get_signal_strengths_sum(true);

	result2_ = print_display();

	return true;
}

int main(void) {
	AoC2022_day10 day10;

	return day10.main_execution();
}
