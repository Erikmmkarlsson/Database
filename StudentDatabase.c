#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 100
#pragma warning(disable:4996) //Disable scanf warnings
#pragma warning(disable:6031)

/* Declaration of functions.*/
void menu();											
size_t addStudent();
int resizeArray(size_t n);
struct studentInformation getStudentInfo();
void modifyStudent();
size_t deleteStudent();
void printStudent(int index);
void searching();
int saveFile();
int loadFile();
int searchArray(char *thisSSN);
void viewStudents();

void programStatistics();


/*Declaration of globals*/
struct studentInformation *stud_Array;
size_t numStudents;

/*TODO: Include header file for this struct?*/
struct studentInformation
{
	char personalNumber[MAX];
	char name[MAX];
	char gender[MAX];
	char studyProgram[MAX];
	char age[MAX];
	char email[MAX];
};


int main()
{
	stud_Array = malloc(sizeof(struct studentInformation));
	if (stud_Array == NULL) // Checks for allocation fault
	{
		puts("Error in initial memory allocation.");
		return -1;
	}

	menu();

	free(stud_Array);

	return 0;
}


void menu()
{
	numStudents = 0;

	int choice = 0;	

	while (choice != 7)
	{
		printf("\nMENU\n1. Add a new student\n");
		printf("2. Modify existing student\n");
		printf("3. Delete\n");
		printf("4. Search\n");
		printf("5. Save\n");
		printf("6. Load\n");
		printf("7. Exit\n");
		printf("8. View all students\n");
		printf("Please enter a choice: ");

		scanf("%d", &choice);

		switch (choice)
		{
		case 1:
			printf("\nAdding a new student. Amount of students is %zd\n", numStudents); /*Extra info TODO:remove?*/

			numStudents = addStudent(); 

			printf("\nNew amount of students is %zd\n", numStudents); /*Extra*/
			printf("Your added student is a %s named %s, and aged %s.\n", /*Extra*/
				stud_Array[numStudents-1].gender, 
				stud_Array[numStudents-1].name, 
				stud_Array[numStudents-1].age); /* [numStudents -1] because array start at 0, but the number of students
													is atleast 1 if index 0 is filled.*/
			break;
		case 2:
			printf("\nModify existing student.\n");
			modifyStudent();

			break;
		case 3:
			printf("\nDelete existing student.\n");
			numStudents = deleteStudent();

			break;
		case 4:
			printf("\nSearch:\n");
			searching();

			break;
		case 5:
			saveFile();

			break;
		case 6:
			printf("\nLoading file will clear current database, are you sure you want to continue? [y/n]: ");
			getchar(); //Catching \n character in buffer

			if (getchar() == 'y')
			{
				loadFile();
			}
			else
			{
				printf("Loading file aborted.\n");
			}
			break;
		case 7: 
			printf("Exiting. \nDo you want to save current student database? [y/n]: ");
			getchar(); //Catching \n character

			if (getchar() == 'y') saveFile();
			
			break;
		case 8:
			printf("\nHere's all the students in the database:\n");
			viewStudents();

			break;
		default:
			printf("\nCouldn't get that. Try something else. ");
		}
		system("PAUSE");
	}
} 

void searching() 
{
	printf("1. Personal number\n");
	printf("2. Name\n");
	printf("3. Study Program\n");
	printf("4. Statistics\n");
	printf("Enter an option: ");

	char searchInput[MAX];

	int totalAmount = 0, males = 0, females = 0, found = 0, age = 0;
	float totalAge = 0;
	int choice = 0;
	size_t i;

	scanf("%d", &choice);

	switch (choice)
	{
	case 1:
		printf("\nEnter personal number: ");
		scanf("%s", searchInput);
		

		for (i = 0; i < numStudents; i++)
		{
			if (strcmp(searchInput, stud_Array[i].personalNumber) == 0)
			{
				printf("Student found:");
				printStudent(i);
				found = 1;
				break;
			}
		}
		if (found == 0) printf("Student not found.\n");
		break;

	case 2:
		printf("\nEnter name: ");
		getchar();
		scanf("%[^\n]s", searchInput);

		for (i = 0; i < numStudents; i++)
		{
			if (strcmp(searchInput, stud_Array[i].name) == 0 )
			{
				printf("\nStudent found:");
				printStudent(i);
				found = 1;
			}
		}
		if (found == 0) printf("Student not found.\n");
		break;

	case 3:
		printf("\nEnter Study program: ");
		getchar();
		scanf("%[^\n]s", searchInput);

		for (i = 0; i < numStudents; i++)
		{
			if (strcmp(searchInput, stud_Array[i].studyProgram) == 0)
			{
				printf("\nStudent found:");
				printStudent(i);
				found = 1;
			}
		}
		if (found == 0)	printf("Student not found.\n");
		break;

	case 4:
		printf("\nStatistics:\n");
		
		for (i = 0; i < numStudents; i++)
		{
			age = atoi(stud_Array[i].age);
			totalAge += age;
			
			if (strcmp("male", stud_Array[i].gender) == 0)
			{
				males++;
			}
			else if (strcmp("female", stud_Array[i].gender) == 0)
			{
				females++;
			}
		}
		printf("Total amount of students: %d\n", numStudents);
		printf("Amount of males: %d\n", males);
		printf("Amount of females: %d\n", females);
		printf("Average age: %.2f\n", totalAge/numStudents);

		programStatistics();
	}


}

void printStudent(int index)
{
	printf("\nName: %s\n", stud_Array[index].name);
	printf("Personal Number: %s\n", stud_Array[index].personalNumber);
	printf("age: %s\n", stud_Array[index].age);
	printf("email: %s\n", stud_Array[index].email);
	printf("gender: %s\n", stud_Array[index].gender);
	printf("Study program: %s\n", stud_Array[index].studyProgram);

}

size_t deleteStudent()
{
	char thisSSN[MAX];
	printf("Enter personal number: ");
	scanf("%s", &thisSSN);

	int index = searchArray(thisSSN);

	if (index != -1)
	{
		for (size_t i = index; i < numStudents; i++)
		{
			stud_Array[i] = stud_Array[i + 1];
		}
		printf("\nStudent %s deleted.\n", thisSSN);
		return numStudents - 1; /* Here we don't realloc the array as it's not necessary, this'll happen correctly when another student
								 is added, and since we use numStudent counter for all the functions to keep track of the size 
								 and display and writing& reading*/
	}
	else
	{
		printf("No such student could be found.\n");
		return numStudents;
	}
}

void programStatistics()
{
	int amountOfPrograms = 0;

	for (int i = 0; i < numStudents; i++)
	{
		char* temp = stud_Array[i].studyProgram;

		for (int j = i+1; j < numStudents; j++)
		{
			if (strcmp(temp, stud_Array[j].studyProgram) == 0)
			{
				amountOfPrograms++;
			}
		}
	}
}




void viewStudents()
{
		size_t i;
		for (i = 0; i < numStudents; i++) 
		{
			printf("Name: %s ", stud_Array[i].name);
			printf("SSN: %s \n", stud_Array[i].personalNumber);
			printf("program: %s \n", stud_Array[i].studyProgram);
		}

}

size_t addStudent()
{
	/*Whenever we call addStudent, we add another to the numStudent counter.
	The first resize doesn't do anything because numStudents starts at 0. 
	TODO: Maybe not do an unnecessary resize the first time?*/

	resizeArray(numStudents + 1);
	stud_Array[numStudents] = getStudentInfo(); /*not +1 because index starts at 0 */

	return numStudents + 1; 
}

int resizeArray(size_t n)
{
	struct studentInformation* temp;

	if (stud_Array != NULL)
	{
		temp = realloc(stud_Array, n * sizeof(struct studentInformation)); //To prevent C6308

		if (temp != NULL)
		{
			stud_Array = temp;
		}
		else if (temp == NULL)
		{
			printf("Error in memory reallocation.\n");
			return -1;
		}
	}

	return 0;
}

struct studentInformation getStudentInfo()
{
	struct studentInformation thisStudent;

	printf("\nPlease enter personal number: ");
	scanf("%s", thisStudent.personalNumber);

	getchar();	/*To solve buffer fail. The scanf on the line below fails with scanf above, because the newline
							 character in input is spewed on the below. See https://www.geeksforgeeks.org/problem-with-scanf-when-there-is-fgetsgetsscanf-after-it/ for ref*/

	printf("\nPlease enter Name: ");
	scanf("%[^\n]s", thisStudent.name);

	printf("\nPlease enter gender: ");
	scanf("%s", thisStudent.gender);

	getchar();
	printf("\nPlease enter study program: ");
	scanf("%[^\n]s", thisStudent.studyProgram);

	printf("\nPlease enter age: ");
	scanf("%s", thisStudent.age);

	printf("\nPlease enter email: ");
	scanf("%s", thisStudent.email);

	/*int personalNumber;
	char name[MAX];
	char gender[MAX];
	char studyProgram[MAX];
	int age;
	char email[MAX];*/

	return thisStudent;
}

void modifyStudent()
{
	printf("Please enter personal number: ");

	char thisSSN[MAX];
	scanf("%s", thisSSN);


	int index = searchArray(thisSSN); /* looking for the index of the student
											to be modified, if one exists*/

	if (index != -1)
	{
		printf("Student with personal number %s successfully found.\n", thisSSN);

		stud_Array[index] = getStudentInfo();

	}
	else
		printf("No such student could be found, please try again.\n");
}

int searchArray(char *thisSSN)
{

	for (size_t i = 0; i < numStudents; i++)
	{
		if (strcmp(thisSSN, stud_Array[i].personalNumber) == 0)
		{
			return i; /*Returns the index of the student with the right SSN. */
		}
	}
	return -1;
}

int saveFile()
{
	printf("Save file.\nPlease enter a filename (all data will be overwritten if the file already exists): ");
	char fileName[MAX];
	scanf("%s", fileName);

	strcat(fileName, ".txt");

	FILE* foo = fopen(fileName, "w");
	if (foo == NULL)
	{
		printf("Error opening file\n");
		return -1;
	}

	fprintf(foo, "%zd\n", numStudents); //Saves the amount of students

	for (size_t i = 0; i < numStudents; i++) {
		fprintf(foo, "%s\n%s\n%s\n%s\n%s\n%s\n",
			stud_Array[i].name,
			stud_Array[i].personalNumber,
			stud_Array[i].gender,
			stud_Array[i].studyProgram,
			stud_Array[i].email,
			stud_Array[i].age);
	}

	fclose(foo);

	printf("File %s saved.\n", fileName);
	return 0;
}

int loadFile()
{
	printf("Load file.\nPlease enter a filename: ");

	char fileName[MAX];
	scanf("%s", fileName);

	strcat(fileName, ".txt"); 

	FILE* bar = fopen(fileName, "r");
	if (bar == NULL)
	{
		printf("Error opening file\n");
		return -1;
	}

	fscanf(bar, "%zd\n", &numStudents); //Loads first line that tells the amount of students

	resizeArray(numStudents); //important to resize array based on how many students to load in

	for (size_t i = 0; i < numStudents; i++) 
	{
		fscanf(bar, "%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n",
			stud_Array[i].name,
			stud_Array[i].personalNumber,
			stud_Array[i].gender,
			stud_Array[i].studyProgram,
			stud_Array[i].email,
			stud_Array[i].age);
	}

	fclose(bar);

	printf("File loaded.\n");
	return 0;

}