/*
 * menu.h
 *
 *  Created on: 15 mar 2023
 *      Author: Michał Kłębokowski
 */

#ifndef MENU_MENU_H_
#define MENU_MENU_H_

//
//TODO: 2.Setup here for your needs
//
//Define how many menu positions have to be displayed at once
#define MENU_ROWS_TO_DISPLAY 6

//Define maximum sub-menu level
#define MENU_MAX_LEVELS_NUMBER 4

//Define your first member (in main menu only (level 0) )
#define MENU_FIRST_MEMBER &Member1

//Comment this definition of you don't want menu looping after last member
#define MENU_LOOPING


//
//Menu main struct
//
typedef struct Menu_Struct Menu_t;

struct Menu_Struct {
	const char *Name;
	Menu_t *Next;
	Menu_t *Prev;
	Menu_t *Child;
	Menu_t *Parent;
	void (*FunctionPtr)(void);
};

//
//TODO: 3. Declare with extern here your menu members from menu.c file
//

extern Menu_t Member1;
	extern Menu_t Member1_1;
	extern Menu_t Member1_2;
	extern Menu_t Member1_3;
extern Menu_t Member2;
	extern Menu_t Member2_1;
	extern Menu_t Member2_2;
	extern Menu_t Member2_3;
extern Menu_t Member3;
	extern Menu_t Member3_1;
extern Menu_t Member4;
	extern Menu_t Member4_1;
extern Menu_t Member5;
	extern Menu_t Member5_1;
extern Menu_t Member6;
	extern Menu_t Member6_1;
extern Menu_t Member7;
	extern Menu_t Member7_1;
extern Menu_t Member8;
	extern Menu_t Member8_1;
extern Menu_t Member9;
	extern Menu_t Member9_1;
extern Menu_t Member10;
	extern Menu_t Member10_1;
extern Menu_t Member11;
	extern Menu_t Member11_1;

//
//Funciotns
//

void Menu_RefreshScreen(void);
void Menu_Next(void);
void Menu_Prev(void);
void Menu_Select(void);
void Menu_Back(void);
uint8_t Menu_CountMembers(void);
#endif /* MENU_MENU_H_ */
