#pragma once
#include <vector>
#include <limits>

class Dijkstra
{
public:
    // Initializes a Dijkstra object with a specified number of vertices.
    Dijkstra(int vertices);
    ~Dijkstra(){};

    // Adds an edge from src to dest with a specified weight.
    void add_edge(int src, int dest, int weight);

    // Computes the shortest path from a source vertex src to all other vertices.
    std::vector<int> shortest_path(int src);

    // Generates a random graph with edges having weights up to 'max_weight'.
    void generate_random_graph(int max_weight = 10);

private:
    int num_vertices; // Number of vertices in the graph.
    std::vector<std::vector<std::pair<int, int>>> adj_list; // Adjacency list representing the graph. Each pair is (destination, weight).
};
