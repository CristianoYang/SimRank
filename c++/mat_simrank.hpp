#pragma once
#ifndef MAT_SIMRANK_HPP
#define MAT_SIMRANK_HPP

#include <iostream>
#include <opencv2/opencv.hpp>
#include <ctime>

using namespace std;
using namespace cv;

class MatSimRank {
private:
	Mat graph;
	int vertex;
	float C;
	int K;
	Mat sim_mat;

public:
	MatSimRank(int** G, int _vertex, float _C = 0.8, int _K = 8);
	~MatSimRank();

	//return graph
	Mat getGraph();
	//getTransferMatrix
	void getTransferMatrix();
	//simrank
	Mat simrank();
};

MatSimRank::MatSimRank(int** G, int _vertex, float _C, int _K) {
	vertex = _vertex;
	C = _C;
	K = _K;
	//initial sim_mat
	sim_mat = Mat::eye(Size(vertex, vertex), CV_32FC1);
	//initial graph
	graph = Mat::zeros(Size(vertex, vertex), CV_32FC1);
	for (int i = 0;i < graph.rows;i++)
		for (int j = 0;j < graph.cols;j++)
			graph.at<float>(i, j) = G[i][j];
}

MatSimRank::~MatSimRank() {}

Mat MatSimRank::getGraph() {
	return graph;
}

void MatSimRank::getTransferMatrix() {
	for (int i = 0;i < vertex;i++) {
		//sum of col
		float num = 0;
		for (int j = 0;j < graph.rows;j++)
			num += graph.at<float>(j, i);
		//normalize
		for (int j = 0;j < graph.rows;j++)
			if (num != 0)
				graph.at<float>(j, i) /= num;
	}
}

Mat MatSimRank::simrank() {
	//start time
	clock_t start, end;
	double duration;
	start = clock();

	//identity matrix
	Mat I = Mat::eye(Size(vertex, vertex), CV_32FC1);

	//generate transfer matrix
	getTransferMatrix();

	for (int k = 0;k < K;k++) {
		Mat temp = C * graph.t() * sim_mat * graph;
		sim_mat = temp + I - Mat::diag(temp.diag());
	}

	//end time
	end = clock();
	duration = (double)(end - start) / CLOCKS_PER_SEC;
	cout << "运行时间：" << duration << endl;
	cout << endl;
	return sim_mat;
}

#endif