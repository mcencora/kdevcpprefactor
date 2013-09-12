cmake_minimum_required(VERSION 2.8)
INCLUDE( UseQt4 )
set( TEST_LIBRARIES ${QT_QTTEST_LIBRARY} ${QT_LIBRARIES} )
set( TESTS_SRC refactorTester.cpp )
QT4_AUTOMOC( ${TESTS_SRC} )
ADD_EXECUTABLE( refactorTester ${TESTS_SRC} )
TARGET_LINK_LIBRARIES( refactorTester ${TEST_LIBRARIES} )
#ADD_TEST( NAME refactorTester COMMAND RefactorTester ) // is it required?