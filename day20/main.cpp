#include "./../common/aoc.hpp"
#include <algorithm>

const int64_t C_DECRYPTION_KEY = 811589153;

struct number_str {
	int64_t value;
	int16_t index;
};

bool operator==(const number_str& a, const number_str& b) {
	return (a.index == b.index) && (a.value == b.value);
}

class AoC2022_day20 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<number_str> mixed_sequence_;
	number_str zero_;
	bool zero_found_;
	int64_t decode(const bool part2);
	void mixing(std::vector<number_str>& data);
	int64_t get_groove_coords(const std::vector<number_str>& data);
};

bool AoC2022_day20::init(const std::vector<std::string> lines) {
	int64_t value;
	mixed_sequence_.clear();

	for (size_t i = 0; i < lines.size(); i++) {

		value = std::stoi(lines[i]);

		mixed_sequence_.push_back({value, static_cast<int16_t>(i)});
	}

	return true;
}

int32_t AoC2022_day20::get_aoc_day() {
	return 20;
}

int32_t AoC2022_day20::get_aoc_year() {
	return 2022;
}

void AoC2022_day20::tests() {
	int64_t result;

	if (init({"1", "2", "-3", "3", "-2", "0", "4"})) {
		result = decode(false); // 3
		result = decode(true);	// 1623178306
	}
}

void AoC2022_day20::mixing(std::vector<number_str>& data) {
	int64_t position, pos_new;
	int64_t size;
	number_str number;
	std::vector<number_str>::iterator it;

	size = mixed_sequence_.size() - 1;
	zero_found_ = false;

	for (size_t i = 0; i < mixed_sequence_.size(); i++) {
		number = mixed_sequence_[i];

		if (!number.value) {
			zero_ = number;
			zero_found_ = true;
			continue;
		}

		it = std::find(data.begin(), data.end(), number);
		if (it == data.end()) {
			assert(false);
		}

		position = std::distance(data.begin(), it);

		pos_new = position + number.value;
		pos_new %= size;

		if (pos_new < 0) {
			pos_new += size;
		}

		if (position != pos_new) {
			data.erase(it);
			data.insert(data.begin() + pos_new, number);
		}
	}
}

int64_t AoC2022_day20::get_groove_coords(const std::vector<number_str>& data) {
	int32_t position;

	assert(zero_found_);

	auto it = std::find(data.begin(), data.end(), zero_);
	if (it == data.end()) {
		assert(false);
	}

	position = std::distance(data.begin(), it);
	int64_t result = data[(position + 1000) % data.size()].value;
	result += data[(position + 2000) % data.size()].value;
	result += data[(position + 3000) % data.size()].value;

	return result;
}

int64_t AoC2022_day20::decode(const bool part2) {
	std::vector<number_str> data;
	int64_t result = 0;

	if (part2) {
		for (auto& number : mixed_sequence_) {
			number.value *= C_DECRYPTION_KEY;
		}

		data = mixed_sequence_;
		for (size_t i = 0; i < 10; i++) {
			mixing(data);
		}

		result = get_groove_coords(data);
	} else {
		data = mixed_sequence_;

		mixing(data);
		result = get_groove_coords(data);
	}

	return result;
}

bool AoC2022_day20::part1() {
	int64_t result;

	result = decode(false);

	result1_ = std::to_string(result);

	return true;
}

bool AoC2022_day20::part2() {
	int64_t result;

	result = decode(true);

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2022_day20 day20;

	return day20.main_execution();
}
