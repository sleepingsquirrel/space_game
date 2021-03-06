#include <stdint.h>
#include <stdbool.h>

#define HEIGHT 26
#define WIDTH 50
#define EFFECT_COUNT 6
#define ITEM_EFFECT_COUNT 10
#define MAX_CARDS 16
#define MOVEW 6
#define MOVEH 4
#define ITEMS 6

//struct prototypes
struct satalite;
struct room;
struct door;
struct item;
struct _player;

typedef struct room_type
{
	char **descriptions;
	char *name;
	int id;
	int min_num;
	uint8_t probabilaty;
	uint8_t sat_has;
	struct room_type *next;
	//add room specifec loot and stuff
} room_type;

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
    bool used;
    struct satalite *parent;
    struct room *next;
    struct door *doors;
	room_type *type;
	bool seen;
} room;

typedef struct satalite
{
    const char *faction;
    uint8_t map[HEIGHT][WIDTH];
    int sat_size, rooms_num, doors_num, factionID, level, seed;
    bool plotted;
    char *name;
    struct room *rooms;
    struct room *starting_room;
    struct satalite *next;
    struct _player *parent;
    room_type *room_types;
} satalite;

//card struct
typedef struct _Card
{
    //Card name
    char *name;
    //Cost in oxygen, energy, health
    int cost_o;
    int cost_e;
    int cost_h;
    //relative power level of card
    int rarity;
    //damage in oxygen, energy, health
    int dam_o;
    int dam_e;
    int dam_h;
    //list of which effects this card does and does not have
    bool effects[EFFECT_COUNT];
    struct _Card *next;
} Card;

typedef struct item
{
    char *name;
    int uses, level, type;
    int o, h, e, s;
    bool infinite, combat;
    bool effect[ITEM_EFFECT_COUNT];
    Card *spell;
} item;

//player & enemy struct
typedef struct _player
{
    //PC only
	satalite *sat;
	room *room;
	unsigned int speed;
	unsigned int gold;

	//common stats
	int health;
	int oxygen;
	int energy;
	int maxHealth;
	int maxOxygen;
	int maxEnergy;
	Card *deck[MAX_CARDS];
    item *items[ITEMS];
    item *wand;
	//enemies only
	struct _player *next;
	char *name;

    //inventory
} _player;

//function prototypes
//functions in sat_gen.c
satalite *sat_gen(_player *player, int level, int seed, room_type *room_types);
room *draw_to_map(int x, int y, int w, int h, int data, satalite *sat);
void gen_map();
void door_gen(room *parent, room *doorp, char direction, int x, int y);
room *room_gen(int x1, int y1, int w1, int h1, int roomNum1, satalite *p);
bool check_empty(int x, int y, int w, int h, int data, satalite *sat);
void make_doors(room *p);
void gen_room_types(satalite *sat);
room_type *rand_room_type(room_type *start, satalite *sat);
void free_sat(satalite *sat);
void free_rooms(room *p);
void free_doors(door *p);
void room_effects(_player *player);

//functions in draw.c
const char *color(int num);
void draw_seen_map(satalite *sat);
void draw_map(uint8_t map[HEIGHT][WIDTH]);

//functions in main.c
void INThandler(int sig);
void Kill();
Card *getCards();
_player *getEnemies();

//functions in input.c
int get_command();
void move(_player *player, satalite *sat);
void search(_player *player);

//funcions in cards.c
Card *loadcards(const char *filename);
void free_card(Card *next);
Card *find_card(char *name);
Card *random_card();

//functions in combat.c
void shuffle(Card *c[MAX_CARDS]);
void print_card(Card *current);
bool fight(_player *player, _player *enemy);
void quit();
void play_card(Card *card, Card *enemy_card, _player *player, _player *enemy);
int check_life(_player *player, _player *enemy);
void enemy_turn(Card *enemy_card, _player *player, _player *enemy, int modifier);
int strcasecmp(const char *s1, const char *s2);

//functions in room_type_load.c
room_type *load_room_types(const char *filename);
void free_room_types(room_type *type);

//functions in enemies.c
_player *load_enemies(const char *filename);
void free_enemy(_player *p);
void print_enemy(_player *current);
bool encounter(_player *player);

//functions in items.c
item *gen_item(_player *player);
char *gen_item_name(item *input);
char *gen_potion_name(item *current);
void gen_loot(_player *player);
void free_item(item *current);
void inventory(_player *player);
void use_potion(_player *player, item *current);

//functions in sat_name.c
char *sat_name();

