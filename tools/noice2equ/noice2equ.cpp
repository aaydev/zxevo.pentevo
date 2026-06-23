
/* last update: 31.05.2022 savelij */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define DEFINE		"DEFINE"

// открытие файла
FILE *OpenFile(const char *filename, const char *mode)
{
	FILE *namebuf;

	if(!(namebuf = fopen(filename, mode)))
	{
		printf("file %s not found\n", filename);
		exit(1);
	}
	return namebuf;
}

// чтение файла
unsigned char *ReadFile(FILE *name, char *filename, unsigned int filesize)
{
	unsigned char *buf;

	if(!(buf = (unsigned char *)malloc(filesize)))
	{
		printf("memory allocation error\n");
		exit(1);
	}

	unsigned int rdsize = fread(buf, 1, filesize, name);
	if(rdsize != filesize)
	{
		printf("file %s reading error\n", filename);
		exit(1);
	}
	return buf;
}

// запись файла
int WriteFile(int size, FILE *dst, unsigned char *buff)
{
	return fwrite(buff, 1, size, dst);
}

// получение размера файла
int FileSize(FILE *p)
{
	int pos, size;

	pos = ftell(p);
	fseek(p, 0, SEEK_END);
	size = ftell(p);
	fseek(p, pos, SEEK_SET);
	return size;
}

// краткая встроенная помощь
void usage(void)
{
	printf("Converting *.noi format noice to block EQU\n");
	printf("Build: %s  %s\n", __DATE__, __TIME__);
	printf("Usage: noice2equ infile outfile\n");

	exit(0);
}

int main(int argc, char **argv)
{
	FILE *in;
	FILE *out;
	char rdBuf[200];
	char wrBuf[200];
	char *rdStat;
	char *inLine;

	if ( argc < 2 ) usage();

	in = OpenFile(argv[1], "rb");
	out = OpenFile(argv[2], "wb");

	do
	{
		rdStat = fgets(rdBuf, sizeof(rdBuf), in);
		inLine = strstr(rdBuf, DEFINE);
		if ( inLine != 0 )
		{
			int i = 0;
			int posName = 0;
			int posEqu = 0;

			while(1)
			{
				if ( rdBuf[i++] == ' ' ) break;
			}

			rdBuf[i-1] = 0;
			posName = i++;
			
			while(1)
			{
				if ( rdBuf[i++] == ' ' )
				{
					rdBuf[i-1] = 0;
					posEqu = i;
					break;
				}
			}
			
			while(1)
			{
				if ( rdBuf[i++] < ' ' )
				{
					rdBuf[i-1] = 0;
					break;
				}
			}

			strcpy(wrBuf, &rdBuf[posName]);
			strcat(wrBuf, "\tEQU\t");
			strcat(wrBuf, &rdBuf[posEqu]);
			strcat(wrBuf, "\r\n");

			WriteFile(strlen(wrBuf), out, (unsigned char *)wrBuf);
		}
	}
	while(rdStat != 0);

	fclose(in);
	fclose(out);

	return 0;
}
