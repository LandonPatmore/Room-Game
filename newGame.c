#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
	int id;
	struct Room* loc;
} Creature;

//prototypes
void setUp(void);
void userInput(void);
void look(void);
int moveCreature(Creature* c, Room* neighbor);
int emptySpaceIn(Room* room);
void removeCreature(Creature* c, Room* room);
void addCreatureToRoom(Creature* c, Room* room);
void cleanDirty(int cd);
void creatureLike(void);
void randomMove(Creature* c, int direction);


//global variables
int userRespect = 40;
Creature* user;
Creature* creatures;
Room* rooms;

//random seed for each time the program starts

//actual functions
void setUp(void){
	int roomAmount;
	int creatureAmount;
	int userLocation;

	printf("%s\n", "Enter amount of rooms: ");
	scanf("%d", &roomAmount);
	rooms = (Room*)malloc(sizeof(Room) * roomAmount);

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
	creatures = (Creature*)malloc(sizeof(Creature) * creatureAmount);

	for(int i = 0; i < creatureAmount; i++){
		int type;
		int location;

		printf("Creature %d\n", i);
		scanf("%d %d", &type, &location);
		creatures[i].type = type;
		creatures[i].id = i;

		if(type == 0){
			user = &creatures[i];
		}

		addCreatureToRoom(&creatures[i], &rooms[location]);
	}
}

void userInput(void){
	char command[10] = "";
	printf("\n");
	while(strcmp(command, "exit") != 0){
		printf("%s\n", "Enter a command: ");
		scanf("%s", command);
		if(strcmp(command, "look") == 0){
			look();
		} else if(strcmp(command, "exit") == 0){
			printf("%s\n", "Goodbye");
			exit(0);
		} else if (strcmp(command, "clean") == 0){
			cleanDirty(0);
		} else if(strcmp(command, "dirty") == 0){
			cleanDirty(1);
		} else if(strcmp(command, "north") == 0){
			moveCreature(user, user->loc->n);
		} else if(strcmp(command, "south") == 0){
			moveCreature(user, user->loc->s);
		} else if(strcmp(command, "east") == 0){
			moveCreature(user, user->loc->e);
		} else if(strcmp(command, "west") == 0){
			moveCreature(user, user->loc->w);
		}

		printf("The PC's respect after this command is %d\n", userRespect);
	}
}

void look(void){

	int state = user->loc->state;
	char cleanliness[12] = "";
	if(state == 0){
		strcpy(cleanliness, "Clean");
	} else if(state == 1){
		strcpy(cleanliness, "Half-Dirty");
	} else {
		strcpy(cleanliness, "Dirty");
	}
	printf("Room %d, %s, ", user->loc->id, cleanliness);
	printf("%s", "neighbors");
	Room lookRoom = *user->loc;
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
		int cT;
		int cId;
		if(user->loc->crs[i] != NULL){
			cT = user->loc->crs[i]->type;
			cId = user->loc->crs[i]->id;
			if(cT == 0){
				printf("%s\n", "PC");
			} else if(cT == 1){
				printf("%s %d\n", "Animal", cId);
			} else{
				printf("%s %d\n", "Human", cId);
			}
		}
	}	
	printf("\n");
}

void cleanDirty(int cd){
	int state = user->loc->state;
	if(cd == 0){
		if(state > 0){
			user->loc->state = state - 1;
			creatureLike();
		}
	} else {
		if(state < 2){
			user->loc->state = state + 1;
			creatureLike();
		}
	}
}

int moveCreature(Creature* c, Room* neighbor){
	//check if neighbor exists
	//check if there is space in that neighbor for one more creature
	//remove the creature from its current room
	//add the creature to its new room

	if(neighbor == NULL){
		printf("No neighbor in the direction!\n");
		return 0;
	}
	if(!emptySpaceIn(neighbor)){
		printf("No room in that room!\n");
		return 0;
	}
	removeCreature(c, c->loc);
	addCreatureToRoom(c, neighbor);
	return 1;
}

void removeCreature(Creature* c, Room* room){
	for(int i = 0; i < 10; i++){
		if(c->id == room->crs[i]->id){
			room->crs[i] = NULL;
			break;
		}
	}
	c->loc = NULL;
}

void addCreatureToRoom(Creature* c, Room* room){
	c->loc = room;
	for(int i = 0; i < 10; i++){
		if(room->crs[i] == NULL){
			room->crs[i] = c;
			break;
		}
	}
}

int emptySpaceIn(Room* room){
	int count = 0;
	for(int i = 0; i < 10; i++){
		if(room->crs[i] != NULL){
			count++;
		}
	}
	if(count == 10){
		return 0;
	}
	return 1;
}

void creatureLike(void){
	srand(time(NULL));
	for(int i = 0; i < 10; i++){
		int direction = rand() % 4;
		if(user->loc->crs[i] != NULL){
			if(user->loc->crs[i]->type == 1 && user->loc->state != 0 && user->loc->state != 1){
				printf("%s\n", "Animal doesn't like this room!");
				userRespect = userRespect - 1;
				randomMove(user->loc->crs[i], direction);
			} else if(user->loc->crs[i]->type == 2 && user->loc->state != 1 && user->loc->state != 2){
				printf("%s\n", "Human doesn't like this room!");
				userRespect = userRespect - 1;
				randomMove(user->loc->crs[i], direction);
			}
		}
	}
}

void randomMove(Creature* c, int direction){
	switch(direction){
		case 0:
			printf("%d Moving North\n", c->id);
			moveCreature(c, user->loc->n);
			break;
		case 1:
			printf("%d Moving South\n", c->id);
			moveCreature(c, user->loc->s);
			break;
		case 2:
			printf("%d Moving East\n", c->id);
			moveCreature(c, user->loc->e);
			break;
		case 3:
			printf("%d Moving West\n", c->id);
			moveCreature(c, user->loc->w);
			break;
	}
}

int main(void){
	setUp();
	userInput();
	return 0;
}