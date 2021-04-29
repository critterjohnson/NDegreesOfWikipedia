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

        std::shared_ptr<WikiPage> addPageFromUrl(std::string url);
        std::shared_ptr<WikiPage> addWikiPage(WikiPage*);
        std::shared_ptr<WikiPage> getPage(std::string url);
        void populateSubPages(std::string url);
        std::vector<std::string> getLevel(std::string url, int level);
        int findUrl(std::string);

    private:
        std::unordered_map<std::string, std::shared_ptr<WikiPage>> pages;
        void linkPages(std::shared_ptr<WikiPage> page1, std::shared_ptr<WikiPage> page2);
};

#endif
