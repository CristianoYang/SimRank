import numpy as np
import time

#calculate input degree
def getInputDegree(a, graph):
    return graph[:, a].sum()

#get input list
def getInputNode(a, graph, vertex):
    index = []
    mat_col = graph[:, a]
    for j in range(vertex):
        if mat_col[j] != 0:
            index.append(j)
    return index

# calculate two nodes similarity
def note_sim(a, b, C, S, graph, vertex):
    if a == b:
        return 1
    Ia = getInputDegree(a, graph)
    Ib = getInputDegree(b, graph)
    if Ia == 0 or Ib == 0:
        return 0
    prefix = C / (Ia * Ib)
    postfix = 0
    for ai in getInputNode(a, graph, vertex):
        for bj in getInputNode(b, graph, vertex):
            postfix += S[ai, bj]
    return prefix * postfix

# simrank
def simrank(graph, vertex, C = 0.8, times = 5):
    #initial similarity matrix
	S = np.matrix(np.identity(vertex))
    
    #start the time
	start = time.process_time()

	for run in range(times):
		new_note_sim = np.matrix(np.identity(vertex))
		for a in range(vertex):
			for b in range(vertex):
				new_note_sim[a, b] = note_sim(a, b, C, S, graph, vertex)
		S = new_note_sim
		
	#end the time
	end = time.process_time()
	
	print('RunTime：', end - start)
	return S