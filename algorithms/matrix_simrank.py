import numpy as np
import time

#generate transfer matrix
def getTransferMatrix(graph, vertex):
	for i in range(vertex):
		#sum of col
		num = sum(graph[:, i])
		#normalize
		for j in range(vertex):
			if num != 0:
				graph[j, i] = graph[j, i] / num
			
	return graph

#simrank
def simrank(graph, vertex, C = 0.8, K = 5):
	#initial similarity matrix
	S = np.matrix(np.identity(vertex))
	
	#identity matrix
	I = np.matrix(np.identity(vertex))
	
	#start the time
	start = time.process_time()
	
	#generate transfer matrix
	graph = getTransferMatrix(graph, vertex)
	
	for i in range(K):
		temp = C * graph.T * S * graph
		S = temp + I - np.diag(np.diag(temp))
	#end the time
	end = time.process_time()
	
	print('RunTime：', end - start)
	return S
