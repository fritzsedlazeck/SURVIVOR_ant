/*
 * GFFParser.cpp
 *
 *  Created on: Aug 15, 2016
 *      Author: fsedlaze
 */

#include "GFFParser.h"

std::string parse_name(char * buffer){
	size_t i=0;
	std::string name;
	bool flag=false;
	while(buffer[i]!=';'){
		if(flag){
			if(buffer[i]!='"'){
				name+=buffer[i];
			}
		}
		if(buffer[i]=='=' || buffer[i]==' ' ){
			flag=!flag;
		}
		i++;
	}
	//std::cout<<"Name: "<<name<<std::endl;
	return name;
}
std::vector<Anno_str> GFFParser::parse_gff(std::string filename) {
	std::vector<Anno_str> entries;
	size_t buffer_size = 2000000;
	char*buffer = new char[buffer_size];
	std::ifstream myfile;

	myfile.open(filename.c_str(), std::ifstream::in);
	if (!myfile.good()) {
		std::cout << "VCF Parser: could not open file: " << filename.c_str() << std::endl;
		exit(0);
	}

	myfile.getline(buffer, buffer_size);

	while (!myfile.eof()) {
		int count = 0;
		Anno_str tmp;
		// gene_id "ENSG00000223972";
		for (size_t i = 0; i < buffer_size && buffer[i] != '\n' && buffer[i] != '\0'; i++) {
			if (count == 0 && buffer[i] != '\t') {
				tmp.start.chr += buffer[i];
				tmp.stop.chr += buffer[i];
			}
			if (count == 3 && buffer[i - 1] == '\t') {
				tmp.exp='_';
				tmp.start.position = atoi(&buffer[i]);
			}
			if (count == 4 && buffer[i - 1] == '\t') {
				tmp.stop.position = atoi(&buffer[i]);
			}
			if(count==8 && buffer[i-1]=='\t'){
				tmp.exp=parse_name(&buffer[i]);
			}

			if (buffer[i] == '\t') {
				count++;
			}
		}
		entries.push_back(tmp);
		myfile.getline(buffer, buffer_size);
	}

	myfile.close();
	return entries;
}

