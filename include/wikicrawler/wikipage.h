#ifndef WIKIPAGE_H
#define WIKIPAGE_H

#include <vector>
#include <string>

class WikiPage {
    public:
        std::string url;
        std::vector<std::string> links;

        WikiPage() = default;
        WikiPage(std::string url);
        WikiPage(const WikiPage& ref) {
            url = ref.url;
            links = ref.links;
        } // copy constructor

        bool linksContains(std::string url);
        std::vector<WikiPage> generateSubPages();
};

#endif