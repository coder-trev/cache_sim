/*
 *	File: cacheLine.h
 *	Description: A class the simulates of line in a cache.
 *               It is associated with the Cache and CacheBlock
 *               classes to simulate a cache.
 *               Line contains (associativity) number of CacheBlock objects.
 *               It contains the associativity field and a dynamic array
 *               of CacheBlock objects.
 *
 *	Author: Trevor Skelton
 */

#ifndef __CACHELINE_H__
#define __CACHELINE_H__

#include "cacheBlock.h"

typedef unsigned int uint;

class CacheLine
{
	public:
		CacheLine(uint a);
		CacheBlock* getBlock(int i) const;
		CacheBlock* getEmptyBlock() const;
		CacheBlock* getTagMatchBlock(uint tag) const;
		bool hasEmptyBlock() const;
		bool hasTagMatch(uint tag) const;
		void incBlocks();
		void printLine() const;
		~CacheLine();
		
	private:
		uint m_Assoc;
		CacheBlock** m_Blocks;
};

/**************************************************************/
/*                      Implementations                       */
/**************************************************************/

/* Constructor */
CacheLine::CacheLine(uint a)
{
	// assign member values
	m_Assoc = a;
	
	// allocate and fill array of CacheBlock objects
	m_Blocks = new CacheBlock*[m_Assoc];			
	for(int i = 0; i < m_Assoc; i++)
	{
		m_Blocks[i] = new CacheBlock();
	}
}
/**************************************************************/

/* getBlock(i) - returns a reference to a block in the line at given index */
CacheBlock* CacheLine::getBlock(int i) const
{
	return m_Blocks[i];
}
/**************************************************************/

/* 
 *	getEmptyBlock() - returns a reference to a block in the line with a valid
 *                    bit of zero
 */
CacheBlock* CacheLine::getEmptyBlock() const
{
	CacheBlock* block = NULL;
	
	// check all blocks in the line
	for(int i = 0; i < m_Assoc; i++)
	{
		block = m_Blocks[i];	// get block in the line
		if(block->getV() == 0) return block;	// return if valid is zero
	}
		
	block = NULL;
	return block;
}
/**************************************************************/

/* 
 *	getTagMathBlock(tag) - returns a reference to a block in the line
 *                              that matches a given tag value
 */
CacheBlock* CacheLine::getTagMatchBlock(uint tag) const
{
	CacheBlock* block = NULL;
	
	// check all blocks in the line
	for(int i = 0; i < m_Assoc; i++)
	{
		block = m_Blocks[i];	// get block in the line
		if(block->getTag() == tag) return block;	// return block with tag match
	}
	
	block = NULL;
	return block;
}
/**************************************************************/

/* 
 *	hasEmptyBlock() - returns indicating if the line has a block with valid bit
 *                    as zero
 */
bool CacheLine::hasEmptyBlock() const
{
	CacheBlock* block = NULL;
	
	// check all blocks in the line
	for(int i = 0; i < m_Assoc; i++)
	{
		block = m_Blocks[i];	// get the block in the line
		if(block->getV() == 0) return true;	// return true if valid is zero
	}
	
	return false;
}
/**************************************************************/

/* 
 *	hasTagMatch(tag) - return indicating if the line has a block that
 *                          matches a given tag
 */
bool CacheLine::hasTagMatch(uint tag) const
{
	CacheBlock* block = NULL;
	
	// check all blocks in the line
	for(int i = 0; i < m_Assoc; i++)
	{
		block = m_Blocks[i];	// get the block in the line
		// return true if a tag match is found
		if(block->getV() == 1 && block->getTag() == tag) return true;
	}
	
	return false;
}
/**************************************************************/

/* incBlocks() - increments all the block's counters in the line */
void CacheLine::incBlocks()
{
	CacheBlock* block = NULL;
	
	// check all blocks in the line
	for(int i = 0; i < m_Assoc; i++)
	{
		block = m_Blocks[i];	// get the block in the line
		// increment the block's counter if it is valid
		if(block->getV() == 1) block->incCounter();	
	}
}
/**************************************************************/

/* printLine() - prints the blocks in the line */
void CacheLine::printLine() const
{
	for(int i = 0; i < m_Assoc; i++)
	{
		printf("\tblock %i: ", i);
		m_Blocks[i]->printBlock();
	}
}
/**************************************************************/

/* Deconstructor */
CacheLine::~CacheLine()
{	
	for(int i = 0; i < m_Assoc; i++)
	{
		if(m_Blocks[i])
		{
			delete m_Blocks[i];
			m_Blocks[i] = NULL;
		}
	}
	
	delete [] m_Blocks;
	m_Blocks = NULL;
}
/**************************************************************/

#endif
