#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <cctype>
#include <map>
#include <set>

enum facing_direction_t { E_FACING_DIRECTION_RIGHT = 0, E_FACING_DIRECTION_DOWN = 1, E_FACING_DIRECTION_LEFT = 2, E_FACING_DIRECTION_UP = 3 };

enum edge_position_t { E_EDGE_POSITION_RIGHT = 0, E_EDGE_POSITION_DOWN = 1, E_EDGE_POSITION_LEFT = 2, E_EDGE_POSITION_UP = 3 };

struct cube_edge_str {
	uint8_t side;
	edge_position_t edge;
	bool swap_axes;
	bool invert_axe;
};

class AoC2022_day22 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::set<coord_str> walls_;
	std::map<int32_t, coord_str> row_limits_, column_limits_;
	std::vector<std::pair<coord_str, coord_str>> sizes_;
	std::map<uint8_t, std::pair<coord_str, coord_str>> tiles_;
	std::map<uint8_t, std::map<edge_position_t, cube_edge_str>> edges_;
	coord_str max_, min_, tile_size_;
	size_t path_idx_;
	std::string path_;
	bool next_path_step_count_;
	bool is_path_finished();
	bool turn_left();
	int32_t get_steps();
	int64_t get_final_password(const bool part2 = false);
	bool can_move(coord_str from, const facing_direction_t direction, const bool part2, coord_str& new_pos, facing_direction_t& new_facing);
	facing_direction_t turn(const facing_direction_t current_facing, const bool turn_left);
	facing_direction_t get_opposite_facing(const facing_direction_t current_facing);
	coord_str do_one_step(coord_str from, const facing_direction_t direction);
	coord_str validate_step(coord_str from, const facing_direction_t direction);
	edge_position_t get_edge_from_facing(const facing_direction_t facing);
	facing_direction_t get_new_facing_from_edge(const edge_position_t edge);
};

bool AoC2022_day22::init(const std::vector<std::string> lines) {
	int32_t row, col, min, max, last_size, size_cnt;
	bool part2 = false, map_active;
	coord_str coord;

	walls_.clear();
	column_limits_.clear();
	row_limits_.clear();
	path_.clear();
	path_idx_ = 0;
	max_ = {INT32_MIN, INT32_MIN};
	min_ = {INT32_MAX, INT32_MAX};
	last_size = size_cnt = 0;
	sizes_.clear();
	tile_size_.x = INT32_MAX;
	tile_size_.y = INT32_MAX;

	for (int32_t i = 0; i < static_cast<int32_t>(lines.size()); i++) {
		if (lines[i].empty()) {
			if (!part2) {
				part2 = true;
				continue;
			} else {
				std::cout << "Invalid notes format" << std::endl;
				return false;
			}
		}

		row = i + 1;

		if (part2) {
			if (lines[i].find_first_not_of("0123456789LR") != std::string::npos) {
				std::cout << "Invalid path specification at line " << row << std::endl;
			}
			path_.append(lines[i]);
		} else {
			min = INT32_MAX;
			max = INT32_MIN;
			map_active = false;
			for (int32_t c = 0; c < static_cast<int32_t>(lines[i].size()); c++) {
				col = c + 1;

				switch (lines[i][c]) {
					case ' ':
						break;
					case '#':
						walls_.insert({col, row});
						map_active = true;
						break;
					case '.':
						map_active = true;
						break;
					default:
						std::cout << "Invalid map character at line " << row << " at position " << col << std::endl;
						return false;
				}

				if (map_active) {
					min = std::min(min, col);
					max = std::max(max, col);

					if (column_limits_.count(col) > 0) {
						coord_str tmp = column_limits_[col];
						column_limits_[col] = {std::min(row, tmp.x), std::max(row, tmp.y)};
					} else {
						column_limits_[col] = {row, row};
					}
				}
			}

			row_limits_[row] = {min, max};
			max_ = {std::max(max_.x, max), std::max(max_.y, row)};
			min_ = {std::min(min_.x, min), std::min(min_.y, row)};

			int32_t size = max - min + 1;

			if (i) {
				if (size != last_size) {
					sizes_.push_back({{last_size, size_cnt}, coord});
					coord = {min, row};
					last_size = size;
					size_cnt = 1;
				} else {
					size_cnt++;
				}
			} else {
				coord = {min, row};
				last_size = size;
				size_cnt = 1;
			}
		}
	}

	sizes_.push_back({{last_size, size_cnt}, coord});

	for (const auto& s : sizes_) {
		tile_size_.x = std::min(s.first.x, tile_size_.x);
		tile_size_.y = std::min(s.first.y, tile_size_.y);
	}

	if (tile_size_.x != tile_size_.y) {
		std::cout << "Error: tile sizes are not square." << std::endl;
		return false;
	}

	return true;
}

int32_t AoC2022_day22::get_aoc_day() {
	return 22;
}

int32_t AoC2022_day22::get_aoc_year() {
	return 2022;
}

void AoC2022_day22::tests() {
	int64_t result;

	if (init({"        ...#", "        .#..", "        #...", "        ....", "...#.......#", "........#...", "..#....#....", "..........#.",
			  "        ...#....", "        .....#..", "        .#......", "        ......#.", "", "10R5L5R10L4R5L5"})) {
		result = get_final_password();	   // 6032
		result = get_final_password(true); // 5031
	}
}

coord_str AoC2022_day22::do_one_step(coord_str from, const facing_direction_t direction) {
	switch (direction) {
		case E_FACING_DIRECTION_LEFT:
			return from + coord_step_west;

		case E_FACING_DIRECTION_RIGHT:
			return from + coord_step_east;

		case E_FACING_DIRECTION_UP:
			return from + coord_step_north;

		case E_FACING_DIRECTION_DOWN:
			return from + coord_step_south;

		default:
			std::cerr << "Invalid facing direction" << std::endl;
			assert(false);
			return coord_str();
	}
}

coord_str AoC2022_day22::validate_step(coord_str from, const facing_direction_t direction) {
	coord_str limits;

	if ((direction == E_FACING_DIRECTION_LEFT || direction == E_FACING_DIRECTION_RIGHT)) {
		limits = row_limits_[from.y];
		if (from.x < limits.x) {
			from.x = limits.y;
		} else if (from.x > limits.y) {
			from.x = limits.x;
		}
	} else {
		limits = column_limits_[from.x];
		if (from.y < limits.x) {
			from.y = limits.y;
		} else if (from.y > limits.y) {
			from.y = limits.x;
		}
	}

	return from;
}

edge_position_t AoC2022_day22::get_edge_from_facing(const facing_direction_t facing) {
	return static_cast<edge_position_t>(facing);
}

facing_direction_t AoC2022_day22::get_new_facing_from_edge(const edge_position_t edge) {
	switch (edge) {
		case E_EDGE_POSITION_LEFT:
			return E_FACING_DIRECTION_RIGHT;
		case E_EDGE_POSITION_RIGHT:
			return E_FACING_DIRECTION_LEFT;
		case E_EDGE_POSITION_UP:
			return E_FACING_DIRECTION_DOWN;
		case E_EDGE_POSITION_DOWN:
			return E_FACING_DIRECTION_UP;
		default:
			std::cerr << "Invalid edge position" << std::endl;
			assert(false);
			return facing_direction_t();
	}
}

bool AoC2022_day22::can_move(coord_str from, const facing_direction_t direction, const bool part2, coord_str& new_pos, facing_direction_t& new_facing) {

	new_facing = direction;

	if (part2) {
		uint8_t tile_idx = 0;
		cube_edge_str edge;

		for (const auto& tile : tiles_) {
			if (from.x >= tile.second.first.x && from.x <= tile.second.second.x && from.y >= tile.second.first.y && from.y <= tile.second.second.y) {
				tile_idx = tile.first;
				break;
			}
		}

		if (tile_idx == 0) {
			std::cout << "Error: position not found in any tile." << std::endl;
			return false;
		}

		if (((from.x == tiles_[tile_idx].first.x) && (direction == E_FACING_DIRECTION_LEFT)) ||
			((from.x == tiles_[tile_idx].second.x) && (direction == E_FACING_DIRECTION_RIGHT)) ||
			((from.y == tiles_[tile_idx].first.y) && (direction == E_FACING_DIRECTION_UP)) ||
			((from.y == tiles_[tile_idx].second.y) && (direction == E_FACING_DIRECTION_DOWN))) {

			edge = edges_[tile_idx][get_edge_from_facing(direction)];

			new_pos = from - tiles_[tile_idx].first;

			if (edge.swap_axes) {
				std::swap(new_pos.x, new_pos.y);
			}

			new_facing = get_new_facing_from_edge(edge.edge);

			switch (edge.edge) {
				case E_EDGE_POSITION_LEFT:
					new_pos.x = 0;

					if (edge.invert_axe) {
						new_pos.y = tile_size_.y - 1 - new_pos.y;
					}
					break;
				case E_EDGE_POSITION_RIGHT:
					new_pos.x = tile_size_.x - 1;

					if (edge.invert_axe) {
						new_pos.y = tile_size_.y - 1 - new_pos.y;
					}
					break;
				case E_EDGE_POSITION_UP:
					new_pos.y = 0;

					if (edge.invert_axe) {
						new_pos.x = tile_size_.x - 1 - new_pos.x;
					}
					break;
				case E_EDGE_POSITION_DOWN:
					new_pos.y = tile_size_.y - 1;

					if (edge.invert_axe) {
						new_pos.x = tile_size_.x - 1 - new_pos.x;
					}
					break;
				default:
					std::cerr << "Invalid edge position" << std::endl;
					assert(false);
					return false;
					break;
			}

			new_pos = new_pos + tiles_[edge.side].first;

		} else {
			new_pos = do_one_step(from, direction);
		}
	} else {
		new_pos = do_one_step(from, direction);
		new_pos = validate_step(new_pos, direction);
	}

	if (walls_.count(new_pos) > 0) {
		return false;
	}

	return true;
}

bool AoC2022_day22::is_path_finished() {
	return (path_idx_ >= path_.size());
}

bool AoC2022_day22::turn_left() {
	if (!is_path_finished()) {
		char c = path_[path_idx_];
		path_idx_++;

		return c == 'L';
	}

	return false;
}

int32_t AoC2022_day22::get_steps() {
	std::string number = "";

	if (!is_path_finished()) {
		while (isdigit(path_[path_idx_])) {
			number += path_[path_idx_];
			path_idx_++;

			if (is_path_finished()) {
				break;
			}
		}
	}

	return std::stoi(number);
}

int64_t AoC2022_day22::get_final_password(const bool part2) {
	facing_direction_t facing = E_FACING_DIRECTION_RIGHT, new_facing;
	coord_str pos = {row_limits_[1].x, 1}, newpos;

	path_idx_ = 0;
	next_path_step_count_ = true;

	if (part2) {
		tiles_.clear();
		uint8_t side = 1;
		std::map<edge_position_t, cube_edge_str> tile_edges;

		for (const auto& s : sizes_) {
			uint8_t cnt = 1;
			while (cnt * tile_size_.x <= s.first.x) {
				tiles_[side] = {coord_str(s.second.x + ((cnt - 1) * tile_size_.x), s.second.y),
								coord_str(s.second.x + (cnt * tile_size_.x - 1), s.second.y + tile_size_.y - 1)};
				cnt++;
				side++;
			}
		}

		edges_.clear();

		if (tile_size_ == coord_str(4, 4)) {
			/* cube map for part 2 - testing map with tiles 4x4
				---
				|1|
				-------
				|2|3|4|
				---------
				|5|6|
				-----
				side	UL -> DR
				1	(9, 1) -> (12, 4)
				2	(1, 5) -> (4, 8)
				3	(5, 5) -> (8, 8)
				4	(9, 5) -> (12, 8)
				5	(9, 9) -> (12, 12)
				6	(13, 9) -> (16, 12)
			*/
			// 1
			tile_edges.clear();
			tile_edges[E_EDGE_POSITION_UP] = {2, E_EDGE_POSITION_UP, false, true};
			tile_edges[E_EDGE_POSITION_DOWN] = {4, E_EDGE_POSITION_UP, false, false};
			tile_edges[E_EDGE_POSITION_LEFT] = {3, E_EDGE_POSITION_UP, true, false};
			tile_edges[E_EDGE_POSITION_RIGHT] = {6, E_EDGE_POSITION_RIGHT, false, true};
			edges_[1] = tile_edges;
			// 2
			tile_edges.clear();
			tile_edges[E_EDGE_POSITION_UP] = {1, E_EDGE_POSITION_UP, false, true};
			tile_edges[E_EDGE_POSITION_DOWN] = {5, E_EDGE_POSITION_DOWN, false, true};
			tile_edges[E_EDGE_POSITION_LEFT] = {6, E_EDGE_POSITION_DOWN, true, true};
			tile_edges[E_EDGE_POSITION_RIGHT] = {3, E_EDGE_POSITION_LEFT, false, false};
			edges_[2] = tile_edges;
			// 3
			tile_edges.clear();
			tile_edges[E_EDGE_POSITION_UP] = {1, E_EDGE_POSITION_LEFT, true, false};
			tile_edges[E_EDGE_POSITION_DOWN] = {5, E_EDGE_POSITION_LEFT, true, true};
			tile_edges[E_EDGE_POSITION_LEFT] = {2, E_EDGE_POSITION_RIGHT, false, false};
			tile_edges[E_EDGE_POSITION_RIGHT] = {4, E_EDGE_POSITION_LEFT, false, false};
			edges_[3] = tile_edges;
			// 4
			tile_edges.clear();
			tile_edges[E_EDGE_POSITION_UP] = {1, E_EDGE_POSITION_DOWN, false, false};
			tile_edges[E_EDGE_POSITION_DOWN] = {5, E_EDGE_POSITION_UP, false, false};
			tile_edges[E_EDGE_POSITION_LEFT] = {3, E_EDGE_POSITION_RIGHT, false, false};
			tile_edges[E_EDGE_POSITION_RIGHT] = {6, E_EDGE_POSITION_UP, true, true};
			edges_[4] = tile_edges;
			// 5
			tile_edges.clear();
			tile_edges[E_EDGE_POSITION_UP] = {4, E_EDGE_POSITION_DOWN, false, false};
			tile_edges[E_EDGE_POSITION_DOWN] = {2, E_EDGE_POSITION_DOWN, false, true};
			tile_edges[E_EDGE_POSITION_LEFT] = {3, E_EDGE_POSITION_DOWN, true, true};
			tile_edges[E_EDGE_POSITION_RIGHT] = {6, E_EDGE_POSITION_LEFT, false, false};
			edges_[5] = tile_edges;
			// 6
			tile_edges.clear();
			tile_edges[E_EDGE_POSITION_UP] = {4, E_EDGE_POSITION_RIGHT, true, true};
			tile_edges[E_EDGE_POSITION_DOWN] = {2, E_EDGE_POSITION_LEFT, true, true};
			tile_edges[E_EDGE_POSITION_LEFT] = {5, E_EDGE_POSITION_RIGHT, false, false};
			tile_edges[E_EDGE_POSITION_RIGHT] = {1, E_EDGE_POSITION_LEFT, false, true};
			edges_[6] = tile_edges;
		} else if (tile_size_ == coord_str(50, 50)) {
			/*
				map from input with tiles 50x50
				  -----
				  |1|2|
				  -----
				  |3|
				-----
				|4|5|
				-----
				|6|
				---

				side	UL -> DR
				1	(51, 1) -> (100, 50)
				2	(101, 1) -> (150, 50)
				3	(51, 51) -> (100, 100)
				4	(1, 101) -> (50, 150)
				5	(51, 101) -> (100, 150)
				6	(1, 151) -> (50, 200)
			*/
			// 1
			tile_edges.clear();
			tile_edges[E_EDGE_POSITION_UP] = {6, E_EDGE_POSITION_LEFT, true, false};
			tile_edges[E_EDGE_POSITION_DOWN] = {3, E_EDGE_POSITION_UP, false, false};
			tile_edges[E_EDGE_POSITION_LEFT] = {4, E_EDGE_POSITION_LEFT, false, true};
			tile_edges[E_EDGE_POSITION_RIGHT] = {2, E_EDGE_POSITION_LEFT, false, false};
			edges_[1] = tile_edges;

			// 2
			tile_edges.clear();
			tile_edges[E_EDGE_POSITION_UP] = {6, E_EDGE_POSITION_DOWN, false, false};
			tile_edges[E_EDGE_POSITION_DOWN] = {3, E_EDGE_POSITION_RIGHT, true, false};
			tile_edges[E_EDGE_POSITION_LEFT] = {1, E_EDGE_POSITION_RIGHT, false, false};
			tile_edges[E_EDGE_POSITION_RIGHT] = {5, E_EDGE_POSITION_RIGHT, false, true};
			edges_[2] = tile_edges;

			// 3
			tile_edges.clear();
			tile_edges[E_EDGE_POSITION_UP] = {1, E_EDGE_POSITION_DOWN, false, false};
			tile_edges[E_EDGE_POSITION_DOWN] = {5, E_EDGE_POSITION_UP, false, false};
			tile_edges[E_EDGE_POSITION_LEFT] = {4, E_EDGE_POSITION_UP, true, false};
			tile_edges[E_EDGE_POSITION_RIGHT] = {2, E_EDGE_POSITION_DOWN, true, false};
			edges_[3] = tile_edges;

			// 4
			tile_edges.clear();
			tile_edges[E_EDGE_POSITION_UP] = {3, E_EDGE_POSITION_LEFT, true, false};
			tile_edges[E_EDGE_POSITION_DOWN] = {6, E_EDGE_POSITION_UP, false, false};
			tile_edges[E_EDGE_POSITION_LEFT] = {1, E_EDGE_POSITION_LEFT, false, true};
			tile_edges[E_EDGE_POSITION_RIGHT] = {5, E_EDGE_POSITION_LEFT, false, false};
			edges_[4] = tile_edges;

			// 5
			tile_edges.clear();
			tile_edges[E_EDGE_POSITION_UP] = {3, E_EDGE_POSITION_DOWN, false, false};
			tile_edges[E_EDGE_POSITION_DOWN] = {6, E_EDGE_POSITION_RIGHT, true, false};
			tile_edges[E_EDGE_POSITION_LEFT] = {4, E_EDGE_POSITION_RIGHT, false, false};
			tile_edges[E_EDGE_POSITION_RIGHT] = {2, E_EDGE_POSITION_RIGHT, false, true};
			edges_[5] = tile_edges;

			// 6
			tile_edges.clear();
			tile_edges[E_EDGE_POSITION_UP] = {4, E_EDGE_POSITION_DOWN, false, false};
			tile_edges[E_EDGE_POSITION_DOWN] = {2, E_EDGE_POSITION_UP, false, false};
			tile_edges[E_EDGE_POSITION_LEFT] = {1, E_EDGE_POSITION_UP, true, false};
			tile_edges[E_EDGE_POSITION_RIGHT] = {5, E_EDGE_POSITION_DOWN, true, false};
			edges_[6] = tile_edges;
		} else {
			// Handle the case when tile_size_ is not 4x4 or 50x50
			assert(false && "Unsupported tile size for part 2");
			return 0;
		}
	}

	while (!is_path_finished()) {
		if (next_path_step_count_) {
			int32_t steps = get_steps();

			for (int32_t i = 0; i < steps; i++) {
				if (can_move(pos, facing, part2, newpos, new_facing)) {
					pos = newpos;
					facing = new_facing;
				} else {
					break;
				}
			}
		} else {
			bool turn2left = turn_left();

			facing = turn(facing, turn2left);
		}
		next_path_step_count_ = !next_path_step_count_;
	}

	return (pos.y * 1000) + (pos.x * 4) + facing;
}

facing_direction_t AoC2022_day22::turn(const facing_direction_t current_facing, const bool turn_left) {
	switch (current_facing) {
		case E_FACING_DIRECTION_DOWN:
			return turn_left ? E_FACING_DIRECTION_RIGHT : E_FACING_DIRECTION_LEFT;
		case E_FACING_DIRECTION_LEFT:
			return turn_left ? E_FACING_DIRECTION_DOWN : E_FACING_DIRECTION_UP;
		case E_FACING_DIRECTION_UP:
			return turn_left ? E_FACING_DIRECTION_LEFT : E_FACING_DIRECTION_RIGHT;
		case E_FACING_DIRECTION_RIGHT:
			return turn_left ? E_FACING_DIRECTION_UP : E_FACING_DIRECTION_DOWN;
		default:
			return current_facing;
	}
}

facing_direction_t AoC2022_day22::get_opposite_facing(const facing_direction_t current_facing) {
	switch (current_facing) {
		case E_FACING_DIRECTION_DOWN:
			return E_FACING_DIRECTION_UP;
		case E_FACING_DIRECTION_LEFT:
			return E_FACING_DIRECTION_RIGHT;
		case E_FACING_DIRECTION_UP:
			return E_FACING_DIRECTION_DOWN;
		case E_FACING_DIRECTION_RIGHT:
			return E_FACING_DIRECTION_LEFT;
		default:
			assert(false && "Invalid facing direction");
			return current_facing;
	}
}

bool AoC2022_day22::part1() {
	int64_t result;

	result = get_final_password();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2022_day22::part2() {
	int64_t result;

	result = get_final_password(true);

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2022_day22 day22;

	return day22.main_execution();
}
