#ifndef WIKIGRAPH_H
#define WIKIGRAPH_H

#include <string>
#include <vector>
#include <memory>

#include "wikipage.h"
#include "wikigraphnode.h"

class WikiGraph {
    public:
        std::vector<std::shared_ptr<WikiGraphNode>> nodes;

        void addNodeIfNotExists(std::shared_ptr<WikiGraphNode> node);
        void addWikiPage(WikiPage wikipage);
        void addWikiPageWithEdge(WikiPage wikipage, std::shared_ptr<WikiGraphNode> edge);
        // void addWikiPageWithEdges(WikiPage wikipage, std::vector<std::shared_ptr<WikiGraphNode>> pages);
        int findUrl(std::string url);
};

#endif