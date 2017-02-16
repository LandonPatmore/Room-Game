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
void look(Room* rooms, int* userLocation);
void userInput(Room* rooms, int roomAmount);
void cleanDirty(Room* rooms, int* userLocation, int type);
void move(Room* rooms, int* userLocation, char* direction);

//user respect level (global)
int userRespect = 40;

//actual functions
void userInput(Room* rooms, int roomAmount){
	char command[10] = "";
	int* currentUserLocation;

	for(int i = 0; i < roomAmount; i++){
		for(int j = 0; j < 10; j++){
			if(rooms[i].crs[j] != NULL && rooms[i].crs[j]->type == 0){
				currentUserLocation = &rooms[i].crs[j]->loc->id;
				printf("%s %d\n", "found PC", i);
			}
		}
	}

	Room* setRooms = rooms;

	while(strcmp(command, "exit") != 0){
		printf("%s\n", "Enter a command: ");
		scanf("%s", command);
		if(strcmp(command, "look") == 0){
			look(setRooms, currentUserLocation);
		} else if(strcmp(command, "exit") == 0){
			printf("%s\n", "Goodbye");
			exit(0);
		} else if (strcmp(command, "clean") == 0){
			cleanDirty(setRooms, currentUserLocation, 0);
		} else if(strcmp(command, "dirty") == 0){
			cleanDirty(setRooms, currentUserLocation, 1);
		} else if(strcmp(command, "north") == 0){
			printf("User location before move: %d\n", *currentUserLocation);
			move(setRooms, currentUserLocation, "north");
			printf("User location after move: %d\n", *currentUserLocation);
		} else if(strcmp(command, "south") == 0){
			move(setRooms, currentUserLocation, "south");
		} else if(strcmp(command, "east") == 0){
			move(setRooms, currentUserLocation, "east");
		} else if(strcmp(command, "west") == 0){
			move(setRooms, currentUserLocation, "west");
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

		for(int j = 0; j < 10; j++){
			if(rooms[location].crs[j] == NULL){
				creatures[i].loc = &rooms[location];
				rooms[location].crs[j] = &creatures[i];
				break;
			}
		}
	}
	userInput(rooms, roomAmount);

}

void look(Room* rooms, int* userLocation){
	printf("Room %d, ", *userLocation);

	int state = rooms[*userLocation].state;
	char cleanliness[12] = "";
	if(state == 0){
		strcpy(cleanliness, "Clean");
	} else if(state == 1){
		strcpy(cleanliness, "Half-Dirty");
	} else {
		strcpy(cleanliness, "Dirty");
	}
	printf("%s", "neighbors");
	Room lookRoom = rooms[*userLocation];
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
		if(rooms[*userLocation].crs[i] != NULL){
			lookCreature = rooms[*userLocation].crs[i]->type;
			if(lookCreature == 0){
				printf("%s\n", "PC");
			} else if(lookCreature == 1){
				printf("%s %d\n", "Animal", i);
			} else{
				printf("%s %d\n", "NPC", i);
			}
		}
	}	


	printf("\n");
}

void creatureLook(){

}

void move(Room* rooms, int* userLocation, char* direction){
	Room r = rooms[*userLocation];
	if(strcmp(direction, "north") == 0){
		if(r.n != NULL){
			*userLocation = r.n->id;
			return;
		}
	} else if(strcmp(direction, "south") == 0){
		if(r.s != NULL){
			*userLocation = r.s->id;
			return;
		}
	} else if(strcmp(direction, "east") == 0){
		if(r.e != NULL){
			*userLocation = r.e->id;
			return;
		}
	} else if(strcmp(direction, "west") == 0){
		if(r.w != NULL){
			*userLocation = r.w->id;
			return;
		}
	}

	printf("There is no neighbor to the %s\n", direction);
}

void cleanDirty(Room* rooms, int* userLocation, int type){
	int state = rooms[*userLocation].state;
	int *respectChange;
	if(type == 0){
		printf("%s\n", "cleaning...");
		if(state > 0){
			rooms[*userLocation].state = state - 1;

		}
	} else {
		printf("%s\n", "dirtying...");
		if(state < 2){
			rooms[*userLocation].state = state + 1;
		}
	}
}

void creatureCleanDirty(){

}

int main(void){
	setUp();
	return 0;
}