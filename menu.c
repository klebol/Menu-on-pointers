/*
 * menu.c
 *
 *  Created on: 15 mar 2023
 *      Author: Michal Klebokowski
 */
#include "main.h"
#include "menu.h"
#include "string.h"
#include "../oled_ssd1106/ssd1106.h"

//
//TODO: 1. Define here your menu members, remember to add extern declarations in menu.h file!
//
/*--------------Name, *Next, *Prev, *Child, *Parent, *Function---
 * WARNING: First member's *Prev pointer have to be NULL,
 * & Last member's *Next pointer have to be NULL!  */
Menu_t Member1 = {"Danie", &Member2, NULL, &Member1_1, NULL, NULL};
		Menu_t Member1_1 = {"Sos ostry", &Member1_2, NULL, NULL, &Member1, NULL};
		Menu_t Member1_2 = {"Sos ladodny", &Member1_3, &Member1_1, NULL, &Member1, NULL};
		Menu_t Member1_3 = {"Sos mieszany", NULL, &Member1_2, NULL, &Member1, NULL};

Menu_t Member2 = {"Zupa", &Member3, &Member1, &Member2_1, NULL, NULL};
		Menu_t Member2_1 = {"Grzybowa", &Member2_2, NULL, NULL, &Member2, NULL};
		Menu_t Member2_2 = {"Ogorkowa", &Member2_3, &Member2_1, NULL, &Member2, NULL};
		Menu_t Member2_3 = {"Barszczz", NULL, &Member2_2, NULL, &Member2, NULL};

Menu_t Member3 = {"Tortilla", &Member4, &Member2, &Member3_1, NULL, NULL};
		Menu_t Member3_1 = {"Test", NULL, NULL, NULL, &Member3, NULL};

Menu_t Member4 = {"Pita", &Member5, &Member3, &Member4_1, NULL, NULL};
		Menu_t Member4_1 = {"Test", NULL, NULL, NULL, &Member4, NULL};

Menu_t Member5 = {"Falafel", &Member6, &Member4, &Member5_1, NULL, NULL};
		Menu_t Member5_1 = {"Test", NULL, NULL, NULL, &Member5, NULL};

Menu_t Member6 = {"Suwlak", &Member7, &Member5, &Member6_1, NULL, NULL};
		Menu_t Member6_1 = {"Test", NULL, NULL, NULL, &Member6, NULL};

Menu_t Member7 = {"Bulka", &Member8, &Member6, &Member7_1, NULL, NULL};
		Menu_t Member7_1 = {"Test", NULL, NULL, NULL, &Member7, NULL};

Menu_t Member8 = {"Box", &Member9, &Member7, &Member8_1, NULL, NULL};
		Menu_t Member8_1 = {"Test", NULL, NULL, NULL, &Member8, NULL};

Menu_t Member9 = {"Pizza", &Member10, &Member8, &Member9_1, NULL, NULL};
		Menu_t Member9_1 = {"Test", NULL, NULL, NULL, &Member9, NULL};

Menu_t Member10 = {"Sushi", &Member11, &Member9, &Member10_1, NULL, NULL};
		Menu_t Member10_1 = {"Test", NULL, NULL, NULL, &Member10, NULL};

Menu_t Member11 = {"Flaki", NULL, &Member10, &Member11_1, NULL, NULL};
		Menu_t Member11_1 = {"Test", NULL, NULL, NULL, &Member11, NULL};
//
//End of member list
//


//
//Global variables
//
static Menu_t *MenuCurrentMember = MENU_FIRST_MEMBER;
static Menu_t *FirstMemberToDisplayPtr = MENU_FIRST_MEMBER;

static uint8_t LevelsIndexes[MENU_MAX_LEVELS_NUMBER];
static uint8_t LevelsFirstMembersToDisplay[MENU_MAX_LEVELS_NUMBER];
static uint8_t MenuLevel;

//
//Functions
//

void Menu_RefreshScreen(void)
{
	static Menu_t *LastMenuPtr = NULL;
	Menu_t *Tmp;
	uint8_t i = 0;
	uint8_t CenterCalculate;

	if(LastMenuPtr == MenuCurrentMember) return;

	while(LevelsIndexes[MenuLevel] > (LevelsFirstMembersToDisplay[MenuLevel] + MENU_ROWS_TO_DISPLAY - 1) )		//while our menu member is in out of screen range...
	{
		LevelsFirstMembersToDisplay[MenuLevel]++;																//"scroll down" by increasing first to display member
		FirstMemberToDisplayPtr = FirstMemberToDisplayPtr->Next;
	}
	while(LevelsIndexes[MenuLevel] < LevelsFirstMembersToDisplay[MenuLevel] )
	{
		LevelsFirstMembersToDisplay[MenuLevel]--;																//or "scroll up"
		FirstMemberToDisplayPtr = FirstMemberToDisplayPtr->Prev;
	}

	OLED_ClearBuffer(BLACK);
	OLED_MoveCursor(0, 0);
	OLED_WriteString("-------M E N U-------",WHITE);

	Tmp = FirstMemberToDisplayPtr;															//temporary pointer for displaying
	while((i != MENU_ROWS_TO_DISPLAY) && Tmp != NULL)										//print all menu rows
	{
		if(MenuCurrentMember == Tmp)														//if actually printed member is current member, add >> <<
		{
			CenterCalculate = ((DISPLAY_WIDTH - ((strlen(Tmp->Name) + 6) * 6) ) / 2);
			OLED_MoveCursor(CenterCalculate, (i+1)*8);
			OLED_WriteString(">> ", WHITE);
			OLED_WriteString((char*)Tmp->Name, WHITE);
			OLED_WriteString(" <<", WHITE);
		}
		else																				//if not, just print it
		{
			CenterCalculate = ((DISPLAY_WIDTH - (strlen(Tmp->Name) * 6) ) / 2);
			OLED_MoveCursor(CenterCalculate, (i+1)*8);
			OLED_WriteString((char*)Tmp->Name, WHITE);
		}
		i++;																				//increment for printing next member
		Tmp = Tmp->Next;
	}
	OLED_SendBuffer();
	LastMenuPtr = MenuCurrentMember;

}

void Menu_Next(void)
{
	if(MenuCurrentMember->Next != NULL)
	{
		MenuCurrentMember = MenuCurrentMember->Next;	//change current member pointer to next member
		LevelsIndexes[MenuLevel]++;						//increase current level index
	}
#ifdef MENU_LOOPING
	else												//if there is no next member, go to first member (looping)
	{
		while(MenuCurrentMember->Prev != NULL)
		{
			MenuCurrentMember = MenuCurrentMember->Prev;
		}
		LevelsIndexes[MenuLevel] = 0;
	}
#endif

	Menu_RefreshScreen();
}

void Menu_Prev(void)
{
	if(MenuCurrentMember->Prev != NULL)
	{
		MenuCurrentMember = MenuCurrentMember->Prev;
		LevelsIndexes[MenuLevel]--;
	}
#ifdef MENU_LOOPING
	else
	{
		while(MenuCurrentMember->Next != NULL)
		{
			MenuCurrentMember = MenuCurrentMember->Next;
			LevelsIndexes[MenuLevel]++;
		}
	}
#endif

	Menu_RefreshScreen();
}



void Menu_Select(void)
{
	if(MenuCurrentMember->Child != NULL)				//if there is a child...
	{
		MenuCurrentMember = MenuCurrentMember->Child;
		FirstMemberToDisplayPtr = MenuCurrentMember;
		MenuLevel++;
	}

	if(MenuCurrentMember->FunctionPtr != NULL)			//if there is a function pointer...
	{
		MenuCurrentMember->FunctionPtr();
	}

	Menu_RefreshScreen();
}

void Menu_Back(void)
{
	uint8_t i;

	if(MenuCurrentMember->Parent != NULL)
	{
		LevelsIndexes[MenuLevel] = 0;
		MenuCurrentMember = MenuCurrentMember->Parent;
		FirstMemberToDisplayPtr = MenuCurrentMember;
		MenuLevel--;

		for(i = 0; i < (LevelsIndexes[MenuLevel] - LevelsFirstMembersToDisplay[MenuLevel]) ; i++)
		{
			FirstMemberToDisplayPtr = FirstMemberToDisplayPtr->Prev;
		}
	}

	Menu_RefreshScreen();
}

uint8_t Menu_CountMembers(void)
{
	Menu_t *Tmp = MenuCurrentMember;
	uint8_t Counter = 1;

	while(Tmp->Next != NULL)
	{
		Counter++;
		Tmp = Tmp->Next;
	}
	Tmp = MenuCurrentMember;
	while(Tmp->Prev != NULL)
	{
		Counter++;
		Tmp = Tmp->Prev;
	}

	return Counter;
}


