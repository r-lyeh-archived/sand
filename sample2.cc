#include "sand.hpp"

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#pragma comment(lib, "user32.lib")
#endif

#include <algorithm>
#include <time.h>

unsigned HZ = 60, updates = 0, frames = 0;

bool update() {
    // your logic here
    updates++;
    return sand::lock(HZ);
}
void render() {
    // your render here
    frames++;
}

int main() {

    for(;;) {
        bool did_render = 0;

        if( update() ) {
            did_render = 1;
            render();
        }

#ifdef _WIN32
        if( GetAsyncKeyState(VK_UP) & 0x8000 )  HZ+=std::min(std::max(int(HZ*0.01), 1), 5);
        if( GetAsyncKeyState(VK_DOWN) & 0x8000 ) if(HZ > std::max(int(HZ*0.01), 1)) HZ-=std::max(int(HZ*0.01), 1);
        if( GetAsyncKeyState(VK_ESCAPE) & 0x8000 ) break;
        if( GetAsyncKeyState(VK_SPACE) & 0x8000 ) Sleep( rand() % 80 );
#endif

        char bar[] = "\\|/-";
        auto current_time = time(NULL);
        auto sec = localtime(&current_time)->tm_sec;

        printf( "(%d/%d) [x] update [%c] render %c %02d %c \r",
            sand::get_fps(), HZ, did_render ? 'x' : ' ', bar[updates%4], sec, bar[frames%4] );
    }
}
