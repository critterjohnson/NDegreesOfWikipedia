#include <iostream>

#include <cpr/cpr.h>
#include <wikicrawler/wikipage.h>

int main() {
    WikiPage wikipage = WikiPage("https://en.wikipedia.org/wiki/Getabako");
    for (std::string url : wikipage.links) {
        std::cout << url << std::endl;
    }

    WikiPage newPage = wikipage.populatePages();
    for (WikiPage page : newPage.pages) {
        for (std::string url : page.links) {
            std::cout << url << std::endl;
        }
    }
}
