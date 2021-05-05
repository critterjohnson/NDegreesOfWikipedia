#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

#include <cpr/cpr.h>
#include <wikicrawler/wikipage.h>
#include <wikicrawler/wikigraph.h>

int main() {
    std::string startingUrl = "https://en.wikipedia.org/wiki/Getabako";
    //std::string endingUrl = "https://en.wikipedia.org/wiki/Cupboard";
    std::string endingUrl = "https://en.wikipedia.org/wiki/Dishware";

    WikiPage startPage(startingUrl);
    WikiGraph graph;

    std::atomic_bool found = false;
    if (!startPage.linksContains(endingUrl)) {
        // lambda that each thread runs
        auto f = [&found, &graph, endingUrl](std::string startUrl) {
            std::shared_ptr<WikiPage> startPage(new WikiPage(startUrl));
            std::vector<std::shared_ptr<WikiPage>> pages; // vector with all pages encountered during traversal
            pages.push_back(startPage);
            std::vector<std::string> links = startPage->links;

            // go through each url and sub-url in order until it's found in any thread
            int i = 0;
            while (!found) {
                std::shared_ptr<WikiPage> page(new WikiPage(links[i]));
                pages.push_back(page);
                for (std::string link : page->links) {
                    links.push_back(link);
                    //std::cout << link << std::endl;

                    if (link == endingUrl) {
                        found = true;
                        std::cout << "found " << endingUrl << std::endl;
                    }
                }
                i++;
            }

            std::scoped_lock lock{graph.mtx};
            for (std::shared_ptr<WikiPage> page : pages) {
                graph.addWikiPage(page);
            }
        };

        std::vector<std::thread*> threads;
        std::cout << "spawning threads" << std::endl;
        for (std::string url : startPage.links) {
            threads.push_back(new std::thread(f, url));
        }

        while (!found) {
        }

        std::cout << "constructing graph" << std::endl;
        for (std::thread* t : threads) {
            t->join();
        }

        std::scoped_lock lock{graph.mtx};
        std::vector<std::shared_ptr<WikiPage>> pages = graph.getWikiPages();
        for (std::shared_ptr<WikiPage> p : pages) {
            std::cout << p->url; // TODO: this segfaults somehow?
            for (std::shared_ptr<WikiPage> p : p->getRelationships()) {
                std::cout << " -> " << p->url;
            }
            std::cout << std::endl << std::endl;
        }
    }
}
