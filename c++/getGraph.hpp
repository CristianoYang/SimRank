#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

/*
	generate random graph, edges num less than ratio * vertex * vertex
*/
class GetGraph{
private:
	int vertex;
	float ratio;
	int** graph;
	
public:
	//construct function
	GetGraph(int _vertex, float _ratio);
	~GetGraph();
	
	//return graph
	 int** generate_graph();
};

GetGraph::GetGraph(int _vertex, float _ratio){
	//initial parameters
	vertex = _vertex;
	ratio = _ratio;
	graph = new int*[vertex];
	for (int i = 0;i < vertex; i++) {
		graph[i] = new int[vertex];
	}
	for (int i = 0;i < vertex;i++) {
		for (int j = 0;j < vertex;j++) {
			graph[i][j] = 0;
		}
	}
}

GetGraph::~GetGraph(){
	delete[] graph;
}

int** GetGraph::generate_graph(){
	//generate random num
	srand((int)time(NULL));
	//edges num
	int edge_num = (int) (ratio * vertex * vertex);
	while(edge_num > 0){
		int i = rand()%vertex;
		int j = rand()%vertex;
		graph[i][j] = 1;
		edge_num--;
	}
	//the diagonal is not 1
	for(int i=0;i < vertex;i++)
		graph[i][i] = 0;
		
	return graph;
}
#endif
