#ifndef _GRAPH_H_
#define _GRAPH_H_ value

#include <ostream>
#include <sstream>
#include <algorithm>
#include <functional>

#include "property.h"

typedef unsigned int VertexIdType;

class GraphElement
{
public:
    GraphElement() {}
    virtual ~GraphElement() {}

    virtual void PrintSummary(std::ostream& os) const = 0;

    std::shared_ptr<PropertyValue>& operator[] (const std::string& key)
    {
        return dict_[key];
    }

    // std::shared_ptr<PropertyValue> operator[] (const std::string& key) const
    // {
    //     return dict_[key];
    // }

    bool hasProperty(const std::string& key)
    {
        return dict_.find(key) != dict_.end();
    }

protected:
    virtual void PrintProperty(std::ostream& os) const
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
    element.PrintSummary(os);
    return os;
}


class Vertex : public GraphElement
{
public:
    Vertex(const VertexIdType& id)
        : id_(id) {}
    virtual ~Vertex() {}

public:
    virtual void PrintSummary(std::ostream& os) const
    {
        os << "Vertex: " << id_;
        GraphElement::PrintProperty(os);
    }

protected:
    VertexIdType id_;
};

class Edge : public GraphElement
{
public:
    Edge(const VertexIdType& end_1,
         const VertexIdType& end_2)
    {
        end_1_ = end_1;
        end_2_ = end_2;
    }

    virtual ~Edge() {}

public:
    virtual void PrintSummary(std::ostream& os) const
    {
        os << "Edge: " << end_1_ << " -> " << end_2_;
        GraphElement::PrintProperty(os);
    }

protected:
    VertexIdType end_1_;
    VertexIdType end_2_;
};

// A labeler is a label creator which takes a GraphElement object
// and produces the label that can is readable to human especially
// for debug and visualization purposes.
typedef std::function<std::string(GraphElement&)> Labeler;

class Graph : public GraphElement
{
public:
    Graph() {}
    virtual ~Graph() {};

public:
    virtual void PrintSummary(std::ostream& os) const
    {
        os << "Graph: " << std::endl;
        os << "  # edges:    " << std::endl;
        os << "  # vertexes: ";
        GraphElement::PrintProperty(os);
    }

    void AddVertex(const Vertex& vertex) {}
    void AddVertices(const std::vector<Vertex>& vertex) {}

    void AddEdge(const Edge& edge) {}
    void AddEdges(const std::vector<Edge>& edge) {}

    std::vector<std::shared_ptr<Edge>> GetEdges(const VertexIdType& source,
        const VertexIdType& dest){}
    std::vector<std::shared_ptr<Edge>> GetInEdges(const VertexIdType& source){}
    std::vector<std::shared_ptr<Edge>> GetOutEdges(const VertexIdType& dest){}

    int GetInDegree(const VertexIdType& source) {}
    int GetOutDegree(const VertexIdType& dest) {}

    Graph GetNeighbors(const VertexIdType& vid, int levels) {}

    void WriteGraphViz(std::ostream& os, Labeler VertexLabeler, Labeler EdgeLabeler)
    {}

    void LoadRDF(std::ostream& os, Labeler VertexLabeler, Labeler EdgeLabeler) {}
    void WriteRDF(std::ostream& os, Labeler VertexLabeler, Labeler EdgeLabeler) {}

protected:
    // vertex set
    // edge set
    // several indices and maps
};

#endif
