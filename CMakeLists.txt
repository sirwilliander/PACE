INCLUDE_DIRECTORIES(
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_BINARY_DIR}
)

LINK_DIRECTORIES(
  ${PROJECT_BINARY_DIR}/lemon
)

# Uncomment (and adjust) the following two lines. 'myprog' is the name
# of the final executable ('.exe' will automatically be added to the
# name on Windows) and 'myprog-main.cc' is the source code it is
# compiled from. You can add more source files separated by
# whitespaces. Moreover, you can add multiple similar blocks if you
# want to build more than one executables.

ADD_EXECUTABLE(elso_program elso_program.cc)
ADD_EXECUTABLE(base_class PACE/base_class.cc)
ADD_EXECUTABLE(exampleDraw PACE/exampleDraw.cc)
ADD_EXECUTABLE(feedback_set PACE/feedback_set.cc)

ADD_LIBRARY(dfs_library PACE/base_graph.cc PACE/print_template.cc)

TARGET_LINK_LIBRARIES(elso_program lemon)
TARGET_LINK_LIBRARIES(exampleDraw lemon)
TARGET_LINK_LIBRARIES(feedback_set dfs_library)

