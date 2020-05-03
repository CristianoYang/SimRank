#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "getGraph.hpp"
#include "naive_simrank.hpp"
#include "optimized_simrank.hpp"
#include "mat_simrank.hpp"

using namespace std;

int main(){
	int vertex = 5;
	float ratio = 0.3;
	
	//generate random graph
	GetGraph graph(vertex, ratio);
	int** G = graph.generate_graph();
	
	//view graph
	for(int i=0;i < vertex;i++){
		for(int j=0;j < vertex;j++)
			cout << G[i][j] << " ";
		cout << endl;
	}
	cout << endl;
	
	
	//naive_simrank
	NaiveSimRank naive_simrank(G, vertex);
	float** sim_mat = naive_simrank.simrank();
	
	//view sim_mat
	for(int i=0;i < vertex;i++){
		for(int j=0;j < vertex;j++)
			cout << sim_mat[i][j] << "\t";
		cout << endl;
	}
	cout << endl;
	
	
	//optimized_simrank
	OptimizedSimRank optimized_simrank(G, vertex);
	sim_mat = optimized_simrank.simrank();
	
	//view sim_mat
	for(int i=0;i < vertex;i++){
		for(int j=0;j < vertex;j++)
			cout << sim_mat[i][j] << "\t";
		cout << endl;
	}
	cout << endl;

	//mat_simrank
	MatSimRank mat_simrank(G, vertex);
	Mat S = mat_simrank.simrank();
	cout << S << endl;
	
}








