#include <iostream>
#include <memory>
#include <string>

#include <cpr/cpr.h>
#include <wikicrawler/wikipage.h>
#include <wikicrawler/wikigraph.h>

int main() {
    std::string startingUrl = "https://en.wikipedia.org/wiki/Getabako";
    std::string endingUrl = "https://en.wikipedia.org/wiki/Cupboard";
    // std::string endingUrl = "https://en.wikipedia.org/wiki/Tableware";

    WikiGraph graph;
    graph.createPage(startingUrl);

    int i = 0;
    while (true) {
        std::shared_ptr<WikiPage> page = graph.pages[i];
        std::cout << page->url << std::endl;
        if (graph.getPage(endingUrl) != nullptr) {
            std::cout << "found " << endingUrl << std::endl;
            break;
        }
        // TODO: fix theoretically infinite loop - if a node already exists don't add its children again
        graph.fetchAndAddSubPages(page->url);
        i++;
    }
    std::cout << std::endl;

    for (std::shared_ptr<WikiPage> page : graph.pages) {
        std::cout << page->url << " -> ";
        auto relationships = page->getRelationships();
        for (std::shared_ptr<WikiPage> r : relationships) {
            std::cout << r->url << " - ";
        }
        std::cout << std::endl;
    }
}
