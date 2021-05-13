#include <bits/stdc++.h>
#include <cstddef>
#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>

#include <wikicrawler/wikigraph.h>

bool WikiGraph::pagesContains(std::string url) {
    return (pages.count(url) > 0);
}

std::shared_ptr<WikiPage> WikiGraph::addPageFromUrl(std::string url) {
    if (pagesContains(url)) {
        return pages[url].page;
    }
    pages[url] = {size, std::shared_ptr<WikiPage>(new WikiPage(url))};
    size++;
    return pages[url].page;
}

std::shared_ptr<WikiPage> WikiGraph::addWikiPage(std::shared_ptr<WikiPage> page) {
    if (pages.count(page->url) > 0) {
        return pages[page->url].page;
    }
    std::shared_ptr<WikiPage> ptr(page);
    pages[page->url] = {size, ptr};
    size++;
    linkWithLinkedPages(ptr);
    return ptr;
}

std::shared_ptr<WikiPage> WikiGraph::getPage(std::string url) {
    if (pages.count(url) > 0) {
        return pages[url].page;
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
        wikipages.push_back(value.page);
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
            std::shared_ptr<WikiPage> wPage = pages[link].page;
            for (std::shared_ptr<WikiPage> relation : wPage->relationships) {
                if (relation->url == link)
                    break;
            }
            linkPages(page, wPage);
        }
    }
}

std::vector<std::shared_ptr<WikiPage>> WikiGraph::shortest_path(std::string source, std::string destination) {
    std::vector<std::shared_ptr<WikiPage>> pages_in_path;
    if (!pagesContains(source) || !pagesContains(destination))
        return pages_in_path;

    struct heap_node {
        std::string url;
        int vertex_num;
        int distance;
    };

    struct node_greater {
        int operator() (const heap_node& hn1, const heap_node& hn2) {
            return hn1.distance > hn2.distance;
        }
    };

    // set up min heaps
    std::vector<heap_node> min_heap;
    heap_node sptSet[size];

    node source_node = pages[source];
    min_heap.push_back(heap_node{
                source,
                source_node.vertex_num,
                0
            });
    for (const auto [key, value] : pages) {
        min_heap.push_back(heap_node{
                    key,
                    value.vertex_num,
                    INT_MAX
                });
    }
    std::make_heap(min_heap.begin(), min_heap.end(), node_greater());

    while (!min_heap.empty()) {
        heap_node n = min_heap.front();
        std::pop_heap(min_heap.begin(), min_heap.end());
        sptSet[n.vertex_num] = n;

        for (std::shared_ptr<WikiPage> pg : pages[n.url].page->relationships) {
            node pg_node = pages[pg->url];
            heap_node pg_heap_node;
            bool found; // check if found in min-heap
            for (heap_node node : min_heap) {
                if (node.url == pg_node.page->url) {
                    found = true;
                    pg_heap_node = node;
                    break;
                }
            }
            if (found) {
                if (pg_heap_node.distance > n.distance + 1) {
                    pg_heap_node.distance = n.distance;
                }
            }
        }
    }

     //find closest node
    //heap_node closest = sptSet[0];
    //for (heap_node n : sptSet) {
        //if (n.distance < closest.distance) {
            //closest = n;
        //}
    //}

    //std::vector<std::shared_ptr<WikiPage>> path;
     //calculate path information
    //int lastDistance = closest.distance;
    //for (std::shared_ptr<WikiPage> pg : pages[closest.url].page->relationships) {
        //if (sptSet[pages[pg->url].vertex_num].distance == lastDistance - 1) {
            //lastDistance -= 1;
            //path.push_back(pages[pg->url].page);
        //}
    //}
    //return path;
}

