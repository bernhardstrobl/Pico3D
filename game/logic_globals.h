uint32_t global_time;

#define QUARTER_DAY 16384 //duration of a quarter day in ms

#define DAY_R 13
#define DAY_G 14
#define DAY_B 15

#define NIGHT_R 2
#define NIGHT_G 1
#define NIGHT_B 8

int8_t daylight = 0; //gives current quarter of day (0 - day, 1 - dusk, 2 - night, 3 - dawn)

//Timeout game if no input detected at gamescom
#ifdef GAMESCOM
int32_t input_idle_timer = 0;
#define INPUT_TIMEOUT 2500
#endif

//general game properties
#define AREA_OUTSKIRTS 1
#define AREA_CITY_CENTER 2
#define AREA_YAKUZA_ALLEY 3
#define AREA_DOWNTOWN 4
#define AREA_OUTSKIRT_STABLES 5
#define SHOTGUN_DAMAGE 20
int32_t player_area = 0;

//player properties
int32_t player_health = 100;
int32_t player_ammo = 100;
int32_t player_kills = 0;
int32_t player_money = 100;


//Gameplay menus
#define MENU_START 1
#define MENU_MAIN 2
#define MENU_SHOP 3
#define MENU_HOTEL 4
#define MENU_DEATH 5
#define DEATH_DURATION 400 //the amount of frames the game stays in the death animation before reset
int32_t death_reset = 0;
int32_t menu = MENU_START;
int32_t demo_progress = 0;

//Times and durations should be a power of 2 for npc interpolations to allow bit-shifts instead of expensive divides

//npc logic
#ifdef NO_NPCS
#define MAX_NPCS 0
#else
#define MAX_NPCS 50
#endif
#define NPC_TRIANGLE_BUDGET 800
#define NPC_SPEED 30
#define NPC_VIEW_DISTANCE (FIXED_POINT_FACTOR * 20)
#define NPC_DESTROY_DISTANCE (FIXED_POINT_FACTOR * 50) //when npc leaves this range, destroy them (unused)
#define NPC_WALK_TIME 128 //How long an animated walk cycle is

//quest npcs
#define MAX_QUEST_NPCS 3
#define QUEST_NPC_TALK_DISTANCE (FIXED_POINT_FACTOR * 3)
int32_t close_npc; //if a player is close to a quest/shop npc, allow the use of the A button to talk

#define QUEST_AMMO_COST 10
#define QUEST_AMMO_PURCHASE 5
#define QUEST_KILL_REWARD 3

//zombie logic
#ifdef NO_NPCS
#define MAX_ZOMBIES 0
#else
#define MAX_ZOMBIES 50
#endif
#define ZOMBIE_TRIANGLE_BUDGET 800
#define ZOMBIE_HEALTH 100
#define ZOMBIE_WALK_SPEED 30
#define ZOMBIE_RUN_SPEED 60
#define ZOMBIE_VIEW_DISTANCE (FIXED_POINT_FACTOR * 20)
#define ZOMBIE_SHOOT_DISTANCE (FIXED_POINT_FACTOR * 5) //range of gun
#define ZOMBIE_ATTACK_DISTANCE (FIXED_POINT_FACTOR * 1.5) //attack distance of zombie when in range
#define ZOMBIE_DESTROY_DISTANCE (FIXED_POINT_FACTOR * 50) //when npc leaves this range, destroy them (unused)
#define ZOMBIE_TRACK_DISTANCE_DAY (FIXED_POINT_FACTOR * 10) //distance to the player at which zombies track during day
#define ZOMBIE_TRACK_DISTANCE_NIGHT (FIXED_POINT_FACTOR * 20) //distance to the player at which zombies track at night
#define ZOMBIE_WALK_TIME 128
#define ZOMBIE_RUN_TIME 64
#define ZOMBIE_ATTACK_DURATION 8
#define ZOMBIE_SLOUCH_DURATION 32 //recovery time of a zombie once shot
#define ZOMBIE_DEATH_DURATION 32 //animation time for zombie to die
#define ZOMBIE_DESPAWN_DURATION 1024 //time taken for the zombie to disappear (decompose i guess)
#define ZOMBIE_ATTACK_DAMAGE 1



//input
#define INPUT_SENSITIVITY 0.1
int32_t brightness = 75; //default brightness set by picosystem sdk
#define BACKLIGHT_INCREMENT 5

struct npc{
    int8_t status = -1; //-1 is an unused npc
    int8_t direction; //direction npc is facing
    int16_t progress; //walking animations etc.
    color_t shirt_color; //walking animations etc.
    int8_t health; //health (only applies to zombies)
    int8_t dialogue; //the current dialogue of the quest npc
    int32_t x;
    int32_t y;
};


struct npc npc_list[MAX_NPCS];

struct npc npc_quest_list[MAX_QUEST_NPCS];

struct npc zombie_list[MAX_ZOMBIES];


int32_t last_rand = 0;

//we have a general purpose pseudorandom RNG here (complete garbage but it will do)
int32_t random(int32_t rand_min, int32_t rand_max) {
    int32_t rand = (time() + last_rand) % ((rand_max + 1) - rand_min);
    last_rand = rand;
    return rand_min + rand;
    //return 4;
}