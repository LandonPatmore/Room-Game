#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//structs
typedef struct Room {
	int state;
	int id;
	struct Room* n;
	struct Room* s;
	struct Room* e;
	struct Room* w;
	struct Creature* crs[10];
} Room;

typedef struct Creature {
	int type;
	struct Room* loc;
} Creature;

//prototypes
void look(Room** rooms, int* userLocation);
void userInput(Room* rooms, int userLocation);

//actual functions
void userInput(Room* rooms, int userLocation){
	char command[10] = "";
	int *currentUserLocation = &userLocation;
	Room** setRooms = &rooms;

	while(strcmp(command, "exit") != 0){
		printf("%s\n", "Enter a command: ");
		scanf("%s", command);
		if(strcmp(command, "look") == 0){
			printf("%s\n", "You Looked!");
			look(setRooms, currentUserLocation);
		} else if(strcmp(command, "exit") == 0){
			printf("%s\n", "Goodbye");
			exit(0);
		} else {
			printf("%s\n", "Unkown Command.");
		}
	}
}

void setUp(void){
	int roomAmount;
	int creatureAmount;
	int userLocation;

	printf("%s\n", "Enter amount of rooms: ");
	scanf("%d", &roomAmount);
	Room* rooms = (Room*)malloc(sizeof(Room) * roomAmount);

	for(int i = 0; i < roomAmount; i++){
		int north, south, east, west;
		printf("Room #%d\n", i);
		scanf("%d %d %d %d %d", &rooms[i].state, &north, &south, &east, &west);
		rooms[i].id = i;

		if(north == -1){
			rooms[i].n = NULL;
		} else {
			rooms[i].n = &rooms[north];
		}
		if(south == -1){
			rooms[i].s = NULL;
		} else {
			rooms[i].s = &rooms[south];
		}
		if(east == -1){
			rooms[i].e = NULL;
		} else {
			rooms[i].e = &rooms[east];
		}
		if(west == -1){
			rooms[i].w = NULL;
		} else {
			rooms[i].w = &rooms[west];
		}

		for(int c = 0; c < 10; c++){
			rooms[i].crs[c] = NULL;
		}
	}

	printf("%s\n", "Enter amount of creatures: ");
	scanf("%d", &creatureAmount);
	Creature* creatures = (Creature*)malloc(sizeof(Creature) * creatureAmount);

	for(int i = 0; i < creatureAmount; i++){
		int type;
		int location;

		printf("Creature %d\n", i);
		scanf("%d %d", &type, &location);
		creatures[i].type = type;
		
		if(type == 0){
			userLocation = location;
		}

		for(int j = 0; j < 10; j++){
			if(rooms[location].crs[j] == NULL){
				rooms[location].crs[j] = &creatures[i];
				break;
			}
		}
	}
	userInput(rooms, userLocation);

}

void look(Room** rooms, int* userLocation){
	printf("Room %d, ", *userLocation);

	int state = (*rooms)[*userLocation].state;
	char cleanliness[12] = "";
	if(state == 0){
		strcpy(cleanliness, "Clean");
	} else if(state == 1){
		strcpy(cleanliness, "Half-Dirty");
	} else {
		strcpy(cleanliness, "Dirty");
	}
	printf("%s", "neighbors");
	Room lookRoom = (*rooms)[*userLocation];
	if(lookRoom.n != NULL){
		printf(" %d to the North,", lookRoom.n->id);
	}
	if(lookRoom.s != NULL){
		printf(" %d to the South,", lookRoom.s->id);
	}
	if(lookRoom.e != NULL){
		printf(" %d to the East,", lookRoom.e->id);
	}
	if(lookRoom.w != NULL){
		printf(" %d to the West,", lookRoom.w->id);
	}

	printf("%s\n", " contains:");
	for(int i = 0; i < 10; i++){
		int lookCreature;
		if((*rooms)[*userLocation].crs[i] != NULL){
			lookCreature = (*rooms)[*userLocation].crs[i]->type;
			if(lookCreature == 0){
				printf("%s\n", "PC");
			} else if(lookCreature == 1){
				printf("%s\n", "Animal");
			} else{
				printf("%s\n", "NPC");
			}
		}
	}	


	printf("\n");
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
	return 0;
}