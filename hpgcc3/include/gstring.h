//& *** (c) 2006-2011 The HPGCC3 Team ***
//& Claudio Lapilli
//& Ingo Blank
//&
//& This file is licensed under the terms and conditions of the
//& HPGCC3 license that is included with the source distribution.
//& *** (c) 2006-2011 The HPGCC3 Team ***

#ifndef GSTRING_H_
#define GSTRING_H_

#ifndef MEMBLOCK_H_
#include <memblock.h>
#endif

class gFont;

class gString : public MemBlock
{
	
	virtual void vTable();
public:
	
	
	int TextWidth(int Offset,gFont& f);
	int TextWidth(int Offset,int EndOffset,gFont& f);
	int PrevLine(int Offset);	// RETURN OFFSET OF START OF PREVIOUS LINE, OR -1 IF NO MORE LINES 
	int NextLine(int Offset);	// RETURN OFFSET OF START OF NEXT LINE, OR -1 IF NO MORE LINES
	int StartOfLine(int Offset);	// RETURN OFFSET OF START OF CURRENT LINE 
	int EndOfPrevLine(int Offset); // RETURN POINTER TO THE \n OR \r CHARACTER AT THE END OF PREVIOUS LINE
	int EndOfLine(int Offset); // RETURN OFFSET TO THE END OF CURRENT LINE
	int StartOfText();
	int EndOfText();
	int Strlen();
	int NumLines();
	int PrevWord(int Offset);
	int NextWord(int Offset);
	
	BOOL IsValidOffset(int Offset);
	char GetChar(int Offset);
	void InsertChar(int offset,char a);
	void Delete(int offset);
		
	gString();
	virtual ~gString();
};

#endif /*GSTRING_H_*/
