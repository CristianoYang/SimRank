#ifndef OPTIMIZED_SIMRANK_HPP
#define OPTIMIZED_SIMRANK_HPP

#include <iostream>
#include <vector>
#include <ctime>
#include <cmath>
#include <set>
using namespace std;

class OptimizedSimRank{
private:
	int** graph;
	int vertex;
	float C;
	int K;
	float Der;
	float** sim_mat;
	
public:
	OptimizedSimRank(int** G, int _vertex, float _C = 0.8, int _K = 6, float _Der = 0.05);
	~OptimizedSimRank();
	
	//get input degree of a
	int getInputDegree(int a);
	//get output degree of a
	int getOutputDegree(int a);
	//get input nodes of a
	vector<int> getInputNodes(int a);
	//get input nodes of a
	vector<int> getOutputNodes(int a);
	
	//calculate δ
	vector<float> getDerList();
	
	//calculate partial_a
	vector<float> getPartial(int a);
	
	//get essential nodes of a
	set<int> getEssential(int a);
	
	//node_sim
	float node_sim(int a, int b, vector<float> partial, float der);
	
	//simrank
	float** simrank();
};

OptimizedSimRank::OptimizedSimRank(int** G, int _vertex, float _C, int _K, float _Der){
	graph = G;
	vertex = _vertex;
	C = _C;
	K = _K;
	Der = _Der;
	//init sim_mat
	sim_mat = new float*[vertex];
	for(int i=0;i < vertex;i++)
		sim_mat[i] = new float[vertex];
		
	for (int i = 0;i < vertex;i++) {
		for (int j = 0;j < vertex;j++) {
			if(i == j)
				sim_mat[i][j] = 1;
			else
				sim_mat[i][j] = 0;
		}
	}
}

OptimizedSimRank::~OptimizedSimRank(){
	delete[] sim_mat;
}

int OptimizedSimRank::getInputDegree(int a){
	int sum=0;
	for(int i=0;i < vertex;i++)
		sum += graph[i][a];
	return sum;
}

int OptimizedSimRank::getOutputDegree(int a){
	int sum=0;
	for(int i=0;i < vertex;i++)
		sum += graph[a][i];
	return sum;
}

vector<int> OptimizedSimRank::getInputNodes(int a){
	vector<int> list;
	for(int i=0;i < vertex;i++){
		if(graph[i][a] != 0)
			list.push_back(i);
	}
	return list;
}

vector<int> OptimizedSimRank::getOutputNodes(int a){
	vector<int> list;
	for(int i=0;i < vertex;i++){
		if(graph[a][i] != 0)
			list.push_back(i);
	}
	return list;
}

vector<float> OptimizedSimRank::getDerList(){
	vector<float> list;
	for(int i=0;i < K;i++)
		list.push_back(Der / (K * pow(C, K-i-1)));
	return list;
}

set<int> OptimizedSimRank::getEssential(int a){
	set<int> b_set;
	vector<int>::iterator it_v;
	set<int>::iterator it_s;
	//first get input nodes of a
	vector<int> index = getInputNodes(a);
	
	//get nodes whose similarity with index is non-zero
	set<int> temp;
	if(!index.empty()){
		for(it_v=index.begin();it_v!=index.end();it_v++)
			for(int i=0;i < vertex;i++)
				if(sim_mat[*it_v][i] != 0)
					temp.insert(i);
	}
	
	//get output nodes of temp
	if(!temp.empty()){
		for(it_s=temp.begin();it_s!=temp.end();it_s++){
			index = getOutputNodes(*it_s);
			if(!index.empty()){
				for(it_v=index.begin();it_v!=index.end();it_v++)
					b_set.insert(*it_v);
			}
		}
	}
	
	return b_set;
}

vector<float> OptimizedSimRank::getPartial(int a){
	vector<float> list;
	vector<int>::iterator it;
	//first get input nodes of a
	vector<int> index = getInputNodes(a);
	
	//calculate partial_a(i)
	for(int i=0;i < vertex;i++){
		if(index.empty()){
			list.push_back(0);
		}
		else{
			float result = 0;
			for(it=index.begin();it!=index.end();it++)
				result += sim_mat[*it][i];
			list.push_back(result);
		}
	}
	return list;
}

float OptimizedSimRank::node_sim(int a, int b, vector<float> partial, float der){
	if(a == b)
		return 1;
	int Ia = getInputDegree(a);
	int Ib = getInputDegree(b);
	if(Ia == 0 or Ib == 0)
		return 0;
	//prefix
	float prefix = C / (Ia * Ib);
	//postfix
	float postfix = 0;
	vector<int>::iterator it;
	vector<int> index = getInputNodes(b); //input nodes of b
	if(!index.empty()){
		for(it=index.begin();it!=index.end();it++){
			postfix += partial[*it];
		}
	}
	float result = prefix * postfix;
	//Determine whether result greater than threshold
	if(result > der || sim_mat[a, b] != 0)
		return result;
		
	return 0;
}

float** OptimizedSimRank::simrank(){
	//calculate δ
	vector<float> der = getDerList();
	//start time
	clock_t start, end;
	double duration;
	start = clock();
	
	//iteration
	for(int k=0;k < K;k++){
		//traverse a
		for(int a=0;a < vertex;a++){
			//if the output degree of a is zero, calculate the last iteration
            //next a
            if(getOutputDegree(a) == 0 && k != K-1)
            	continue;
            //calculate partial_a
			vector<float> partial = getPartial(a);
			//get essential nodes of a
			set<int> b_set = getEssential(a);
			set<int>::iterator it;
			//calculate nodes_sim
			for(it=b_set.begin();it!=b_set.end();it++){
				if(*it >= a){
					sim_mat[a][*it] = node_sim(a, *it, partial, der[k]);
					sim_mat[*it][a] = sim_mat[a][*it];
				}
			}
		}
	}
	
	//end time
	end = clock();
	duration = (double)(end - start) / CLOCKS_PER_SEC;
	cout << "运行时间：" << duration << endl;
	cout << endl;
	return sim_mat;
}










#endif
