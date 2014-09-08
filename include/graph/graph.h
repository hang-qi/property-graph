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

#include "property.h"

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


class Vertex : public GraphElement
{
public:
    Vertex(const VertexIdType& id) : id_(id) {}
    Vertex(const Vertex& other) : id_(other.id_) {}

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

class Edge : public GraphElement
{
public:
    Edge(const VertexIdType& source, const VertexIdType& target)
         : source_(source), target_(target) {}

    Edge(const Edge& other) : source_(other.source_), target_(other.target_) {}

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

// A labeler is a label creator which takes a GraphElement object
// and produces the label that can is readable to human especially
// for debug and visualization purposes.
typedef std::function<std::string(GraphElement&)> Labeler;

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

    void add_vertex(const Vertex& v)
    {
        if (vertices_.find(v.id()) != vertices_.end())
        {
            throw std::runtime_error("The vertex already exists.");
        }
        vertices_[v.id()] = Adjacency(v);
    }
    void add_vertex(const std::vector<Vertex>& vertices)
    {
        for (const Vertex& v : vertices)
        {
            add_vertex(v);
        }
    }

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

    // void WriteGraphViz(std::ostream& os, Labeler VertexLabeler, Labeler EdgeLabeler)
    // {}

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
    std::unordered_map<VertexIdType, Adjacency> vertices_;
    int num_edges_;
};

#endif

