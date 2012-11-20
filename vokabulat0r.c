/*
 The contents of this file are subject to the Mozilla Public License				
 Version 1.1 (the "License"); you may not use this file except in					 
 compliance with the License. You may obtain a copy of the License at			 
 http://www.mozilla.org/MPL/																								
																																						
 Software distributed under the License is distributed on an "AS IS"				
 basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the		
 License for the specific language governing rights and limitations				 
 under the License.																												 
																																						
 Alternatively, the contents of this file may be used under the terms			 
 of the GNU Lesser General Public license (the	"LGPL License"), in which case the	
 provisions of LGPL License are applicable instead of those									
 above.																																		 
																																						
 For feedback and questions about my Files and Projects please mail me,		 
 Alexander Matthes (Ziz) , zizsdl_at_googlemail.com												 
*/

#define VERSION "0.1"
#define PRO_MARK_1 '\314'
#define PRO_MARK_2 '\201'
#define PRO_MARK "\314\201"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
int count = 0;

typedef struct sElement *pElement;
typedef struct sElement {
	char word[128];
	char pron[128];
	char pran[128];
	char mean[128];
	char whole_line[512];
	char was_saved;
	pElement before;
	pElement next;
} tElement;

pElement firstElement = NULL;
pElement lastElement = NULL;

typedef char unicode[10];

#define PATTERN_COUNT 73

const unicode pattern[PATTERN_COUNT] =
	{PRO_MARK," ","?",",","!","-","…",
	 "А","Б","В","Г","Д","Е", "Ж","З","И","Й","К","Л","М","Н","О","П","Р","С","Т","У","Ф", "Х", "Ц",   "Ч",  "Ш",      "Щ","Ъ", "Ы","Ь","Э", "Ю", "Я", "Ё",
	 "а","б","в","г","д","е", "ж","з","и","й","к","л","м","н","о","п","р","с","т","у","ф", "х", "ц",   "ч",  "ш",      "щ","ъ", "ы","ь","э", "ю", "я", "ё"};
const unicode solution[PATTERN_COUNT] =
	{PRO_MARK," ","?",",","!","-","…",
	 "A","B","W","G","D","E","Ch","S","I","J","K","L","M","N","O","P","R","ẞ","T","U","F","Ch","Ts","Tsch","Sch","Schtsch", "","Ui", "","Ä","Ju","Ja","Jo",
	 "a","b","w","g","d","e","ch","s","i","j","k","l","m","n","o","p","r","ß","t","u","f","ch","ts","tsch","sch","schtsch", "","ui", "","ä","ju","ja","jo"};

//АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯЁ
//абвгдежзийклмнопрстуфхцчшщъыьэюяё

int lastLength;

int testForPron(char* input)
{
	for (int i = 0; i < PATTERN_COUNT; i++)
	{
		const char* pat = pattern[i];
		int c;
		for (c = 0; pat[c]!= 0; c++)
			if (pat[c] != input[c])
				break;
		if (pat[c] == 0) //Match
		{
			lastLength = c;
			return i;
		}
	}
	//lastLength = 1;
	//return 1; //' '
}

void createPron(char* output,char* input)
{
	for (int i = 0; input[i] != 0; i+=lastLength)
		sprintf(output,"%s%s",output,solution[testForPron(&(input[i]))]);
}

void createPran(char* output,char* input)
{
	//Searching for the prouncing
	int pos = -1;
	int i;
	for (i = 0; input[i] != 0; i++)
		if (input[i] == PRO_MARK_1 && input[i+1] == PRO_MARK_2)
			pos = i;
	if (pos == -1)
	{
		sprintf(output,"%s",input);
		return;
	}
	for (i = 0; i < pos-1 && input[i] != 0; i++)
		if (input[i] == 'o')
			output[i] = 'a';
		else
			output[i] = input[i];
	for (; i < pos+2 && input[i] != 0; i++) //The marks
		output[i] = input[i];
	for (; input[i] != 0; i++)
		if (input[i] == 'o')
			output[i] = 'e';
		else
			output[i] = input[i];
}


pElement parse(char* line)
{
	pElement element = malloc(sizeof(tElement));
	if (firstElement)
		lastElement->next = element;
	else
		firstElement = element;
	element->next = NULL;
	element->before = lastElement;
	lastElement = element;
	
	element->word[0] = 0;
	element->pron[0] = 0;
	element->pran[0] = 0;
	element->mean[0] = 0;
	element->was_saved = 0;
	sprintf(element->whole_line,"%s",line);
	//Converting to cyrilic:
	//Searching \t or ' '
	char* found = strchr ( line, '\t');
	if (found)
		found[0] = 0;
	int add = 0;
	int i;
	for (i = 0;line[i]!=0;i++)
	{
		if (line[i] == '_')
		{
			element->word[i+add] = PRO_MARK_1;
			add++;
			element->word[i+add] = PRO_MARK_2;
		}
		else
			element->word[i+add] = line[i];
	}
	element->word[i+add] = 0;
	found++;
	while (found[0] == '\t')
		found++;
	for (i = 0; found[i]!='\n' && found[i]!=0;i++)
		element->mean[i] = found[i];
	element->mean[i] = 0;
	createPron(element->pron,element->word);
	createPran(element->pran,element->pron);
	count++;
	return element;
}

#define SHUFFLE_DEEP 5000

void shuffle()
{
	for (int i = 0; i < SHUFFLE_DEEP; i++)
	{
		/*pElement now = firstElement;
		for (int j = 0; j < count; j++)
		{
			if (now->before)
				printf("  %s",now->before->word);
			else
				printf("  0");
			printf("<-%s->",now->word);
			if (now->next)
				printf("%s\n",now->next->word);
			else
				printf("0\n");
			
			now=now->next;
		}*/
		int a = rand()%count;
		int b = rand()%count;
		if (a == b || abs(a-b) == 1)
			continue;
		pElement A,B;
		for (A = firstElement; a>0; A = A->next)
			a--;
		for (B = firstElement; b>0; B = B->next)
			b--;
		//Switching A and B.
		pElement Anext   = A->next;
		pElement Abefore = A->before;
		pElement Bnext   = B->next;
		pElement Bbefore = B->before;
		
		if (Anext)
			Anext->before = B;
		if (Abefore)
			Abefore->next = B;
		if (Bnext)
			Bnext->before = A;
		if (Bbefore)
			Bbefore->next = A;
		
		A->next   = Bnext;
		A->before = Bbefore;
		B->next   = Anext;
		B->before = Abefore;
		
		if (A->before == NULL)
			firstElement = A;
		if (B->before == NULL)
			firstElement = B;
		if (A->next == NULL)
			lastElement = A;
		if (B->next == NULL)
			lastElement = B;
	}
}

int main(int argc,char* args[])
{
	srand(time(NULL));
	printf("Welcome to \033[1;31mVokabulat0r\033[0m version %s.\n",VERSION);
	printf("Use: vokabulat0r file\n");
	int normal_way = 1;
	if (argc < 2)
	{
		printf("No file\n");
		return 1;
	}
	int i;
	for (i = 1; i < argc; i++)
	{
		FILE* file;
		file = fopen(args[i],"rt");
		if (!file)
		{
			printf("Can't open %s... So changing to other mode.\n",args[i]);
			normal_way = 0;
			continue;
		}
		printf("Reading %s and formating to cyrilic font\n",args[i]);
		char line[256];
		while (fgets(line, 256, file) != NULL)
		{
			pElement element = parse(line);
			printf("Added %s (%s, %s) = %s\n",element->word,element->pron,element->pran,element->mean);
		}
		fclose(file);
	}
	printf("Shuffling...");
	shuffle();
	printf("done\n");
	int a = 0;
	int b = 0;
	while (firstElement)
	{
		if (normal_way)
		{
			printf("%i of %i+%i: %s? ",a+1,count,b,firstElement->word);
			getchar();
			printf("\"%s\" (\"%s\")",firstElement->pron,firstElement->pran);
			getchar();
			printf("-> %s ",firstElement->mean);
		}
		else
		{
			printf("%i of %i+%i: %s? ",a+1,count,b,firstElement->mean);
			getchar();
			printf("-> %s (\"%s\", \"%s\") ",firstElement->word,firstElement->pron,firstElement->pran);
		}
		char input[256];
		scanf ("%s",input);  
		getchar();
		a++;
		pElement element = firstElement;
		firstElement = firstElement->next;
		if (input[0] == 'n')
		{
			if (element->was_saved == 0)
			{
				fprintf(stderr,"%s",element->whole_line);
				element->was_saved = 1;
			}
			lastElement->next = element;
			element->before = lastElement;
			element->next = NULL;
			lastElement = element;
			b++;
		}
	}
	return 0;
}
