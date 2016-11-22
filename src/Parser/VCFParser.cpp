/*
 * VCFParser.cpp
 *
 *  Created on: Aug 15, 2016
 *      Author: fsedlaze
 */

#include "VCFParser.h"

position_str VCFParser::parse_stop(const char * buffer) {
	size_t i = 0;
	bool chr_flag = false;
	position_str pos;
	pos.chr = "";
	pos.position = -1;
	while (buffer[i] != '\t' && (buffer[i] != '\n' && buffer[i] != '\0')) {
		if (strncmp(&buffer[i], ";END=", 5) == 0) {
			pos.position = atoi(&buffer[i + 5]);
		}
		if ((strncmp(&buffer[i], "END=", 4) == 0 && i == 0)) {
			pos.position = atoi(&buffer[i + 4]);
		}
		if (strncmp(&buffer[i], "CHR2=", 5) == 0) {
			i = i + 5;
			chr_flag = true;
		}
		if (buffer[i] == ';') {
			chr_flag = false;
		}
		if (chr_flag) {
			pos.chr += buffer[i];
		}

		i++;
	}
	return pos;
}

position_str parse_pos(char * buffer) {
	size_t i=0;
	int count=0;
	position_str pos;
	while(buffer[i]!='\t'){
		if(count==1 && ((buffer[i]!='[' ||buffer[i]!=']' ) && buffer[i]!=':' )){
			pos.chr+=buffer[i];
		}
		if(count==2&& buffer[i-1]==':'){
			pos.position=atoi(&buffer[i]);
		}
		if((buffer[i]==']'||buffer[i]=='[') ||buffer[i]==':'  ){
			count++;
		}
		i++;
	}

	/*std::string tmp = std::string(buffer);
	size_t found = tmp.find(':');

	pos.chr = tmp.substr(0, found);
	found++;
	pos.position = atoi(&tmp[found]);*/
	//std::cout << pos.chr << "| " << pos.pos << "|" << std::endl;
	return pos;
}

int parse_support(char * buffer) {
	size_t i = 0;
	int support = 0;
	while (buffer[i] != '\t' && buffer[i] != '\0') {

		if (strncmp(&buffer[i], "VT_AC=", 6) == 0) {
			support = atoi(&buffer[i + 6]);
		}
		if ((strncmp(&buffer[i], ";SU=", 4) == 0 || strncmp(&buffer[i], ";RE=", 4) == 0) || (strncmp(&buffer[i], ";PE=", 4) == 0 || strncmp(&buffer[i], ";SR=", 4) == 0)) { // SU: Lumpy, RE: Sniffles
			support += atoi(&buffer[i + 4]);
		}
//TOOD extned for the tags that other caller use!
		i++;
	}
	return support;
}

short get_type(std::string type) {
	if (strncmp(type.c_str(), "DEL", 3) == 0) {
		return 0;
	} else if (strncmp(type.c_str(), "DUP", 3) == 0) {
		return 1;
	} else if (strncmp(type.c_str(), "INV", 3) == 0) {
		return 2;
	} else if (strncmp(type.c_str(), "TRA", 3) == 0 || strncmp(type.c_str(), "BND", 3) == 0) {
		return 3;
	} else if (strncmp(type.c_str(), "INS", 3) == 0) {
		return 4;
	} else {
		std::cerr << "Unknown type!" << std::endl; // << type << std::endl;
	}
	return -1;
}

std::string get_most_effect(std::string alt, int ref) {
	size_t i = 0;
	std::string most_alt = "";

	std::string tmp = "";
	while (i < alt.size()) {
		if (alt[i] == ',') {
			int size = most_alt.size();
			int curr = tmp.size();
			if (abs(ref - curr) > abs(ref - size)) {
				most_alt = tmp;
			}
			tmp.clear();
		} else {
			tmp += alt[i];
		}
		i++;
	}
	return most_alt;
}
std::string VCFParser::get_header(std::string filename){
	size_t buffer_size = 2000000;
		char*buffer = new char[buffer_size];
		std::ifstream myfile;

		myfile.open(filename.c_str(), std::ifstream::in);
		if (!myfile.good()) {
			std::cout << "VCF Parser: could not open file: " << filename.c_str() << std::endl;
			exit(0);
		}
		myfile.getline(buffer, buffer_size);
		std::string header;
		while (!myfile.eof()) {
			if (buffer[0] == '#') {
				header+=std::string(buffer);
				header+='\n';
			}
			myfile.getline(buffer, buffer_size);
		}
		myfile.close();
		return header;
}
std::vector<entry_str> VCFParser::parse_entries(std::string filename) {
	std::vector<entry_str> entries;
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
		if (buffer[0] != '#') {
			int count = 0;
			entry_str tmp;
			tmp.is_secondary=false;
			tmp.stop.position = -1;
			tmp.type = -1;
			tmp.line=std::string(buffer);
			std::string ref;
			std::string alt;
			tmp.num_reads = -1;
			//std::cout<<buffer<<std::endl;
			for (size_t i = 0; i < buffer_size && buffer[i] != '\0' && buffer[i] != '\n'; i++) {

				if (count == 0 && buffer[i] != '\t') {
					tmp.start.chr += buffer[i];
				}
				if (count == 1 && buffer[i - 1] == '\t') {
					tmp.start.position = atoi(&buffer[i]);
					//std::cout<<tmp.start.pos<<std::endl;
				}
				if (count == 3 && buffer[i] != '\t') {
					ref += buffer[i];
				}
				if (count == 4 && buffer[i] != '\t') {
					//C,CATATATATATAT
					alt += buffer[i];
				}
				if (tmp.stop.position == -1 && (count == 7 && buffer[i - 1] == '\t')) {
					tmp.stop = parse_stop(&buffer[i]);
					//std::cout<<tmp.stop.chr<<std::endl;
				}
				if (count == 7 && strncmp(&buffer[i], "SVTYPE=", 7) == 0) {
					tmp.type = get_type(std::string(&buffer[i + 7]));
				}
				if (count == 4 && buffer[i - 1] == '<') {
					tmp.type = get_type(std::string(&buffer[i]));
				}
				if (tmp.stop.position == -1 && (count == 4 && (buffer[i - 1] == '[' || buffer[i - 1] == ']'))) {
					tmp.stop = parse_pos(&buffer[i]);
				}

				if (buffer[i] == '\t') {
					count++;
				}
			}

			if (tmp.stop.position == -1) {

				std::size_t found = alt.find(",");
				if (found != std::string::npos) {
					alt = get_most_effect(alt, (int) ref.size());
				}
				tmp.stop.chr = tmp.start.chr;
				int len = (int) ref.size() - (int) alt.size();
				tmp.stop.position = tmp.start.position + abs(len);
				if (len > 0) {
					tmp.type = 0;
				} else if (len < 0) {
					tmp.type = 1;
				}
			}
			if (tmp.stop.chr.empty()) {
				tmp.stop.chr = tmp.start.chr;
			}
			if (tmp.type == 3) { //a bit of a hack because of tra and their annotation!
				entry_str tmp2;
				tmp2 = tmp;
				tmp2.is_secondary=true;
				tmp2.start = tmp2.stop;
				tmp2.num_reads = entries.size() + 1;
				entries.push_back(tmp2);
			}
			entries.push_back(tmp);
		}
		myfile.getline(buffer, buffer_size);
	}
	myfile.close();
	return entries;
}

std::string VCFParser::assign_lines(std::vector<SVS_Node *> & points, std::string filename) {
	size_t buffer_size = 2000000;
	char*buffer = new char[buffer_size];
	std::ifstream myfile;

	myfile.open(filename.c_str(), std::ifstream::in);
	if (!myfile.good()) {
		std::cout << "VCF Parser: could not open file: " << filename.c_str() << std::endl;
		exit(0);
	}
	myfile.getline(buffer, buffer_size);
	std::string header;
	while (!myfile.eof()) {
		if (buffer[0] != '#') {
			int start = 0;
			std::string chr = "";
			int count=0;
			for (size_t i = 0; i < buffer_size && buffer[i] != '\0' && buffer[i] != '\n'; i++) {
				if (count == 0 && buffer[i] != '\t') {
					chr += buffer[i];
				}
				if (count == 1 && buffer[i - 1] == '\t') {
					start= atoi(&buffer[i]);
					break;
					//std::cout<<tmp.start.pos<<std::endl;
				}
				if(buffer[i]=='\t'){
					count++;
				}
			}
			std::cout<<"entry: "<<chr<<" "<<start<<std::endl;
			for(size_t i=0;i<points.size();i++){
				if(strcmp(chr.c_str(),points[i]->start.chr.c_str())==0 && points[i]->start.position == start){
					points[i]->line=buffer;
					break;
				}
			}
		} else {
			for (size_t i = 0; i < buffer_size && buffer[i] != '\0' && buffer[i] != '\n'; i++) {
				header += buffer[i];
			}
		}

		myfile.getline(buffer, buffer_size);
	}
	return header;
}
