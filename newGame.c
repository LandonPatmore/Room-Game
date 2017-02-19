#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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
	int rnd[4];
	struct Room* loc;
} Creature;

//prototypes
void setUp(void);
void userInput(void);
void look(void);
int moveCreature(Creature* c, Room* neighbor);
int emptySpaceIn(Room* room);
void creatureLikeness(int ty, Creature* c);
void removeCreature(Creature* c, Room* room);
void addCreatureToRoom(Creature* c, Room* room);
void cleanDirty(int cd, Creature* c);
void randomMove(Creature* c);
void creatureAction(Creature* c);
void drillHole(Creature* c);
void userRespectChange(int ch);
void gameChecker(void);
int checkUserInput(char c1, char c2);
void creatureCommands(Creature* c, char c2);
void creatureDiscord(Creature* c, int t);


//global variables
int userRespect = 40;
Creature* user;
Creature* creatures;
Room* rooms;

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

	scanf("%d", &creatureAmount);
	creatures = (Creature*)malloc(sizeof(Creature) * creatureAmount);

	for(int i = 0; i < creatureAmount; i++){
		int location;
		scanf("%d %d", &creatures[i].type, &location);
		creatures[i].id = i;

		for(int j = 0; j < 4; j++){
			creatures[i].rnd[j] = 0;
		}

		if(creatures[i].type == 0){
			user = &creatures[i];
		}

		addCreatureToRoom(&creatures[i], &rooms[location]);
	}
}

void userInput(void){
	char command[10] = "";
	while(strcmp(command, "exit") != 0){
		gameChecker();

		scanf("%s", command);
		
		if(!checkUserInput(command[0], command[2])){
			if(strcmp(command, "look") == 0){
				look();
			} else if (strcmp(command, "clean") == 0){
				cleanDirty(0, user);
			} else if(strcmp(command, "dirty") == 0){
				cleanDirty(1, user);
			} else if(strcmp(command, "north") == 0){
				if(moveCreature(user, user->loc->n)){
					printf("%s\n", "You left towards the North");
				}
			} else if(strcmp(command, "south") == 0){
				if(moveCreature(user, user->loc->s)){
					printf("%s\n", "You left towards the South");
				}
			} else if(strcmp(command, "east") == 0){
				if(moveCreature(user, user->loc->e)){
					printf("%s\n", "You left towards the East");
				}
			} else if(strcmp(command, "west") == 0){
				if(moveCreature(user, user->loc->w)){
					printf("%s\n", "You left towards the West");
				}
			}
			printf("\n");
		}
	}
}

int checkUserInput(char c1, char c2){
	if(isdigit(c1)){
		for(int i = 0; i < 10; i++){
			Creature* cr = user->loc->crs[i];
			if(cr != NULL && cr->id == atoi(&c1)){
				creatureCommands(cr, c2);
				return 1;
			}
		}
	}
	return 0;
}

void creatureCommands(Creature* c, char c2){
	int type = c->type;

	if(c2 == 'l'){
		look();
	} else if (c2 == 'c'){
		cleanDirty(0, c);
	} else if(c2 == 'd'){
		cleanDirty(1, c);
	} else if(c2 == 'n'){
		if(moveCreature(c, c->loc->n)){
			printf("%d Moved North by the PC\n", c->id);
		} else {
			creatureDiscord(c, type);
		}
	} else if(c2 == 's'){
		if(moveCreature(c, c->loc->s)){
			printf("%d Moved South by the PC\n", c->id);
		} else {
			creatureDiscord(c, type);
		}
	} else if(c2 == 'e'){
		if(moveCreature(c, c->loc->e)){
			printf("%d Moved East by the PC\n", c->id);
		} else {
			creatureDiscord(c, type);
		}
	} else if(c2 == 'w'){
		if(moveCreature(c, c->loc->w)){
			printf("%d Moved West by the PC\n", c->id);
		} else {
			creatureDiscord(c, type);
		}
	}

	printf("\n");
}

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

	Room* r = user->loc;
	if(r->n != NULL){
		printf(" %d to the North,", r->n->id);
	}
	if(r->s != NULL){
		printf(" %d to the South,", r->s->id);
	}
	if(r->e != NULL){
		printf(" %d to the East,", r->e->id);
	}
	if(r->w != NULL){
		printf(" %d to the West,", r->w->id);
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
}


void creatureDiscord(Creature* c, int t){
	if(t == 1){
		printf("%d Grumbles because it can't move!\n", c->id);
	} else {
		printf("%d Growls becuase it can't move!\n", c->id);
	}
	userRespectChange(-1);
}

void cleanDirty(int cd, Creature* c){
	int state = user->loc->state;
	if(cd == 0){
		if(state > 0){
			user->loc->state = state - 1;
			creatureLikeness(0, c);
		}
	} else {
		if(state < 2){
			user->loc->state = state + 1;
			creatureLikeness(1, c);
		}
	}
}

void creatureLikeness(int ty, Creature* c){
	int checker[4] = {0, 0, 0, 0};
	if(c->type == 1 && ty == 0){
		printf("%d licks your face a lot because you made it clean the room! ", c->id);
		checker[0] = 1;
		userRespectChange(3);
	} else if(c->type == 2 && ty == 0) {
		printf("%d grumbles a lot because you made it clean the room! ", c->id);
		checker[1] = 1;
		userRespectChange(-3);
	} else if(c->type == 2 && ty == 1){
		printf("%d smiles a lot because you made it dirty the room! ", c->id);
		checker[2] = 1;
		userRespectChange(3);
	} else if(c->type == 1 && ty == 1){
		printf("%d growls a lot because you made it dirty the room! ", c->id);
		checker[3] = 1;
		userRespectChange(-3);
	}

	for(int i= 0; i < 10; i++){
		Creature* c = user->loc->crs[i];
		int state = user->loc->state;
		if(c != NULL){
			if(ty == 0){
				if(c->type == 1){
					if(checker[0] == 0){
						printf("%d licks your face because you cleaned! ", c->id);
						userRespectChange(1);
					}
				} else if(c->type == 2){
					if(checker[1] == 0){
						printf("%d Grumbles because you cleaned! ", c->id);
						userRespectChange(-1);
					}
					if(state == 0){
						randomMove(c);
					}
				}
			} else{
				if(c->type == 2){
					if(checker[2] == 0){
						printf("%d smiles becuase you dirtied! ", c->id);
						userRespectChange(1);
					}
				} else if(c->type == 1){
					if(checker[3] == 0){
						printf("%d Growls because you dirtied! ", c->id);
						userRespectChange(-1);
					}
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
			for(int j = 0; j < 4; j++){
				c->rnd[j] = 0;
			}
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

	int count = 0;
	for(int i = 0; i < 4; i++){
		if(c->rnd[i] == 1){
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
			if(!moveCreature(c, l->n)){
				// printf("%d Couldn't leave North\n", c->id);
				c->rnd[0] = 1;
				randomMove(c);
			} else {
				printf("%d Left towards North\n", c->id);
			}
			break;
		case 1:
			if(!moveCreature(c, l->s)){
				// printf("%d Couldn't leave South\n", c->id);
				c->rnd[1] = 1;
				randomMove(c);
			} else {
				printf("%d Left towards South\n", c->id);
			}
			break;
		case 2:
			if(!moveCreature(c, l->e)){
				// printf("%d Couldn't leave East\n", c->id);
				c->rnd[2] = 1;
				randomMove(c);
			} else {
				printf("%d Left towards East\n", c->id);
			}
			break;
		case 3:
			if(!moveCreature(c, l->w)){
				// printf("%d Couldn't leave West\n", c->id);
				c->rnd[3] = 1;
				randomMove(c);
			} else {
				printf("%d Left towards West\n", c->id);
			}
			break;
	}
}

void drillHole(Creature* c){
	printf("Creature %d left through the ceiling!\n", c->id);
	int ido= c->id;
	removeCreature(c, c->loc);
	for (int i = 0; i < 10; i++){
		Creature* cr = user->loc->crs[i];
		if(cr != NULL){
			if(cr->type == 1){
				printf("%d Growls because %d Left through the Ceiling! ", cr->id, ido);
				userRespectChange(-1);
			} else if(cr->type == 2){
				printf("%d Grumbles becuase %d Left through the Ceiling! ", cr->id, ido);
				userRespectChange(-1);
			}
		}
	}
}

void creatureAction(Creature* c){
	int state = c->loc->state;
	if(c->type == 1 && state != 0 && state != 1){
		c->loc->state = state - 1;
	} else if(c->type == 2 && state != 1 && state != 2){
		c->loc->state = state + 1;
	}
}

void userRespectChange(int ch){
	userRespect = userRespect + (ch);
	printf("Respect is now %d\n", userRespect);
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