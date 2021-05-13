#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

#include <cpr/cpr.h>
#include <wikicrawler/wikipage.h>
#include <wikicrawler/wikigraph.h>

int main() {
    std::string startingUrl = "https://en.wikipedia.org/wiki/Roblox";
    std::string endingUrl = "https://en.wikipedia.org/wiki/Minecraft";
    std::cout << startingUrl << " -> " << endingUrl << std::endl;

    WikiPage startPage(startingUrl);
    WikiGraph graph;

    std::mutex cout_mtx;
    std::atomic_bool found = false;
    if (!startPage.linksContains(endingUrl)) {
        // lambda that each thread runs
        auto f = [&found, &graph, &cout_mtx, endingUrl](std::string startUrl) {
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

                    if (link == endingUrl && !found) {
                        found = true;
                        std::scoped_lock lck(cout_mtx);
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
        {
            std::scoped_lock lck(cout_mtx);
            std::cout << "spawning threads" << std::endl;
        }
        for (std::string url : startPage.links) { // TODO: really shouldn't let the number of links on the first page determine the number of threads
            threads.push_back(new std::thread(f, url));
        }

        while (!found) {
        }

        { 
            std::scoped_lock lck(cout_mtx);
            std::cout << "constructing graph" << std::endl;
        }
        for (std::thread* t : threads) {
            t->join();
        }

        std::scoped_lock lock{graph.mtx};
        std::vector<std::shared_ptr<WikiPage>> pages = graph.getWikiPages();
        for (std::shared_ptr<WikiPage> p : pages) {
            std::scoped_lock lck(cout_mtx);
            std::cout << p->url;
            for (std::shared_ptr<WikiPage> p : p->getRelationships()) {
                std::cout << " -> " << p->url;
            }
            std::cout << std::endl << std::endl;
        }
    } else {
        std::cout << "ending url " << endingUrl << " found on starting page " << startingUrl << std::endl;
    }
}
