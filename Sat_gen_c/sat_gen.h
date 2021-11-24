#include <stdint.h>

#define HEIGHT 26
#define WIDTH 50


struct satalite;
struct room;
struct door;
struct satalite *sat_gen(int size, int seed);
struct room *draw_to_map(int x, int y, int w, int h, int data, struct satalite *sat);
const char *color(int num);
void draw_seen_map(struct satalite *sat);
void draw_map(uint8_t map[HEIGHT][WIDTH]);
void gen_map();
bool check_empty(int x, int y, int w, int h, int data, struct satalite *sat);
void make_doors(struct room *p);
void free_sat(struct satalite *sat);
void free_rooms(struct room *p);
void free_doors(struct door *p);

struct door
{
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
