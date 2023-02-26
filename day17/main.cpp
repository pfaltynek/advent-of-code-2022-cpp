#include "./../common/aoc.hpp"

const uint64_t C_ROCKS_COUNT_PART1 = 2022;
const uint64_t C_ROCKS_COUNT_PART2 = 1000000000000;
const int32_t C_ROCK_VERT_POSITION = 3;

/*
rock shapes encoded as 16bit numbers to keep chambeer width 7 units + left and right borders -> effectively 9 lowest bits used only
	and shape initial position (2 units from left)
####	-> 	".... .... ..## ##.."	->	0x003C

.#.		-> 	".... .... ...# ...."	->	0x0010
###		-> 	".... .... ..## #..."	->	0x0038
.#.		-> 	".... .... ...# ...."	->	0x0010

..#		-> 	".... .... .... #..."	->	0x0008
..#		-> 	".... .... .... #..."	->	0x0008
###		-> 	".... .... ..## #..."	->	0x0038

#		-> 	".... .... ..#. ...."	->	0x0020
#		-> 	".... .... ..#. ...."	->	0x0020
#		-> 	".... .... ..#. ...."	->	0x0020
#		-> 	".... .... ..#. ...."	->	0x0020

##		-> 	".... .... ..## ...."	->	0x0030
##		-> 	".... .... ..## ...."	->	0x0030
*/

typedef std::vector<uint16_t> shape;
const shape C_ROCK_SHAPE_MINUS = {0x003C};
const shape C_ROCK_SHAPE_PLUS = {0x0010, 0x0038, 0x0010};
const shape C_ROCK_SHAPE_J = {0x0038, 0x0008, 0x0008};
const shape C_ROCK_SHAPE_PIPE = {0x0020, 0x0020, 0x0020, 0x0020};
const shape C_ROCK_SHAPE_DOT = {0x0030, 0x0030};
const uint16_t C_BORDERS = 0x0101;
const uint16_t C_CAVE_BOTTOM = 0x01FF;

const std::vector<shape> C_ROCK_SHAPES = {C_ROCK_SHAPE_MINUS, C_ROCK_SHAPE_PLUS, C_ROCK_SHAPE_J, C_ROCK_SHAPE_PIPE, C_ROCK_SHAPE_DOT};

class AoC2022_day17 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::string jet_directions_;
	size_t jet_direction_idx_;
	size_t rock_shape_idx_;
	shape cave_;
	void add_rock_to_cave();
	bool apply_jet_direction(const bool direction_left, shape& rock);
	bool shape_in_conflict(const shape& rock, const int64_t position);
	shape get_next_rock_shape();
	bool get_next_jet_direction();
	uint64_t get_rock_tower_height_part1();
	uint64_t get_rock_tower_height_part2();
	void print();
	std::string print_line(const uint16_t line);
};

bool AoC2022_day17::init(const std::vector<std::string> lines) {

	jet_directions_.clear();

	for (size_t i = 0; i < lines.size(); i++) {
		const size_t pos = lines[i].find_first_not_of("<>");

		if (pos != std::string::npos) {
			std::cout << "Invalid jet direction at line " << i + 1 << " position " << pos + 1 << std::endl;
			return false;
		} else {
			jet_directions_.append(lines[i]);
		}
	}

	return true;
}

shape AoC2022_day17::get_next_rock_shape() {
	shape result = C_ROCK_SHAPES[rock_shape_idx_];

	if (++rock_shape_idx_ >= C_ROCK_SHAPES.size()) {
		rock_shape_idx_ = 0;
	}

	return result;
}

bool AoC2022_day17::apply_jet_direction(const bool direction_left, shape& rock) {
	for (auto& line : rock) {
		if (direction_left) {
			line = line << 1;
		} else {
			line = line >> 1;
		}

		if ((line & C_BORDERS) != 0) {
			return false;
		}
	}

	return true;
}

bool AoC2022_day17::get_next_jet_direction() {
	bool result = jet_directions_[jet_direction_idx_] == '<';

	if (++jet_direction_idx_ >= jet_directions_.size()) {
		jet_direction_idx_ = 0;
	}

	return result;
}

std::string AoC2022_day17::print_line(const uint16_t line) {
	std::string result = {};
	uint16_t bit = 1;

	while (bit < C_CAVE_BOTTOM) {
		if ((bit & C_BORDERS) != 0) {
			result.insert(0, "|");
		} else if ((bit & line) != 0) {
			result.insert(0, "#");
		} else {
			result.insert(0, ".");
		}

		bit = bit << 1;
	}

	return result;
}

void AoC2022_day17::print() {
	std::cout << std::endl;
	uint16_t line;
	std::string sline;

	for (auto rit = cave_.rbegin(); rit != cave_.rend(); ++rit) {
		line = *rit;
		std::cout << print_line(line) << std::endl;
	}

	std::cout << "+=======+" << std::endl;

	std::cout << std::endl;
}

bool AoC2022_day17::shape_in_conflict(const shape& rock, const int64_t position) {
	bool result = false;

	for (size_t i = position; i < std::min(position + rock.size(), cave_.size()); i++) {
		if ((cave_[i] & rock[i - position]) != 0) {
			result = true;
			break;
		}
	}

	return result;
}

void AoC2022_day17::add_rock_to_cave() {
	int64_t pos;

	shape next = get_next_rock_shape();
	shape jet_applied;

	pos = cave_.size() + C_ROCK_VERT_POSITION;

	while (true) {
		jet_applied = next;
		if (apply_jet_direction(get_next_jet_direction(), jet_applied)) {
			if (!shape_in_conflict(jet_applied, pos)) {
				next = jet_applied;
			}
		}

		pos--;

		if (pos < 0) {
			pos++;
			break;
		} else {
			if (shape_in_conflict(next, pos)) {
				pos++;
				break;
			}
		}
	}

	for (size_t i = pos; i < pos + next.size(); i++) {
		if (i < cave_.size()) {
			cave_[i] |= next[i - pos];
		} else {
			cave_.push_back(next[i - pos]);
		}
	}
	// print(cave);
}

uint64_t AoC2022_day17::get_rock_tower_height_part1() {
	cave_.clear();
	rock_shape_idx_ = 0;
	jet_direction_idx_ = 0;

	for (uint64_t r = 0; r < C_ROCKS_COUNT_PART1; r++) {
		add_rock_to_cave();
	}

	return cave_.size();
}

uint64_t AoC2022_day17::get_rock_tower_height_part2() {
	uint64_t result = 0, k;
	size_t prev = 0;
	std::string diffs, pattern;
	std::vector<size_t> cave_sizes;
	size_t first, second, length, pos, rest, count;
	bool reset_pattern;

	cave_.clear();
	diffs.clear();
	diffs.reserve(10000);
	cave_sizes.clear();
	rock_shape_idx_ = 0;
	jet_direction_idx_ = 0;

	for (size_t r = 0; r < 10000; r++) {
		add_rock_to_cave();
		diffs += ('0' + static_cast<uint8_t>(cave_.size() - prev));
		cave_sizes.push_back(cave_.size());
		prev = cave_.size();
	}

	first = 0;
	length = 10;
	second = first + length;

	while (true) {
		pattern = diffs.substr(first, length);
		pos = diffs.find(pattern, second);

		if (pos == std::string::npos) {
			first++;
			second = first + length;
		} else {
			length = pos - first;
			second = first + length;
			pattern = diffs.substr(first, length);
			pos = diffs.find(pattern, second);

			reset_pattern = false;
			if (pos != second) {
				reset_pattern = true;
			} else {
				size_t next = pos;

				for (size_t check = 0; check < 3; check++) {
					next += length;
					pos = diffs.find(pattern, next);

					if (pos != next) {
						reset_pattern = true;
						break;
					}
				}
			}

			if (reset_pattern) {
				first = second;
				length = 10;
				second = first + length;
			} else {
				break;
			}
		}
	}

	k = cave_sizes[first + length - 1] - cave_sizes[first - 1];

	count = (C_ROCKS_COUNT_PART2 - first) / length;
	rest = C_ROCKS_COUNT_PART2 - (count * length) - first;

	result = (k * count) + cave_sizes[first - 1 + rest];

	return result;
}

int32_t AoC2022_day17::get_aoc_day() {
	return 17;
}

int32_t AoC2022_day17::get_aoc_year() {
	return 2022;
}

void AoC2022_day17::tests() {
	int64_t result;

	if (init({">>><<><>><<<>><>>><<<>>><<<><<<>><>><<>>"})) {
		result = get_rock_tower_height_part1(); // 3068
		result = get_rock_tower_height_part2(); // 1514285714288
	}
}

bool AoC2022_day17::part1() {
	int64_t result;

	result = get_rock_tower_height_part1();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2022_day17::part2() {
	int64_t result;

	result = get_rock_tower_height_part2();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2022_day17 day17;

	return day17.main_execution();
}
