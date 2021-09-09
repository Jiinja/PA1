#pragma once

#define _CRT_SECURE_NO_WARNINGS


#include <stdio.h>
#include <string.h>
#include <stdlib.h> //atoi (ascii to int)

typedef enum sleep
{
	NONE = 0, ASLEEP = 1, AWAKE = 2, REALLYAWAKE = 3
}Sleep;

typedef struct fitbit
{
	char patient[9];
	char minute[9];
	double calories;
	double distance;
	unsigned int floors;
	unsigned int heartrate;
	unsigned int steps;
	Sleep sleeplevel;
}FitbitData;

int addToArray(FitbitData fitbitdata[], char* line, int lineNumber, char* patientNumber);
char* dupeCheck(char* line);
void dataPrinter(FitbitData fitbitdata[], FILE* outfile);
char* calcString(FitbitData fitbitdata[]);



