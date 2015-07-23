#ifndef _GRAPH_H_
#define _GRAPH_H_ value

#include <ostream>
#include <sstream>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <memory>
#include <unordered_map>
#include <list>
#include <set>

#include "property.h"

/// The data type of vertex id.
typedef unsigned int VertexIdType;

class GraphElement
{
public:
    GraphElement() {}
    virtual ~GraphElement() {}

    virtual void print_summary(std::ostream& os) const = 0;

    std::shared_ptr<PropertyValue>& operator[] (const std::string& key)
    {
        return dict_[key];
    }

    const std::shared_ptr<PropertyValue>& operator[](std::string key) const
    {
        return const_cast<GraphElement&>(*this)[key];
    }

    // std::shared_ptr<const PropertyValue> operator[] (const std::string& key) const
    // {
    //     std::cout << "ACCESS\n";
    //     if (dict_.find(key) != dict_.end())
    //     {
    //         // We need to cast 'this' pointer to non-const version.
    //         // Because unordered_map::operator[] does not suppose const
    //         // since it will add an item if the key does not exist.
    //         // But since we are explicitly check the existence,
    //         // we are safe to perform a const cast.
    //         return const_cast<GraphElement*>(this)->dict_[key];
    //     }
    //     else
    //     {
    //         std::cout << "NON\n";
    //         throw std::runtime_error("Cannot find key.");
    //     }
    // }

    bool has_property(const std::string& key)
    {
        return dict_.find(key) != dict_.end();
    }

    std::vector<std::string> get_properties() const
    {
        std::vector<std::string> props;
        for(auto p : dict_) {
            props.push_back(p.first);
        }
        return props;
    }

protected:
    virtual void print_property(std::ostream& os) const
    {
        if (!dict_.size() == 0)
        {
            os << "\n  Properties: " << dict_.size() << " items";
            for (const auto& pair : dict_)
            {
                os << "\n    " << pair.first << ": \t" << pair.second;
            }
        }
    }

protected:
    PropertyDict dict_;
};

std::ostream& operator<< (std::ostream& os, const GraphElement& element)
{
    element.print_summary(os);
    return os;
}

/// The class for a vertex.
/**
In a graph, each vertex shall have a unique id.
*/
class Vertex : public GraphElement
{
public:
    Vertex(const VertexIdType& id) : id_(id) {}
    Vertex(const Vertex& other) : id_(other.id_)
    {
        for (auto& p : other.get_properties()) {
            dict_[p] = other[p];
        }
    }

    virtual ~Vertex() {}

public:
    const VertexIdType& id() const {return id_;}

    virtual void print_summary(std::ostream& os) const
    {
        os << "Vertex: " << id_;
        GraphElement::print_property(os);
    }

protected:
    VertexIdType id_;
};

/// The class for an edge.
/**
In a graph, each edge must specify the vertex id of the source and target.
*/
class Edge : public GraphElement
{
public:
    Edge(const VertexIdType& source, const VertexIdType& target)
         : source_(source), target_(target) {}

    Edge(const Edge& other) : source_(other.source_), target_(other.target_)
    {
        for (auto& p : other.get_properties())
        {
            dict_[p] = other[p];
        }
    }

    virtual ~Edge() {}

public:
    virtual void print_summary(std::ostream& os) const
    {
        os << "Edge: " << source_ << " -> " << target_;
        GraphElement::print_property(os);
    }

    const VertexIdType& source() const {return source_;}
    const VertexIdType& target() const {return target_;}

protected:
    VertexIdType source_;
    VertexIdType target_;
};

//@{
/**
A labeler is a label creator which takes a GraphElement object
and produces the label that can is readable to human especially
for debug and visualization purposes.
*/
typedef std::function<std::string(const std::shared_ptr<Vertex>&)> VertexLabeler;
typedef std::function<std::string(const std::shared_ptr<Edge>&)> EdgeLabeler;
//@}

class Adjacency
{
public:
    Adjacency() {}
    Adjacency(const Vertex& v)
    {
        vertex = std::make_shared<Vertex>(v);
    }

    std::shared_ptr<Vertex> vertex;
    std::list<std::shared_ptr<Edge>> out_edges;
    std::list<std::shared_ptr<Edge>> in_edges;
};

/// The class for a graph.
/**
We assume the library is to be used to construct sparse graphs.
In particular, the number of vertices can be large, but the degree of each
vertex is reasonably small
(in general cases, in-degree is 1, out-degree is around 10).

The graph is implemented using adjacency list.
This motivates us to implement our adjacency list using an
``std::unordered_map`` for vertex list
 and two ``std::list`` for edge lists, for in-edges and out-edges respectively.
 The purpose of two edges lists for each vertex is to find out parents fast.
*/
class Graph : public GraphElement
{
public:
    Graph() : num_edges_(0) {}
    virtual ~Graph() {};

public:
    virtual void print_summary(std::ostream& os) const
    {
        os << "Graph: " << std::endl;
        os << "  # edges:    " << num_edges_ << std::endl;
        os << "  # vertices: " << vertices_.size();
        GraphElement::print_property(os);
    }

    //@{
    /***/
    /// Add a vertex into the graph.
    void add_vertex(const Vertex& v)
    {
        if (vertices_.find(v.id()) != vertices_.end())
        {
            throw std::runtime_error("The vertex already exists.");
        }
        vertices_[v.id()] = Adjacency(v);
    }
    /// Add a list of vertices into the graph.
    void add_vertex(const std::vector<Vertex>& vertices)
    {
        for (const Vertex& v : vertices)
        {
            add_vertex(v);
        }
    }
    //@}

    //@{
    void add_edge(const Edge& e)
    {
        check_vertex_exist(e.source());
        check_vertex_exist(e.target());
        std::shared_ptr<Edge> edge = std::make_shared<Edge>(e);
        vertices_[edge->target()].in_edges.push_back(edge);
        vertices_[edge->source()].out_edges.push_back(edge);
        ++num_edges_;
    }
    void add_edge(const std::vector<Edge>& edges)
    {
        for (const Edge& e : edges)
        {
            add_edge(e);
        }
    }
    //@}

    std::shared_ptr<Vertex> get_vertex(const VertexIdType& vid)
    {
        return vertices_[vid].vertex;
    }

    //@{
    std::vector<std::shared_ptr<Edge>> get_in_edges(const VertexIdType& vid)
    {
        check_vertex_exist(vid);
        std::vector<std::shared_ptr<Edge>> in_edges;
        for (const std::shared_ptr<Edge>& e : vertices_[vid].in_edges)
        {
            in_edges.push_back(e);
        }
        return in_edges;
    }

    std::vector<std::shared_ptr<Edge>> get_out_edges(const VertexIdType& vid)
    {
        check_vertex_exist(vid);
        std::vector<std::shared_ptr<Edge>> out_edges;
        for (const std::shared_ptr<Edge>& e : vertices_[vid].out_edges)
        {
            out_edges.push_back(e);
        }
        return out_edges;
    }
    //@}

    //@{
    int get_in_degree(const VertexIdType& vid) const
    {
        check_vertex_exist(vid);
        return const_cast<Graph*>(this)->vertices_[vid].in_edges.size();
    }

    int get_out_degree(const VertexIdType& vid) const
    {
        check_vertex_exist(vid);
        return const_cast<Graph*>(this)->vertices_[vid].out_edges.size();
    }
    //@}

    int get_num_vertices()
    {
        return vertices_.size();
    }

    int get_unused_id()
    {
        VertexIdType maxVal = 0;
        for (auto p : vertices_)
        {
            if (p.first > maxVal)
            {
                maxVal = p.first;
            }
        }
        return maxVal + 1;
    }

    /// Output graph structure to GraphViz dot format.
    void write_graphviz(std::ostream& os, VertexLabeler fVertexLabeler=nullptr,
        EdgeLabeler fEdgeLabeler=nullptr)
    {
        os << "digraph g {\n";
        for (auto p : vertices_)
        {
            if (fVertexLabeler != nullptr)
            {
                os << p.first << "[label=\"" << fVertexLabeler(p.second.vertex) << "\"];\n";
            }
            if (p.second.out_edges.size() > 0)
            {
                for (auto e : p.second.out_edges)
                {
                    os << p.first << " -> " << e->target();
                    if (fEdgeLabeler != nullptr)
                    {
                        os << " [ label= \"" << fEdgeLabeler(e) << "\"]";
                    }
                    os << ";\n";
                }
            }
        }
        os << "}\n";
    }

    std::string get_gml()
    {
        std::stringstream result;
        result << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
        result << "<graphml xmlns=\"http://graphml.graphdrawing.org/xmlns\" ";
        result << "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" ";
        result << "xsi:schemaLocation=\"http://graphml.graphdrawing.org/xmlns ";
        result << "http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd\">\n";

        // Loop through all verticies, and find all properties used
        std::set<std::string> vPropNames;
        std::set<std::string> ePropNames;
        for (auto p : vertices_) {
            Vertex v = *(p.second.vertex);
            for (auto propName : v.get_properties()) {
                vPropNames.insert(propName);
            }
            for (auto edge : get_out_edges(v.id())) {
                for (auto propName : edge -> get_properties()) {
                    ePropNames.insert(propName);
                }
            }
        }
        for (auto propName : vPropNames) {
            result << "<key id=\"" + propName + "V\" for=\"node\" attr.name=\"" + propName + "\" attr.type=\"string\"/>\n";
        }

        for (auto propName : ePropNames) {
            result << "<key id=\"" + propName + "E\" for=\"edge\" attr.name=\"" + propName + "\" attr.type=\"string\"/>\n";
        }

        result << "<graph id=\"G\" edgedefault=\"directed\">\n";

        for (auto p : vertices_) {
            Vertex v = *(p.second.vertex);
            result << "  <node id=\"" + std::to_string(v.id()) + "\">\n";
            for (auto propName : v.get_properties()) {
                std::string propValue = get_value<std::string>(v[propName]);
                result << "    <data key=\"" + propName + "V\">";
                result << propValue;
                result << "</data>\n";
            }
            result << "  </node>\n";
        }

        for (auto p : vertices_) {
            Vertex v = *(p.second.vertex);
            if (p.second.out_edges.size() > 0) {
                for (auto e : p.second.out_edges) {
                    result << "  <edge source=\"" + std::to_string(p.first) +
                              "\" target=\"" + std::to_string(e->target()) + "\">\n";

                    for (auto propName : e -> get_properties()) {
                        std::string propValue = get_value<std::string>((*e)[propName]);
                        result << "    <data key=\"" + propName + "E\">";
                        result << propValue;
                        result << "</data>\n";
                    }

                    result << "  </edge>\n";
                }
            }
        }

        result << "</graph>\n";
        result << "</graphml>\n";
        return result.str();
    }

    // void LoadRDF(std::ostream& os, Labeler VertexLabeler, Labeler EdgeLabeler) {}
    // void WriteRDF(std::ostream& os, Labeler VertexLabeler, Labeler EdgeLabeler) {}

protected:
    void check_vertex_exist(const VertexIdType& vid) const
    {
        if (vertices_.find(vid) == vertices_.end())
        {
            throw std::runtime_error("The vertex does not exist.");
        }
    }

protected:
    /// The adjacency list of the graph.
    /**
    The vertex list is implemented using a ``std::unordered_map`` to support
    constant time look up for vertices.
    */
    std::unordered_map<VertexIdType, Adjacency> vertices_;

    /// A counter for number of edges in the graph.
    unsigned int num_edges_;
};

#endif

