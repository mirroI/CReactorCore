set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)

set(SOURCE
        reactor_core_global.h publisher.h ../test/main.cpp mono.h mono_sink.h subscriber.h lambda_mono_subscriber.h operators.h optimizable_operator.h mono_create.h mono_subscribe_on.h mono_operator.h mono_map.h map_subscriber.h)

add_library(${PROJECT_NAME} SHARED ${SOURCE})
#add_library(${PROJECT_NAME} ${SOURCES})
add_library(Cifrazia::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

if (NOT BUILD_SHARED_LIBS)
    target_compile_definitions(${PROJECT_NAME} PUBLIC
            QTLOTTIE_STATICs
            )
endif ()

if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /utf-8)
    if (NOT (CMAKE_BUILD_TYPE STREQUAL "Debug"))
        target_compile_options(${PROJECT_NAME} PRIVATE /guard:cf)
        target_link_options(${PROJECT_NAME} PRIVATE /GUARD:CF)
    endif ()
endif ()

target_link_libraries(${PROJECT_NAME} PUBLIC Qt${QT_VERSION_MAJOR}::Core)

target_compile_definitions(${PROJECT_NAME} PRIVATE
        #        QT_NO_CAST_FROM_ASCII
        #        QT_NO_CAST_TO_ASCII
        #        QT_NO_KEYWORDS
        #        QT_DEPRECATED_WARNINGS
        #        QT_DISABLE_DEPRECATED_BEFORE=0x060100
        REACTOR_CORE_LIBRARY
        )

target_include_directories(${PROJECT_NAME} PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>"
        )
