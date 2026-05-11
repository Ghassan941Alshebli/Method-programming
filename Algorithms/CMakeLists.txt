find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_library(Algorithms
    vigenere.cpp
    vigenere.h
    md5.cpp
    md5.h
      secant.cpp
      secant.h
      graph.cpp
      graph.h
)

target_link_libraries(Algorithms
    Qt${QT_VERSION_MAJOR}::Core
)