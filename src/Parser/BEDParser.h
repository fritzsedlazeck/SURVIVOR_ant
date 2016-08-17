/*
 * BEDParser.h
 *
 *  Created on: Aug 15, 2016
 *      Author: fsedlaze
 */

#ifndef PARSER_BEDPARSER_H_
#define PARSER_BEDPARSER_H_
#include "../Parser.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <stdio.h>
#include <string.h>

class BEDParser{


public:
	BEDParser(){
	}
	~BEDParser(){
	}
	std::vector<Anno_str> parse_bed(std::string filename);
};

#endif /* PARSER_BEDPARSER_H_ */
