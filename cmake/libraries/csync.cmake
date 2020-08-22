file(GLOB_RECURSE CSYNC_SOURCES
    ./include/*.h
    ./src/*.c
)

add_library(csync SHARED ${CSYNC_SOURCES})
target_link_libraries(csync pthread)

add_executable(csync-test-c ./tests/csync_test.c)
target_link_libraries(csync-test-c cmocka csync pthread)
add_test(NAME CsyncTestC COMMAND csync-test-c)
