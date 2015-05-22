/*
 *  File: cacheBlock.h
 *	Description: A class that simulates a block in a cache.
 *               It is associated with the CacheLine and Cache
 *               classes to simulate a cache.
 *               Block contains no data field.
 *               It contains a valid field, tag field, and a counter
 *               field.
 *               
 *	NOTE: Dirty bit data field left out. Can easily be added for full
 *  cache design
 * 
 *	Author: Trevor Skelton
 */

#ifndef __CACHEBLOCK_H__
#define __CACHEBLOCK_H__

#include <sstream>
#include <string>

using namespace std;

typedef unsigned int uint;

class CacheBlock
{
	public:
		CacheBlock(uint x=0, uint y=0, uint c=0) : m_V(x), m_Tag(y), m_Counter(c) {}
		void setV(uint x) {m_V=x;}
		void setTag(uint x) {m_Tag=x;}
		void resetCounter() {m_Counter=0;}
		void incCounter() {m_Counter++;}
		uint getV() const {return m_V;}
		uint getTag() const {return m_Tag;}
		int getCounter() const {return m_Counter;}
		void printBlock() const;
		
	private:
		uint m_V, m_Tag;
		int m_Counter;
};

/**************************************************************/
/*                      Implementations                       */
/**************************************************************/

/* printBlock() - prints out the contents of a block */
void CacheBlock::printBlock() const
{
	printf("v: %d tag: %d counter: %d\n", m_V, m_Tag, m_Counter);
}
/**************************************************************/

#endif
