//& *** (c) 2006-2011 The HPGCC3 Team ***
//& Claudio Lapilli
//& Ingo Blank
//&
//& This file is licensed under the terms and conditions of the
//& HPGCC3 license that is included with the source distribution.
//& *** (c) 2006-2011 The HPGCC3 Team ***


#ifndef GUIAUX_H_
#define GUIAUX_H_

#ifndef GUI_H_
#include <gui.h>
#endif


class STDConsole {
public:


	virtual void vTable();
	
	gFont *Font;
	gApplication *App;
	gForm *Form;
	gSoftMenu *Menu;
	gEditBox *Editor;
	gLoopData *Loop;
	int MaxSize;
	volatile int LastKey;
	int volatile lock;
	HEVENT update_event;

	int put(const char *text);
	int puts(const char *text);
	int getchar();
	char *gets(char *str);
	void clear();
	void gotoxy(int x,int y);
	void wherexy(int *x,int*y);
	void clreol();

	void SetSizeLimit(int limit);
	void ConsoleBreak();
	void FlushAllAndClose();
	STDConsole();
	STDConsole(int fontstyle,int MaxSize);
	virtual ~STDConsole();
	
};



#endif /*GUIAUX_H_*/
