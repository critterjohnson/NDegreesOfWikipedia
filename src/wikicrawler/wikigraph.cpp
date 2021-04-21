#include <cstddef>
#include <iostream>
#include <memory>
#include <vector>

#include <wikicrawler/wikigraph.h>

std::shared_ptr<WikiPage> WikiGraph::createPage(std::string url) {
    std::shared_ptr<WikiPage> pagePtr = std::shared_ptr<WikiPage>(new WikiPage(url));
    pages.push_back(pagePtr);
    return pagePtr;
}

void linkPages(std::shared_ptr<WikiPage> page1, std::shared_ptr<WikiPage> page2) {
    page1->createRelationship(page2);
    page2->createRelationship(page1);
}

// TODO: this should do error handling if it doesn't exist
std::shared_ptr<WikiPage> WikiGraph::getPage(std::string url) {
    for (std::shared_ptr<WikiPage> page : pages) {
        if (page->url == url) {
            return page;
        }
    }
    return nullptr;
}

void WikiGraph::fetchAndAddSubPages(std::string url) {
    std::shared_ptr<WikiPage> pagePtr = getPage(url);
    if (pagePtr == nullptr) {
        return;
    }

    std::vector<std::shared_ptr<WikiPage>> subPages = pagePtr->generateSubPages();
    for (std::shared_ptr<WikiPage> page : subPages) {
        pages.push_back(page);
    }
}
