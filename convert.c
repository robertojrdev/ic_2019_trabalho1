#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

struct level
{
	int scoreToPass;
	int min_value;
	int max_value;
};

struct game
{
	int score;
	int rounds;
	int currentLevel;
	struct level levels[5];
	bool gameOn;
};

enum EndGameCause
{
	Win,
	RanOutOfRounds,
	Quit
};

#define MSG_WELCOME "WELCOME TO SORTIFY!"
#define MSG_SORT "Sort the following numbers:"
#define MSG_SORT2 "Please sort the numbers"
#define MSG_WELL "Well done!"
#define MSG_WIN "Congratulations, you win!"
#define MSG_OVER "Game Over."
#define MSG_WRONG "Wrong answer."
#define MSG_MAX "You have reached the maximum number of moves."
#define MSG_BYE "Bye."
#define MSG_UNKNOWN "Unknown option."
#define MAX_ROUNDS 30
#define INCREMENT_POINTS_ON_RIGHT_ANSWER 5
/* Use puts() to print constant strings */

int rand_number(const int, const int);
void print_status(const int, const int, const int);
void print_menu(void);

void initiate_game(struct game * r_game);
void read_inputs(struct game * r_game);
void next_round(struct game * r_game);
void get_question_numbers(struct level curLevel, int *values);
void finish_round(struct game * r_game, bool answeredCorrectly);
bool check_answer(int *question, int *answer);
void end_game(struct game * r_game, enum EndGameCause cause);
int comparision(const void *a, const void *b);

int main(int argc, char **argv)
{
	//start a new game
	struct game current_game;
	initiate_game(&current_game);

	//set random seed
	if (argc == 2)
	{
		char *c;
		int seed = strtol(argv[1], &c, 10);
		srand(seed);
	}
	else
	{
		srand(time(0));
	}

	//print msg and menu
	puts(MSG_WELCOME);
	print_menu();

	//game loop
	while (current_game.gameOn == true)
	{
		read_inputs(&current_game);
	}

	return 0;
}

//assign game values
void initiate_game(struct game * r_game)
{
	(*r_game).currentLevel = 0;
	(*r_game).rounds = 0;
	(*r_game).score = 0;
	(*r_game).gameOn = true;

	struct level * levels = (*r_game).levels;

	levels[0].scoreToPass = 10;
	levels[0].min_value = 0;
	levels[0].max_value = 10;

	levels[1].scoreToPass = 20;
	levels[1].min_value = 0;
	levels[1].max_value = 30;

	levels[2].scoreToPass = 30;
	levels[2].min_value = -50;
	levels[2].max_value = 30;

	levels[3].scoreToPass = 40;
	levels[3].min_value = -100;
	levels[3].max_value = 0;

	levels[4].scoreToPass = 50;
	levels[4].min_value = -200;
	levels[4].max_value = -100;
}

//read menu inputs
void read_inputs(struct game * r_game)
{
	char input;
	scanf(" %c", &input);

	switch (input)
	{
	case 'p': //next challenge
		next_round(r_game);
		break;

	case 'q': //quit
		end_game(r_game, Quit);
		break;

	case 'm': //info
		print_menu();
		break;

	case 's': //status
		print_status((*r_game).currentLevel, (*r_game).score, (*r_game).rounds);
		break;

	default: //unknown input
		puts(MSG_UNKNOWN);
		break;
	}
}

//play the next round
void next_round(struct game * r_game)
{
	//generate random numbers for the current level
	int questionsCount = 4;
	struct level curLevel = (*r_game).levels[(*r_game).currentLevel];
	int questions[questionsCount];
	get_question_numbers(curLevel, questions);

	//print question to player
	puts(MSG_SORT);
	printf("%d, %d, %d, %d\n", questions[0], questions[1], questions[2], questions[3]);

	//read answer
	int answer[questionsCount];
	scanf("%d %d %d %d", &answer[0], &answer[1], &answer[2], &answer[3]);

	//check if answered correctly and finish round
	bool answeredCorrectly = check_answer(questions, answer);
	finish_round(r_game, answeredCorrectly);
}

//finish round, update status and check for end game
void finish_round(struct game * r_game, bool answeredCorrectly)
{
	//show message
	char *message = answeredCorrectly == true ? MSG_WELL : MSG_WRONG;
	puts(message);

	//update score
	(*r_game).score += answeredCorrectly == true ? INCREMENT_POINTS_ON_RIGHT_ANSWER : 0;

	//update level
	if ((*r_game).score > (*r_game).levels[(*r_game).currentLevel].scoreToPass)
		(*r_game).currentLevel++;

	//update rounds
	(*r_game).rounds++;

	//finish game if achieve the end of all levels or maximum rounds
	int levelAmmount = sizeof((*r_game).levels) / sizeof((*r_game).levels[0]);
	if ((*r_game).currentLevel >= levelAmmount)
		end_game(r_game, Win);
	else if ((*r_game).rounds >= MAX_ROUNDS)
		end_game(r_game, RanOutOfRounds);
}

//generate a random number for each values list index using level params
void get_question_numbers(struct level curLevel, int *values)
{
	int amount = sizeof(values);
	int i;
	for (i = 0; i < amount; i++)
	{
		values[i] = rand_number(curLevel.min_value, curLevel.max_value);
	}
}

//sort question array and then compare with answer, return true if values and order are equals
bool check_answer(int *question, int *answer)
{
	qsort(question, sizeof(question), sizeof(int), comparision);
	unsigned int i;
	for (i = 0; i < sizeof(question); i++)
	{
		if (question[i] != answer[i])
			return false;
	}

	return true;
}

//used to compare int values on qsort function
int comparision(const void *a, const void *b)
{
	//need to explicitly tell that a and b are int pointers and get their values
	return (*(int *)a - *(int *)b);
}

//finish game
void end_game(struct game * r_game, enum EndGameCause cause)
{
	switch (cause)
	{
	case Win:
		puts(MSG_WIN);
		print_status((*r_game).currentLevel, (*r_game).score, (*r_game).rounds);
		puts(MSG_OVER);
		break;

	case RanOutOfRounds:
		puts(MSG_MAX);
		print_status((*r_game).currentLevel, (*r_game).score, (*r_game).rounds);
		puts(MSG_OVER);
		break;

	case Quit:
		print_status((*r_game).currentLevel, (*r_game).score, (*r_game).rounds);
		puts(MSG_BYE);
		break;

	default:
		break;
	}

	(*r_game).gameOn = false;
}

/* generate a random integer between min and max */
int rand_number(const int min, const int max)
{
	if (max < min)
	{
		puts("Max must be larger than Min");
		exit(0);
	}
	int n = abs(max - min) + 1;
	return (rand() % n) + min;
}

/* print the game status */
void print_status(const int level, const int score, const int plays)
{
	puts("+-----------------------------+");
	printf("| level:  %02d                  |\n", level + 1);
	printf("| points: %02d                  |\n", score);
	printf("| plays:  %02d                  |\n", plays);
	puts("+-----------------------------+");
}

/* print the option menu */
void print_menu(void)
{
	puts("+-----------------------------+");
	puts("| SORTIFY                     |");
	puts("| p - next challenge          |");
	puts("| q - quit                    |");
	puts("| m - print this information  |");
	puts("| s - show your status        |");
	puts("+-----------------------------+");
}