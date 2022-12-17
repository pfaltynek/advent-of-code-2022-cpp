#include "./../common/aoc.hpp"
#include "./../common/string-utils.hpp"

struct item {
	std::vector<item> list;
	uint16_t value;
	bool is_list;
};
typedef std::vector<item> item_list;

class AoC2022_day13 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	uint64_t get_right_ordered_packets_sum();
	uint64_t get_decoder_key();
	int8_t are_packets_right_ordered(item_list l, item_list r);
	std::vector<item_list> packets_;
	bool decode_packet(const std::string& line, size_t& idx, item_list& result);
	bool is_list(const std::string line);
};

bool AoC2022_day13::is_list(const std::string line) {
	if ((line.front() == '[') && (line.back() == ']')) {
		return true;
	}

	return false;
}

bool AoC2022_day13::decode_packet(const std::string& line, size_t& idx, item_list& result) {
	std::vector<std::string> items;
	item_list tmp;
	item it;
	bool finished = false, digit = false;
	uint16_t value = 0;

	result.clear();

	while (!finished) {
		switch (line[idx]) {
			case '[':
				it.is_list = true;
				it.value = 0;
				idx++;
				if (!decode_packet(line, idx, it.list)) {
					return false;
				} else {
					result.push_back(it);
				}
				break;

			case ']':
				if (digit) {
					digit = false;
					it.value = value;
					it.list.clear();
					it.is_list = false;
					result.push_back(it);
				}
				idx++;
				finished = true;
				break;

			case ',':
				if (digit) {
					digit = false;
					it.value = value;
					it.list.clear();
					it.is_list = false;
					result.push_back(it);
				}
				idx++;
				break;

			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				if (!digit) {
					digit = true;
					value = line[idx] - '0';
				} else {
					value *= 10;
					value += line[idx] - '0';
				}
				idx++;
				break;

			default:
				std::cout << "Unsupported char in line" << std::endl;
				return false;
				break;
		}
	}

	return true;
}

bool AoC2022_day13::init(const std::vector<std::string> lines) {
	size_t idx = 0, idx1, idx2;
	item_list p1, p2;

	packets_.clear();

	while (idx + 1 < lines.size()) {
		if (lines[idx].empty()) {
			idx++;
			continue;
		}

		idx1 = 0;
		idx2 = 0;

		if (lines[idx].front() != '[') {
			std::cout << "Packet start not found at line " << idx + 1;
			return false;
		} else {
			idx1++;
		}

		if (!decode_packet(lines[idx], idx1, p1)) {
			std::cout << "Invalid packet at line " << idx + 1;
			return false;
		} else {
			idx++;

			if (lines[idx].front() != '[') {
				std::cout << "Packet start not found at line " << idx + 1;
				return false;
			} else {
				idx2++;
			}

			if (!decode_packet(lines[idx], idx2, p2)) {
				std::cout << "Invalid packet at line " << idx + 1;
				return false;
			} else {
				packets_.push_back(p1);
				packets_.push_back(p2);
				idx++;
			}
		}
	}

	return true;
}

int8_t AoC2022_day13::are_packets_right_ordered(item_list l, item_list r) {
	size_t cnt = std::min(l.size(), r.size());
	int8_t subresult;
	item item;

	for (size_t i = 0; i < cnt; i++) {

		if (l[i].is_list && !r[i].is_list) {
			item.is_list = false;
			item.list.clear();
			item.value = r[i].value;
			r[i].is_list = true;
			r[i].list.push_back(item);
		} else if (!l[i].is_list && r[i].is_list) {
			item.is_list = false;
			item.list.clear();
			item.value = l[i].value;
			l[i].is_list = true;
			l[i].list.push_back(item);
		}

		if (l[i].is_list && r[i].is_list) {
			subresult = are_packets_right_ordered(l[i].list, r[i].list);

			if (subresult != 0) {
				return subresult;
			}
		} else if (!l[i].is_list && !r[i].is_list) {
			if (l[i].value > r[i].value) {
				return -1;
			} else if (l[i].value < r[i].value) {
				return 1;
			}
		}
	}

	if (l.size() < r.size()) {
		return 1;
	} else if (l.size() > r.size()) {
		return -1;
	}

	return 0;
}

uint64_t AoC2022_day13::get_right_ordered_packets_sum() {
	uint64_t result = 0;

	for (size_t i = 0; i < packets_.size() / 2; i++) {
		if (are_packets_right_ordered(packets_[2 * i], packets_[2 * i + 1]) > 0) {
			result += i + 1;
		}
	}

	return result;
}

uint64_t AoC2022_day13::get_decoder_key() {
	item_list key1, key2;
	item it1, it2;
	std::vector<item_list> packets(packets_);
	bool sorted = false;
	size_t idx1, idx2;

	it1.is_list = false;
	it1.value = 2;
	it1.list.clear();

	it2.is_list = true;
	it2.value = 0;
	it2.list.clear();
	it2.list.push_back(it1);
	key1.push_back(it2);
	it2.list[0].value = 6;
	key2.push_back(it2);

	packets.push_back(key1);
	packets.push_back(key2);

	while (!sorted) {
		sorted = true;

		for (size_t i = 1; i < packets.size(); i++) {
			if (are_packets_right_ordered(packets[i - 1], packets[i]) < 0) {
				std::swap(packets[i - 1], packets[i]);
				sorted = false;
			}
		}
	}

	idx1 = 0;
	idx2 = 0;

	for (size_t i = 0; i < packets.size(); i++) {
		if (packets[i].size() == 1) {
			if (packets[i][0].is_list) {
				if (packets[i][0].list.size() == 1) {
					if (!packets[i][0].list[0].is_list) {
						if (packets[i][0].list[0].value == 2) {
							idx1 = i + 1;
						}
						if (packets[i][0].list[0].value == 6) {
							idx2 = i + 1;
						}
					}
				}
			}
		}
	}

	return idx1 * idx2;
}

int32_t AoC2022_day13::get_aoc_day() {
	return 13;
}

int32_t AoC2022_day13::get_aoc_year() {
	return 2022;
}

void AoC2022_day13::tests() {
	int64_t result;

	if (init({"[1,1,3,1,1]",
			  "[1,1,5,1,1]",
			  "",
			  "[[1],[2,3,4]]",
			  "[[1],4]",
			  "",
			  "[9]",
			  "[[8,7,6]]",
			  "",
			  "[[4,4],4,4]",
			  "[[4,4],4,4,4]",
			  "",
			  "[7,7,7,7]",
			  "[7,7,7]",
			  "",
			  "[]",
			  "[3]",
			  "",
			  "[[[]]]",
			  "[[]]",
			  "",
			  "[1,[2,[3,[4,[5,6,7]]]],8,9]",
			  "[1,[2,[3,[4,[5,6,0]]]],8,9]"})) {
		result = get_right_ordered_packets_sum(); // 13
		result = get_decoder_key();				  // 140
	}
}

bool AoC2022_day13::part1() {
	int64_t result;

	result = get_right_ordered_packets_sum();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2022_day13::part2() {
	int64_t result;

	result = get_decoder_key();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2022_day13 day13;

	return day13.main_execution();
}
