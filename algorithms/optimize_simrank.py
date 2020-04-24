import numpy as np
import time

#calculate output degree
def getOutputDegree(a, graph):
    return graph[a].sum()

#calculate input degree
def getInputSum(a, graph):
    return graph[:, a].sum()

#get input list
def getInputNode(a, graph, vertex):
    index = []
    mat_col = graph[:, a]
    for j in range(vertex):
        if mat_col[j] != 0:
            index.append(j)
    return index

#get output list
def getOutputNode(a, graph, vertex):
    index = []
    for i in range(vertex):
        if graph[a, i] != 0:
            index.append(i)
    return index
	
#calculate δ
def getDerList(C, K, Der):
    der = []
    for i in range(K):
        der.append(Der / (K * pow(C, K-i-1)))
    return der

#calculate partial_a
def getPartial(graph, vertex, a, S):
    partial = []
    #first get input nodes of a
    index = getInputNode(a, graph, vertex)
            
    #calculate partial_a(i)        
    for i in range(vertex):
        if len(index) == 0:
            partial.append(0)
        else:
            result = 0
            for x in index:
                result = result + S[x, i]
            partial.append(result)
    return partial

#get essential nodes of a
def getEssential(graph, vertex, a, S):
    b_list = set()
    #first get input nodes of a
    index = getInputNode(a, graph, vertex)
	
    #get nodes whose similarity with index is non-zero
    temp = set()
    if len(index) != 0:
        for x in index:
            for i in range(vertex):
                if S[x, i] != 0:
                    temp.add(i)
    temp = list(temp)
    
    #get output nodes of temp
    if len(temp) != 0:
        for x in temp:
            index = getOutputNode(x, graph, vertex)
            if len(index) != 0:
                for i in index:
                    b_list.add(i)
    b_list = list(b_list)
    
    return b_list

#calculate nodes_sim
def calculate(graph, vertex, a, b, der, partial, C, S):
	if a == b:
		return 1
	
	Ia = getInputSum(a, graph)
	Ib = getInputSum(b, graph)
	
	if Ia == 0 or Ib == 0:
		return 0

    #prefix
	prefix = C / (Ia * Ib)
    #postfix
	postfix = 0
	index = getInputNode(b, graph, vertex) #input nodes of b
	if len(index) != 0:
		for i in index:
			postfix += partial[i]
	result = prefix * postfix
    #Determine whether result greater than threshold
	if result > der or S[a, b] != 0:
		return result
    
	return 0
	
#simrank优化算法
#三个参数，C是衰败因子，K是迭代次数，Der是阈值误差
def simrank(graph, vertex, C = 0.8, K = 5, Der = 0):
    #initial similarity matrix
	S = np.matrix(np.identity(vertex))
	
    #calculate δ
	der = getDerList(C, K, Der)
    
	#start the time
	start = time.process_time()
	
    #iteration
	for k in range(K):
		new_note_sim = np.matrix(np.identity(vertex))
        
        #traverse a
		for a in range(vertex):
			#if the output degree of a is zero, calculate the last iteration
            #next a
			if getOutputDegree(a, graph) == 0 and k != K-1:
				continue
            #calculate partial_a
			partial = getPartial(graph, vertex, a, S)
			#get essential nodes of a
			b_list = getEssential(graph, vertex, a, S)
			#calculate nodes_sim
			for b in b_list:
				new_note_sim[a, b] = calculate(graph, vertex, a, b, der[k], partial, C, S)
                
        #uodate S
		S = new_note_sim
		
    #end the time
	end = time.process_time()
    
	print('RunTime：', end - start)
	return S