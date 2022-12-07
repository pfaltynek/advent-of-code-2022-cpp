#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <regex>
#include <stack>

const size_t C_START_OF_PACKET_LENGTH = 4;
const size_t C_START_OF_MESSAGE_LENGTH = 14;

class AoC2022_day06 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	int64_t get_start_message(const size_t length);
	std::string message_;
};

bool AoC2022_day06::init(const std::vector<std::string> lines) {

	message_.clear();

	for (size_t i = 0; i < lines.size(); i++) {
		message_.append(lines[i]);
	}

	return true;
}

int64_t AoC2022_day06::get_start_message(const size_t length) {
	int64_t result = -1;
	size_t end, start = 0, max;
	bool again;

	end = start + length - 1;
	max = message_.size();

	while (end < max) {
		again = false;

		for (size_t i = start; i < end; i++) {
			for (size_t j = i + 1; j <= end; j++) {
				if (message_[i] == message_[j]) {
					start = i + 1;
					end = start + length - 1;
					again = true;
					break;
				}
			}

			if (again) {
				break;
			}
		}

		if (!again) {
			result = start + length;
			break;
		}
	}

	return result;
}

int32_t AoC2022_day06::get_aoc_day() {
	return 6;
}

int32_t AoC2022_day06::get_aoc_year() {
	return 2022;
}

void AoC2022_day06::tests() {
	int64_t result;

	if (init({"bvwbjplbgvbhsrlpgdmjqwftvncz"})) {
		result = get_start_message(C_START_OF_PACKET_LENGTH); // 5
		result = get_start_message(C_START_OF_MESSAGE_LENGTH); // 23
	}
	if (init({"nppdvjthqldpwncqszvftbrmjlhg"})) {
		result = get_start_message(C_START_OF_PACKET_LENGTH); // 6
		result = get_start_message(C_START_OF_MESSAGE_LENGTH); // 23
	}
	if (init({"nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg"})) {
		result = get_start_message(C_START_OF_PACKET_LENGTH); // 10
		result = get_start_message(C_START_OF_MESSAGE_LENGTH); // 29
	}
	if (init({"zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw"})) {
		result = get_start_message(C_START_OF_PACKET_LENGTH); // 11
		result = get_start_message(C_START_OF_MESSAGE_LENGTH); // 26
	}
	if (init({"mjqjpqmgbljsphdztnvjfqwrcgsmlb"})) {
		result = get_start_message(C_START_OF_MESSAGE_LENGTH); // 19
	}
}

bool AoC2022_day06::part1() {
	int64_t result;

	result = get_start_message(C_START_OF_PACKET_LENGTH);

	result1_ = std::to_string(result);

	return true;
}

bool AoC2022_day06::part2() {
	int64_t result;

	result = get_start_message(C_START_OF_MESSAGE_LENGTH);

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2022_day06 day06;

	return day06.main_execution();
}
