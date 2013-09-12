#ifndef REFACTORTESTER_HPP
#define REFACTORTESTER_HPP

#include <QtTest/QTest>

class RefactorTester : public QObject
{
    Q_OBJECT
public:
    RefactorTester() {}
    virtual ~RefactorTester() {}

private slots:
    void simpleTest();
    void simpleTest2();
};

#endif // REFACTORTESTER_HPP