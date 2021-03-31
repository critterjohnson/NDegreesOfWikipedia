#include <memory>
#include <iostream>
#include <vector>

#include <wikicrawler/wikigraphnode.h>

WikiGraphNode::WikiGraphNode(WikiPage wikipage) {
    page = wikipage;
}

void WikiGraphNode::addEdgeIfNotExists(std::shared_ptr<WikiGraphNode> node) {
    for (std::shared_ptr<WikiGraphNode> n : edges) {
        if (n->page.url == node->page.url) {
            return;
        }
    }
    edges.push_back(node); // TODO: sort edges somehow?
}

void WikiGraphNode::addEdgesIfNotExists(std::vector<std::shared_ptr<WikiGraphNode>> nodes) {
    for (std::shared_ptr<WikiGraphNode> node : nodes) {
        addEdgeIfNotExists(node);
    }
} 
