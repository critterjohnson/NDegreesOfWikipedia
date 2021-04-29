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
            WikiPage startPage(startUrl);
            std::unordered_map<std::string, WikiPage> pages; // map with all pages encountered during traversal
            pages[startUrl] = startPage;
            std::vector<std::string> links = startPage.links;

            // go through each url and sub-url in order until it's found in any thread
            int i = 0;
            while (!found) {
                WikiPage page(links[i]);
                pages[links[i]] = page;
                for (std::string link : page.links) {
                    links.push_back(link);
                    std::cout << link << std::endl;

                    if (link == endingUrl) {
                        found = true;
                    }
                }
                i++;
            }

            std::scoped_lock lock(graph.mtx);
        };

        std::cout << "spawning threads" << std::endl;
        for (std::string url : startPage.links) {
            new std::thread(f, url);
        }

        while (!found) {
        }
    }

    //WikiGraph graph;
    //graph.addPage(startingUrl);
    //graph.populateSubPages(startingUrl);

    //if (!(graph.findUrl(endingUrl) == 1)) {
        //graph.populateSubPages(startingUrl);

        //bool found = false;

        //auto f = [&found, endingUrl](std::string url) {
            //WikiGraph graph;
            //graph.addPage(url);

            //int i = 1;
            //int lastLevelLength = 0;
            //while (!(graph.findUrl(endingUrl) == 1)) {
                //if (found)
                    //break;
                //std::cout << "level: " << i << std::endl;
                //auto level = graph.getLevel(url, i);

                //for (int j = lastLevelLength; j < level.size(); j++) {
                    //if (found)
                        //break;
                    //graph.addPage(level[j]);
                    //std::cout << level[j] << std::endl;
                    //if (j % 30 == 0)
                        //if (graph.findUrl(endingUrl) == 1)
                            //break;
                //}

                //lastLevelLength = level.size();
                //i++;
            //}
            //found = true;
        //};
        
        //for (std::string u : graph.getPage(startingUrl)->links) {
            //new std::thread(f, u);
        //}
        //while (!found) {
        //}

         //while loop here that checks the graph every 10 seconds to see if found to replace checking in each thread

        //std::cout << "found " << endingUrl << std::endl;
    //}

    //int i = 1;
    //int lastLevelLength = 0;
    //while (!(graph.findUrl(endingUrl) == 1)) {
        //std::cout << "level: " << i << std::endl;
        //auto level = graph.getLevel(startingUrl, i);
        //for (int j = lastLevelLength; j < level.size(); j++) {
            //graph.addPage(level[j]);
            //std::cout << level[j] << std::endl;
            //if (j % 30 == 0)
                //if (graph.findUrl(endingUrl) == 1)
                    //break;
        //}
        //lastLevelLength = level.size();
        //i++;
    //}
    //std::cout << "found " << endingUrl << std::endl;
}
