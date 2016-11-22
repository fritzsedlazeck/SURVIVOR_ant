/*
 * VCFPrinter.h
 *
 *  Created on: Aug 24, 2015
 *      Author: fsedlaze
 */

#ifndef PRINT_VCFPRINTER_H_
#define PRINT_VCFPRINTER_H_
#include "../Parser.h"
#include "../Paramer.h"
#include <string>
#include <iostream>
#include <sstream>
#include <stdio.h>
class VCFPrinter{
private:
	FILE *file;

	std::string trans_type(short type);
	int get_support(std::vector<SV_str> caller_info);
	double get_avglen(std::vector<SV_str>caller_info);
	const std::string currentDateTime();
	std::string print_info(SVS_Node* entry, std::pair<int, int> & summary);

public:
	VCFPrinter(){
		file=NULL;
	}

	~VCFPrinter(){
	}
	void print_header(std::string header);
	void init();
	void print_entry(SVS_Node* entry,int id,std::pair<int,int> & summary);
	void print_orig_entry(SVS_Node* entry, int id, std::pair<int, int> & summary);
	void close_file(){
		fclose(file);
	}
};



#endif /* PRINT_VCFPRINTER_H_ */
