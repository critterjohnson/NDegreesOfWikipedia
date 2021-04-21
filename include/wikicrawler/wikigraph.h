#ifndef WIKIGRAPH_H
#define WIKIGRAPH_H

#include <string>
#include <vector>
#include <memory>

#include "wikipage.h"

class WikiGraph {
    public:
        std::shared_ptr<WikiPage> createPage(std::string url);
        void linkPages(std::shared_ptr<WikiPage> page1, std::shared_ptr<WikiPage> page2);
        std::shared_ptr<WikiPage> getPage(std::string url);

        // finds a WikiPage in this graph (if it already exists) and generates and adds its sub pages
        void fetchAndAddSubPages(std::string url);
        std::vector<std::shared_ptr<WikiPage>> pages; // TODO: make this private and provide access to pages through an iterator
};

#endif
