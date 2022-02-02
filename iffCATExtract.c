#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <libiff/iff.h>
#include <libiff/id.h>
#include <libiff/cat.h>

#define IFFCATEXTRACT_VERSION "1.0.0"

static void usage(void)
{
	fprintf(stderr,
			"iffCATExtract %s\n"
			"\n"
			"Usage: iffCATExtract <input.iff> <outputDir>\n"
			"  -h, --help              Output this help and exit\n"
			"  -V, --version           Output version and exit\n"
			"\n", IFFCATEXTRACT_VERSION);
	exit(EXIT_FAILURE);
}

char * inputFilePath=0;
char * outputDirPath=0;

char * strchrtrim(char * str, char letter)
{
    if(!str)
        return str;

    while(*str==letter && *str)
	{
        strcpy(str, str+1);
	}

	return str;
}

char * strrchrtrim(char * str, char letter)
{
    if(!str)
       	return str;

    while(str[(strlen(str)-1)] == letter && strlen(str))
	{
        str[(strlen(str)-1)] = '\0';
	}

    return str;
}


static void parse_options(int argc, char **argv)
{
	int i;

	for(i=1;i<argc;i++)
	{
		//int lastarg = i==argc-1;

		if(!strcmp(argv[i],"-h") || !strcmp(argv[i], "--help"))
		{
			usage();
		}
		else if(!strcmp(argv[i],"-V") || !strcmp(argv[i], "--version"))
		{
			printf("iffCATExtract %s\n", IFFCATEXTRACT_VERSION);
			exit(EXIT_SUCCESS);
		}
		else
		{
			break;
		}
	}

	argc -= i;
	argv += i;

	if(argc<2)
		usage();

	inputFilePath = argv[0];
	outputDirPath = argv[1];
}

int main(int argc, char ** argv)
{
	parse_options(argc, argv);

	FILE * fd = fopen(inputFilePath, "r");
	printf("a\n");
	if(fd==0)
	{
		fprintf(stderr, "Failed to open input file [%s] Error: %s\n", inputFilePath, strerror(errno));
		return EXIT_FAILURE;
	}

	IFF_Chunk * chunk = IFF_readFd(fd, NULL, 0);
    if(!chunk)
	{
		fclose(fd);
		fprintf(stderr, "Failed to read initial chunk\n");
		return EXIT_FAILURE;
	}

	if(IFF_compareId(chunk->chunkId, "CAT ")!=0)
	{
		IFF_free(chunk, NULL, 0);
		fclose(fd);
		fprintf(stderr, "IFF is not a CAT file [%c%c%c%c]\n", chunk->chunkId[0], chunk->chunkId[1], chunk->chunkId[2], chunk->chunkId[3]);
		return EXIT_FAILURE;
	}

	IFF_CAT * cat = (IFF_CAT*)chunk;
	char filePath[2048];

	for(unsigned int i=0;i<cat->chunkLength;i++)
	{
		IFF_Chunk * subChunk = cat->chunk[i];
		sprintf(filePath, "%s/%05d_%c%c%c%c.iff", outputDirPath, i, subChunk->chunkId[0], subChunk->chunkId[1], subChunk->chunkId[2], subChunk->chunkId[3]);
		IFF_write(filePath, subChunk, NULL, 0);
	}

	IFF_free(chunk, NULL, 0);
	fclose(fd);
}
