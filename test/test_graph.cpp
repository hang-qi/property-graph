#include <iostream>
#include "graph.h"

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
    std::cout << g << std::endl;

    return 0;
}
