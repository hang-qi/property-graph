A Lightweight Property Graph Model
==================================

This is a light-weight graph library that allows you to construct a graph
in memory with a simple interface.
As a nice feature, you can associate an arbitrary number of properties
with arbitary types to vertice and edges.


    // Create a vertex with three properties.
    Vertex v1(1);
    v1["name"]   = make_value<std::string>("human");
    v1["weight"] = make_value<float>(0.8);
    v1["type"]   = make_value<std::string>("AND");

    // Create another vertex.
    Vertex v2(2);

    // Create a graph.
    Graph g;
    g["name"] = make_value<std::string>("AOG");
    g.add_vertex(v1);
    g.add_vertex(v2);
    g.add_edge(Edge(1,2));
