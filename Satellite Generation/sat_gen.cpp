//cd Public/space\ game/Satellite\ Generation/
#include <iostream>
#include <cstdint>
#include <string>
#include <time.h>
#include <list>
//yay c++
using namespace std;
string colors[] = {"\033[1;39;49m", "\033[1;37;47m", "\033[1;37;46m", "\033[1;37;45m", "\033[1;37;44m", "\033[1;37;43m", "\033[1;37;42m", "\033[1;37;41m"};
class satalite;
class room
{
    public:
        satalite *parent;
        list<room *> doors;
        int x,y,w,h,roomNum;
        void gen_doors();
        room(int x1, int y1, int w1, int h1, int roomNum1, satalite *p)
        {
            x = x1;
            y = y1;
            w = w1;
            h = h1;
            roomNum = roomNum1;
            parent = p;
        }
};

class satalite
{
    public:
        uint8_t map[26][50];
        list<room> rooms;
        int sat_size;
        void draw_map();
        room *find_room(x,y);
        satalite(int size, int seed)
        {
            srand(seed);
            sat_size = size;
            gen_map();
            for (int i = 0; i < rooms.size(); i++)
            {
                rooms[i].gen_doors();
            }
        }
    private:
        void draw_to_map(int x, int y, int w, int h, int data);
        string color(int num);
        bool check_empty(int x, int y, int w, int h, int data);
        void gen_map();
};
//init vars for the satalite

//



int main(int argc, char *argv[])
{
    if (argc != 2) // check if we have the right amount of aruments
    {
        cout << "please have a argument\n";
        return 0;
    }
    //set seed to time (i wish that there was a better way)
    satalite sat(atoi(argv[1]), time(0));
    sat.draw_map(); //draw map
    room *player_room = sat.find_room(25,12);
    return 0;
}

void satalite::gen_map() //generate satalite
{
    //draw center room of the satalite
    for (int y = 0; y < 25; y++)
        for (int x = 0; x < 50; x++)
            map[y][x] = 0;
    int tempranx = (1 + rand() % 10) / 4 + 2;
    int temprany = (1 + rand() % 10) / 8 + 2;
    draw_to_map(25 - tempranx / 2, 12 - temprany / 2, tempranx, temprany, 1);
    for (int size = 1; size < sat_size && rooms.size() < sat_size; size++)
        for (int i = 2; i < size + 2; i++)
            for (int y = 0; y < 25; y++)
                for (int x = 0; x < 50; x++)
                    if (map[y][x] == i - 1 && rand() % 3 == 0)
                    {
                        tempranx = (size - i) / 3 + rand() % 5;
                        temprany = (size - i) / 3 + rand() % 5;
                        if (map[y + 1][x] == 0)
                        {
                            draw_to_map(x - tempranx / 4, y + 1, fmax(tempranx / 2, 1), fmax(temprany, 1), i);
                        }
                        if (map[y - 1][x] == 0)
                        {
                            draw_to_map(x - tempranx / 4, y - temprany, fmax(tempranx / 2, 1), fmax(temprany, 1), i);
                        }
                        if (map[x + 1][y] == 0)
                        {
                            draw_to_map(x - tempranx, y - temprany / 4, fmax(tempranx, 1), fmax(temprany / 2, 1), i);
                        }
                        if (map[x - 1][y] == 0)
                        {
                            draw_to_map(x + 1, y - temprany / 4, fmax(tempranx, 1), fmax(temprany / 2, 1), i);
                        }
                        if (rooms.size() >= sat_size)
                        {
                            return;
                        }
                    }
    // for (int y = 0; y < 25; y++)
    //     for(int x = 0; x < 50; x++)
    //         if (map[y][x] != 0)
    //             map[y][x] = 1;
}

bool satalite::check_empty(int x, int y, int w, int h, int data)
{
    for (int rx = -1; rx <= w; rx++)
        for (int ry = -1; ry <= h; ry++)
            if (!((rx < 0 || ry < 0 || rx == w || ry == h) && map[ry + y][rx + x] == data - 1) && map[ry + y][rx + x] != 0)
            {
                return false;
            }
    return true;
}

void satalite::draw_to_map(int x, int y, int w, int h, int data)
{
    if (check_empty(x, y, w, h, data) && x > 0 && y > 0 && x + w < 50 && y + h < 25)
    {
        for (int rx = 0; rx < w; rx++)
            for (int ry = 0; ry < h; ry++)
            {
                map[ry + y][rx + x] = data;
            }
        rooms.push_back(room(x, y, w, h, data, *this));
        // cout << ' ' << rooms << ' ' << data << ' ' << x << ' ' << y << ' ' << w << ' ' << h << "\n";
        // draw_map();
    }
}

void satalite::draw_map()
{
    cout << "Rooms: " << rooms.size() << "\n";
    string str = "";
    for (int y = 0; y < 25; y++)
    {
        for (int x = 0; x < 50; x++)
        {
            str += color((int) map[y][x]) + (char)(map[y][x] + '0') + " ";
        }
        str += "\n";
    }
    str += colors[0];
    cout << str;
}

string satalite::color(int num)//get colot from number
{
    if (num == 0)
    {
        return colors[0];
    }
    else
    {
        return colors[(num + 6) % 7 + 1];
    }
}

room *find_room(int x,int y)
{
    for (int i = 0; i < rooms.size(); i++)
    {
        if (rooms[i].x <= x && rooms[i].x + rooms[i].w >= x && rooms[i].y <= y && rooms[i].y + rooms[i].h >= y)
        {
            return &rooms[i];
        }
    }
}
void room::gen_doors()
{
    bool isin;
    room *current;
    for (int rx = -1; rx < w+1; rx++)
        for (int ry = -1; ry < h+1; ry++)
            if (parent->map[ry + y][rx + x] != data && parent->map[ry + y][rx + x] != 0)
            {
                isin = false;
                current = parent.find_room(ry + y,  rx + x)
                for (int i = 0; i < doors.size(); i++)
                    if (doors[i] == current)
                    {
                        isin = true;
                    }
                if (current != NULL && !isin)
                {
                    doors.push_back(current);
                }

            }
}