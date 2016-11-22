/*
 * Parser.h
 *
 *  Created on: May 29, 2012
 *      Author: fritz
 */

#ifndef PARSER_H_
#define PARSER_H_
#include <vector>
#include <string>


struct position_str {
	std::string chr;
	int position;
};

struct entry_str {
	bool is_secondary;
	int num_reads;
	position_str start;
	position_str stop;
	short type;
	std::string line;
};
struct SV_str {
	int total_lenght;
	std::vector<position_str > start;
	std::vector<position_str > stop;
	std::vector<short> type;
	std::vector<int> supporting;
};

struct Anno_str {
	std::string exp;
	position_str start;
	position_str stop;
};

class SVS_Node {
public:
	//just for testing!

	SVS_Node() {
		type = -1;
		caller.clear();
		annotations.clear();
	}
	~SVS_Node() {
		caller.clear();
		annotations.clear();
	}
	//TODO change that to getter and setter!
	short type;
	position_str start;
	position_str stop;
	std::vector<SV_str> caller;
	std::vector<Anno_str> annotations;
	std::string line;
	bool is_secondary;
};
/*
class Parser {
public:
	virtual ~Parser() {
	}
	;
	virtual Alignment * parseRead(uint16_t mappingQv) = 0;
	virtual void parseReadFast(uint16_t mappingQv, Alignment *& align) = 0;
};*/

#endif /* PARSER_H_ */
