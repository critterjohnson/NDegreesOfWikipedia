#include <iostream>
#include <memory>
#include <string>

#include <cpr/cpr.h>
#include <wikicrawler/wikipage.h>
#include <wikicrawler/wikigraph.h>
#include <wikicrawler/wikigraphnode.h>

int main() {
    std::string startingUrl = "https://en.wikipedia.org/wiki/Getabako";
    std::string endingUrl = "https://en.wikipedia.org/wiki/Cupboard";

    WikiGraph graph;
    WikiPage page(startingUrl);
    graph.addWikiPage(page);

    int i = 0;
    while (true) {
        std::shared_ptr<WikiGraphNode> node = graph.nodes[i];
        std::cout << node->page.url << std::endl;
        if (graph.findUrl(endingUrl) != -1) {
            std::cout << "found " << endingUrl << std::endl;
            break;
        }
        for (std::string link : node->page.links) {
            WikiPage page(link);
            graph.addWikiPageWithEdge(page, node);
        }
        i++;
    }

    std::cout << std::endl;

    for (std::shared_ptr<WikiGraphNode> node : graph.nodes) {
        std::cout << node->page.url;
        for (std::shared_ptr<WikiGraphNode> edge : node->edges) {
            std::cout << " -> " << edge->page.url;
        }
        std::cout << std::endl;
    }
}
