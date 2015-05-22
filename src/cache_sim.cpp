/* 
 * 	Homework 6 - Cache Simulator
 * 
 *	File: cache_sim.cpp
 *	Description: Main program that is designed to simulate a cache using
 *               self-made Cache object. This is designed to parse a *.din
 *               trace file.
 *
 *  Accompanying Files: cache.h, cacheLine.h, cacheBlock.h
 *
 * 	Author: Trevor Skelton
 */
 
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include "cache.h"

using namespace std;

typedef unsigned int uint;

// prototypes
void Usage(char*  pname);
void doSim(char* , uint, uint, uint, char, char);

// globals
uint usize = 0;
uint ubsize = 0;
uint uassoc = 0;
char urepl = 'f';
char uwalloc = 'a';
uint numAccesses=0;uint numMisses = 0;

/* Main */
int main(int argc, char* argv[])
{
  char* filename;
  int i;

  // check for valid arguement number to proceed
  if (argc < 2) {
     Usage(argv[0]);
     exit(-1);
  }
  
  // get filename
  filename = argv[1];
  
  // parse arguments
  for (i = 2; i < argc; i++) {
     if (!strcmp(argv[i],"-l1-usize")){
        i++;
        usize = atoi(argv[i]);
     } else if (!strcmp(argv[i],"-l1-ubsize")){
        i++;
        ubsize = atoi(argv[i]);
      }
     else if (!strcmp(argv[i],"-l1-uassoc")){
        i++;
        uassoc = atoi(argv[i]);
      }
     else if (!strcmp(argv[i],"-l1-urepl")){
        i++;
        urepl = argv[i][0];
      }
     else if (!strcmp(argv[i],"-l1-uwalloc")){
        i++;
        uwalloc = argv[i][0];
      }
     else {
       printf("Ignoring urecognized option: %s\n",argv[i]);
     }
  }

  printf("Running with input: %s, l1-usize=%d, l1-ubsize=%d, l1-assoc=%d, l1-repl=%c, l1-uwalloc=%c \n",filename,usize,ubsize,uassoc,urepl,uwalloc);

	// function to do the cache simulator
  doSim(filename,usize,ubsize,uassoc,urepl,uwalloc);
  
  printf ("Demand Accesses  %d\n",numAccesses);
  printf ("Demand Misses %d\n",numMisses);
  return 0;
}
/**************************************************************/

/* Usage() - prints out the valid options for the user */
void Usage(char* pname) 
{
  printf("Usage: %s infile <options>\n",pname);
  printf( "Options:\n");
  printf( "<-l1-usize num_bytes>    : total size in bytes\n");
  printf( "<-l1-ubsize num_bytes>   : block size in bytes\n");
  printf( "<-l1-uassoc num_levels>   : associativity level\n");
  printf( "<-l1-urepl type>         : replacement policy, 'l' - LRU, 'r'-random, 'f' fifo\n");
  printf( "<-l1-uwalloc type>      : write allocation policy, 'a' - always, 'n'-never\n");
}
/**************************************************************/

/* 
 *	doSim(fname, size, bsize, assoc, repl, walloc) - parses the *.din file
 *                                                   and runs the cache
 *                                                   simulation
 */
void doSim(char*  fname, uint size, uint blockSize, uint assoc, char repl, char walloc)
{
	char hitMiss = '\0';	// hit/miss flag
	uint numCacheLines = size / (assoc * blockSize); // calculate number of lines
	printf("Number of cache lines is: %d\n", numCacheLines);	

	// instantiate a Cache object
	Cache* cache = new Cache(size, blockSize, assoc, repl, walloc);	
	
	// open file
	ifstream in;
	in.open(fname);
	if(!in.is_open())
	{
 		printf("Error: -File Does Not Exist-");
 		exit(-1);
	}
	// parse file
	else
	{
		uint lbl, addr;
		uint tag, blckAddr, cacheIndex;
		while(in >> lbl) // get access type
		{			
			in >> hex >> addr;	// get address
			in.ignore(255, '\n'); // skip the rest
			
			// extract tag and cache index
			tag = addr / (numCacheLines * blockSize);
			blckAddr = addr / blockSize;
			cacheIndex = blckAddr % numCacheLines;
			
			// access the cache
			if(lbl == 0) hitMiss = cache->read(tag, cacheIndex);
			else if(lbl == 1) hitMiss = cache->write(tag, cacheIndex);
			else if(lbl == 2) hitMiss = cache->instrFetch(tag, cacheIndex);
			
			// update accesses
			if(hitMiss == 'm') numMisses++;
			numAccesses++;
		}
		// close file
		in.close();
	}	
	
	// commment/uncomment to print out the cache
	//cache->printCache();
	
	delete cache; cache = NULL;
}
/**************************************************************/



