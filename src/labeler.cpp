#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <stdint.h>
#include "options.h"

typedef std::map<std::string, uint64_t> dict_t;

uint64_t dict_load(const char *dict_file, dict_t &dict) {
	uint64_t max = 0;
	if (dict_file) {
		std::ifstream dict_fs(dict_file);
		if(dict_fs) {
			std::string line;
			while (getline(dict_fs, line)) {
				std::stringstream ss(line);
				std::string token;
				uint64_t id;
				ss >> token;
				ss >> id;
				dict[token] = id;
				if (id > max) max = id;
			}
			dict_fs.close();
			max++;
		}
	}
	return max;
}

void dict_store(const char *dict_file, dict_t &dict) {
	if (dict_file) {
		std::ofstream dict_fs(dict_file);
		if(dict_fs) {
			for (auto elem: dict) {
				dict_fs << elem.first << " " << elem.second << std::endl;
			}
			dict_fs.close();
		}
	}
}

void permutate(dict_t &dict, uint64_t &id, char seperator, std::istream &in, std::ostream &out) {
	std::string line;
	while (getline(in, line)) {
		std::stringstream ss(line);
		std::string token;
		while (getline(ss, token, seperator)) {
			auto itr = dict.find(token);
			if (itr == dict.end()) dict[token] = id++;
			out << dict[token] << " ";
		}
		out << std::endl;
	}
}

void permutate(dict_t &dict, uint64_t &id, char seperator, const char *input_file, const char *output_file) {
	if (input_file) {
		std::ifstream ifs(input_file);
		if (ifs) {
			if (output_file) {
				std::ofstream ofs(output_file);
				if (ofs) {
					permutate(dict, id, seperator, ifs, ofs);
					ofs.close();
				}
				else permutate(dict, id, seperator, ifs, std::cout);
			}
			else {
				permutate(dict, id, seperator, ifs, std::cout);
			}
			ifs.close();
		}
	}
	else {
		permutate(dict, id, seperator, std::cin, std::cout);
	}
}

int main(int argc, char* argv[]) {
	using namespace std;
	using namespace opt;

	if (chkOption(argv, argv + argc, "-h")) {
		cout << "labeler [options]" << endl
			<< " -h:\t ask for help" << endl
			<< " -d:\t dictionary" << endl
			<< " -s:\t seperator" << endl
			<< " -i:\t input file" << endl
			<< " -o:\t output file" << endl;
		return 0;
	}

	char* dict_file = getOption(argv, argv + argc, "-d");
	char* seperator = getOption(argv, argv + argc, "-s");
	char* input_file = getOption(argv, argv + argc, "-i");
	char* output_file = getOption(argv, argv + argc, "-o");

	dict_t dict;
	uint64_t id = dict_load(dict_file, dict);

	char s = seperator ? seperator[0] : ' ';
	permutate(dict, id, s, input_file, output_file);

	dict_store(dict_file, dict);

	return 0;
}
