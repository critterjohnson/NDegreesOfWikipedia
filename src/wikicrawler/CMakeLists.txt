file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${NDegreesOfWikipedia_SOURCE_DIR}/include/wikicrawler/*.h")
file(GLOB CPP_LIST CONFIGURE_DEPENDS "${NDegreesOfWikipedia_SOURCE_DIR}/src/wikicrawler/*.cpp")

add_library(wikicrawler ${CPP_LIST} ${HEADER_LIST})

target_include_directories(wikicrawler PUBLIC ../../include)

target_link_libraries(wikicrawler cpr::cpr)
