#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include <cpr/cpr.h>
#include <wikicrawler/wikipage.h>

const std::string bannedStrings[] = {":", "%", "Terms_of_Use", "Privacy_policy", "Cookie_statement", "Main_Page"};

// TODO: the url of a page can change after a redirect - for example, wiki/Dishware becomes wiki/Tableware - this needs to be
// able to handle that by following redirects and adding an alias for the url

// gets page text from a url
std::string fetchPageText(std::string url) {
    cpr::SslOptions sslOpts = cpr::Ssl(cpr::ssl::CaPath{"/etc/ssl/certs"}); //! this shouldn't be necessary and is Linux specific (bad)
    cpr::Response r = cpr::Get(cpr::Url{url}, sslOpts);

    if (r.error.message != "") {
        throw std::runtime_error("error fetching wiki page: " + r.error.message);
    }

    return r.text;
}

// gets all references to other Wikipedia pages (anything with "/wiki/" in it)
std::vector<std::string> getWikiRefs(std::string text) {
    std::vector<std::string> refs = std::vector<std::string>();
    size_t i = 0;
    while (i != -1 && i != std::string::npos) {
        i = text.find("/wiki/", i);
        if (i > text.size()) {
            break;
        }
        refs.push_back(text.substr(i, text.find("\"", i) - i));
        i++;
    }
    return refs;
}

// trims strings containing any strings in bannedStrings from the url and returns a new list (old list unmodified)
std::vector<std::string> trimWikiRefs(const std::vector<std::string> refs) {
    std::vector<std::string> trimmedUrls;
    for (int i = 0; i < refs.size(); i++) {
        std::string url = refs[i];
        bool ok = true;
        for (std::string banned : bannedStrings) {
            size_t f = url.find(banned);
            if (f != std::string::npos || f < url.size()) {
                ok = false;
                break;
            }
        }
        if (ok && !(std::count(trimmedUrls.begin(), trimmedUrls.end(), url))) {
            trimmedUrls.push_back(refs[i]);
        }
    }

    return trimmedUrls;
}

// turns Wikipedia page references into real links ("/wiki/foo" becomes "https://en.wikipedia.org/wiki/foo") and returns a new list (old list unmodified)
std::vector<std::string> linkifyWikiRefs(const std::vector<std::string> refs) {
    std::vector<std::string> linkified;
    for (std::string ref : refs) {
        linkified.push_back("https://en.wikipedia.org" + ref);
    }
    return linkified;
}

std::vector<std::string> removeCircularUrls(const std::vector<std::string> urls, std::string originalUrl) {
    std::vector<std::string> removed;
    for (std::string url : urls) {
        if (url != originalUrl) {
            removed.push_back(url);
        }
    }
    return removed;
}

// constructs a new WikiPage from the given url, but does NOT fetch sub-pages
WikiPage::WikiPage(std::string url) {
    this->url = url;
    if (url == "") {
        return;
    }

    std::string pageText = fetchPageText(url);
    if (pageText == "") {
        return; // TODO: error
    }
    links = linkifyWikiRefs(trimWikiRefs(getWikiRefs(pageText)));
    links = removeCircularUrls(links, url);
}

bool WikiPage::linksContains(std::string url) {
    return std::count(links.begin(), links.end(), url);
}

// generates all sub pages for this vector
std::vector<std::shared_ptr<WikiPage>> WikiPage::generateSubPages() {
    std::vector<std::shared_ptr<WikiPage>> pages;
    for (std::string url : links) {
        std::shared_ptr<WikiPage> pagePtr = std::shared_ptr<WikiPage>(new WikiPage(url));
        pagePtr->createRelationship(shared_from_this());
        createRelationship(pagePtr);
        pages.push_back(pagePtr);
    }
    return pages;
}

std::vector<std::shared_ptr<WikiPage>> WikiPage::getRelationships() {
    return relationships;
}

void WikiPage::createRelationship(std::shared_ptr<WikiPage> page) {
    // add relationship to this node
    bool add = true;
    for (std::shared_ptr<WikiPage> p : relationships) {
        if (p->url == page->url) {
            add = false;
        }
    }
    if (add) {
        relationships.push_back(page); // TODO: sort edges somehow?
    }
}

void WikiPage::createRelationships(std::vector<std::shared_ptr<WikiPage>> pages) {
    for (std::shared_ptr<WikiPage> page : pages) {
        createRelationship(page);
    }
}
