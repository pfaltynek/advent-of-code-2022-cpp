#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <map>
#include <regex>

const size_t C_MAX_DIR_SIZE = 100000;
const size_t C_TOTAL_DISK_SIZE = 70000000;
const size_t C_NEEDED_FREE_DISK_SIZE = 30000000;
const std::string C_PATH_DELIMITER = "/";
const std::regex C_REGEX_CD("^\\$ cd (\\/|[a-zA-Z]+|..)$");
const std::regex C_REGEX_LS("^\\$ ls");
const std::regex C_REGEX_DIR("^dir (\\w+)$");
const std::regex C_REGEX_FILE("^(\\d+) (\\w+\\.?\\w{0,3})$");

struct fs_item_str {
	bool is_dir;
	std::string name;
	size_t size;
};

class AoC2022_day07 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	uint64_t get_small_dirs_sizes_sum();
	uint64_t get_smallest_dir_size_to_free_for_update();
	uint64_t get_one_dir_size(const std::string dir, uint64_t& total_sum);
	std::map<std::string, std::vector<fs_item_str>> fs_;
	std::map<std::string, uint64_t> dir_sizes_;
	std::string start_dir_;
	uint64_t total_used_space_;
	std::string get_full_path(const std::vector<std::string> path);
	std::string get_full_path_extended(const std::string path, const std::string subdir);
};

bool AoC2022_day07::init(const std::vector<std::string> lines) {
	std::vector<std::string> path = {};
	std::string curr_path = "", tmp;
	std::smatch sm;

	fs_.clear();
	start_dir_ = "";

	for (size_t i = 0; i < lines.size(); i++) {
		if (lines[i][0] == '$') {
			if (std::regex_match(lines[i], sm, C_REGEX_CD)) {
				tmp = sm.str(1);
				if (start_dir_.empty()) {
					start_dir_ = tmp;
				}

				if (tmp == "..") {
					path.pop_back();
					curr_path = get_full_path(path);
				} else {
					path.push_back(tmp);
					curr_path = get_full_path(path);

					if (fs_.count(curr_path) > 0) {
						int x = 8;
						return 0;
					}
				}
			} else if (!std::regex_match(lines[i], sm, C_REGEX_LS)) {
				std::cout << "Invalid command at line " << i + 1 << std::endl;
				return false;
			}
		} else {
			fs_item_str fsi;

			if (std::regex_match(lines[i], sm, C_REGEX_DIR)) {
				fsi.is_dir = true;
				fsi.name = sm.str(1);
				fsi.size = 0;
				fs_[curr_path].push_back(fsi);
			} else if (std::regex_match(lines[i], sm, C_REGEX_FILE)) {
				fsi.is_dir = false;
				fsi.name = sm.str(2);
				fsi.size = static_cast<size_t>(std::stoi(sm.str(1)));
				fs_[curr_path].push_back(fsi);
			} else {
				std::cout << "Invalid directory content at line " << i + 1 << std::endl;
				return false;
			}
		}
	}

	if (start_dir_.empty()) {
		std::cout << "File system seems to be empty" << std::endl;
		return false;
	}

	return true;
}

std::string AoC2022_day07::get_full_path(const std::vector<std::string> path) {
	std::string result = {};

	for (size_t i = 0; i < path.size(); i++) {
		result.append(path[i]);

		if (result.back() != C_PATH_DELIMITER[0]) {
			result.append(C_PATH_DELIMITER);
		}
	}

	return result;
}

std::string AoC2022_day07::get_full_path_extended(const std::string path, const std::string subdir) {
	std::string result = path;

	result.append(subdir);

	if (result.back() != C_PATH_DELIMITER[0]) {
		result.append(C_PATH_DELIMITER);
	}

	return result;
}

uint64_t AoC2022_day07::get_small_dirs_sizes_sum() {
	uint64_t result = 0;

	total_used_space_ = 0;
	dir_sizes_ = {};

	total_used_space_ = get_one_dir_size(start_dir_, result);

	dir_sizes_[start_dir_] = total_used_space_;

	if (total_used_space_ <= C_MAX_DIR_SIZE) {
		result += total_used_space_;
	}

	return result;
}

uint64_t AoC2022_day07::get_one_dir_size(const std::string dir, uint64_t& total_sum) {
	uint64_t result = 0;
	std::string path;

	for (size_t i = 0; i < fs_[dir].size(); i++) {
		if (fs_[dir][i].is_dir) {
			path = get_full_path_extended(dir, fs_[dir][i].name);
			uint64_t sub_dir = get_one_dir_size(path, total_sum);
			dir_sizes_[path] = sub_dir;

			if (sub_dir <= C_MAX_DIR_SIZE) {
				total_sum += sub_dir;
			}

			result += sub_dir;
		} else {
			result += fs_[dir][i].size;
		}
	}

	return result;
}

uint64_t AoC2022_day07::get_smallest_dir_size_to_free_for_update() {
	uint64_t result = UINT64_MAX;
	const uint64_t min_needed_space_to_free = total_used_space_ - (C_TOTAL_DISK_SIZE - C_NEEDED_FREE_DISK_SIZE);

	for (auto& pair : dir_sizes_) {
		if ((pair.second > min_needed_space_to_free) && (pair.second < result)) {
			result = pair.second;
		}
	}

	return result;
}

int32_t AoC2022_day07::get_aoc_day() {
	return 7;
}

int32_t AoC2022_day07::get_aoc_year() {
	return 2022;
}

void AoC2022_day07::tests() {
	int64_t result;

	if (init({"$ cd /",	 "$ ls",	"dir a",  "14848514 b.txt", "8504156 c.dat", "dir d",		  "$ cd a",	  "$ ls",
			  "dir e",	 "29116 f", "2557 g", "62596 h.lst",	"$ cd e",		 "$ ls",		  "584 i",	  "$ cd ..",
			  "$ cd ..", "$ cd d",	"$ ls",	  "4060174 j",		"8033020 d.log", "5626152 d.ext", "7214296 k"})) {
		result = get_small_dirs_sizes_sum();				 // 95437
		result = get_smallest_dir_size_to_free_for_update(); // 24933642
	}
}

bool AoC2022_day07::part1() {
	int64_t result;

	result = get_small_dirs_sizes_sum();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2022_day07::part2() {
	int64_t result;

	result = get_smallest_dir_size_to_free_for_update();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2022_day07 day07;

	return day07.main_execution();
}
