#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//enums
enum directions{
	NORTH, SOUTH, EAST, WEST
};

//structs
typedef struct Room
{
	int state;
	struct Room* n;
	struct Room* s;
	struct Room* e;
	struct Room* w;
	struct Creature* crs[10];
} Room;

typedef struct Creature
{
	int type;
	struct Room* loc;
} Creature;

//prototypes
void debugInfo(Room* rooms, int len);

//actual functions
void userInput(void){

}

void setUp(void){
	int roomAmount;
	int creatureAmount;

	printf("%s\n", "Enter amount of rooms: ");
	scanf("%d", &roomAmount);
	Room* rooms = (Room*)malloc(sizeof(Room) * roomAmount);

	for(int i = 0; i < roomAmount; i++){
		int north, south, east, west;
		printf("Room #%d\n", i);
		scanf("%d %d %d %d %d", &rooms[i].state, &north, &south, &east, &west);

		if(north == -1){
			rooms[i].n = NULL;
		} else {
			rooms[i].n = &rooms[NORTH];
		}
		if(south == -1){
			rooms[i].s = NULL;
		} else {
			rooms[i].s = &rooms[SOUTH];
		}
		if(east == -1){
			rooms[i].e = NULL;
		} else {
			rooms[i].e = &rooms[EAST];
		}
		if(west == -1){
			rooms[i].w = NULL;
		} else {
			rooms[i].w = &rooms[WEST];
		}

		for(int c = 0; c < 10; c++){
			rooms[i].crs[c] = NULL;
		}
	}

	// printf("%s\n", "Enter amount of creatures: ");
	// scanf("%d", &creatureAmount);
	// Creature* creatures = (Creature*)malloc(sizeof(Creature) * creatureAmount);

	// for(int i = 0; i < creatureAmount; i++){
	// 	int location;
	// 	printf("Creature %d\n", i);
	// 	scanf("%d %d", &creatures[i].type, &location);

	// 	for(int j = 0; j < 10; j++){
	// 		if(rooms[location].crs[j] == NULL){
	// 			rooms[location].crs[j] = &creatures[i];
	// 			break;
	// 		}
	// 	}
	// }
	debugInfo(rooms, roomAmount);


}

void debugInfo(Room* rooms, int len){
	for(int i = 0; i < len; i++){
		printf("%p %p\n", &rooms[i], rooms[i].n);
		// for(int j = 0; j < 10; j++){
		// 	// if(rooms[i].crs[j] != NULL){
		// 	printf("%p %p\n", &rooms[i],&rooms[i].n);
		// 		// printf("%p\n", &rooms[i].crs[j]->loc);
		// 	// }
		// }
	}
}

void look(){

}

void creatureLook(){

}

void move(){

}

void cleanDirty(){

}

void creatureCleanDirty(){

}

int main(void){
	setUp();
	userInput();
	return 0;
}