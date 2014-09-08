#include <iostream>
#include "graph/graph.h"

int main()
{
    std::cout << "Create two vertices and edges.\n";

    Vertex v1(1);
    v1["name"] = make_value<std::string>("human");
    v1["weight"] = make_value<float>(0.8);
    v1["type"] = make_value<std::string>("AND");

    Vertex v2(2);

    Edge e(1, 2);

    std::cout << "Print out vertex and edge info.\n";
    std::cout << v1 << std::endl;
    std::cout << v2 << std::endl;
    std::cout << e << std::endl;

    std::cout << "Create graph.\n";
    Graph g;
    g["name"] = make_value<std::string>("spatio AOG");
    g.add_vertex(v1);
    g.add_vertex(v2);
    g.add_edge(Edge(1,2));
    std::cout << g << std::endl;

    std::cout << "In  degree of v1: " << g.get_in_degree(1) << std::endl;
    std::cout << "Out degree of v1: " << g.get_out_degree(1) << std::endl;
    std::cout << "In  degree of v2: " << g.get_in_degree(2) << std::endl;
    std::cout << "Out degree of v2: " << g.get_out_degree(2) << std::endl;

    return 0;
}
