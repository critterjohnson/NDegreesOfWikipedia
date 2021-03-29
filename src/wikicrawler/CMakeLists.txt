file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${NDegreesOfWikipedia_SOURCE_DIR}/include/wikicrawler/*.h")

add_library(wikicrawler wikipage.cpp ../../include/wikicrawler/wikipage.h)# ${HEADER_LIST})

target_include_directories(wikicrawler PUBLIC ../../include)

target_link_libraries(wikicrawler cpr::cpr)
