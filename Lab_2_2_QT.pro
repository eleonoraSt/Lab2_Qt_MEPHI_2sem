TEMPLATE = app
TARGET = name_of_the_app

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    src/BitSequence.cpp \
    src/Window.cpp \
    src/main.cpp \
    tests.cpp

HEADERS += include/errors.h include/Node.h include/LinkedList.h include/DynamicArray.h \
    include/BitSequence.h \
    include/ImmutableListSequence.h \
    include/ListSequence.h \
    include/Sequence.h include/ListSequence.h include/ArraySequence.h \
    include/ImmutableListSequence.h include/ImmutableArraySequence.h \
    include/AdaptiveSequence.h \
    include/Window.h