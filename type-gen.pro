
TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -save-temps

INCLUDEPATH += \
    include

SOURCES += \
    main.cpp

HEADERS += \
    include/type-gen/type-gen.hpp
