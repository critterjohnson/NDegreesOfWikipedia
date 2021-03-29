#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include <cpr/cpr.h>
#include <wikicrawler/wikipage.h>

const std::string bannedStrings[] = {":", "%", "Terms_of_Use", "Privacy_policy", "Cookie_statement", "Main_Page"};

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
        std::string url = refs.at(i);

        bool ok = true;
        for (std::string banned : bannedStrings) {
            size_t f = url.find(banned);
            if (f != std::string::npos || f < url.size()) {
                ok = false;
                break;
            }
        }
        if (ok && !(std::count(trimmedUrls.begin(), trimmedUrls.end(), url))) {
            trimmedUrls.push_back(refs.at(i));
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

WikiPage::WikiPage(std::string url) {
    this->url = url;

    std::string pageText = fetchPageText(url);
    links = linkifyWikiRefs(trimWikiRefs(getWikiRefs(pageText)));
    links = removeCircularUrls(links, url);
}

WikiPage WikiPage::populatePages() {
    WikiPage newPage = *this;
    newPage.pages = std::vector<WikiPage>();
    for (std::string url : newPage.links) {
        newPage.pages.push_back(WikiPage(url));
    }
    return newPage;
}
