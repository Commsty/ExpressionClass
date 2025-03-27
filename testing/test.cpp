#include "../include/Expression.hpp"
#include <vector>
#include <iomanip>
#include <iostream>
#include <cmath>

namespace
{
    struct Test
    {
        std::string TestName;
        bool verdict;
    };

    void PrintResults(const std::vector<Test> &Tests)
    {
        size_t MaxLen_TestName = 0;
        for (const auto &test : Tests)
            MaxLen_TestName = std::max(MaxLen_TestName, test.TestName.length());

        std::cout
            << std::left << std::setw((int)MaxLen_TestName) << "Test Name" << "  Verdict";
        std::cout << std::setfill('-') << std::setw((int)MaxLen_TestName + 10) << "\n"
                  << std::setfill(' ');
        std::cout << std::endl;

        for (const auto &test : Tests)
        {
            std::cout << std::left << std::setw((int)MaxLen_TestName) << test.TestName << "  "
                      << (test.verdict ? "\033[32mOK\033[0m" : "\033[31mFAIL\033[0m") << "\n";
        }
    }
};

bool testThatEvalWorks1()
{
    // given
    Expression a("45+sin(x)*y");
    const std::map<std::string, long double> vars{{"x", 3}, {"y", 0.5}};

    // when
    long double res = a.evaluate(&vars);

    // then
    long double expected = 45.07056;
    return fabsl(expected - res) < 0.0001;
}

bool testThatEvalWorks2()
{
    // given
    Expression a("sin(cos(sin(exp(x^3))))");
    const std::map<std::string, long double> vars{{"x", 1.45}};

    // when
    long double res = a.evaluate(&vars);

    // then
    long double expected = 0.649145;
    return fabsl(expected - res) < 0.0001;
}

bool testThatEvalWorks3()
{
    // given
    Expression a("ln(x*y^3)");
    const std::map<std::string, long double> vars{{"x", 1000.1}, {"y", 23.45}};

    // when
    long double res = a.evaluate(&vars);

    // then
    long double expected = 16.37247;
    return fabsl(expected - res) < 0.0001;
}

bool testThatEvalWorks4()
{
    // given
    Expression a("exp(x-y^3)");
    const std::map<std::string, long double> vars{{"x", 1}, {"y", 0.453}};

    // when
    long double res = a.evaluate(&vars);

    // then
    long double expected = 2.47698;
    return fabsl(expected - res) < 0.0001;
}

bool testThatDiffWorks1()
{
    // given
    Expression a("cos(x)-sin(x)");

    // when
    Expression res = a.differentiate("x");

    // then
    std::string expected = "((-1)*sin(x))-cos(x)";
    return res.getString() == expected;
}

bool testThatDiffWorks2()
{
    // given
    Expression a("y*cos(sin(cos(x)))");

    // when
    Expression res = a.differentiate("x");

    // then
    std::string expected = "y*((-1)*sin(sin(cos(x)))*(cos(cos(x))*((-1)*sin(x))))";
    return res.getString() == expected;
}

bool testThatDiffWorks3()
{
    // given
    Expression a("ln(1/x)*exp(y*x)");

    // when
    Expression res = a.differentiate("x");

    // then
    std::string expected = "(((-1)/x^2)/(1/x))*exp(y*x)+ln(1/x)*(exp(y*x)*y)";
    return res.getString() == expected;
}

bool testThatDiffWorks4()
{
    // given
    Expression a("x^5+x^4+5*x^3-1");

    // when
    Expression res = a.differentiate("x");

    // then
    std::string expected = "5*x^(4)+4*x^(3)+5*3*x^(2)";
    return res.getString() == expected;
}

int main()
{
    std::vector<Test> MyTests;
    //
    if (testThatEvalWorks1())
        MyTests.push_back({"testThatEvalWorks1", true});
    else
        MyTests.push_back({"testThatEvalWorks1", false});
    //
    if (testThatEvalWorks2())
        MyTests.push_back({"testThatEvalWorks2", true});
    else
        MyTests.push_back({"testThatEvalWorks2", false});
    //
    if (testThatEvalWorks3())
        MyTests.push_back({"testThatEvalWorks3", true});
    else
        MyTests.push_back({"testThatEvalWorks3", false});
    //
    if (testThatEvalWorks4())
        MyTests.push_back({"testThatEvalWorks4", true});
    else
        MyTests.push_back({"testThatEvalWorks4", false});
    //
    if (testThatDiffWorks1())
        MyTests.push_back({"testThatDiffWorks1", true});
    else
        MyTests.push_back({"testThatDiffWorks1", false});
    //
    if (testThatDiffWorks2())
        MyTests.push_back({"testThatDiffWorks2", true});
    else
        MyTests.push_back({"testThatDiffWorks2", false});
    //
    if (testThatDiffWorks3())
        MyTests.push_back({"testThatDiffWorks3", true});
    else
        MyTests.push_back({"testThatDiffWorks3", false});
    //
    if (testThatDiffWorks4())
        MyTests.push_back({"testThatDiffWorks4", true});
    else
        MyTests.push_back({"testThatDiffWorks4", false});

    PrintResults(MyTests);

    return 0;
}