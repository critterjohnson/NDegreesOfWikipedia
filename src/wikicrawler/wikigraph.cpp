#include <cstddef>
#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>

#include <wikicrawler/wikigraph.h>

std::shared_ptr<WikiPage> WikiGraph::addPageFromUrl(std::string url) {
    if (pages.count(url) > 0) {
        return pages[url];
    }
    pages[url] = std::shared_ptr<WikiPage>(new WikiPage(url));
    return pages[url];
}

std::shared_ptr<WikiPage> WikiGraph::addWikiPage(std::shared_ptr<WikiPage> page) {
    if (pages.count(page->url) > 0) {
        return pages[page->url];
    }
    std::shared_ptr<WikiPage> ptr(page);
    pages[page->url] = ptr;
    linkWithLinkedPages(ptr);
    return ptr;
}

std::shared_ptr<WikiPage> WikiGraph::getPage(std::string url) {
    if (pages.count(url) > 0) {
        return pages[url];
    } else {
        return nullptr;
    }
}

void WikiGraph::populateSubPages(std::string url) {
    std::shared_ptr<WikiPage> page = getPage(url);
    if (page == nullptr) {
        return;
    }

    for (std::string u : page->links) {
        linkPages(page, addPageFromUrl(u));
    }
}

int WikiGraph::findUrl(std::string url) {
    for (auto p = pages.begin(); p != pages.end(); p++) {
        if (p->first == url) {
            return 1;
        }
    }
    return 0;
}

std::vector<std::shared_ptr<WikiPage>> WikiGraph::getWikiPages() {
    std::vector<std::shared_ptr<WikiPage>> wikipages;
    for (const auto [key, value] : pages) {
        wikipages.push_back(value);
    }
    return wikipages;
}

void WikiGraph::linkPages(std::shared_ptr<WikiPage> page1, std::shared_ptr<WikiPage> page2) {
    page1->createRelationship(page2);
    page2->createRelationship(page1);
}

void WikiGraph::linkWithLinkedPages(std::shared_ptr<WikiPage> page) {
    for (std::string link : page->links) {
        if (pages.find(link) != pages.end()) {
            std::shared_ptr<WikiPage> wPage = pages[link];
            for (std::shared_ptr<WikiPage> relation : wPage->relationships) {
                if (relation->url == link)
                    break;
            }
            linkPages(page, wPage);
        }
    }
}
