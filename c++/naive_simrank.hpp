#ifndef NAIVE_SIMRANK_HPP
#define NAIVE_SIMRANK_HPP

#include <iostream>
#include <ctime>
#include <vector>
using namespace std;

class NaiveSimRank{
private:
	int** graph;
	int vertex;
	float C;
	int K;
	float** sim_mat;
	
public:
	NaiveSimRank(int** G, int _vertex, float _C = 0.8, int _K = 5);
	~NaiveSimRank();
	
	//get input degree of a
	int getInputDegree(int a);
	//get input nodes of a
	vector<int> getInputNodes(int a);
	
	//node_sim
	float node_sim(int a, int b);
	
	//simrank
	float** simrank();
};

NaiveSimRank::NaiveSimRank(int** G, int _vertex, float _C, int _K){
	graph = G;
	vertex = _vertex;
	C = _C;
	K = _K;
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

NaiveSimRank::~NaiveSimRank(){
	delete[] sim_mat;
}

int NaiveSimRank::getInputDegree(int a){
	int sum=0;
	for(int i=0;i < vertex;i++)
		sum += graph[i][a];
	return sum;
}

vector<int> NaiveSimRank::getInputNodes(int a){
	vector<int> list;
	for(int i=0;i < vertex;i++){
		if(graph[i][a] != 0)
			list.push_back(i);
	}
	return list;
}

float NaiveSimRank::node_sim(int a, int b){
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
	vector<int>::iterator it_a;
	vector<int>::iterator it_b;
	vector<int> a_list = getInputNodes(a);
	vector<int> b_list = getInputNodes(b);
	for(it_a = a_list.begin();it_a != a_list.end();it_a++){
		for(it_b = b_list.begin();it_b != b_list.end();it_b++){
			postfix += sim_mat[*it_a][*it_b];
		}
	}
	return prefix * postfix;
}

float** NaiveSimRank::simrank(){
	//start time
	clock_t start, end;
	double duration;
	start = clock();
	
	for(int k=0;k < K;k++){
		for(int a=0;a < vertex;a++){
			for(int b=a;b < vertex;b++){
				sim_mat[a][b] = node_sim(a, b);
				sim_mat[b][a] = sim_mat[a][b];
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
