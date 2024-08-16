#include "tspdynp.hpp"
#include <iostream> // for debug purposes
#include <limits> // for UINT_MAX
#include <vector>

long findMinJourneyCost(const std::vector<std::vector<long>> &graph, unsigned start, unsigned visited, unsigned n, std::vector<std::vector<long>> &memo){
	if(visited == (1 << n) - 1){ // all nodes have been visited
		if(graph[start][0] == 0) return 1e9;
		return graph[start][0];
	}
	if(memo[visited][start] != -1) return memo[visited][start];

	long cost = 1e9;

	for(unsigned vertex = 0; vertex < n; vertex++){
		if((visited & (1 << vertex)) == 0 && graph[start][vertex] != 0){ // node hasnt been visited and has an edge
			// mark as visited
			unsigned newVisited = visited|(1 << vertex);
			long currCost = graph[start][vertex];
			long nextCost = findMinJourneyCost(graph, vertex, newVisited, n, memo);

			if(nextCost != 1e9) currCost += nextCost;
			else currCost = 1e9;

			cost = std::min(cost, currCost);
		}
	}
	memo[visited][start] = cost;
	return memo[visited][start];
}

void findMinJourney(const std::vector<std::vector<long>> &graph, unsigned start, unsigned visited, unsigned n, std::vector<std::vector<long>> &memo, std::vector<unsigned> &journey){
	if(visited == (1 << n) - 1) return; // reached all nodes

	long cost = 1e9;
	unsigned node;

	for(unsigned vertex = 0; vertex < n; vertex++){
		if((visited & (1 << vertex)) == 0 && graph[start][vertex] != 0){
			long currCost = graph[start][vertex] + memo[visited|(1 << vertex)][vertex];
			if(currCost < cost){
				cost = currCost;
				node = vertex;
			}
		}
	}
	journey.push_back(node);
	//make call to find next node to visit and marking the vertex we took now
	unsigned newVisited = visited|(1 << node);
	findMinJourney(graph, node, newVisited, n, memo, journey);
}


// Solve TSP exactly, using dynamic programming.
// The grading script will use this exact function signature, so do not change it!
// The graph is given as an adjacency matrix, HOWEVER, no edges have a cost of '0'.
// The value graph[i][j] tells you the cost of an edge from i TO j.  This graph is directed.
//         However, if graph[i][j]==0, this means there is no edge from i to j, instead of having one of that cost.
std::vector<unsigned> tsp_dynamic_program(const std::vector<std::vector<long>> & graph, unsigned n){
    std::vector<unsigned> journey{0}; // will be the most optimal journey
	std::vector<std::vector<long>> memo(1 << n, std::vector<long>(n, -1));
    
	findMinJourneyCost(graph, 0, 1, n, memo);
	findMinJourney(graph, 0, 1, n, memo, journey);

    return journey;
}


// Returns the cost of the journey described here.
long costOfJourney(const std::vector<std::vector<long>> & graph, const std::vector<unsigned> & journey){
    unsigned from = journey[journey.size()-1], to = journey[0];
    long cost = graph[from][to]; // cost from the last element to first element
    
    for(int i = journey.size()-2; i >=0; i--){
        from = journey[i];
        to = journey[i+1];
        cost += graph[from][to];
    }
    return cost;
}