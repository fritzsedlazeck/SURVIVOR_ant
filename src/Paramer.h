/*
 * Paramer.h
 *
 *  Created on: Aug 20, 2015
 *      Author: fsedlaze
 */

#ifndef PARAMER_H_
#define PARAMER_H_

#include <string.h>
#include <string>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include <ctime>

class Parameter {
private:
	Parameter() {

	}
	~Parameter() {

	}
	static Parameter* m_pInstance;
public:
	std::string sv_vcf;
	std::vector<std::string> vcf_files;
	std::vector<std::string> bed_files;
	std::vector<std::string> gff_files;
	std::string output;
	int svs_dist;
	int anno_dist;
	int min_length;
	bool use_type;
	bool debug;
	int max_caller;
	void parse_intput(std::vector<std::string> & container, std::string command){
		if(command.empty()){
			container.clear();
			return;
		}
		std::string name;
		for(size_t i=0;i<command.size();i++){
			if(command[i]==','){
				container.push_back(name);
				name.clear();
			}else{
				name+=command[i];
			}
		}
		container.push_back(name);
		name.clear();
	}

	static Parameter* Instance() {
		if (!m_pInstance) {
			m_pInstance = new Parameter;
		}
		return m_pInstance;
	}

	double meassure_time(clock_t begin ,std::string msg){
		return 0;
		clock_t end = clock();
		double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
		std::cout << msg<<" " << elapsed_secs<<std::endl;
		return elapsed_secs;
	}
};

#endif /* PARAMER_H_ */
