#include "./../common/aoc.hpp"
#include <map>

class AoC2022_day25 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<std::string> snafus_;
	std::vector<int64_t> snafu_bases_;
	std::map<char, int64_t> snafu_to_decimal_ = {{'=', -2}, {'-', -1}, {'0', 0}, {'1', 1}, {'2', 2}};

	int64_t get_fuel_sum();
	int64_t SNAFU_to_decimal(const std::string& snafu);
	std::string decimal_to_SNAFU(int64_t decimal);
};

bool AoC2022_day25::init(const std::vector<std::string> lines) {
	int64_t snafu_base = 1;
	uint32_t size_max = 0;

	snafus_.clear();
	snafu_bases_.clear();

	for (uint32_t line = 0; line < lines.size(); line++) {
		if (lines[line].find_first_not_of("=-012") != std::string::npos) {
			std::cout << "Invalid SNAFU number in line " << line + 1 << std::endl;
			return false;
		}

		if (lines[line].size() > size_max) {
			size_max = lines[line].size();
		}

		snafus_.push_back(lines[line]);
	}

	for (uint32_t i = 0; i <= size_max + 3; i++) {
		snafu_bases_.push_back(snafu_base);
		snafu_base *= 5;
	}

	return true;
}

int32_t AoC2022_day25::get_aoc_day() {
	return 25;
}

int32_t AoC2022_day25::get_aoc_year() {
	return 2022;
}

int64_t AoC2022_day25::get_fuel_sum() {
	int64_t result = 0;

	for (const auto& snafu : snafus_) {
		result += SNAFU_to_decimal(snafu);
	}

	return result;
}

int64_t AoC2022_day25::SNAFU_to_decimal(const std::string& snafu) {
	int64_t result = 0;

	for(auto rit = snafu.rbegin(); rit != snafu.rend(); ++rit) {
		result += snafu_to_decimal_[*rit] * snafu_bases_[rit - snafu.rbegin()];
	}

	return result;
}

std::string AoC2022_day25::decimal_to_SNAFU(int64_t decimal) {
	std::string result = "";

	while (decimal > 0) {
		int64_t remainder = decimal % 5;
		decimal /= 5;

		if (remainder > 2) {
			remainder -= 5;
			decimal++;
		}

		for (const auto& [key, value] : snafu_to_decimal_) {
			if (value == remainder) {
				result = key + result;
				break;
			}
		}
	}

	return result;
}

void AoC2022_day25::tests() {
	std::string result;

	if (init({"1=-0-2", "12111", "2=0=", "21", "2=01", "111", "20012", "112", "1=-1=", "1-12", "12", "1=", "122"})) {
		result = decimal_to_SNAFU(get_fuel_sum()); // 4890 -> "2=-1=0"
	}
}

bool AoC2022_day25::part1() {
	int64_t result;

	result = get_fuel_sum();

	result1_ = decimal_to_SNAFU(result);

	return true;
}

bool AoC2022_day25::part2() {
	int64_t result;

	result = 0;

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2022_day25 day25;

	return day25.main_execution();
}
