add_executable(CompilerProfiler
  cpp/main.cpp
  cpp/utils/CompileEvent.h
  cpp/utils/CompileEvent.cpp
  cpp/json/JsonParser.h
  cpp/json/JsonParser.cpp

  cpp/exceptions/exceptions.h
  qml/qml.qrc
)

target_compile_definitions(CompilerProfiler
PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(CompilerProfiler
PRIVATE Qt5::Core Qt5::Quick LTL)
