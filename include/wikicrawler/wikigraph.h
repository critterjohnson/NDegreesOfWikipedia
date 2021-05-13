#ifndef WIKIGRAPH_H
#define WIKIGRAPH_H

#include <string>
#include <vector>
#include <memory>
#include <mutex>
#include <unordered_map>

#include "wikipage.h"

class WikiGraph {
    public:
        std::mutex mtx;
        int size = 0;

        std::shared_ptr<WikiPage> addPageFromUrl(std::string);
        std::shared_ptr<WikiPage> addWikiPage(std::shared_ptr<WikiPage>);
        std::shared_ptr<WikiPage> getPage(std::string);
        void populateSubPages(std::string url);
        int findUrl(std::string);
        std::vector<std::shared_ptr<WikiPage>> getWikiPages();

        std::vector<std::shared_ptr<WikiPage>> shortest_path(std::string, std::string);

    private:
        struct node {
            int vertex_num;
            std::shared_ptr<WikiPage> page;
        };
        std::unordered_map<std::string, node> pages;

        bool pagesContains(std::string);
        void linkPages(std::shared_ptr<WikiPage> page1, std::shared_ptr<WikiPage> page2);
        void linkWithLinkedPages(std::shared_ptr<WikiPage> page);
};

#endif
