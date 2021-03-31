#include <iostream>
#include <memory>
#include <vector>

#include <wikicrawler/wikigraph.h>
#include <wikicrawler/wikigraphnode.h>

void WikiGraph::addNodeIfNotExists(std::shared_ptr<WikiGraphNode> node) {
    for (std::shared_ptr<WikiGraphNode> nodePtr : nodes) {
        if (node->page.url == nodePtr->page.url) {
            return;
        }
    }
    nodes.push_back(node);
}

void WikiGraph::addWikiPage(WikiPage wikipage) {
    addNodeIfNotExists(std::shared_ptr<WikiGraphNode>(new WikiGraphNode(wikipage)));
}

void WikiGraph::addWikiPageWithEdge(WikiPage wikipage, std::shared_ptr<WikiGraphNode> edge) {
    std::shared_ptr<WikiGraphNode> edgeInGraph = edge;
    for (std::shared_ptr<WikiGraphNode> node : nodes) {
        if (edge->page.url == node->page.url) {
            edgeInGraph = node;
            break;
        }
    }

    WikiGraphNode* node = new WikiGraphNode(wikipage);
    node->addEdgeIfNotExists(edgeInGraph);
    addNodeIfNotExists(std::shared_ptr<WikiGraphNode>(node));
}

// void WikiGraph::addWikiPageWithEdges(WikiPage wikipage, std::vector<std::shared_ptr<WikiGraphNode>> pages) {
//     WikiGraphNode node(wikipage);
//     ; // TODO: sort this somehow?
//     for (WikiPage page : pages) {
//         WikiGraphNode pageNode(page);
//         node.addEdgeIfNotExists(pageNode);
//         pageNode.addEdgeIfNotExists(node);
//         nodes.push_back(pageNode); // TODO: should check if this node already exists
//     }
// }

int WikiGraph::findUrl(std::string url) {
    for (int i = 0; i < nodes.size(); i++) {
        std::shared_ptr<WikiGraphNode> node = nodes[i];
        if (node->page.url == url) {
            return i;
        }
    }
    return -1;
}
