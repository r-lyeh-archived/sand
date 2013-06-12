#include <iostream>
#include <iomanip>
#include <cassert>

#include "sand.hpp"

int main()
{
    using namespace sand;

    double then = now();
    std::cout << to_nanoseconds(now() - then) << std::endl;

    std::cout << runtime() << std::endl;

    std::cout << locale( now(), "us-EN" ) << std::endl;
    std::cout << locale( now(), "es-ES" ) << std::endl;
    std::cout << locale( now() + years(10), "es-ES" ) << std::endl;
    std::cout << locale( now() - years(10), "es-ES" ) << std::endl;

    std::cout << format( now(), "%Y-%m-%d %H:%M:%S" ) << std::endl;
    std::cout << format( now() + years(10), "%Y-%m-%d %H:%M:%S" ) << std::endl;

    assert( ago( seconds( 0) ) == "just now" );
    assert( ago( seconds( 1) ) == "just now" );
    assert( ago( seconds(60) ) == "a minute ago" );
    assert( ago( seconds(61) ) == "a minute ago" );
    assert( ago( minutes( 1) ) == "a minute ago" );
    assert( ago( minutes(60) ) == "an hour ago" );
    assert( ago( minutes(61) ) == "an hour ago" );
    assert( ago(   hours( 1) ) == "an hour ago" );
    assert( ago(   hours(24) ) == "yesterday" );
    assert( ago(   hours(25) ) == "yesterday" );
    assert( ago(   days( 1) ) == "yesterday" );
    assert( ago(   days( 5) ) == "5 days ago" );
    assert( ago(   days( 7) ) == "7 days ago" );
    assert( ago(   days(12) ) == "12 days ago" );
    assert( ago(   days(13) ) == "13 days ago" );
    assert( ago(   days(14) ) == "2 weeks ago" );
    assert( ago(   days(32) ) == "a month ago" );
    assert( ago(  weeks(52) ) == "11 months ago" );
    assert( ago(  weeks(53) ) == "a year ago" );

    assert( pretty( days(-1) ) == "yesterday" );
    assert( pretty( days(+1) ) == "tomorrow" );

    std::cout << diff( now(), now() + days(1) ) << std::endl;
    std::cout << diff( now(), now() - days(1) ) << std::endl;
    std::cout << diff( now(), now() + days(-1) ) << std::endl;
    std::cout << diff( now(), now() - days(-1) ) << std::endl;

    assert( dt().ns() > 0 );

    std::cout << diff( calendar("1970-01-01 00:00:00"), now() ) << std::endl;

    {
        auto
            past = calendar( "1972-06-10 17:00:00" ),
            present = now(),
            future = calendar( "2091-12-31 23:59:55" );

        assert( str(past) == "1972-06-10 17:00:00" );
      //assert( str(present) == "your current time and date" );
        assert( str(future) == "2091-12-31 23:59:55" );

        past ++;
        present ++;
        future += 6.0;

        assert( str(past) == "1972-06-10 17:00:01" );
      //assert( str(present) == "your current time and date plus one second" );
        assert( str(future) == "2092-01-01 00:00:01" );
    }

    {
        double rtc = now();
        double then = calendar( "2010-12-31 23:59:59" );
#if 0
        double then2 = years(2010) + months(12) + days(31) + hours(23) + minutes(59) + seconds(59);

        std::cout << years(2010) << std::endl;
        std::cout << months(12) << std::endl;
        std::cout << days(31) << std::endl;
        std::cout << hours(23) << std::endl;
        std::cout << minutes(59) << std::endl;
        std::cout << seconds(59) << std::endl;

        std::cout << std::setprecision(20) << then << " vs " << then2 << std::endl;
        std::cout << str(then) << " vs " << str(then2) << std::endl;
        assert( str(then) == str(then2) );
#endif

        assert( year(then) == 2010 );
        assert( month(then) == 12 );
        assert( day(then) == 31 );
        assert( hour(then) == 23 );
        assert( minute(then) == 59 );
        assert( second(then) == 59 );

        std::cout << "[ ] print ";
            std::cout << "- rtc : " << std::setprecision(20) << rtc << " -> " << str(rtc) << " -> " << diff(now(), rtc);
            std::cout << "\r[x]" << std::endl;

        std::cout << "[ ] serialization ";
            std::cout << "- rtc : " << str(rtc);
            std::cout << "\r[x]" << std::endl;

        std::cout << "[ ] print ";
            std::cout << "- then : " << std::setprecision(20) << then << " -> " << str(then) << " -> " << diff(then, now());
            std::cout << "\r[x]" << std::endl;

        std::cout << "[ ] serialization ";
            std::cout << "- then : " << str(then);
            std::cout << "\r[x]" << std::endl;

        std::cout << "[ ] logical/arithmetical tests";
            rtc ++;
            then ++;
            assert( rtc == rtc );
            assert( rtc != then );
            assert( rtc + 1 > rtc );
            std::cout << "\r[x]" << std::endl;

        std::cout << "[ ] daylight savings tests";
            double winter = calendar( "2010-12-31 23:59:59" );
            double autumn = calendar( "1991-10-10 17:00:00" );
            assert( str(winter) == "2010-12-31 23:59:59" );
            assert( str(autumn) == "1991-10-10 17:00:00" );
            std::cout << "\r[x]" << std::endl;
    }

    {
        dt dt;
        sleep( seconds(1.5) );
        std::cout << dt.s() << ',' << dt.ms() << ',' << dt.us() << ',' << dt.ns() << std::endl;
        double t = dt.s();
        std::cout << to_seconds(t) << ',' << to_milliseconds(t) << ',' << to_microseconds(t) << ',' << to_nanoseconds(t) << std::endl;
    }

    return 0;
}
