#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

//structs
typedef struct Room {
	int state; //the state of the room
	int id; //id of room to easily find it
	struct Room* n; //north neighbor
	struct Room* s; //south neighbor
	struct Room* e; //east neighbor
	struct Room* w; //west neighbor
	struct Creature* crs[10]; //the creatures pointer array of all the current creatures in the room
} Room;

typedef struct Creature {
	int type; //type of creature
	int id; //creature id for easily finding it
	int rnd[4]; //used to check if the creature has already attempted to go to a room and also for drillCeiling
	int cL; //creatureLikeness check
	struct Room* loc; //the room in which the creature is actually in
} Creature;

//prototypes
void setUp(void); //sets up all of the rooms and the creatures
void userInput(void); //takes the user's input and calls functions based on the input
void look(void); //looks into the room the user is currently in and gives all the info about it
int moveCreature(Creature* c, Room* neighbor); //attempts to move the creature
int emptySpaceIn(Room* room); //checks to see if there is empty space in the room
void creatureLikeness(int ty, Creature* c); //checks to see if the creature likes/dislikes what the user just commanded
void removeCreature(Creature* c, Room* room); //removes the creature from the current room
void addCreatureToRoom(Creature* c, Room* room); //adds the creature to a room
void cleanDirty(int cd, Creature* c); //checks to see if the user has cleaned/dirtied the room and changes the room state
void randomMove(Creature* c); //randomly moves the creature to an adjacent room
void creatureAction(Creature* c); //called when the creature moves to another room and it doesn't like the state of the room
void drillHole(Creature* c); //called when the creature can't move to any adjacent room
void userRespectChange(int ch); //changes the user's respect based on the input and creatures' reactions
int gameChecker(void); //checks to see if the respect has gone above 80 or dropped below 0 and ends the game
int checkUserInput(char c1[10]); //checks to see if the input is directing the creature to do something or not
void creatureCommands(Creature* c, char c2); //does the action the user told a creature to do
void creatureDiscord(Creature* c); //called when the user tells the creature to move, but it can't
void freeMem(void); //frees memory once the program is done running

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
	rooms = malloc(sizeof(Room) * roomAmount);

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
	creatures = malloc(sizeof(Creature) * creatureAmount);

	for(int i = 0; i < creatureAmount; i++){
		int location;
		scanf("%d %d", &creatures[i].type, &location);
		creatures[i].id = i;
        creatures[i].cL = 0;

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
		if(!gameChecker()){

			//printf("%s\n", "Enter a command: ");
            scanf("%s", command);
            
            if(!checkUserInput(command)){
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
        } else {
            break;
        }
	}
    freeMem();
    exit(0);
}

int checkUserInput(char c1[10]){
	char* token = strtok(c1, ":");
	if(isdigit(*token)){
		for(int i = 0; i < 10; i++){
			Creature* cr = user->loc->crs[i];
			if(cr != NULL && cr->id == atoi(token)){
				token = strtok(NULL, ":");
				creatureCommands(cr, token[0]);
				return 1;
			}
		}
	}
	return 0;
}

void creatureCommands(Creature* c, char c2){
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
			creatureDiscord(c);
		}
	} else if(c2 == 's'){
		if(moveCreature(c, c->loc->s)){
			printf("%d Moved South by the PC\n", c->id);
		} else {
			creatureDiscord(c);
		}
	} else if(c2 == 'e'){
		if(moveCreature(c, c->loc->e)){
			printf("%d Moved East by the PC\n", c->id);
		} else {
			creatureDiscord(c);
		}
	} else if(c2 == 'w'){
		if(moveCreature(c, c->loc->w)){
			printf("%d Moved West by the PC\n", c->id);
		} else {
			creatureDiscord(c);
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
		Creature* c = user->loc->crs[i];
		if(c != NULL){
			if(c->type == 0){
				printf("%s\n", "PC");
			} else if(c->type == 1){
				printf("%s %d\n", "Animal", c->id);
			} else{
				printf("%s %d\n", "Human", c->id);
			}
		}
	}	
}


void creatureDiscord(Creature* c){
	if(c->type == 1){
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
	int state = user->loc->state;
	if(c->type == 1 && ty == 0){
		printf("%d licks your face a lot because you made it clean the room! ", c->id);
		c->cL = 1;
		userRespectChange(3);
	} else if(c->type == 2 && ty == 0) {
		printf("%d grumbles a lot because you made it clean the room! ", c->id);
		c->cL = 1;
		userRespectChange(-3);
		if(state == 0){
			randomMove(c);
		}
	} else if(c->type == 2 && ty == 1){
		printf("%d smiles a lot because you made it dirty the room! ", c->id);
		c->cL = 1;
		userRespectChange(3);
	} else if(c->type == 1 && ty == 1){
		printf("%d growls a lot because you made it dirty the room! ", c->id);
		c->cL = 1;
		userRespectChange(-3);
		if(state == 2){
			randomMove(c);
		}
	}

	for(int i= 0; i < 10; i++){
		Creature* c = user->loc->crs[i];
		if(c != NULL){
			if(ty == 0 && c->type == 1 && c->cL == 0){
				printf("%d licks your face because you cleaned! ", c->id);
				userRespectChange(1);
			} else if(ty == 0 && c->type == 2 && c->cL == 0){
				printf("%d Grumbles because you cleaned! ", c->id);
				userRespectChange(-1);
				if(state == 0){
					randomMove(c);
				}
			} else if(ty == 1 && c->type == 2 && c->cL == 0){
				printf("%d smiles becuase you dirtied! ", c->id);
				userRespectChange(1);
			} else if(ty == 1 && c->type == 1 && c->cL == 0){
				printf("%d Growls because you dirtied! ", c->id);
				userRespectChange(-1);
				if(state == 2){
					randomMove(c);
				}
			}
		}
	}
	c->cL = 0; //resets the creatureLikeness check
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
				c->rnd[0] = 1;
				randomMove(c);
			} else {
				printf("%d Left towards North\n", c->id);
			}
			break;
		case 1:
			if(!moveCreature(c, l->s)){
				c->rnd[1] = 1;
				randomMove(c);
			} else {
				printf("%d Left towards South\n", c->id);
			}
			break;
		case 2:
			if(!moveCreature(c, l->e)){
				c->rnd[2] = 1;
				randomMove(c);
			} else {
				printf("%d Left towards East\n", c->id);
			}
			break;
		case 3:
			if(!moveCreature(c, l->w)){
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
	int old= c->id;
	removeCreature(c, c->loc);
	for (int i = 0; i < 10; i++){
		Creature* cr = user->loc->crs[i];
		if(cr != NULL){
			if(cr->type == 1){
				printf("%d Growls because %d Left through the Ceiling! ", cr->id, old);
				userRespectChange(-1);
			} else if(cr->type == 2){
				printf("%d Grumbles becuase %d Left through the Ceiling! ", cr->id, old);
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

void freeMem(void){
    free(rooms);
    free(creatures);
}

int gameChecker(void){
	if(userRespect > 80){
		printf("THE ALMIGHTY PC HAS BEEN PRAISED WITH A SCORE OF: %d\n", userRespect);
        return 1;
	} else if(userRespect < 0){
		printf("THE UNWORTHY PC IS DESGRACEFUL WITH A SCORE OF: %d\n", userRespect);
        return 1;
	}
    return 0;
}

int main(void){
	srand(time(NULL));
	setUp();
	userInput();
	return 0;
}
