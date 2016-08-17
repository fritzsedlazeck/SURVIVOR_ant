/*
 * VCFParser.h
 *
 *  Created on: Aug 15, 2016
 *      Author: fsedlaze
 */

#ifndef PARSER_VCFPARSER_H_
#define PARSER_VCFPARSER_H_

#include "../Parser.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <stdio.h>
#include <string.h>
class VCFParser{
	position_str parse_stop(const char * buffer);

public:
	VCFParser(){
	}
	~VCFParser(){
	}
	std::vector<entry_str> parse_entries(std::string filename);
	std::string assign_lines(std::vector<SVS_Node *> & points, std::string filename);
	std::string get_header(std::string filename);
};



#endif /* PARSER_VCFPARSER_H_ */
