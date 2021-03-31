#ifndef WIKIGRAPHNODE_H
#define WIKIGRAPHNODE_H

#include "wikipage.h"
#include <memory>
#include <vector>

class WikiGraphNode {
    public:
        WikiPage page;
        std::vector<std::shared_ptr<WikiGraphNode>> edges;

        WikiGraphNode(WikiPage wikipage);

        // adds an edge to edges if it is not already present
        void addEdgeIfNotExists(std::shared_ptr<WikiGraphNode> node);
        void addEdgesIfNotExists(std::vector<std::shared_ptr<WikiGraphNode>> nodes);
};

#endif