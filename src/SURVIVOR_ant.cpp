//============================================================================
// Name        : Sniffles.cpp
// Author      : Fritz Sedlazeck
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
// phil: cd ~/hetero/philipp/pacbio/example-svs/reads
//For mac: cmake -D CMAKE_C_COMPILER=/opt/local/bin/gcc-mp-4.7 -D CMAKE_CXX_COMPILER=/opt/local/bin/g++-mp-4.7 ..
#include <iostream>
#include "Paramer.h"
#include "tree/IntervallTree.h"
#include <tclap/CmdLine.h>
#include "print/VCFPrinter.h"
#include "Parser/VCFParser.h"
#include "Parser/BEDParser.h"
#include "Parser/GFFParser.h"

Parameter* Parameter::m_pInstance = NULL;
//cmake -D CMAKE_C_COMPILER=/opt/local/bin/gcc-mp-4.7 -D CMAKE_CXX_COMPILER=/opt/local/bin/g++-mp-4.7 ..

void read_parameters(int argc, char *argv[]) {
	TCLAP::CmdLine cmd("SURVIVOR_ant version 0.0.1", ' ', "0.0.1");
	TCLAP::ValueArg<std::string> arg_svvcf("i", "svvcf", "The vcf file that we should annotate", true, "", "string");
	TCLAP::ValueArg<std::string> arg_vcf("v", "vcf", "VCF input file name(s)", false, "", "string");
	TCLAP::ValueArg<std::string> arg_bed("b", "bed", " bed annotation(s)", false, "", "string");
	TCLAP::ValueArg<std::string> arg_gff("g", "gff", " gff annotation(s)", false, "", "string");
	TCLAP::ValueArg<std::string> arg_out("o", "output", " output file name", false, "", "string");
	TCLAP::ValueArg<int> arg_svsdist("", "svs_distance", "Maximum distance to group SV together. Default: 1000", false, 1000, "int");
	TCLAP::ValueArg<int> arg_annodist("", "anno_distance", "Maximum distance to assign annotations to SVs. Default: 1000", false, 1000, "int");
	TCLAP::ValueArg<int> arg_minlength("l", "min_length", "Minimum length of SV to be reported. Default: 20", false, 20, "int");

	cmd.add(arg_svvcf);
	cmd.add(arg_svsdist);
	cmd.add(arg_annodist);
	cmd.add(arg_bed);
	cmd.add(arg_gff);
	cmd.add(arg_vcf);
	cmd.add(arg_minlength);
	cmd.add(arg_out);

	//parse cmd:
	cmd.parse(argc, argv);
	std::cout << "BED FILE: " << arg_bed.getValue() << std::endl;
	Parameter::Instance()->parse_intput(Parameter::Instance()->bed_files, arg_bed.getValue());
	Parameter::Instance()->parse_intput(Parameter::Instance()->vcf_files, arg_vcf.getValue());
	Parameter::Instance()->parse_intput(Parameter::Instance()->gff_files, arg_gff.getValue());
	Parameter::Instance()->debug = true;
	Parameter::Instance()->output = arg_out.getValue();
	Parameter::Instance()->max_caller = Parameter::Instance()->vcf_files.size() + 1; //+1 because of the inital vcf
	Parameter::Instance()->sv_vcf = arg_svvcf.getValue();
	Parameter::Instance()->svs_dist = arg_svsdist.getValue();
	Parameter::Instance()->anno_dist = arg_annodist.getValue();
	Parameter::Instance()->use_type = false;
}

//TODO: check overlap for annotation depending on the type!!!
//TODO: add annotation exp. to the vcf node!

int main(int argc, char *argv[]) {
	try {
		//init parameter and reads user defined parameter from command line.
		read_parameters(argc, argv);

		IntervallTree bst;
		TNode *root = NULL;

		//TODO extend for parameter.
		Parameter::Instance()->use_type = true;

		//Run through the vcf files to merge them!
		VCFParser * parser_vcf = new VCFParser();
		std::vector<std::string> entry_lines;
		std::string header = parser_vcf->get_header(Parameter::Instance()->sv_vcf);
		std::vector<entry_str> entries = parser_vcf->parse_entries(Parameter::Instance()->sv_vcf);
		for (size_t j = 0; j < entries.size(); j++) {
			if (entries[j].stop.chr.empty()) {
				entries[j].stop.chr = entries[j].start.chr; //some files do just report the first chr;
			}
			bst.insert(entries[j].start, entries[j].stop, entries[j].type, entries[j].line, entries[j].num_reads, entries[j].is_secondary, 0, root);
		}

		//Annotate with additional vcf files:
		for (size_t i = 0; i < Parameter::Instance()->vcf_files.size(); i++) {
			std::vector<entry_str> entries = parser_vcf->parse_entries(Parameter::Instance()->vcf_files[i]);
			//add to tree;
			for (size_t j = 0; j < entries.size(); j++) {
				if (entries[j].stop.chr.empty()) {
					entries[j].stop.chr = entries[j].start.chr; //some files do just report the first chr;
				}
				bst.add_vcf(entries[j].start, entries[j].stop, entries[j].type, i + 1, root);
			}
		}
		delete parser_vcf;

		//Annotate with additional bed files:
		BEDParser * parser_bed = new BEDParser();
		for (size_t i = 0; i < Parameter::Instance()->bed_files.size(); i++) {
			std::vector<Anno_str> entries = parser_bed->parse_bed(Parameter::Instance()->bed_files[i]);
			//annotate tree;
			for (size_t j = 0; j < entries.size(); j++) {
				if (entries[j].exp.empty()) {
					entries[j].exp = Parameter::Instance()->bed_files[i];
				}
				bst.add_anno(entries[j].start, entries[j].stop, entries[j].exp, root);
			}
		}
		delete parser_bed;

		//Annotate with additional gff files:
		GFFParser * parser_gff = new GFFParser();
		for (size_t i = 0; i < Parameter::Instance()->gff_files.size(); i++) {
			std::vector<Anno_str> entries = parser_gff->parse_gff(Parameter::Instance()->gff_files[i]);
			//annotate tree;
			for (size_t j = 0; j < entries.size(); j++) {
				bst.add_anno(entries[j].start, entries[j].stop, entries[j].exp, root);
			}
		}
		delete parser_gff;

		//init printer:
		VCFPrinter * printer = new VCFPrinter();
		printer->init();
		std::vector<SVS_Node *> points;
		bst.get_breakpoints(root, points);

		for (size_t i = 0; i < points.size(); i++) {
			//we have to collapse the tra again!
			if (points[i]->type == 3) {
				if (points[i]->is_secondary) {
					std::cout<<"SEARCH"<<std::endl;
					for (size_t j = 0; j < points.size(); j++) {
						if (!points[j]->is_secondary && (points[i]->start.position == points[j]->stop.position && points[i]->type == points[j]->type)) {
							//found!
							for (size_t t = 0; t < points[i]->annotations.size(); t++) {
								points[j]->annotations.push_back(points[j]->annotations[t]);
							}
						}

					}
				}
			}
		}
		std::cout << "start printing" << std::endl;
		//print:
		std::pair<int, int> summary;
		int num = 0;
		std::vector<std::string> file;

		std::cout << "print entries " << std::endl;
		printer->print_header(header);
		for (size_t i = 0; i < points.size(); i++) {
			if (!points[i]->is_secondary) {
				num++;
				printer->print_orig_entry(points[i], i, summary);
			}
		}
		printer->close_file();

		std::cout << "Successfully finished" << std::endl;
		std::cout << "total entries: " << num << std::endl;
		std::cout << "overlapping vcf entries: " << summary.first << std::endl;
		std::cout << "overlapping annotations: " << summary.second << std::endl;
	} catch (TCLAP::ArgException &e) {  // catch any exceptions
		std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
	}
	return 0;
}
