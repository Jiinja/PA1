/*****************************************************************************************
* Programmer: Josh Maloy                                                                 *
* Class: CptS 122, Fall 2021; Lab Section 1                                              *
* Programming Assignment: PA1                                                            *
* Date: September 1, 2021                                                                *
* Description: this program has multiple functions used by the main program that are     *
* necessary to carry out the main programs description.                                  *
******************************************************************************************/




#include "Header.h"

int addToArray(FitbitData fitbitdata[], char* line, int lineNumber, char* patientNumber) //adds the given line to the array at the given position in the array. also checks for correct patient id nunmber
{
	
	strcpy(fitbitdata[lineNumber].patient, strtok(line, ","));		//copying patientID into the fitbitdata array
	if (strcmp(fitbitdata[lineNumber].patient, patientNumber) != 0) {return -1; }  //if the ID is wrong, ignore the line and continue
	strcpy(fitbitdata[lineNumber].minute, strtok(NULL, ","));  //putting all of the data in the line into the struct variables
	fitbitdata[lineNumber].calories = atof(strtok(NULL, ","));
	fitbitdata[lineNumber].distance = atof(strtok(NULL, ","));
	fitbitdata[lineNumber].floors = atoi(strtok(NULL, ","));
	fitbitdata[lineNumber].heartrate = atoi(strtok(NULL, ","));
	fitbitdata[lineNumber].steps = atoi(strtok(NULL, ","));
	char* sleepe = strtok(NULL, "\n");
	if (sleepe == NULL) { sleepe = "0"; } //if there is no value then strok returns NULL. if null, set sleepe to 0
	int sleeper = atoi(sleepe);					//changing "0" to 0
	if (sleeper == 0) { fitbitdata[lineNumber].sleeplevel = 0; }      // these if/else statements check which int sleeper is and sets sleep level to the appropriate value
	else if (sleeper == 1) { fitbitdata[lineNumber].sleeplevel = 1; }
	else if (sleeper == 2) { fitbitdata[lineNumber].sleeplevel = 2; }
	else if (sleeper == 3) { fitbitdata[lineNumber].sleeplevel = 3; } 
	return 1;
}

char* dupeCheck(char* line) //take a line and checks for two commas in a row. if there are two commas in a row it puts a 0 in between them.
{
	while (strstr(line, ",,") != NULL)           //working together with Aidan Gooding making this while loop.
	{
		char* dataCleanser = strstr(line, ",,");
		int positionSaver = dataCleanser - line;
		char temp[50];
		strcpy(temp, line);
		line[positionSaver + 1] = '0';
		for (int j = positionSaver + 1; j < strlen(temp); ++j) { line[j + 1] = temp[j]; }
	}
	return line;
}

char* calcString(FitbitData fitbitdata[]) //takes the array of structs, finds all required datapoints, and returns a formatted string ready to be put into an excel file.
{
	double totalCal = 0.0;  //initializing totals
	double totalDist = 0.0;
	int totalFloors = 0;
	int totalSteps = 0;
	int totalBeats = 0;
	unsigned int maxSteps = 0;
	int minutesBeat = 0;
	int maxMinute = 0;

	int sleepTotal = 0;
	int sleepStart = 0;
	int sleepEnd = 0;
	int sleepMax = 0;
	int maxStart = 0;
	int maxEnd = 0;

	for (int i = 0; i < 1440; i++)	//this loop iterates through each fitbitdata value in the array and does the needed calculations
	{
		totalCal = totalCal + fitbitdata[i].calories;
		totalDist = totalDist + fitbitdata[i].distance;
		totalFloors = totalFloors + fitbitdata[i].floors;
		totalSteps = totalSteps + fitbitdata[i].steps;
		totalBeats = totalBeats + fitbitdata[i].heartrate;
		if (fitbitdata[i].heartrate != 0) { minutesBeat++; }  //if heartrate is 0 at the time, it doesnt count for the average
		if (fitbitdata[i].steps >= maxSteps) 
		{
			maxMinute = i;						//this if statement finds which location in the array has the max steps value. 
			maxSteps = fitbitdata[i].steps;
		}

		if (fitbitdata[i].sleeplevel == 3) //if sleep level is REALLYAWAKE
		{
			if (sleepTotal == 0) { sleepStart = i; } //if the running total is 0, set current location to start
			sleepTotal += 3;							//add 3 beacuse REALLYAWAKE = 3
			sleepEnd = i;                               //each one is a possible end
		}
		else if (fitbitdata[i].sleeplevel == 2)
		{
			if (sleepTotal == 0) { sleepStart = i; } //same as above comments, just 2/AWAKE insteal of 3/REALLYAWAKE
			sleepTotal += 2;
			sleepEnd = i;
		}
		else 
		{
			if (sleepTotal > sleepMax) //if the running total is larger than the previous max running total
			{
				sleepMax = sleepTotal;	//set the max total to the running total
				maxEnd = sleepEnd;		//set the end time of the max total to the end time of running total
				maxStart = sleepStart;  //set the start time of the max total to the start time of running total
			}
			sleepTotal = 0; //reset all "running total" variables to 0.
			sleepStart = 0;
			sleepEnd = 0;
		}
	}
	char minMax[9];
	strcpy(minMax, fitbitdata[maxMinute].minute); //setting minMax to the minute at the location where max steps was found
	totalBeats = totalBeats / minutesBeat;		  //calculating average bpm

	char* output[55];
	sprintf(output, "%f,%f,%d,%d,%d,%s,%s:%s", 
		totalCal, 
		totalDist, 
		totalFloors, 
		totalSteps, 
		totalBeats,								//putting all of the values into a correctly formatted string ready to print to the results file
		minMax,fitbitdata[maxStart].minute, 
		fitbitdata[maxEnd].minute);
	return output; //return the formatted string
}

void dataPrinter(FitbitData fitbitdata[], FILE* outfile) //takes a fitbitdata array and a file and prints out the array to the file and to the console
{
	int sleepLevelPrinter = 0;
	for (int p = 0; p < 1440; p++) //for loop iterating through the entire array
	{
		if (fitbitdata[p].sleeplevel == 0) { sleepLevelPrinter = 0; }
		else if (fitbitdata[p].sleeplevel == 1) { sleepLevelPrinter = 1; }
		else if (fitbitdata[p].sleeplevel == 2) { sleepLevelPrinter = 2; } //getting sleep level becasue it is an enum
		else if (fitbitdata[p].sleeplevel == 3) { sleepLevelPrinter = 3; }

		fprintf(outfile, "%s,%s,%f,%f,%d,%d,%d,%d\n",		//printing to the file
			fitbitdata[p].patient,
			fitbitdata[p].minute,
			fitbitdata[p].calories,
			fitbitdata[p].distance,			
			fitbitdata[p].floors,
			fitbitdata[p].heartrate,
			fitbitdata[p].steps,
			sleepLevelPrinter
		);
		printf("%s,%s,%f,%f,%d,%d,%d,%d\n",					//printing to the console
			fitbitdata[p].patient,
			fitbitdata[p].minute,
			fitbitdata[p].calories,
			fitbitdata[p].distance,
			fitbitdata[p].floors,
			fitbitdata[p].heartrate,
			fitbitdata[p].steps, 
			sleepLevelPrinter
		);
	}
}
