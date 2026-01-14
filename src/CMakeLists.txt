set(SOURCES
  main.cpp
  Backend.cpp
  Backend.h
  Settings.cpp
  Settings.h
  Loader.cpp
  Loader.h
  Translator.cpp
  Translator.h
)

qt_add_executable(qt_quick_app
  ${SOURCES}
  qml.qrc
)

target_link_libraries(qt_quick_app PRIVATE Qt6::Quick Qt6::QuickControls2)

find_package(yaml-cpp REQUIRED)
target_link_libraries(qt_quick_app PRIVATE yaml-cpp::yaml-cpp)

if (WIN32)
  set_target_properties(qt_quick_app PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
