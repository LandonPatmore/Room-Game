#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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

void userInput(void){

}

void setUp(void){
	int roomAmount;
	int creatureAmount;
	Room* rooms = (Room*)malloc(sizeof(Room) * roomAmount);

	printf("%s\n", "Enter amount of rooms: ");
	scanf("%d", &roomAmount);

	for(int i = 0; i < roomAmount; i++){
		int state, north, south, east, west;
		printf("Room #%d\n", i);
		scanf("%d %d %d %d %d", &state, &north, &south, &east, &west);
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