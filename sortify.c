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

enum EndGameCause
{
	Win,
	RunOutOfRounds,
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

void next_round();
void finish_round(bool answeredCorrectly);
void create_levels();
void get_question_numbers(struct level curLevel, int *values);
bool check_answer(int *question, int *answer);
void update_values(bool correctAnswer);
void end_game(enum EndGameCause cause);
int comparision(const void *a, const void *b);
void read_inputs();

int score = 0;
int rounds = 0;
int currentLevel = 0;
struct level levels[5];
bool gameOn = true;

int main(int argc, char **argv)
{
	//set random seed with time
	srand(time(0));
	create_levels();

	puts(MSG_WELCOME);
	print_menu();

	while (gameOn == true)
	{
		read_inputs();
	}

	return 0;
}

void create_levels()
{
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

void read_inputs()
{
	char input;
	scanf(" %c", &input);

	switch (input)
	{
	case 'p': //next challenge
		next_round();
		break;
	
	case 'q': //quit
		end_game(Quit);
		break;
	
	case 'm': //info
		print_menu();
		break;
	
	case 's': //status
		print_status(currentLevel, score, rounds);
		break;
	
	default:
		puts(MSG_UNKNOWN);
		break;
	}
}

void next_round()
{
	int questionsCount = 4;
	struct level curLevel = levels[currentLevel];
	int questions[questionsCount];
	get_question_numbers(curLevel, questions);

	puts(MSG_SORT);
	printf("%d, %d, %d, %d\n", questions[0], questions[1], questions[2], questions[3]);

	int answer[questionsCount];
	scanf("%d %d %d %d", &answer[0], &answer[1], &answer[2], &answer[3]);

	bool answeredCorrectly = check_answer(questions, answer);
	finish_round(answeredCorrectly);
}

void finish_round(bool answeredCorrectly)
{
	//show message
	char * message = answeredCorrectly == true ? MSG_WELL : MSG_WRONG;
	puts(message);

	update_values(answeredCorrectly);

	//update rounds
	rounds++;

	//finish game if achieve the end of all levels or maximum rounds
	int levelAmmount = sizeof(levels) / sizeof(levels[0]);
	if (currentLevel >= levelAmmount)
		end_game(Win);
	else if (rounds >= MAX_ROUNDS)
		end_game(RunOutOfRounds);
}

void get_question_numbers(struct level curLevel, int *values)
{
	int amount = sizeof(values);
	int i;
	for (i = 0; i < amount; i++)
	{
		values[i] = rand_number(curLevel.min_value, curLevel.max_value);
	}
}

bool check_answer(int *question, int *answer)
{
	qsort(question, sizeof(question), sizeof(int), comparision);
	for (int i = 0; i < sizeof(question); i++)
	{
		if (question[i] != answer[i])
			return false;
	}

	return true;
}

void update_values(bool correctAnswer)
{
	//update score
	score += correctAnswer == true ? INCREMENT_POINTS_ON_RIGHT_ANSWER : 0;

	//update level
	if (score > levels[currentLevel].scoreToPass)
		currentLevel++;
}

void end_game(enum EndGameCause cause)
{
	switch (cause)
	{
	case Win:
		puts(MSG_WIN);
		print_status(currentLevel, score, rounds);
		puts(MSG_OVER);
		break;

	case RunOutOfRounds:
		puts(MSG_MAX);
		print_status(currentLevel, score, rounds);
		puts(MSG_OVER);
		break;

	case Quit:
		puts(MSG_BYE);
		break;

	default:
		break;
	}

	gameOn = false;
}

int comparision(const void *a, const void *b)
{
	//need to explicitly tell that a and b are int pointers and get their values
	return (*(int *)a - *(int *)b);
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
	printf("| level:  %02d                  |\n", level +1);
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