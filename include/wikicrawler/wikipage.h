#ifndef WIKIPAGE_H
#define WIKIPAGE_H

#include <memory>
#include <string>
#include <vector>

class WikiPage : public std::enable_shared_from_this<WikiPage> {
    public:
        std::string url;
        std::vector<std::string> links;

        WikiPage() = default;
        WikiPage(std::string url);

        bool linksContains(std::string url);
        std::vector<std::shared_ptr<WikiPage>> generateSubPages();
        std::vector<std::shared_ptr<WikiPage>> getRelationships();

        // creates a 1 way relationship between this node and another
        // TODO: really should not be part of public-facing API
        void createRelationship(std::shared_ptr<WikiPage> page);
        void createRelationships(std::vector<std::shared_ptr<WikiPage>> pages);

        bool operator==(const WikiPage& rhs) {
            return url == rhs.url;
        }

    private:
        std::vector<std::shared_ptr<WikiPage>> relationships;
};

#endif