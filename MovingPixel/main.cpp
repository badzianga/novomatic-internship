#include <iostream>
#include <chrono>
#include <cmath>
#if _WIN32
#include <windows.h>
#endif


using namespace std;


class Screen
{
public:
        Screen()
        {
#ifdef _WIN32
                _hConsole= GetStdHandle(STD_OUTPUT_HANDLE);
#endif
                hideCursor();
                saveCursorPosition();
        }


        void clear()
        {
                for(int i = 0; i < height; i++)
                {
                        for(int j = 0; j < width; j++)
                        {
                                _screen[i][j]='x';
                        }
                }
        }


        void draw(char value, float x, float y)
        {
                int xPos = lround(x);
                int yPos = lround(y);
                if(xPos < width && yPos < height)
                {
                        _screen[yPos][xPos]=value;
                }
        }


        void flush()
        {
                restoreCursorPosition();


                for(int i = 0; i < height; i++)
                {
                        for(int j = 0; j < width; j++)
                        {
                                if(_screen[i][j]!='x')
                                {
                                        setRedFont();
                                }
                                else
                                {
                                        setWhiteFont();
                                }
                                cout << _screen[i][j];
                        }
                        cout << "\n";
                }
        }


private:
        void hideCursor()
        {
#ifdef _WIN32
                CONSOLE_CURSOR_INFO cursorInfo; 
                GetConsoleCursorInfo(_hConsole, &cursorInfo);
                cursorInfo.dwSize = 1;
                cursorInfo.bVisible = FALSE;
                SetConsoleCursorInfo(_hConsole, &cursorInfo);
#else
                cout << "\33[?25l";//disable cursor
#endif


        }


        void saveCursorPosition()
        {
#ifdef _WIN32
                CONSOLE_SCREEN_BUFFER_INFO coninfo;
                GetConsoleScreenBufferInfo(_hConsole, &coninfo);
                _prevPos = coninfo.dwCursorPosition;
#else
                cout << "\33[s";
#endif
                
        }


        void restoreCursorPosition()
        {
#ifdef _WIN32
                SetConsoleCursorPosition(_hConsole, _prevPos);
#else
                cout << "\338";
#endif
        }


        void setRedFont()
        {
#ifdef _WIN32
                WORD attributes = FOREGROUND_RED;
                SetConsoleTextAttribute(_hConsole, attributes);
#else
                cout << "\33[31m";//red color
#endif
        }


        void setWhiteFont()
        {
#ifdef _WIN32
                WORD attributes = FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE;
                SetConsoleTextAttribute(_hConsole, attributes);
#else
                cout << "\33[37m";//white color
#endif
        }


        static constexpr int width{60};
        static constexpr int height{20};
        char _screen[height][width];
#ifdef _WIN32
        HANDLE _hConsole;
        COORD _prevPos;
#endif
};


struct Position
{
        float x;
        float y;
};


using Pixel = char;


class Engine
{
public:
        int iterations = 5;  // used in main while loop

        void update(chrono::milliseconds deltaTime)
        {
                timeLeft -= deltaTime.count();
                velocity = distanceLeft / (float)timeLeft;
                distanceLeft -= (float)deltaTime.count() * velocity;
                switch (state)
                {
                case 0:  // A -> B
                        position.x += (float)deltaTime.count() * velocity;
                        if (position.x >= 40 || timeLeft <= 0) {
                                position.x = 40.f;
                                state = 1;
                                timeLeft = 3000;
                                distanceLeft = 10.f;
                        }
                        break;
                case 1:  // B -> C
                        position.y += (float)deltaTime.count() * velocity;
                        if (position.y >= 10 || timeLeft <= 0) {
                                position.y = 10.f;
                                state = 2;
                                timeLeft = 1000;
                                distanceLeft = cToA;
                        }
                        break;
                case 2:  // C -> A
                        position.x -= (float)deltaTime.count() * velocity * cosf(angle);
                        position.y -= (float)deltaTime.count() * velocity * sinf(angle); 
                        if (position.x <= 0 || position.y <= 0 || timeLeft <= 0) {
                                position.x = 0.f;
                                position.y = 0.f;
                                state = 0;
                                timeLeft = 2000;
                                distanceLeft = 40.f;
                                iterations--;
                        }
                        break;
                default:
                        break;
                }
        }


        void render(Screen& screen)
        {
                screen.draw(pixel, position.x, position.y);
        }
private:
        Pixel pixel{1};
        Position position{0.0f, 0.0f};

        int state = 0;
        int timeLeft = 2000;
        float distanceLeft = 40.f;
        float velocity = 0.f;

        const float cToA = sqrtf(40.f * 40.f + 10.f * 10.f);  // distance from C to A
        const float angle = asinf(10.f / distanceLeft);  // movement angle used in C -> A 
};


int main()
{
        Engine engine;
        Screen screen;

        auto startTime = chrono::high_resolution_clock::now();
        while(engine.iterations > 0)
        {
                auto now = chrono::high_resolution_clock::now();
                auto deltaTime = chrono::duration_cast<chrono::milliseconds>(now - startTime);
                if(deltaTime >= 20ms)
                {
                        screen.clear();
                        engine.update(deltaTime);
                        engine.render(screen);
                        screen.flush();
                        startTime = now;
                }
        }

        return 0;
}