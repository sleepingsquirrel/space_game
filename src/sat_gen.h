#include <stdint.h>
#include <stdbool.h>

#define HEIGHT 26
#define WIDTH 50

//struct prototypes
struct satalite;
struct room;
struct door;

//function prototypes
//functions in sat_gen.c
struct satalite *sat_gen(int size, int seed);
struct room *draw_to_map(int x, int y, int w, int h, int data, struct satalite *sat);
void gen_map();
void door_gen(struct room *parent, struct room *doorp, char direction, int x, int y);
struct room *room_gen(int x1, int y1, int w1, int h1, int roomNum1, struct satalite *p);
bool check_empty(int x, int y, int w, int h, int data, struct satalite *sat);
void make_doors(struct room *p);
void free_sat(struct satalite *sat);
void free_rooms(struct room *p);
void free_doors(struct door *p);

//functions in draw.c
const char *color(int num);
void draw_seen_map(struct satalite *sat);
void draw_map(uint8_t map[HEIGHT][WIDTH]);

//functions in main.c
void INThandler(int sig);
void see_room(struct room *rm);

//functions in input.c
int get_command();
void move(struct room **player_room, struct satalite *sat);

struct door
{
    int x, y;
    struct room *doorp;
    struct door *next;
    char direction;
};

struct room
{
    int x, y, w, h, data;
    struct satalite *parent;
    struct room *next;
    struct door *doors;
    bool seen;
};

struct satalite
{
    uint8_t map[HEIGHT][WIDTH];
    int sat_size, rooms_num, doors_num;
    struct room *rooms;
    struct room *starting_room;
};
