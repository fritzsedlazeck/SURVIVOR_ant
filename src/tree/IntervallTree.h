/*
 * IntervallTree.h
 *
 *  Created on: Jun 23, 2015
 *      Author: fsedlaze
 */

#ifndef TREE_INTERVALLTREE_H_
#define TREE_INTERVALLTREE_H_

#include <vector>

#include "TNode.h"
#include "../Paramer.h"

class IntervallTree {
private:
	int max(int, int);
	TNode * srl(TNode *&);
	TNode * drl(TNode *&);
	TNode * srr(TNode *&);
	TNode * drr(TNode *&);
	long overlap(position_str start, position_str stop,short type, SVS_Node * curr_svs);
	bool same_breakpoint(position_str first, position_str second,int max_dist);
	void inorder_addvcf(position_str start, position_str stop, short type,int caller,TNode * p, TNode * root);
	void inorder_addanno(position_str start, position_str stop, std::string msg, TNode * p, TNode * root);
public:
	void insert(position_str start, position_str stop ,short type,std::string line,int num_reads,int caller_id, bool is_secondary, TNode *&p);
	void del(SVS_Node * point, TNode *&);
	int deletemin(TNode *&);
	void find(SVS_Node * point, TNode *&);
	TNode * findmin(TNode*);
	TNode * findmax(TNode*);
	void makeempty(TNode *&);
	void copy(TNode * &, TNode *&);
	TNode * nodecopy(TNode *&);
	void preorder(TNode*);
	void inorder(TNode*,TNode * root);
	void postorder(TNode*);
	int bsheight(TNode*);
	void get_breakpoints(TNode *p,std::vector<SVS_Node *> & points);
	int nonodes(TNode*);
	void collapse_intervalls(TNode *&p);
	void add_vcf(position_str start, position_str stop, short type,int caller, TNode *&p);
	void add_anno(position_str start, position_str stop, std::string msg, TNode *&p);
};

#endif /* TREE_INTERVALLTREE_H_ */
