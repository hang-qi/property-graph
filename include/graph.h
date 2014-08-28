#ifndef _GRAPH_H_
#define _GRAPH_H_ value

#include <ostream>
#include <sstream>

#include "property.h"

class GraphElement
{
public:
    GraphElement(const std::string& id)
        : id_(id) {}
    virtual ~GraphElement() {}

    virtual void Print(std::ostream& os) const = 0;

    // void AddProperty(const std::string& key, const T& value){};
    // void GetProperty(){};

protected:
    virtual void PrintProperty(std::ostream& os) const
    {
        if (!dict_.size() == 0)
        {
            os << "\n  Properties: " << "(" << dict_.size() << ")";
            for (auto it = dict_.begin(); it != dict_.end(); it++)
            {
                os << "\n    " << it->first << ": \t" << it->second << std::endl;
            }
        }
    }

protected:
    PropertyDict dict_;
    std::string id_;
};

std::ostream& operator<< (std::ostream& os, const GraphElement& element)
{
    element.Print(os);
    return os;
}


class Vertex : public GraphElement
{
public:
    Vertex(const std::string& id)
        : GraphElement(id) {}
    virtual ~Vertex() {}

public:
    virtual void Print(std::ostream& os) const
    {
        os << "Vertex: " << id_;
        GraphElement::PrintProperty(os);
    }
};

class Edge : public GraphElement
{
public:
    Edge(const std::string& id,
         const std::string& end_1,
         const std::string& end_2)
        : GraphElement(id)
    {
        end_1_ = end_1;
        end_2_ = end_2;
    }

    Edge(const std::string& end_1,
         const std::string& end_2)
        : GraphElement("")
    {
        std::stringstream ss;
        ss << end_1 << "->" << end_2;
        id_ = ss.str();
        end_1_ = end_1;
        end_2_ = end_2;
    }
    virtual ~Edge() {}

public:
    virtual void Print(std::ostream& os) const
    {
        os << "Edge: " << id_;
        os << " ( " << end_1_ << " -> " << end_2_ << " )";
        GraphElement::PrintProperty(os);
    }

protected:
    std::string end_1_;
    std::string end_2_;
};

class Graph : public GraphElement
{
public:
    Graph(const std::string& id)
        : GraphElement(id) {}
    virtual ~Graph() {};

public:
    virtual void Print(std::ostream& os) const
    {
        os << "Graph: " << id_ << std::endl;
        os << "  # edges:    " << std::endl;
        os << "  # vertexes: ";
        GraphElement::PrintProperty(os);
    }

    void AddVertex(const Vertex& vertex) {};
    void AddEdge(const Edge& edge) {};

protected:
    // vertex set
    // edge set
    // several indices and maps
};

#endif
