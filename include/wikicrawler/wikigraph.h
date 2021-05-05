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
        std::shared_ptr<WikiPage> addWikiPage(std::shared_ptr<WikiPage>);
        std::shared_ptr<WikiPage> getPage(std::string url);
        void populateSubPages(std::string url);
        int findUrl(std::string);
        std::vector<std::shared_ptr<WikiPage>> getWikiPages();

    private:
        std::unordered_map<std::string, std::shared_ptr<WikiPage>> pages;
        void linkPages(std::shared_ptr<WikiPage> page1, std::shared_ptr<WikiPage> page2);
        void linkWithLinkedPages(std::shared_ptr<WikiPage> page);
};

#endif
