add_library(common_lib STATIC
  classifier.pb.cc
  computeSVs.cpp
  filenameHelpers.cpp
  filter.cpp
  gabrielGraph.cpp
  isgabrielEdge.cpp
  nearestSVlabel.cpp
  readFiles.cpp
  squaredDistance.cpp
  types.cpp
  writeFiles.cpp
)

target_include_directories(common_lib PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/../../include
)

target_link_libraries(common_lib PUBLIC
  ${Protobuf_LIBRARIES}
)
