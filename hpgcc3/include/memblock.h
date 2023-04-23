//& *** (c) 2006-2011 The HPGCC3 Team ***
//& Claudio Lapilli
//& Ingo Blank
//&
//& This file is licensed under the terms and conditions of the
//& HPGCC3 license that is included with the source distribution.
//& *** (c) 2006-2011 The HPGCC3 Team ***


#ifndef MEMBLOCK_H_
#define MEMBLOCK_H_

// FLEXIBLE MEMORY BLOCK CLASS
class MemFrag {
public:
int Offset,Size,Used;
char *Data;
MemFrag *Prev,*Next;
BOOL Alloc(int size);
MemFrag();	// DEFAULT CONSTRUCTOR
~MemFrag();	// DEFAULT DESTRUCTOR
};

class MemBlock : MemFrag {
protected:
	MemFrag *Current,*Last;

	MemFrag *GetFrag(int offset);
	void ShiftOffset(MemFrag *block,int shift);
	void Consolidate(MemFrag *block);
	void MemMoveUp(int destoffset,char *src,int bytes);	// FROM EXTERNAL DATA BLOCK
	void MemMoveDn(int destoffset,char *src,int bytes);	// FROM EXTERNAL DATA BLOCK


public:

int TotalUsed;

int WastedSpace();

BOOL Append(int bytes,char *Data=NULL); // ADD BYTES AT THE END
void Shrink(int bytes); // REMOVE BYTES AT THE END
BOOL Insert(int offset,int bytes,char *Data=NULL);	// INSERT AT SPECIFIED OFFSET
void Delete(int offset,int bytes);		// REMOVE BYTES AT SPECIFIED OFFSET

inline char *GetPtr(int offset) { 
	return ((offset>=Current->Offset && offset<Current->Offset+Current->Used)?
			(Current->Data+offset-Current->Offset)
			: (Current=GetFrag(offset),(Current->Data+offset-Current->Offset)));
	};
int GetLowerLimit(int offset);
int GetUpperLimit(int offset);

void MemMove(int destoffset,int srcoffset, int bytes);	// INTERNAL MEMORY MOVEMENT
void MemMove(int destoffset,MemBlock &src,int srcoffset,int bytes);	// FROM EXTERNAL MemBlock OBJECT
void MemMove(int destoffset,char *src,int bytes);	// FROM EXTERNAL DATA BLOCK
void MemMove(char *dest,int srcoffset,int bytes);	// COPY TO EXTERNAL DATA BLOCK

MemBlock();	// CREATE A MEMORY BLOCK OF 0 BYTES
~MemBlock();
};




#endif /*MEMBLOCK_H_*/
