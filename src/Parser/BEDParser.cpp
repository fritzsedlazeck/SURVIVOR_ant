/*
 * BEDParser.cpp
 *
 *  Created on: Aug 15, 2016
 *      Author: fsedlaze
 */

#include "BEDParser.h"

std::vector<Anno_str> BEDParser::parse_bed(std::string filename) {
	std::vector<Anno_str> entries;
	size_t buffer_size = 2000000;
	char*buffer = new char[buffer_size];
	std::ifstream myfile;
	myfile.open(filename.c_str(), std::ifstream::in);
	if (!myfile.good()) {
		std::cout << "BED Parser: could not open file: " << filename.c_str() << std::endl;
		exit(0);
	}
	myfile.getline(buffer, buffer_size);
	while (!myfile.eof()) {
			int count=0;
		Anno_str tmp;
		for(size_t i=0;i<buffer_size && buffer[i]!='\n' && buffer[i]!='\0';i++){
			if(count==0 && buffer[i]!='\t'){
				tmp.start.chr+=buffer[i];
				tmp.stop.chr+=buffer[i];
			}
			if(count==1 && buffer[i-1]=='\t'){
				tmp.start.position=atoi(&buffer[i]);
			}
			if(count==2 && buffer[i-1]=='\t'){
				tmp.stop.position=atoi(&buffer[i]);
			}
			if(count==3 && buffer[i]!='\t'){
				tmp.exp+=buffer[i];
			}
			if(buffer[i]=='\t'){
				count++;
			}
		}
		entries.push_back(tmp);
		myfile.getline(buffer, buffer_size);
	}
	myfile.close();
	return entries;
}

