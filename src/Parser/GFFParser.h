/*
 * GFFParser.h
 *
 *  Created on: Aug 15, 2016
 *      Author: fsedlaze
 */

#ifndef PARSER_GFFPARSER_H_
#define PARSER_GFFPARSER_H_
#include "../Parser.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <stdio.h>
#include <string.h>

class GFFParser{

public:
	GFFParser(){
	}
	~GFFParser(){
	}
	std::vector<Anno_str> parse_gff(std::string filename);
};



#endif /* PARSER_GFFPARSER_H_ */
