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

std::shared_ptr<WikiPage> WikiGraph::addWikiPage(WikiPage* page) {
    // TODO: create links to subpages that already exist in the graph

    if (pages.count(page->url) > 0) {
        return pages[page->url];
    }
    std::shared_ptr<WikiPage> ptr(page);
    pages[page->url] = ptr;
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

std::vector<std::string> WikiGraph::getLevel(std::string url, int level) {
    std::vector<std::string> urls;
    std::shared_ptr<WikiPage> curPage = getPage(url);
    urls.push_back(url);

    int levelStart = 0;
    int levelEnd = 1;
    for (int i = 1; i <= level; i++) {
        for (int j = levelStart; j < levelEnd; j++) {
            curPage = getPage(urls[j]);
            for (std::string u : curPage->links) {
                if(std::find(urls.begin(), urls.end(), u) == urls.end())
                    urls.push_back(u);
            }
        }
        levelStart = levelEnd;
        levelEnd = urls.size();
    }

    return urls;
}

int WikiGraph::findUrl(std::string url) {
    for (auto p = pages.begin(); p != pages.end(); p++) {
        if (p->first == url) {
            return 1;
        }
    }
    return 0;
}

void WikiGraph::linkPages(std::shared_ptr<WikiPage> page1, std::shared_ptr<WikiPage> page2) {
    page1->createRelationship(page2);
    page2->createRelationship(page1);
}
