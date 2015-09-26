#include <iostream>
#include <iomanip>
#include <cassert>
#include "sand.hpp"

int main() {
    using namespace sand;

    sand::timer timer;

    std::cout << format( utc(), "yyyy-mm-ddTHH:MM:MS" ) << std::endl;
    std::cout << format( now(), "d/mmmm/yy HH:MM:SS.MS" ) << std::endl;
    std::cout << format( uptime(), "HH:MM:SS.MS" ) << std::endl;

    assert( pretty( seconds(- 0) ) == "right now" );
    assert( pretty( seconds(- 1) ) == "a second ago" );
    assert( pretty( seconds(- 2) ) == "2 seconds ago" );
    assert( pretty( seconds(-60) ) == "a minute ago" );
    assert( pretty( seconds(-61) ) == "a minute ago" );
    assert( pretty( minutes(- 1) ) == "a minute ago" );
    assert( pretty( minutes(-60) ) == "an hour ago" );
    assert( pretty( minutes(-61) ) == "an hour ago" );
    assert( pretty(   hours(- 1) ) == "an hour ago" );
    assert( pretty(   hours(-24) ) == "yesterday" );
    assert( pretty(   hours(-25) ) == "yesterday" );
    assert( pretty(   days( -1) ) == "yesterday" );
    assert( pretty(   days( -5) ) == "5 days ago" );
    assert( pretty(   days( -7) ) == "7 days ago" );
    assert( pretty(   days(-12) ) == "12 days ago" );
    assert( pretty(   days(-13) ) == "13 days ago" );
    assert( pretty(   days(-14) ) == "2 weeks ago" );
    assert( pretty(   days(-32) ) == "a month ago" );
    assert( pretty(  weeks(-52) ) == "11 months ago" );
    assert( pretty(  weeks(-53) ) == "a year ago" );

    assert( pretty( seconds(+ 0) ) == "right now" );
    assert( pretty( seconds(+ 1) ) == "a second from now" );
    assert( pretty( seconds(+ 2) ) == "2 seconds from now" );
    assert( pretty( seconds(+60) ) == "a minute from now" );
    assert( pretty( seconds(+61) ) == "a minute from now" );
    assert( pretty( minutes(+ 1) ) == "a minute from now" );
    assert( pretty( minutes(+60) ) == "an hour from now" );
    assert( pretty( minutes(+61) ) == "an hour from now" );
    assert( pretty(   hours(+ 1) ) == "an hour from now" );
    assert( pretty(   hours(+24) ) == "tomorrow" );
    assert( pretty(   hours(+25) ) == "tomorrow" );
    assert( pretty(   days( +1) ) == "tomorrow" );
    assert( pretty(   days( +5) ) == "5 days from now" );
    assert( pretty(   days( +7) ) == "7 days from now" );
    assert( pretty(   days(+12) ) == "12 days from now" );
    assert( pretty(   days(+13) ) == "13 days from now" );
    assert( pretty(   days(+14) ) == "2 weeks from now" );
    assert( pretty(   days(+32) ) == "a month from now" );
    assert( pretty(  weeks(+52) ) == "11 months from now" );
    assert( pretty(  weeks(+53) ) == "a year from now" );

    std::cout << pretty( str("2000-01-01 00:00:00") - now() ) << std::endl;

    {
        auto rtc = now();
        auto then = str( "2010-12-31 23:59:59" );
        std::cout << str(then) << std::endl;

        auto then2 = date(2010,12,31) + time(23,59,59,00);
        std::cout << str(then2) << std::endl;
        assert( str(then) == str(then2) );

        assert( year(then) == 2010 );
        assert( month(then) == 12 );
        assert( day(then) == 31 );
        assert( hour(then) == 23 );
        assert( minute(then) == 59 );
        assert( second(then) == 59 );
        assert( millisecond(then) == 0 );

        std::cout << "[ ] print ";
            std::cout << "- rtc : " << std::setprecision(20) << rtc << " -> " << str(rtc) << " -> " << pretty(now() - rtc);
            std::cout << "\r[x]" << std::endl;

        std::cout << "[ ] serialization ";
            std::cout << "- rtc : " << str(rtc);
            std::cout << "\r[x]" << std::endl;

        std::cout << "[ ] print ";
            std::cout << "- then : " << std::setprecision(20) << then << " -> " << str(then) << " -> " << pretty(then - now());
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
            auto winter = str( "2010-12-31T23:59:59.003Z" );
            auto autumn = str( "2001-10-10 17:00:00" );
            assert( str(winter) == "2010-12-31 23:59:59.003" );
            assert( str(autumn) == "2001-10-10 17:00:00.000" );
            std::cout << "\r[x]" << std::endl;
    }


    {
        auto
            past = str( "1972-06-10 17:00:00" ),
            present = now(),
            future = str( "2091-12-31 23:59:55" );

        std::cout << str(past) << std::endl;

        assert( str(past) == "1972-06-10 17:00:00.000" );
      //assert( str(present) == "your current time and date" );
        assert( str(future) == "2091-12-31 23:59:55.000" );

        past += seconds(1);
        present += seconds(1);
        future += seconds(6);

        assert( str(past) == "1972-06-10 17:00:01.000" );
      //assert( str(present) == "your current time and date plus one second" );
        assert( str(future) == "2092-01-01 00:00:01.000" );
    }

    sand::chrono total(4);
    sand::looper looper(0.5);
    while( total.t() < 1 ) {
        std::cout << "looper: " << looper.t() << std::endl;
        sand::sleep(sand::seconds(1)/4);
    }

    std::cout << "---" << std::endl;
    std::cout << "All ok, " << timer.ms() << "ms " << std::endl;
}
