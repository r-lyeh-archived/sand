sand <a href="https://travis-ci.org/r-lyeh/sand"><img src="https://api.travis-ci.org/r-lyeh/sand.svg?branch=master" align="right" /></a>
====

- Sand is a lightweight and simple time library written in C++11.
- Sand is a wrapper around Unix stamps, hires timers, calendars and locales.
- Sand is tiny. One header and one source file.
- Sand is cross-platform. Builds on Windows/Linux/MacosX. Compiles on g++/clang/msvc.
- Sand is stand-alone. OS dependencies only. No third party dependencies.
- Sand is BOOST licensed.

sync api
--------
- `wink()`: @todoc
- `sleep(time)`: @todoc
- `now()`: @todoc
- `runtime()`: @todoc
- `lapse(time)`: @todoc
- `lock(HZ)`: @todoc
- `get_fps()`: @todoc

conversion api
--------------
- `nanoseconds(time)`: @todoc
- `microseconds(time)`: @todoc
- `milliseconds(time)`: @todoc
- `seconds(time)`: @todoc
- `minutes(time)`: @todoc
- `hours(time)`: @todoc
- `days(time)`: @todoc
- `weeks(time)`: @todoc
- `years(time)`: @todoc

- `to_nanoseconds(time)`: @todoc
- `to_microseconds(time)`: @todoc
- `to_milliseconds(time)`: @todoc
- `to_seconds(time)`: @todoc
- `to_minutes(time)`: @todoc
- `to_hours(time)`: @todoc
- `to_days(time)`: @todoc
- `to_weeks(time)`: @todoc
- `to_years(time)`: @todoc

- `calendar(time)`: @todoc

extraction api
--------------
- `seconds(time)`: @todoc
- `minutes(time)`: @todoc
- `hours(time)`: @todoc
- `days(time)`: @todoc
- `weeks(time)`: @todoc
- `years(time)`: @todoc

pretty printing
---------------
- `pretty(time)`: @todoc
- `ago(time)`: @todoc
- `in(time)`: @todoc

format
------
- `format(time,string format[,string locale])`: @todoc
- `locale(time,string locale[,string format])`: @todoc

serialization
-------------
- `str(string)`: @todoc
- `str(time)`: @todoc

classes
-------
- `dt()`: @todoc
- `chrono()`: @todoc
- `looper()`: @todoc

sample
------
```c++
```

possible output
---------------
```
```

special notes
-------------
- g++ users: both `-std=c++11` and `-lrt` may be required when compiling `sand.cpp`
