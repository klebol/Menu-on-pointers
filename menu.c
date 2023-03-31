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



Menu_t *MenuCurrentMember = MENU_FIRST_MEMBER;
uint8_t MenuCurrentIndex;

/*--------------Name, *Next, *Prev, *Child, *Parent, *Funcion---
 * WARNING: First member's *Prev pointer have to be NULL,
 * & Last member's *Next pointer have to be NULL!  */
Menu_t Member1 = {"Danie", &Member2, NULL, NULL, NULL, NULL};
Menu_t Member2 = {"MrKryha", &Member3, &Member1, NULL, NULL, NULL};
Menu_t Member3 = {"Tortilla", &Member4, &Member2, NULL, NULL, NULL};
Menu_t Member4 = {"Pita", &Member5, &Member3, NULL, NULL, NULL};
Menu_t Member5 = {"Falafel", &Member6, &Member4, NULL, NULL, NULL};
Menu_t Member6 = {"Suwlak", &Member7, &Member5, NULL, NULL, NULL};
Menu_t Member7 = {"Bulka", &Member8, &Member6, NULL, NULL, NULL};
Menu_t Member8 = {"Box", &Member9, &Member7, NULL, NULL, NULL};
Menu_t Member9 = {"Pizza", &Member10, &Member8, NULL, NULL, NULL};
Menu_t Member10 = {"Sushi", &Member11, &Member9, NULL, NULL, NULL};
Menu_t Member11 = {"Flaki", NULL, &Member10, NULL, NULL, NULL};


void Menu_RefreshScreen(void)
{
	static Menu_t *LastMenuPtr = NULL;
	static Menu_t *FirstMemberToDisplayPtr = MENU_FIRST_MEMBER;
	static uint8_t FirstMemberToDisplayNumber = 0;
	Menu_t *Tmp;
	uint8_t i = 0;
	uint8_t CenterCalculate;

	if(LastMenuPtr == MenuCurrentMember) return;

	while(MenuCurrentIndex > (FirstMemberToDisplayNumber + MENU_ROWS_TO_DISPLAY - 1) )		//is our menu member in range of screen?
	{
		FirstMemberToDisplayNumber++;
		FirstMemberToDisplayPtr = FirstMemberToDisplayPtr->Next;
	}
	while(MenuCurrentIndex < FirstMemberToDisplayNumber )
	{
		FirstMemberToDisplayNumber--;
		FirstMemberToDisplayPtr = FirstMemberToDisplayPtr->Prev;
	}

	OLED_ClearBuffer(BLACK);
	OLED_MoveCursor(0, 0);
	OLED_WriteString("-------M E N U-------",WHITE);
	Tmp = FirstMemberToDisplayPtr;
	while((i != MENU_ROWS_TO_DISPLAY) && Tmp != NULL)
	{
		if(MenuCurrentMember == Tmp)
		{
			CenterCalculate = ((DISPLAY_WIDTH - ((strlen(Tmp->Name) + 6) * 6) ) / 2);
			OLED_MoveCursor(CenterCalculate, (i+1)*8);
			OLED_WriteString(">> ", WHITE);
			OLED_WriteString((char*)Tmp->Name, WHITE);
			OLED_WriteString(" <<", WHITE);
		}
		else
		{
			CenterCalculate = ((DISPLAY_WIDTH - (strlen(Tmp->Name) * 6) ) / 2);
			OLED_MoveCursor(CenterCalculate, (i+1)*8);
			OLED_WriteString((char*)Tmp->Name, WHITE);
		}
		i++;
		Tmp = Tmp->Next;
	}
	OLED_SendBuffer();
	LastMenuPtr = MenuCurrentMember;

}

void Menu_Next(void)
{
	if(MenuCurrentMember->Next != NULL)
	{
		MenuCurrentIndex++;
		MenuCurrentMember = MenuCurrentMember->Next;

	}
	else
	{
		MenuCurrentIndex = 0;
		MenuCurrentMember = MENU_FIRST_MEMBER;
	}
	Menu_RefreshScreen();
}

void Menu_Prev(void)
{
	if(MenuCurrentMember->Prev != NULL)
	{
		MenuCurrentIndex--;
		MenuCurrentMember = MenuCurrentMember->Prev;
	}
	else
	{
		while(MenuCurrentMember->Next != NULL)
		{
			MenuCurrentIndex++;
			MenuCurrentMember = MenuCurrentMember->Next;
		}
	}
	Menu_RefreshScreen();
}

uint8_t Menu_CountMembers()
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


