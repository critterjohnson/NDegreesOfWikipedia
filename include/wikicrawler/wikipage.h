#ifndef WIKIPAGE_H
#define WIKIPAGE_H

#include <vector>

class WikiPage {
    public:
        std::string url;
        std::vector<std::string> links;
        std::vector<WikiPage> pages;

        WikiPage(std::string url);
        WikiPage(const WikiPage& ref) {
            url = ref.url;
            links = ref.links;
            pages = ref.pages;
        } // copy constructor

        WikiPage& operator=(const WikiPage& ref) { *this = ref; }

        WikiPage populatePages();
};

#endif