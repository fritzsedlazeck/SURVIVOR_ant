/*
 * VCFPrinter.cpp
 *
 *  Created on: Aug 24, 2015
 *      Author: fsedlaze
 */

#include "VCFPrinter.h"

std::string VCFPrinter::trans_type(short type) {
	//0=DEL,1=DUP,2=INV,3=TRA, 4=INS
	switch (type) {
	case 0:
		return "DEL";
		break;
	case 1:
		return "DUP";
		break;
	case 2:
		return "INV";
		break;
	case 3:
		return "TRA";
		break;
	case 4:
		return "INS";
		break;
	default:
		return "NA";
		break;
	}
}
const std::string VCFPrinter::currentDateTime() {
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
	tstruct = *localtime(&now);
	// Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
	// for more information about date/time format
	strftime(buf, sizeof(buf), "%Y%m%d", &tstruct);
	return buf;
}

void VCFPrinter::print_header(std::string header) {
	fprintf(file, "%s", header.c_str());
/*
	fprintf(file, "%s", "##fileformat=VCFv4.1\n");
	std::string time = currentDateTime();
	fprintf(file, "%s", "##fileDate=");
	fprintf(file, "%s", time.c_str());
	fprintf(file, "%s", "\n"); //TODO change!
	fprintf(file, "%s", "##ALT=<ID=DEL,Description=\"Deletion\">\n");
	fprintf(file, "%s", "##ALT=<ID=DUP,Description=\"Duplication\">\n");
	fprintf(file, "%s", "##ALT=<ID=INV,Description=\"Inversion\">\n");
	fprintf(file, "%s", "##ALT=<ID=TRA,Description=\"Translocation\">\n");
	fprintf(file, "%s", "##ALT=<ID=INS,Description=\"Insertion\">\n");
	//rintf(file, "%s", "##FILTER=<ID=LowQual,Description=\"PE support below 3 or mapping quality below 20.\">\n");
	//printf(file, "%s", "##INFO=<ID=CIEND,Number=2,Type=Integer,Description=\"PE confidence interval around END\">\n");
	//fprintf(file, "%s", "##INFO=<ID=CIPOS,Number=2,Type=Integer,Description=\"PE confidence interval around POS\">\n");
	fprintf(file, "%s", "##INFO=<ID=CHR2,Number=1,Type=String,Description=\"Chromosome for END coordinate in case of a translocation\">\n");
	fprintf(file, "%s", "##INFO=<ID=END,Number=1,Type=Integer,Description=\"End position of the structural variant\">\n");
//	fprintf(file, "%s", "##INFO=<ID=PE,Number=1,Type=Integer,Description=\"Paired-end support of the structural variant\">\n");
	fprintf(file, "%s", "##INFO=<ID=MAPQ,Number=1,Type=Integer,Description=\"Median mapping quality of paired-ends\">\n");
	fprintf(file, "%s", "##INFO=<ID=RE,Number=1,Type=Integer,Description=\"read support\">\n");
//	fprintf(file, "%s", "##INFO=<ID=SRQ,Number=1,Type=Float,Description=\"Split-read consensus alignment quality\">\n");
//	fprintf(file, "%s", "##INFO=<ID=CONSENSUS,Number=1,Type=String,Description=\"Split-read consensus sequence\">\n");
//	fprintf(file, "%s", "##INFO=<ID=CT,Number=1,Type=String,Description=\"Paired-end signature induced connection type\">\n");
	fprintf(file, "%s", "##INFO=<ID=IMPRECISE,Number=0,Type=Flag,Description=\"Imprecise structural variation\">\n");
	fprintf(file, "%s", "##INFO=<ID=PRECISE,Number=0,Type=Flag,Description=\"Precise structural variation\">\n");
	fprintf(file, "%s", "##INFO=<ID=SVLEN,Number=1,Type=Integer,Description=\"Length of the SV\">\n");
//	fprintf(file, "%s", "##INFO=<ID=SUBTYPE,Number=1,Type=String,Description=\"What supports the SV.\">\n");
//	fprintf(file, "%s", "##INFO=<SUBTYPE=SP,Description=\"SV supported by split reads\">\n");
//	fprintf(file, "%s", "##INFO=<SUBTYPE=MD,Description=\"SV supported by MD string\">\n");
//	fprintf(file, "%s", "##INFO=<SUBTYPE=CI,Description=\"SV supported by Cigar string\">\n");
	//fprintf(file, "%s", "##INFO=<ID=SVTYPE,Number=1,Type=String,Description=\"Type of structural variant\">\n");
	fprintf(file, "%s", "##INFO=<ID=SVMETHOD,Number=1,Type=String,Description=\"Type of approach used to detect SV\">\n");
	fprintf(file, "%s", "##FORMAT=<ID=GT,Number=1,Type=String,Description=\"Genotype\">\n");
	//fprintf(file, "%s", "##FORMAT=<ID=GL,Number=G,Type=Float,Description=\"Log10-scaled genotype likelihoods for RR,RA,AA genotypes\">\n");
	//fprintf(file, "%s", "##FORMAT=<ID=GQ,Number=1,Type=Integer,Description=\"Genotype Quality\">\n");
	//fprintf(file, "%s", "##FORMAT=<ID=FT,Number=1,Type=String,Description=\"Per-sample genotype filter\">\n");
//	fprintf(file, "%s", "##FORMAT=<ID=RC,Number=1,Type=Integer,Description=\"Normalized high-quality read count for the SV\">\n");
	//fprintf(file, "%s", "##FORMAT=<ID=DR,Number=1,Type=Integer,Description=\"# high-quality reference pairs\">\n");
	fprintf(file, "%s", "##FORMAT=<ID=DR,Number=1,Type=Integer,Description=\"# high-quality reference reads\">\n");
	fprintf(file, "%s", "##FORMAT=<ID=DV,Number=1,Type=Integer,Description=\"# high-quality variant reads\">\n");

	//fprintf(file, "%s", "##FORMAT=<ID=RR,Number=1,Type=Integer,Description=\"# high-quality reference junction reads\">\n");
	//fprintf(file, "%s", "##FORMAT=<ID=RV,Number=1,Type=Integer,Description=\"# high-quality variant junction reads\">\n");
	fprintf(file, "%s", "#CHROM\tPOS\tID\tREF\tALT\tQUAL\tFILTER\tINFO\tFORMAT");
	for (size_t i = 0; i < Parameter::Instance()->vcf_files.size(); i++) {
		fprintf(file, "%c", '\t');
		fprintf(file, "%s", Parameter::Instance()->vcf_files[i].c_str());
	}
	fprintf(file, "%c", '\n');
	*/
}

void VCFPrinter::init() {
	file = fopen(Parameter::Instance()->output.c_str(), "w");
	//print_header();
}

int VCFPrinter::get_support(std::vector<SV_str> caller_info) {
	int support = 0;
	for (size_t i = 0; i < caller_info.size(); i++) {
		if (!caller_info[i].start.empty()) {
			support++;
		}
	}
	return support;
}
double VCFPrinter::get_avglen(std::vector<SV_str> caller_info) {
	double size = 0;
	double num = 0;
	for (size_t i = 0; i < caller_info.size(); i++) {
		size += caller_info[i].total_lenght;
		if (caller_info[i].total_lenght != 0) {
			num++;
		}
	}
	return size / num;
}

int num_support(std::vector<SV_str> caller) {
	int num = 0;
	for (size_t i = 0; i < caller.size(); i++) {
		if (!caller[i].type.empty()) {
			num++;
		}
	}
	return num;
}
std::vector<std::string> parse_segments(std::string entry) {
	std::vector<std::string> entries;
	std::string tmp;
	for (size_t i = 0; i < entry.size(); i++) {
		if (entry[i] == '\t') {
			entries.push_back(tmp);
			tmp.clear();
		} else {
			tmp += entry[i];
		}
	}
	entries.push_back(tmp);
	tmp.clear();
	return entries;
}

std::string VCFPrinter::print_info(SVS_Node* entry, std::pair<int, int> & summary) {
	std::ostringstream convert;
	int support = get_support(entry->caller);
	convert << ";overlapped_VCF=";
	convert << support - 1;
	if(support>1){
		summary.first++;
	}
	convert << ";total_Annotations=";
	convert << entry->annotations.size();

	if (!entry->annotations.empty()) {
		convert << ";overlapped_Annotations=";
		for (size_t i = 0; i < entry->annotations.size(); i++) {
			convert << entry->annotations[i].exp;
			if (i + 1 < entry->annotations.size()) {
				convert << ",";
			}
		}
	}else{
		summary.second++;
	}
	return convert.str();
}
void VCFPrinter::print_orig_entry(SVS_Node* entry, int id, std::pair<int, int> & summary) {

	std::vector<std::string> line_entries = parse_segments(entry->line);
	for (size_t j = 0; j < line_entries.size(); j++) {
		fprintf(file, "%s", line_entries[j].c_str());
		if (j == 7) {
			fprintf(file, "%s", print_info(entry,summary).c_str());
		}
		if (j + 1 < line_entries.size()) {
			fprintf(file, "%c", '\t');
		}
	}
	fprintf(file, "%c", '\n');

}
void VCFPrinter::print_entry(SVS_Node* entry, int id, std::pair<int, int> & summary) {
	std::ostringstream convert;   // stream used for the conversion
	convert << entry->start.chr;
	convert << "\t";
	convert << entry->start.position;
	convert << "\t";
	convert << trans_type(entry->type);
	convert << "00";
	convert << id;
	convert << "SUR\tN\t<";
	convert << trans_type(entry->type);
	convert << ">\t.\t";
	convert << "PASS\t";
	convert << "SUPP=";
	int support = get_support(entry->caller);
	convert << support;
	convert << ";AVGLEN=";
	convert << get_avglen(entry->caller);
	convert << ";SVTYPE=";
	convert << trans_type(entry->type);
	convert << ";SVMETHOD=SURVIVORv2;CHR2=";
	convert << entry->stop.chr;
	convert << ";END=";
	convert << entry->stop.position;
	convert << ";overlapped_VCF=";
	int num = num_support(entry->caller);
	if (num > 1) {
		summary.second++;
	}

	convert << support - 1;
	convert << ";total_Annotations=";
	convert << entry->annotations.size();
	if (!entry->annotations.empty()) {
		convert << ";overlapped_Annotations=";
		for (size_t i = 0; i < entry->annotations.size(); i++) {
			convert << entry->annotations[i].exp;
			if (i + 1 < entry->annotations.size()) {
				convert << ",";
			}
		}
		summary.first++;
	}
	convert << "\t";
	convert << "GT:LN:DV:TY:CO\t";

	for (size_t i = 1; i < entry->caller.size(); i++) {
		convert << "./.:";
		convert << entry->caller[i].total_lenght;
		convert << ":";
		if (entry->caller[i].supporting.empty()) {
			convert << "NaN";
		}
		for (size_t j = 0; j < entry->caller[i].supporting.size(); j++) {
			if (j > 0) {
				convert << ",";
			}
			convert << entry->caller[i].supporting[j];
		}
		convert << ":";
		if (entry->caller[i].type.empty()) {
			convert << "NaN";
		}
		for (size_t j = 0; j < entry->caller[i].type.size(); j++) {
			if (j > 0) {
				convert << ",";
			}
			convert << trans_type(entry->caller[i].type[j]);
		}
		convert << ":";
		if (entry->caller[i].start.empty()) {
			convert << "NaN";
		}
		for (size_t j = 0; j < entry->caller[i].start.size(); j++) {
			if (j > 0) {
				convert << ",";
			}
			convert << entry->caller[i].start[j].chr;
			convert << "_";
			convert << entry->caller[i].start[j].position;
			convert << "-";
			convert << entry->caller[i].stop[j].chr;
			convert << "_";
			convert << entry->caller[i].stop[j].position;
		}
		convert << "\t";
	}
	fprintf(file, "%s", convert.str().c_str());
	fprintf(file, "%c", '\n');
}

