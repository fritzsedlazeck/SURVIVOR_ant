/*
 * TNode.h
 *
 *  Created on: Jun 23, 2015
 *      Author: fsedlaze
 */

#ifndef TREE_TNODE_H_
#define TREE_TNODE_H_

#include <iostream>
#include <cstdlib>
#include <cmath>

#include "../Paramer.h"
#include "../Parser.h"
//#include "TNode.h"
//struct svs_str;
//struct breakpoint_str;
// struct support_str;
class TNode {
private:
	SVS_Node * data;
	//int value;
	int height;
	void init() {
		this->parent = NULL;
		this->left = NULL;
		this->right = NULL;
	}
public:
	TNode * parent;
	TNode * left;
	TNode * right;

	TNode() {
		height = 0;
		init();
		this->data = NULL;
	}
	TNode(SVS_Node * point) {
		init();
		this->data = point;
		//this->data->caller_info[caller_id].num_reads= //TODO!
		height = 0;
	}

	TNode(position_str start, position_str stop, short type, std::string line,int num_reads, int caller_id,bool is_secondary) {
		this->data = new SVS_Node();
		this->data->start = start;
		this->data->stop = stop;
		this->data->type = type;
		this->data->line=line;
		this->data->is_secondary=is_secondary;
		init();
		//TODO: check that!
		for (int i = 0; i < Parameter::Instance()->max_caller; i++) {
			SV_str tmp;
			tmp.total_lenght = 0;
			tmp.type.clear();
			data->caller.push_back(tmp);
		}
	//	std::cout<<"Caller: "<<caller_id<<" vs. "<<this->data->caller.size() <<std::endl;
		//this->data->caller_info[caller_id].num_reads= //TODO!
		this->data->caller[caller_id].start.push_back(start);
		this->data->caller[caller_id].stop.push_back(stop);
		this->data->caller[caller_id].type.push_back(type);
		this->data->caller[caller_id].total_lenght = stop.position - start.position; // take the length of the svs as identifier.
		this->data->caller[caller_id].supporting.push_back(num_reads);

		height = 0;
	}

	~TNode() {

	}

	SVS_Node * get_data() {
		return data;
	}
	int get_height() {
		return height;
	}
	void set_height(int val) {
		this->height = val;
	}

	void add_annotation(position_str start, position_str stop, std::string msg) {
		Anno_str anno;
		anno.start=start;
		anno.stop=stop;
		anno.exp=msg;
		this->data->annotations.push_back(anno);
	}

	void add(position_str start, position_str stop, short type, int num_reads, int caller_id) {
		this->data->caller[caller_id].start.push_back(start);
		this->data->caller[caller_id].stop.push_back(stop);
		this->data->caller[caller_id].type.push_back(type);
		this->data->caller[caller_id].supporting.push_back(num_reads);

		if (this->data->caller[caller_id].total_lenght == 0) { //first time
			this->data->caller[caller_id].total_lenght = stop.position - start.position; // take the length of the svs as identifier.
		} else {
			this->data->caller[caller_id].total_lenght += stop.position - start.position;
		}
	}
};

#endif /* TREE_TNODE_H_ */
