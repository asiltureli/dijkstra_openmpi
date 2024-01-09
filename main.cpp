#include "dijkstra/dijkstra.hpp"
#include <iostream>
#include <mpi.h>
#include <chrono>

#define NUMBER_OF_BIG_GRAPH_NODES 1024 // Number of nodes in a large graph.
#define MAX_WEIGHT                20  // Maximum weight for graph edges.


int test_case_small()
{
    Dijkstra dijkstra(5);

    // Add edges
    // The graph looks like this:
    // (0) --1-- (1) --1-- (2)
    //  |        |         |
    //  2        4         3
    //  |        |         |
    // (3) --1-- (4) ------+
    dijkstra.add_edge(0, 1, 1);
    dijkstra.add_edge(1, 2, 1);
    dijkstra.add_edge(2, 4, 3);
    dijkstra.add_edge(0, 3, 2);
    dijkstra.add_edge(3, 4, 1);
    dijkstra.add_edge(1, 4, 4);

    std::vector<std::vector<int>> adjacency_table(5, std::vector<int>(5, std::numeric_limits<int>::max()));

    // Iterate over each node
    for (int src = 0; src < 5; ++src) {
        std::vector<int> distances = dijkstra.shortest_path(src);

        for (int dest = 0; dest < distances.size(); ++dest) {
            adjacency_table[src][dest] = distances[dest];
        }
    }

    // Print the adjacency table
    for (int i = 0; i < 5; ++i) {
        std::cout << "From vertex " << i << ":\n";
        for (int j = 0; j < 5; ++j) {
            std::cout << "  To vertex " << j << ": " << adjacency_table[i][j] << std::endl;
        }
        std::cout << std::endl;
    }
    return 0;
}

int test_case_big(Dijkstra &dijkstra)
{

    std::vector<std::vector<int>> adjacency_table(NUMBER_OF_BIG_GRAPH_NODES, std::vector<int>(NUMBER_OF_BIG_GRAPH_NODES, std::numeric_limits<int>::max()));

    auto start = std::chrono::high_resolution_clock::now();
    
    // Iterate over each node
    for (int src = 0; src < NUMBER_OF_BIG_GRAPH_NODES; ++src) {
        std::vector<int> distances = dijkstra.shortest_path(src);

        for (int dest = 0; dest < distances.size(); ++dest) {
            adjacency_table[src][dest] = distances[dest];
        }
    }

    auto stop = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < NUMBER_OF_BIG_GRAPH_NODES; ++i) {
        std::cout << "From vertex " << i << ":\n";
        for (int j = 0; j < NUMBER_OF_BIG_GRAPH_NODES; ++j) {
            std::cout << "  To vertex " << j << ": " << adjacency_table[i][j] << std::endl;
        }
        std::cout << std::endl;
    }

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "Time taken non-parallel: " << double(duration.count())/1000 << " seconds" << std::endl;

    return 0;
}

int test_case_big_openmpi(int argc, char **argv, Dijkstra &dijkstra)
{
    MPI_Init(&argc, &argv);

    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Ensure all processes reach this point before measuring time and starting computation
    MPI_Barrier(MPI_COMM_WORLD);
    double start_time = MPI_Wtime();

    // Divide the graph nodes among the available MPI processes.
    int nodes_per_proc = NUMBER_OF_BIG_GRAPH_NODES / world_size;
    int remainder = NUMBER_OF_BIG_GRAPH_NODES % world_size;
    int start_node = world_rank * nodes_per_proc;
    int end_node = (world_rank + 1) * nodes_per_proc;

    // Each process computes shortest paths for its portion of nodes.
    std::vector<int> local_distances;
    for (int i = start_node; i < end_node; ++i) {
        std::vector<int> distances = dijkstra.shortest_path(i);
        local_distances.insert(local_distances.end(), distances.begin(), distances.end());
    }

    double end_time = MPI_Wtime();

    // ----- Gathering results and printing ------ //
    std::vector<int> all_distances;
    std::vector<int> recv_counts(world_size, 0);
    std::vector<int> displacements(world_size, 0);

    if (world_rank == 0) {
        int total_elements = 0;
        for (int i = 0; i < world_size; ++i) {
            // Each process sends distances for the nodes it processed
            recv_counts[i] = (nodes_per_proc + (i < remainder ? 1 : 0)) * NUMBER_OF_BIG_GRAPH_NODES;
            displacements[i] = total_elements;
            total_elements += recv_counts[i];
        }
        all_distances.resize(total_elements);
    }

    // Gather all local_distances into all_distances at root
    MPI_Gatherv(local_distances.data(), local_distances.size(), MPI_INT,
                all_distances.data(), recv_counts.data(), displacements.data(),
                MPI_INT, 0, MPI_COMM_WORLD);

    if (world_rank == 0) {

        // Iterate over each source vertex
        for (int i = 0; i < NUMBER_OF_BIG_GRAPH_NODES; ++i) {
            std::cout << "From vertex " << i << ":\n";
            
            // Iterate over each destination vertex
            for (int j = 0; j < NUMBER_OF_BIG_GRAPH_NODES; ++j) {
                int index = i * NUMBER_OF_BIG_GRAPH_NODES + j;
                std::cout << "  To vertex " << j << ": " << all_distances[index] << std::endl;
            }
            std::cout << std::endl;
        }        

        std::cout << "Time taken parallel: " << (end_time - start_time) << " seconds" << std::endl;
    }
    MPI_Finalize();
    return 0;
}

int main(int argc, char **argv)
{
    // Uncomment to run the small graph test case.
    // return test_case_small();

    // Initialize a Dijkstra object for a large graph and generate a random graph.
    Dijkstra dijkstra(NUMBER_OF_BIG_GRAPH_NODES);
    dijkstra.generate_random_graph(MAX_WEIGHT);
    
    // Uncomment to run the large graph test case without MPI.
    return test_case_big(dijkstra);

    // Uncomment to run the large graph test case with MPI.
    //return test_case_big_openmpi(argc, argv, dijkstra);
}
