#include "dijkstra.hpp"

#include <set>
#include <cstdlib>
#include <ctime>

Dijkstra::Dijkstra(int vertices) 
{
    num_vertices = vertices; 
    srand(time(NULL));
    adj_list.resize(num_vertices); // Initialize the adjacency list for each vertex.
}

// Adds an edge from src to dest with a specified weight.
void Dijkstra::add_edge(int src, int dest, int weight) 
{
    adj_list[src].push_back(std::make_pair(dest, weight));
}

std::vector<int> Dijkstra::shortest_path(int src) 
{
    std::set<std::pair<int, int>> setds; // Set used to efficiently find the vertex with the minimum distance.
    std::vector<int> distances(num_vertices, std::numeric_limits<int>::max()); // Stores the shortest distance to each vertex.

    // Initialize distances and setds.
    setds.insert(std::make_pair(0, src));
    distances[src] = 0;

    while (!setds.empty()) {
        int u = setds.begin()->second; // Vertex with the shortest distance.
        setds.erase(setds.begin()); // Remove this vertex from the set.

        // Iterate through all neighbors of u.
        for (auto& i : adj_list[u]) {
            int v = i.first; // Neighbor vertex.
            int weight = i.second; // Weight of the edge to the neighbor.

            // Check if there's a shorter path to v through u.
            if (distances[v] > distances[u] + weight) {
                // If the current distance is not infinity, remove it from the set.
                if (distances[v] != std::numeric_limits<int>::max()) {
                    setds.erase(setds.find(std::make_pair(distances[v], v)));
                }

                // Update the distance to this vertex.
                distances[v] = distances[u] + weight;
                setds.insert(std::make_pair(distances[v], v));
            }
        }
    }

    return distances;
}

// Creates a random directed graph 
// Directed graph: if we create an edge 0->1 this edge is not usable as 1->0. Only one direction
// In order to go from 1->0, either we need to create another edge as 1->0 or take another path.

// Idea of algorithm:
//      -- Connect each node directed to one of the next 5 nodes
//      -- Connect each node directed to one of the last 5 nodes
// Example:
//      Node number: 17
//          -- Pick a random number between 18-22. And connect it with 17. If number is 20: such as 17->20
//          -- Pick a random number between 12-26. And connect it with 17. If number is 12: such as 12->17
//      At the end you get a very random and very large directed graph
void Dijkstra::generate_random_graph(int max_weight) 
{
    for (int i = 0; i < num_vertices; ++i) {
        // Connect to a random node among the next 5 nodes.
        if (i < num_vertices - 1) {
            int next_node = std::min(i + 1 + rand() % std::min(5, num_vertices - i - 1), num_vertices - 1);
            int weight = rand() % max_weight + 1;
            add_edge(i, next_node, weight);
        }

        // Connect to a random node among the previous 5 nodes.
        if (i > 0) {
            int prev_node = std::max(i - 1 - rand() % std::min(5, i), 0);
            int weight = rand() % max_weight + 1;
            add_edge(i, prev_node, weight);
        }
    }
}
