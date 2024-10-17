#include "GraphSalesman.hpp"

#include <simple_svg_1.0.0.hpp>
using namespace svg;


float GraphSalesman::
distance(NodeSalesman &n0, NodeSalesman &n1)
{
    return (std::sqrtf((n1.east - n0.east) * (n1.east - n0.east) + (float) (n1.south - n0.south)
           * (n1.south - n0.south)));
}

float GraphSalesman::
pathSize(std::vector<NodeSalesman *> &n)
{
    float sz = 0;

    for (int i = 0; i < n.size() - 1; i++) {
        NodeSalesman *d = n[i];
        NodeSalesman *e = n[i + 1];
        sz += distance(*d, *e);
    }
    return sz;
}

void GraphSalesman::
renderGraph(std::string filename, int w, int h,
  std::vector<NodeSalesman *> *path)
{
    Dimensions dimensions(w, h);
    Document doc(filename, Layout(dimensions, Layout::BottomLeft));

    for (auto n = nodesBegin(); n != nodesEnd(); n++) {
        std::vector<NodeSalesman *> a = getAdjacents(*(n->first));
        for (auto m = a.begin(); m != a.end(); m++) {
            float x = (**m).east;
            float y = (**m).south;
            doc << Line(Point(n->first->east, h - n->first->south), Point(x, h - y), Stroke(1, Color::Silver));
        }
    }

    for (auto n = nodesBegin(); n != nodesEnd(); n++) {
        doc << Circle(Point(n->first->east, h - n->first->south), 8, Fill(Color::Black), Stroke(1, Color::Black));
        doc << Text(Point(n->first->east - 10, h - n->first->south + 13), n->first->name, Color::Black,
          Font(10, "Verdana"));
    }

    if (path) {
        for (int i = 0; i < path->size() - 1; i++) {
            NodeSalesman *n = path->at(i);
            NodeSalesman *m = path->at(i + 1);
            float d         = distance(*m, *n);
            doc << Line(Point(n->east, h - n->south), Point(m->east, h - m->south), Stroke(3, Color::Green));
            doc << Text(Point(n->east + 10, h - n->south + 0), std::to_string(i), Color::Green,
              Font(10, "Verdana"));
        }

        float pSize = pathSize(*path);
        doc << Text(Point(10, h - 20), std::to_string(pSize) + " km", Color::Green,
          Font(10, "Verdana"));
    }

    doc.save();
} // GraphSalesman::renderGraph

void GraphSalesman::findNodesInRadius(float dist, NodeSalesman &n, std::vector<NodeSalesman> &nodes,
  std::vector<NodeSalesman> &retNodes)
{
    for (auto m = nodes.begin(); m != nodes.end(); m++) {
        if (distance(n, *m) <= dist && n.name != m->name) {
            retNodes.push_back(*m);
        }
    }
}

NodeSalesman * GraphSalesman::findNode(rc::Graph<NodeSalesman, EdgeSalesman> g, std::string name)
{
    for (auto it = g.nodesBegin(); it != g.nodesEnd(); it++) {
        if (it->first->name == name) {
            return it->first;
        }
    }
    return NULL;
}

// Replace edges path[i]->path[i+1] and path[j]->path[j+1]
// with path[i]->path[j] and path[i+1]->path[j+1]
void GraphSalesman::swapEdges(std::vector<NodeSalesman *> &path, int i, int j)
{
    i += 1;
    while (i < j) {
        NodeSalesman *temp = path[i];
        path[i] = path[j];
        path[j] = temp;
        i++;
        j--;
    }
}

float GraphSalesman::twoOpt(std::vector<NodeSalesman *>& n)
{
    bool better = true;

    while (better == true) {
        better = false;
        for (int i = 0; i < n.size() - 1; i++) {
            for (int j = 0; j < n.size() - 1; j++) {
                if (j != i - 1 && j != i && j != i + 1) {
                    float a = distance(*n[i], *n[i + 1]);
                    float b = distance(*n[j], *n[j + 1]);
                    float c = distance(*n[i], *n[j]);
                    float d = distance(*n[i + 1], *n[j + 1]);
                    if (a + b > c + d) {
                        swapEdges(n, i, j);
                        better = true;
                    }
                }
            }
        }
    }
    return pathSize(n);
}
