#include <iostream>
#include <iomanip>
#include <cassert>

#include "sand.hpp"


int main()
{
    using namespace sand;

    double then = now();
    std::cout << to_nanoseconds(now() - then) << std::endl;

    std::cout << ago( seconds(0) ) << ',';
    std::cout << ago( seconds(1) ) << ',';
    std::cout << ago( seconds(60) ) << ',';
    std::cout << ago( seconds(61) ) << ',';
    std::cout << ago( minutes(1) ) << ',';
    std::cout << ago( minutes(30) ) << ',';
    std::cout << ago( minutes(60) ) << ',';
    std::cout << ago( hours(5) ) << ',';
    std::cout << ago( hours(12) ) << ',';
    std::cout << ago( hours(24) ) << ',';
    std::cout << ago( hours(25) ) << ',';
    std::cout << ago( days(1) ) << ',';
    std::cout << ago( days(12) ) << ',';
    std::cout << ago( days(5) ) << ',';
    std::cout << ago( days(7) ) << ',';
    std::cout << ago( days(13) ) << ',';
    std::cout << ago( days(14) ) << ',';
    std::cout << ago( days(32) ) << ',';
    std::cout << ago( weeks(5) ) << ',';

    for( int i = 0; i <= 32; ++i ) std::cout << ago( days(i) ) << ',';
    for( int i = 0; i <= 53; ++i ) std::cout << ago( weeks(i) ) << ',';

    std::cout << std::endl;

    std::cout << runtime() << std::endl;

    std::cout << format( now(), "us-EN" ) << std::endl;
    std::cout << format( now(), "es-ES" ) << std::endl;
    std::cout << format( now() + years(10), "es-ES" ) << std::endl;
    std::cout << format( now() - years(10), "es-ES" ) << std::endl;

    {
        sand::rtc
            past( "1972-06-10 17:00:00" ),
            present,
            future( "2091-12-31 23:59:50" );

        // we want future to lapse at double speed
        future.shift( 2.0 );

        // eleven seconds
        for( int i = 0; i <= 10; ++i )
        {
               past.update();
            present.update();
             future.update();

            std::cout
                << "past: " << past.str() << " | "
                << "present: " << present.str() << " | "
                << "future: " << future.str()
                << std::endl;

            // sleep for one second
            sand::sleep( 1.0 );
        }
    }

    {
        std::cout << "- sand::rtc is a realtime clock class" << std::endl;
        std::cout << "- treat classes as doubles, measuring in seconds. update by using .update() method" << std::endl;
        std::cout << "- human readable/serialization by using .str() method" << std::endl;
        std::cout << "- check header for other methods" << std::endl;
        std::cout << std::endl;

        sand::rtc rtc;
        sand::rtc rtc_date( "2010-12-31 23:59:59" );

        std::cout << "* after creation (as double)" << std::endl;

            std::cout << "- rtc : " << std::setprecision(20) <<  rtc << std::endl;
            std::cout << "- rtc date : " << std::setprecision(20) << rtc_date << std::endl;

            std::cout << std::endl;

        std::cout << "* after creation (as string)" << std::endl;

            std::cout << "- rtc : " << rtc.str() << std::endl;
            std::cout << "- rtc date : " << rtc_date.str() << std::endl;

            std::cout << std::endl;

        std::cout << "* after freezing app for 1 second" << std::endl;

            sand::sleep(1);

            std::cout << "- rtc : " << rtc.str() << std::endl;
            std::cout << "- rtc date : " << rtc_date.str() << std::endl;

            std::cout << std::endl;

        std::cout << "* after updating (as double)" << std::endl;

            rtc.update();
            rtc_date.update();

            std::cout << "- rtc : " << std::setprecision(20) << rtc << std::endl;
            std::cout << "- rtc date : " << std::setprecision(20) << rtc_date << std::endl;

            std::cout << std::endl;

        std::cout << "* after updating (as string)" << std::endl;

            std::cout << "- rtc : " << rtc.str() << std::endl;
            std::cout << "- rtc date : " << rtc_date.str() << std::endl;

            std::cout << std::endl;

        std::cout << "* logical/arithmetical tests:" << std::endl;

            std::cout << "test #1 rtc       == rtc   : " << ( rtc       == rtc      ? "ok" : "FAILED" ) << std::endl;
            std::cout << "test #2 rtc       != timer : " << ( rtc       != rtc_date ? "ok" : "FAILED" ) << std::endl;
            std::cout << "test #3 timer + 1 >  timer : " << ( rtc + 1   >  rtc      ? "ok" : "FAILED" ) << std::endl;

            std::cout << std::endl;

        std::cout << "* daylight savings tests:" << std::endl;

            sand::rtc winter( "2010-12-31 23:59:59" );
            sand::rtc autumn( "1991-10-10 17:00:00" );

            std::cout << winter.str() << " vs " << "2010-12-31 23:59:59" << std::endl;
            std::cout << autumn.str() << " vs " << "1991-10-10 17:00:00" << std::endl;

            assert( winter.str() == "2010-12-31 23:59:59" );
            assert( autumn.str() == "1991-10-10 17:00:00" );
    }

    std::cout << runtime() << std::endl;

    {
        dt dt;
        sleep( seconds(1.5) );
        std::cout << dt.s() << ',' << dt.ns() << ',' << dt.us() << std::endl;
    }

    assert( dt().ns() > 0 );

    return 0;
}
