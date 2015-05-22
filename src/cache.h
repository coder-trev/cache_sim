/*
 *  File: cache.h
 *	Description: A class that simulates a cache. The associated classes
 *               CacheLine and CacheBlock.
 *               Cache contains calculated number of CacheLine objects
 *               that contain (associativity) number of CacheBlock objects.
 *
 *               Replacement Policy is set by repl data field.
 *               'l' = LRU  &&  f' = FIFO
 *
 *               Write Allocation is set by walloc data field.
 *               'a' = always  &&  'n' = never
 * 	             
 *	Author: Trevor Skelton
 */

#ifndef __CACHE_H__
#define __CACHE_H__

#include "cacheLine.h"

typedef unsigned int uint;

class Cache
{
	public:
		Cache(uint x, uint y, uint z, char a, char b);
		char read(uint tag, uint index);
		char write(uint tag, uint index);
		char instrFetch(uint tag, uint index);
		void replace(uint tag, uint index);
		void printCache() const;
		~Cache();
	
	private:
		uint m_Size, m_BlockSize, m_Assoc;
		char m_Repl, m_Walloc;
		uint m_NumLines;
		CacheLine** m_Lines;
};

/**************************************************************/
/*                      Implementations                       */
/**************************************************************/

/* Constructor */
Cache::Cache(uint x, uint y, uint z, char a, char b)
{
	// assign member values
	m_Size = x;
	m_BlockSize = y;
	m_Assoc = z;	
	m_Repl = a;
	m_Walloc = b;
	
	// calculate number of lines
	m_NumLines = m_Size / (m_Assoc * m_BlockSize);
	
	// allocate and fill array of CacheLine objects
	m_Lines = new CacheLine*[m_NumLines];	
	for(int i = 0; i < m_NumLines; i++)
	{
		m_Lines[i] = new CacheLine(m_Assoc);
	}
}
/**************************************************************/

/* 
 *	read(tag, index) - return indicating a hit/miss after a read access to
 *                     the cache and handle actions appropriately for hit/miss
 */
char Cache::read(uint tag, uint index)
{
	char hitMiss = '\0';	//  hit/miss flag
	if(m_Lines[index]->hasTagMatch(tag))
	{
		hitMiss = 'h';	// set flag
		// reset the matched block's counter if LRU policy
		if(m_Repl == 'l') m_Lines[index]->getTagMatchBlock(tag)->resetCounter();
		m_Lines[index]->incBlocks();	// increment all blocks in the given line
	}
	else
	{
		replace(tag, index);	// replace block on miss
		hitMiss = 'm';	// set flag
	}

	return hitMiss;
}
/**************************************************************/

/* 
 *	write(tag, index) - return indicating a hit/miss after a write access to
 *                      the cache and handle actions appropriately for hit/miss
 *
 *	NOTE: Dirty bit would need to be calculated here if added to the
 *  CacheBlock class for full capability
 */
char Cache::write(uint tag, uint index)
{
	char hitMiss = '\0';	// hit/miss flag
	if(m_Lines[index]->hasTagMatch(tag))
	{
		hitMiss = 'h';	// set flag
		// reset the matched block's counter if LRU policy
		if(m_Repl == 'l') m_Lines[index]->getTagMatchBlock(tag)->resetCounter();
		m_Lines[index]->incBlocks();	// increment all block in the given line	
	}
	else
	{
		if(m_Walloc == 'a')
		{
			// replace block on miss if write allocation is always
			replace(tag, index);				
		}
		hitMiss = 'm';	// set flag
	}

	return hitMiss;
}
/**************************************************************/

/* 
 *	instrFetch(tag, index) - return indicating a hit/miss after an instruction
 *                           fetch access to the cache and handle actions 
 *                           appropriately for hit/miss
 *
 *	NOTE: Currently implemented same as read accesss.  Can be changed to 
 *  extend the cache to have designated fetch implementation 
 */
char Cache::instrFetch(uint tag, uint index)
{
	char hitMiss = '\0';	// hit/miss flag
	if(m_Lines[index]->hasTagMatch(tag))
	{
		hitMiss = 'h';	// set flag
		// reset the matched block's counter if LRU policy
		if(m_Repl == 'l') m_Lines[index]->getTagMatchBlock(tag)->resetCounter();
		m_Lines[index]->incBlocks();	// increment all blocks in the given line
	}
	else
	{
		replace(tag, index);	// replace block on miss
		hitMiss = 'm';	// set flag
	}

	return hitMiss;
}
/**************************************************************/

/* replace(tag, index) - implements the replacing of a block with given policy */
void Cache::replace(uint tag, uint index)
{
	if(m_Assoc == 1)	// 1 block/line
	{
		// replace the only block on the line
		m_Lines[index]->getBlock(0)->setV(1);
		m_Lines[index]->getBlock(0)->setTag(tag);
	}
	else if(m_Assoc > 1)	// multiple blocks/line
	{
		CacheBlock* highC = NULL;
		CacheBlock* block = NULL;
		int max = 0; int tmp = 0;
		
		// check for an empty block in the line
		if(m_Lines[index]->hasEmptyBlock())
		{
			block = m_Lines[index]->getEmptyBlock();
			block->setV(1);
			block->setTag(tag);
			m_Lines[index]->incBlocks();	// increment all the blocks in the line
		}
		// if no empty blocks, replace the block with the highest counter
		// in the line
		else
		{
			// check all blocks in the line and find block with max counter
			for(int i = 0; i < m_Assoc; i++)
			{
				tmp = m_Lines[index]->getBlock(i)->getCounter();
				if(tmp > max) 
				{
					max = tmp;
					highC = m_Lines[index]->getBlock(i);	// block with max counter
				}
			}
			
			// replace the block and reset the counter for it
			highC->setV(1);
			highC->setTag(tag);
			highC->resetCounter();
			
			// increment all block's counters in the line
			for(int i = 0; i < m_Assoc; i++)
			{
				block = m_Lines[index]->getBlock(i);
				block->incCounter();
			}
		}		
	}
}
/**************************************************************/

/* printCache() - prints the lines in the cache */
void Cache::printCache() const
{
	for(int i = 0; i < m_NumLines; i++)
	{
		printf("line %i: \n", i);
		m_Lines[i]->printLine();
	}
}
/**************************************************************/

/* Deconstructor */
Cache::~Cache()
{
	for(int i = 0; i < m_NumLines; i++)
	{
		if(m_Lines[i])
		{
			delete m_Lines[i];
			m_Lines[i] = NULL;
		}
	}
	
	delete [] m_Lines;
	m_Lines = NULL;
}
/**************************************************************/

#endif
