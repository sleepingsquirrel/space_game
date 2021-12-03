#include <stdint.h>
#include <stdbool.h>

#define HEIGHT 26
#define WIDTH 50

//struct prototypes
struct satalite;
struct room;
struct door;

typedef struct door
{
    int x, y;
    struct room *doorp;
    struct door *next;
    char direction;
} door;

typedef struct room
{
    int x, y, w, h, data;
    struct satalite *parent;
    struct room *next;
    struct door *doors;
    bool seen;
} room;

typedef struct satalite
{
    uint8_t map[HEIGHT][WIDTH];
    int sat_size, rooms_num, doors_num;
    struct room *rooms;
    struct room *starting_room;
} satalite;

//player struct
struct _player
{
	satalite *sat;
	room *room;
	unsigned int gold;
	unsigned int health;
	unsigned int maxHealth;
	unsigned int oxygen;
	unsigned int maxOxygen;
	unsigned int energy;
	unsigned int maxEnergy;
	unsigned int speed;
	//inventory
};
//function prototypes
//functions in sat_gen.c
satalite *sat_gen(int size, int seed);
room *draw_to_map(int x, int y, int w, int h, int data, satalite *sat);
void gen_map();
void door_gen(room *parent, room *doorp, char direction, int x, int y);
room *room_gen(int x1, int y1, int w1, int h1, int roomNum1, satalite *p);
bool check_empty(int x, int y, int w, int h, int data, satalite *sat);
void make_doors(room *p);
void free_sat(satalite *sat);
void free_rooms(room *p);
void free_doors(door *p);

//functions in draw.c
const char *color(int num);
void draw_seen_map(satalite *sat);
void draw_map(uint8_t map[HEIGHT][WIDTH]);

//functions in main.c
void INThandler(int sig);

//functions in input.c
int get_command();
void move(room **player_room, satalite *sat);
