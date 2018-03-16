cmake_minimum_required(VERSION 3.0)
project(radiom_library)

set(CMAKE_C_STANDARD 11)

include_directories(include)

set(SOURCE_FILES
    foundation/engine.c foundation/fifobuf.c
    whitener/whitener.c whitener/chacha.c
    distribution/uniform.c distribution/normal.c
    distribution/poisson.c distribution/bernoulli.c)
add_library(radiom SHARED ${SOURCE_FILES})
set_target_properties(radiom PROPERTIES VERSION 0.0.1 SOVERSION 0.0.1)
target_link_libraries(radiom m)
target_link_libraries(radiom rtlsdr)
install(TARGETS radiom LIBRARY DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)
