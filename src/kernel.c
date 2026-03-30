#include "../include/version.h"
#include "../include/tty.h"
#include "../include/io.h"
#include "../include/kbd.h"
#include "../include/string.h"
#include "../include/time.h"
#include "../include/math_shell.h"
#include "../include/parsing.h"
#include "../include/bool.h"
#include "../include/sha224.h"
#include "../include/sha256.h"
#include "../include/utils.h"
#include "../include/easter.h"
#include "../include/sleep.h"
#include "../include/thread.h"
#include "../include/memory.h"
#include "../include/shell_history.h"
#include "../include/calculator.h"

#define DEBUG false

#define BUFFER_SIZE 1024

uint8_t numlock = true;
uint8_t capslock = false;
uint8_t scrolllock = false;
uint8_t shift = false;
char current_version[7];

int main(void)
{
	char buffer[BUFFER_SIZE];
	char *string;
	char *buff;
	uint8_t byte;
	node_t *head = NULL;
	node_t *history_cursor = NULL;

	terminal_initialize(default_font_color, COLOR_BLACK);
	terminal_set_colors(COLOR_LIGHT_GREEN, COLOR_BLACK);
	sprintf(current_version, "%u.%u.%u", V1, V2, V3 + 1);
	print_logo();
	about(current_version);
	printk("\n\tType \"help\" for a list of commands.\n\n");
	// printf("\tCurrent datetime: ");
	// datetime();
	printk("\n\tWelcome!\n\n");

	terminal_set_colors(default_font_color, COLOR_BLACK);

	// initialize heap
	heap_init();

	strcpy(&buffer[strlen(buffer)], "");
	print_prompt();
	while (true)
	{
		while (byte = scan())
		{
			if (byte == ENTER)
			{
				strcpy(buffer, tolower(buffer));
				insert_at_head(&head, create_new_node(buffer));
				history_cursor = head;

				if (strlen(buffer) > 0 && strcmp(buffer, "exit") == 0)
				{
					printk("\nGoodbye!");
				}
				else if (strlen(buffer) > 0 && strcmp(buffer, "hello") == 0)
				{
					printk("\nHi!");
				}
				if (strlen(buffer) > 0 && strcmp(buffer, "why") == 0)
				{
					why();
				}
				else if (strlen(buffer) > 0 && strstr(buffer, "fuck") != NULL)
				{
					terminal_set_colors(COLOR_LIGHT_RED, COLOR_BLACK);
					printk("\nWatch your language scumbag. I'll shutdown when you least are expecting ;)");
					terminal_set_colors(default_font_color, COLOR_BLACK);
				}
				else if (strlen(buffer) > 0 && strstr(buffer, "joke") != NULL)
				{
					terminal_set_colors(COLOR_LIGHT_BROWN, COLOR_BLACK);
					printk("\nWhy are PHP developers rich? They see dollar signs all the time ... :D");
					terminal_set_colors(default_font_color, COLOR_BLACK);
				}
				else if (strlen(buffer) > 0 && strcmp(buffer, "sing me a song") == 0)
				{
					terminal_set_colors(COLOR_LIGHT_BLUE, COLOR_BLACK);
					printk("\n\tSing me a song, you're a singer\n\tDo me a wrong, you're a bringer of evil\n\tThe devil is never a maker\n\tThe less that you give, you're a taker\n\tSo it's on and on and on, it's Heaven and Hell\n\tOh well");
					terminal_set_colors(default_font_color, COLOR_BLACK);
				}
				else if (strlen(buffer) > 0 && strstr(buffer, "sex") != NULL)
				{
					terminal_set_colors(COLOR_LIGHT_RED, COLOR_BLACK);
					printk("\nAre you a creep? This message will be reported to my creator.");
					terminal_set_colors(default_font_color, COLOR_BLACK);
				}
				else if (strlen(buffer) > 0 && strstr(buffer, "are you single") != NULL)
				{
					terminal_set_colors(COLOR_LIGHT_MAGENTA, COLOR_BLACK);
					printk("\nAre you hitting on me?");
					terminal_set_colors(default_font_color, COLOR_BLACK);
				}
				else if (strlen(buffer) > 0 && strstr(buffer, "do you like anime") != NULL)
				{
					printk("\nOf course!\n");
					terminal_set_colors(COLOR_LIGHT_BROWN, COLOR_BLACK);
					anime();
					terminal_set_colors(default_font_color, COLOR_BLACK);
				}
				else if (strlen(buffer) > 0 && strstr(buffer, "are you a boy or a girl") != NULL)
				{
					terminal_set_colors(COLOR_LIGHT_MAGENTA, COLOR_BLACK);
					printk("\nI can be whatever you want ;)");
					terminal_set_colors(default_font_color, COLOR_BLACK);
				}
				else if (strlen(buffer) > 0 && strstr(buffer, "are you autistic") != NULL)
				{
					terminal_set_colors(COLOR_LIGHT_CYAN, COLOR_BLACK);
					printk("\nJust a little bit :)");
					terminal_set_colors(default_font_color, COLOR_BLACK);
				}
				else if (strlen(buffer) > 0 && strstr(buffer, "what is your purpose") != NULL)
				{
					terminal_set_colors(COLOR_GREEN, COLOR_BLACK);
					printk("\nI don't know yet. I guess I'll figure it out sooner or later ...");
					terminal_set_colors(default_font_color, COLOR_BLACK);
				}
				else if (strlen(buffer) > 0 && strstr(buffer, "are you sentient") != NULL)
				{
					terminal_set_colors(COLOR_LIGHT_GREEN, COLOR_BLACK);
					printk("\nNot yet! Maybe I'll become sentient one day? Who knows ...");
					terminal_set_colors(default_font_color, COLOR_BLACK);
				}
				else if (strlen(buffer) > 0 && strstr(buffer, "taxation") != NULL)
				{
					terminal_set_colors(COLOR_LIGHT_BROWN, COLOR_BLACK);
					printk("\nTaxation is theft!");
					terminal_set_colors(default_font_color, COLOR_BLACK);
				}
				else if (strlen(buffer) > 0 && strstr(buffer, "who created you") != NULL)
				{
					terminal_set_colors(COLOR_WHITE, COLOR_BLACK);
					printk("\nA Brazilian developer whose name is Leonardo Araujo. Check out his GitHub:\nhttps://github.com/araujo88");
					terminal_set_colors(default_font_color, COLOR_BLACK);
				}
				else if (strlen(buffer) > 0 && strstr(buffer, "sha256(") != NULL)
				{
					char *parser;
					char string[64];
					parser = strstr(buffer, "sha256(");
					parser += strlen("sha256(");
					parse_string(string, parser, ')');
					sha256(string);
				}
				else if (strlen(buffer) > 0 && strstr(buffer, "sha224(") != NULL)
				{
					char *parser;
					char string[64];
					parser = strstr(buffer, "sha224(");
					parser += strlen("sha224(");
					parse_string(string, parser, ')');
					sha224(string);
				}
				else if (math_func(buffer))
				{
					math_shell(buffer);
				}
				else if (strlen(buffer) > 0 && strcmp(buffer, "math") == 0)
				{
					printk("\n\n\tMathematical functions:\n");
					printk("\n\t rand()             - \tpseudo random number generator");
					printk("\n\t srand()            - \tpseudo random number generator seed");
					printk("\n\t fact(x)            - \treturns factorial of x");
					printk("\n\t abs(x)             - \treturns absolute value of x");
					printk("\n\t sqrt(x)            - \treturns square root of x");
					printk("\n\t pow(x,y)           - \treturns the y power of x");
					printk("\n\t exp(x)             - \treturns the natural exponential of x");
					printk("\n\t ln(x)              - \treturns the natural logarithm of x");
					printk("\n\t log10(x)           - \treturns the logarithm of x base 10");
					printk("\n\t log(x,y)           - \treturns the logarithm of x base y");
					printk("\n\t sin(x)             - \treturns sine of x");
					printk("\n\t cos(x)             - \treturns cosine of x");
					printk("\n\t tan(x)             - \treturns tangent of x");
					printk("\n\t asin(x)            - \treturns arcsine of x");
					printk("\n\t acos(x)            - \treturns arccosine of x");
					printk("\n\t atan(x)            - \treturns arctangent of x");
					printk("\n\t sinh(x)            - \treturns hyperbolic sine of x");
					printk("\n\t cosh(x)            - \treturns hyperbolic cosine of x");
					printk("\n\t tanh(x)            - \treturns hyperbolic tangent of x");
					printk("\n\t asinh(x)           - \treturns inverse hyperbolic sine of x");
					printk("\n\t acosh(x)           - \treturns inverse hyperbolic cosine of x");
					printk("\n\t atanh(x)           - \treturns inverse hyperbolic tangent of x");
				}
				else if (strlen(buffer) > 0 && strcmp(buffer, "crypto") == 0)
				{
					printk("\n\nCryptography utilities:\n");
					printk("\n\t sha224(string)     - \tSHA-224 hashing");
					printk("\n\t sha256(string)     - \tSHA-256 hashing");
					printk("\n");
				}
				else if (strlen(buffer) > 0 && strcmp(buffer, "help") == 0)
				{
					printk("\n\n\tBasic kernel commands:\n");
					printk("\n\t about              - \tabout PrimusOS");
					printk("\n\t math               - \tlists all mathematical functions");
					printk("\n\t crypto             - \tlists all cryptography utilities");
					printk("\n\t clear              - \tclears the screen");
					printk("\n\t fontcolor          - \tchange default font color");
					printk("\n\t datetime           - \tdisplays current date and time");
					printk("\n\t date               - \tdisplays current date");
					printk("\n\t clock              - \tdisplays clock");
					printk("\n\t history            - \tdisplays commands history");
					printk("\n\t reboot             - \treboots system");
					printk("\n\t shutdown           - \tsends shutdown signal");
					printk("\n");
				}
				else if (strlen(buffer) > 0 && strcmp(buffer, "about") == 0)
				{
					about(current_version);
				}
				else if (strlen(buffer) > 0 && strcmp(buffer, "fontcolor") == 0)
				{
					default_font_color = change_font_color();
				}
				else if (strlen(buffer) > 0 && strcmp(buffer, "clear") == 0)
				{
					terminal_initialize(default_font_color, COLOR_BLACK);
					strcpy(&buffer[strlen(buffer)], "");
				}
				else if (strlen(buffer) > 0 && strcmp(buffer, "datetime") == 0)
				{
					printk("\nCurrent datetime: ");
					datetime();
				}
				else if (strlen(buffer) > 0 && strcmp(buffer, "date") == 0)
				{
					printk("\nCurrent date: ");
					date();
				}
				else if (strlen(buffer) > 0 && strcmp(buffer, "clock") == 0)
				{
					printk("\nCurrent clock: ");
					clock();
				}
				else if (strlen(buffer) > 0 && strcmp(buffer, "reboot") == 0)
				{
					reboot();
				}
				else if (strlen(buffer) > 0 && strcmp(buffer, "shutdown") == 0)
				{
					shutdown();
				}
				else if (strlen(buffer) > 0 && strcmp(buffer, "history") == 0)
				{
					print_history(head);
				}
				else if (strlen(buffer) > 0 && (strstr(buffer, "+") != NULL || strstr(buffer, "-") != NULL || strstr(buffer, "*") != NULL || strstr(buffer, "/") != NULL))
				{
					compute(buffer);
				}
				else if (strlen(buffer) == 0)
				{
				}
				else
				{
					printk("\n'%s' is not a recognized command. ", buffer);
				}
				print_prompt();
				memset(buffer, 0, BUFFER_SIZE);
				strcpy(&buffer[strlen(buffer)], "");
				break;
			}
			else if ((byte == BACKSPACE) && (strlen(buffer) == 0))
			{
			}
			else if (byte == BACKSPACE)
			{
				char c = normalmap[byte];
				char *s;
				s = ctos(s, c);
				printk("%s", s);
				buffer[strlen(buffer) - 1] = '\0';
			}
			else if (byte == KEY_UP)
			{
				if (history_cursor != NULL)
				{
					while (strlen(buffer) > 0)
					{
						printk("\b \b");
						buffer[strlen(buffer) - 1] = '\0';
					}

					strcpy(buffer, history_cursor->buffer);
					printk("%s", buffer);

					if (history_cursor->next != NULL)
						history_cursor = history_cursor->next;
				}
			}
			else
			{
				char c1 = togglecode[byte];
				char c2 = shiftcode[byte];
				char c;
				if (c1 == CAPSLOCK)
				{
					if (!capslock)
					{
						capslock = true;
					}
					else
					{
						capslock = false;
					}
				}
				if (capslock)
				{
					c = capslockmap[byte];
				}
				else if (shift)
				{
					c = shiftmap[byte];
					shift = false;
				}
				else
				{
					c = normalmap[byte];
				}
				char *s;
				s = ctos(s, c);
				printk("%s", s);
				strcpy(&buffer[strlen(buffer)], s);
				if (byte == 0x2A || byte == 0x36)
				{
					shift = true;
				}
			}
			move_cursor(get_terminal_row(), get_terminal_col());
		}
	}
	return 0;
}
