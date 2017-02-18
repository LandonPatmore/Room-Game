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
void creatureLikeness(int ty);
void removeCreature(Creature* c, Room* room);
void addCreatureToRoom(Creature* c, Room* room);
void cleanDirty(int cd);
void randomMove(Creature* c);
void creatureAction(Creature* c);
void drillHole(Creature* c);
void userRespectChange(int ch);
void gameChecker(void);


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

	//printf("%s\n", "Enter amount of rooms: ");
	scanf("%d", &roomAmount);
	rooms = (Room*)malloc(sizeof(Room) * roomAmount);

	for(int i = 0; i < roomAmount; i++){
		int north, south, east, west;
		//printf("Room #%d\n", i);
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

	//printf("%s\n", "Enter amount of creatures: ");
	scanf("%d", &creatureAmount);
	creatures = (Creature*)malloc(sizeof(Creature) * creatureAmount);

	for(int i = 0; i < creatureAmount; i++){
		int type;
		int location;

		//printf("Creature %d\n", i);
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
		gameChecker();
		printf("%s\n", "Enter a command: ");
		scanf("%s", command);

		if(strcmp(command, "look") == 0){
			look();
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
		} else if(strcmp(command, "exit") == 0){
			printf("%s\n", "Goodbye!");
			exit(0);
		}

		// char* check = strtok(command, ":");
		// char* check2 = strtok(NULL, ":");
		// for(int i = 0; i < 10; i++){
		// 	Creature* c = user->loc->crs[i];
		// 	if(c != NULL){
		// 		if(atoi(check) == c->id){
		// 			printf("::\n");
		// 		}
		// 	}
		// }

		printf("The PC's respect after this command is %d\n", userRespect);
		printf("\n");
	}
}

// void checkUserInput(){
	
// }

void look(void){

	int state = user->loc->state;
	char cleanliness[11] = "";
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
		if(user->loc->crs[i] != NULL){
			Creature* c = user->loc->crs[i];
			int cType = c->type;
			int cId = c->id;
			if(cType == 0){
				printf("%s\n", "PC");
			} else if(cType == 1){
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
			creatureLikeness(0);
		}
	} else {
		if(state < 2){
			user->loc->state = state + 1;
			creatureLikeness(1);
		}
	}
}

void creatureLikeness(int ty){
	for(int i= 0; i < 10; i++){
		Creature* c = user->loc->crs[i];
		int state = user->loc->state;
		if(c != NULL){
			if(ty == 0){
				if(c->type == 1){
					printf("Animal %d licks your face!\n", c->id);
					userRespectChange(1);
				} else if(c->type == 2){
					printf("Human %d Grumbled!\n", c->id);
					userRespectChange(-1);
					if(state == 0){
						randomMove(c);
					}
				}
			} else{
				if(c->type == 2){
					printf("Human %d smiles!\n", c->id);
					userRespectChange(1);
				} else if(c->type == 1){
					printf("Animal %d Grumbled!\n", c->id);
					userRespectChange(-1);
					if(state == 2){
						randomMove(c);
					}
				}
			}
		}
	}
}

int moveCreature(Creature* c, Room* neighbor){
	//check if neighbor exists
	//check if there is space in that neighbor for one more creature
	//remove the creature from its current room
	//add the creature to its new room

	if(neighbor == NULL){
		//printf("No neighbor in the direction!\n");
		return 0;
	}
	if(!emptySpaceIn(neighbor)){
		//printf("No room in that room!\n");
		return 0;
	}
	removeCreature(c, c->loc);
	addCreatureToRoom(c, neighbor);
	creatureAction(c);
	return 1;
}

void removeCreature(Creature* c, Room* room){
	for(int i = 0; i < 10; i++){
		if(room->crs[i] != NULL){
			if(c->id == room->crs[i]->id){
				room->crs[i] = NULL;
				c->loc = NULL;
				break;
			}
		}
	}
}

void addCreatureToRoom(Creature* c, Room* room){
	for(int i = 0; i < 10; i++){
		if(room->crs[i] == NULL){
			c->loc = room;
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

void randomMove(Creature* c){
	int direction = rand() % 4;
	static int checkers[4] = {0, 0, 0, 0};

	int count = 0;
	for(int i = 0; i < 4; i++){
		if(checkers[i] == 1){
			count++;
		}
	}
	if(count == 4){
		drillHole(c);
		return;
	}

	Room* l = c->loc;

	switch(direction){
		case 0:
			//printf("%d Attempting to leave North\n", c->id);
			if(!moveCreature(c, l->n)){
				checkers[0] = 1;
				randomMove(c);
			} else {
				printf("%d Left towards North\n", c->id);
			}
			break;
		case 1:
			//printf("%d Attempting to leave South\n", c->id);
			if(!moveCreature(c, l->s)){
				checkers[1] = 1;
				randomMove(c);
			} else {
				printf("%d Left towards South\n", c->id);
			}
			break;
		case 2:
			//printf("%d Attempting to leave East\n", c->id);
			if(!moveCreature(c, l->e)){
				checkers[2] = 1;
				randomMove(c);
			} else {
				printf("%d Left towards East\n", c->id);
			}
			break;
		case 3:
			//printf("%d Attempting to leave West\n", c->id);
			if(!moveCreature(c, l->w)){
				checkers[3] = 1;
				randomMove(c);
			} else {
				printf("%d Left towards West\n", c->id);
			}
			break;
	}
}

void drillHole(Creature* c){
	for(int i = 0; i < 10; i++){
		if(c->loc->crs[i] != NULL){
			if(c->id == c->loc->crs[i]->id){
				printf("Creature %d left through the ceiling!\n", c->id);
				removeCreature(c, c->loc);
				break;
			}
		}
	}
}

void creatureAction(Creature* c){
	int state = c->loc->state;
	if(c->type == 1 && state != 0 && state != 1){
		//printf("Animal %d cleaned this room to suit its needs!\n", c->id);
		c->loc->state = state - 1;
	} else if(c->type == 2 && state != 1 && state != 2){
		//printf("Human %d cleaned this room to suit its needs!\n", c->id);
		c->loc->state = state + 1;
	}
}

void userRespectChange(int ch){
	userRespect = userRespect + (ch);
}

void gameChecker(void){
	if(userRespect > 80){
		printf("THE ALMIGHTY PC HAS BEEN PRAISED WITH A SCORE OF: %d\n", userRespect);
		exit(0);
	} else if(userRespect < 0){
		printf("THE UNWORTHY PC IS DESGRACEFUL WITH A SCORE OF: %d\n", userRespect);
		exit(0);
	}
}

int main(void){
	srand(time(NULL));
	setUp();
	userInput();
	return 0;
}