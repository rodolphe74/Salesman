#pragma once

#include "Graph.hpp"

class NodeSalesman : public rc::NodeBase {
public:
    std::string name;
    int east;
    int south;

    NodeSalesman(std::string n, int e, int s) : east(e), south(s)
    {
        name = n;
    }

    friend std::ostream &operator << (std::ostream &os, const NodeSalesman n)
    {
        os << n.name << "[" << n.index << "]" << "=" << n.east << "," << n.south;
        return os;
    }
};

class EdgeSalesman : public rc::EdgeBase {
public:
    std::string name;

    EdgeSalesman(std::string n)
    {
        name = n;
    }

    friend std::ostream &operator << (std::ostream &os, const EdgeSalesman e)
    {
        os << "--{" << e.name << "[" << e.index << "]" << "}--";
        return os;
    }
};

class GraphSalesman : public rc::Graph<NodeSalesman, EdgeSalesman>
{
private:
    void
    swapEdges(std::vector<NodeSalesman *> &path, int i, int j);

public:
    float
    distance(NodeSalesman &n0, NodeSalesman &n1);
    float
    pathSize(std::vector<NodeSalesman *> &n);
    void
    renderGraph(std::string filename, int w, int h,
      std::vector<NodeSalesman *> *path = NULL);
    void
    findNodesInRadius(float dist, NodeSalesman &n, std::vector<NodeSalesman> &nodes,
      std::vector<NodeSalesman> &retNodes);
    NodeSalesman *
    findNode(rc::Graph<NodeSalesman, EdgeSalesman> g, std::string name);
    float
    twoOpt(std::vector<NodeSalesman *>& n);
};
