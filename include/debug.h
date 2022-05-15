#ifndef __IMMGRAPHICS_HEADER_DEBUG__
#define __IMMGRAPHICS_HEADER_DEBUG__

#ifdef _DEBUG

    #include <iostream>
    #define DEBUG_Print(msg) \
        std::cout << "DEBUG-Print(" << __FILE__ << ", " << __LINE__ <<  "): " << msg << std::endl

    #define _CASE(origin, ...) \
        std::cout << "\tTest-Case-" << _case_id << "(" << __LINE__ << ") : origin: " << #origin << " params: (" << ""#__VA_ARGS__ << ") : ";
    #define _CASE_STATIC(...) \
        std::cout << "\tTest-Case-" << _case_id << "(" << __LINE__ << ") : " << " params: (" << ""#__VA_ARGS__ << ") : ";
    #define _JUDGE(exp) \
        if(exp) { std::clog << "pass" << std::endl; } else { ++_err_num; std::clog << "ERROR" << std::endl; } ++_case_id;
    
    #include <string>
    #include <chrono>

    #define BEGIN_TIMER(title) \
        { \
            std::chrono::steady_clock::time_point t1; \
            std::chrono::steady_clock::time_point t2; \
            t1 = std::chrono::steady_clock::now(); \
            std::string _title = #title;

    #define END_TIMER() \
        t2 = std::chrono::steady_clock::now(); \
        std::clog << _title << " take " << std::chrono::duration<double, std::milli>(t2 - t1).count() << " ms" << std::endl; }
    
    #define BEGIN_TEST(name, func) \
        { \
        name _origin; int _case_id = 1; int _err_num = 0; \
        std::cout << "Begin test for " << #name << "::" << #func << "() :" << std::endl;
    
    #define END_TEST(name, func) \
        std::cout << "----" << #name << "::" << #func << " tested end : " << _err_num << " errors ( " << _case_id << " cases )\n" << std::endl; \
        }
    
    #define CASE_RVALUE(origin, result, func, ...) \
        _origin = origin; \
        _CASE(origin, __VA_ARGS__) \
        _JUDGE (_origin.func(__VA_ARGS__) == result)
    
    #define CASE_LVALUE(origin, result, func, ...) \
        _origin = origin; _origin.func(__VA_ARGS__); \
        _CASE(origin, __VA_ARGS__) \
        _JUDGE(_origin == result)
    
    #define CASE_STATIC(result, func, ...) \
        _CASE_STATIC(__VA_ARGS__) \
        _JUDGE(func(__VA_ARGS__) == result)


    
#else
    #define DEBUG_Print(msg)

    #define BEGIN_TIMER()

    #define BEGIN_TEST(name, func)
    #define END_TEST(name, func)
    #define CASE_RVALUE(origin, result, func, ...)
    #define CASE_LVALUE(origin, result, func, ...)
    #define CASE_STATIC(result, func, ...)

#endif

namespace ImmGraphics
{

    extern int CalculateFPS();
    extern float getDeltaTime();

    extern void WaitForSeconds(float seconds);
    
} // namespace ImmGraphics

#endif // !__IMMGRAPHICS_HEADER_DEBUG__
