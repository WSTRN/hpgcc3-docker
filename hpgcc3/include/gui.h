//& *** (c) 2006-2011 The HPGCC3 Team ***
//& Claudio Lapilli
//& Ingo Blank
//&
//& This file is licensed under the terms and conditions of the
//& HPGCC3 license that is included with the source distribution.
//& *** (c) 2006-2011 The HPGCC3 Team ***


#ifndef GUI_H_
#define GUI_H_

#ifndef GGL_H_
#include <ggl.h>
#endif

#ifndef GSTRING_H_
#include <gstring.h>
#endif
#ifndef RESOURCES_H_
#include <resources.h>
#endif


/*!
 * \file gui.h
 * 
 * Graphical User Interface in greyscale.
 * 
 * The GUI module enables easier coding of programs that require user interaction.
 *  
 * 
 * \defgroup guimodule GUI module
 * 
 * For a more detailed explanation and basic concepts, see \ref guimoddocs.
 * 
 * \page guimoddocs The GUI in detail
 *
 * The GUI features grayscale graphics and keyboard interaction for easy coding of applications.
 *
 * \section overview Overview
 *
 * The GUI module handles graphics and user interaction. It is based in forms and controls, which are
 * implemented as C++ classes. Controls interact with the user and with other controls through events.
 * Events are messages distributed through the controls that can be used to identify certain conditions
 * and to react in consequence.
 * For clarity, this section is divided in:
 *
 * \subpage controls
 * 
 * \subpage events
 * 
 * \subpage advanced
 *
 * \page controls GUI controls overview
 * 
 * Controls are elements that perform some interaction with the user. The simplest controls display text 
 * or graphics and don't receive any input. Other more complex controls can receive user input, provide
 * visual feedback, and/or perform specialized tasks.
 * The GUI architecture is object oriented. Each control is represented by C++ classes that encapsulate
 * their basic behavior.
 * 
 * A complete list of controls follows:
 * 
 * \li \ref gApplication - Encapsulates the basic behavior of an application, initializing the hardware, 
 * reading the keyboard, generating events and distributing them among the controls. A program can only
 * contain one gApplication object.
 * \li \ref gControl - Encapsulates the common behavior of all controls. It doesn't have any particular
 * purpose other than being the base class for the other controls. It is not used unless you need to derive
 * your own controls from this class.
 * \li \ref gForm - A form is a universal container control. It can contain other controls and distribute
 * events among them. Always, user interaction happens within a form. A program may contain any number of
 *  forms. The user can switch between forms, but only one form can have the keyboard input at a time.
 * Forms can take the entire screen or appear as windows, and they can optionally display a caption and a
 * border.
 * \li \ref gSoftMenu - The soft menu control makes it possible to use soft menus similar to the calculator's
 * signature menus. This object is always created associated to a form. A form can have only
 * one soft menu. Soft menus can be dynamically updated at any time (i.e. have its items removed, replaced, etc.).
 * A soft menu can be single-line, just like the traditional calculator menus, or extended. Extended soft
 * menus display 3 lines of options, corresponding to keys from A through L for a total of 12 items
 * on-screen simultaneously.
 * \li \ref gStaticText - The static text control simply shows a given text. The text can be updated at any time.
 * \li \ref gStaticTextWrap - This is a more advanced static text that can word-wrap long text, becoming a
 * multiline static text control.
 * \li \ref gCheckBox - This simple control shows a check box and provides its behavior. The user can
 *  check/uncheck the box from the keyboard. The box can have several different styles. Unlike other GUI's,
 * this check box does not have text associated with it.
 * \li \ref gEditBox - This is the main text input control. It provides a space for the user to type text.
 * The control supports single-line and multiline behavior, automatic scrolling, and an optional border.
 * \li \ref gListBox - This control can display a list of items (with an optional tree structure), and
 * let the user select items from the list. Its features include expand/collapse support, check mark support,
 * auto scrolling and optional border.
 * 
 * 
 * Applications typically use arbitrary combinations of the above controls to achieve the desired behavior.
 * 
 * \section creatingcontrols Creating controls
 * 
 * The first control that needs to be created in any program is gApplication. A gApplication object must be
 * created when the program starts and destroyed only at program exit.
 * The simplest way to achieve it is by creating it in the main() function as follows:
 * 
 * \code
 * #include <hpgcc3.h>
 * #include <gui.h>
 * 
 * int main()
 * {
 * gApplication myapp;
 * // ... ADD YOUR PROGRAM HERE
 * }
 * \endcode
 * 
 * The next step is to create one or more forms. Forms require a parent object to be specified
 * in the constructor. The parent object of a gForm *MUST* be the application object. The simplest
 * constructor requires also a caption text and some style flags.
 * 
 * The following sample code creates an empty full screen form with a caption at the top (see 
 * other constructors at the gForm class documentation):
 * 
 * \code
 * #include <hpgcc3.h>
 * #include <gui.h>
 * 
 * int main()
 * {
 * gApplication myapp;
 * gForm myform(myapp,"Form test",S_TOPCAPTION);
 * 
 * // ... ADD MORE INITIALIZATION CODE HERE
 * 
 * return myapp.Run();
 * 
 * }
 * \endcode
 * 
 * The above example goes one step forward: it shows how to run the application message loop.
 * The member gApplication::Run starts the message pump for the recently created form. In case of multiple forms,
 * a form can be passed as an argument, making it the active form.
 * 
 * Notice that the gForm object should NOT be deleted (or run out of scope) before the Form is properly
 * closed. A form can be closed by calling gApplication::CloseForm(), which is automatically done when
 * a program calls gApplication::Terminate(). It is the user's responsibility to check the lifespan of
 * the objects, but it's safe to delete them after the Run() member returned.
 * 
 * Before executing the code example above we need to add one more thing: a way to close the form. Forms don't
 * have any means to close themselves, so the above program will simply run forever, or until interrupted
 * using On-A-F.
 * 
 * The next step is to provide that exit mechanism, which can be done through a soft menu.
 * The gSoftMenu object constructor requires a parent control, which must be a form (other controls don't provide
 * support for their own soft menus), and it also requires a style flag.
 * Once we created the empty softmenu, we need to add items to it. This is achieved by the gSoftMenu::AddItem member.
 * Adding an item is simple: just provide a short string of text to display, and the address of a function that will
 * be called every time the user hits the soft menu. If you add more than the items that fit on the screen, the NXT key
 * will begin to select pages, just like the standard calculator soft menus.
 * 
 * The function that is passed to AddItem is an event handler, and therefore it must follow certain rules in its declaration.
 * For more details see \ref events but for now we'll use one of the standard event handlers, which provide the behavior
 * we need. In this case, all the handler needs to do is to exit the application loop by calling gApplication::Terminate.
 *   
 * The complete sample code now becomes:
 * 
 * \code
 * #include <hpgcc3.h>
 * #include <gui.h>
 * 
 * int main()
 * {
 * gApplication myapp;
 * gForm myform(myapp,"Form test",S_TOPCAPTION);
 * gSoftMenu mymenu(myform,S_SINGLESOFTMENU);
 * 
 * mymenu.AddItem("EXIT",&gOKEvent); // USING STANDARD HANDLER gOKEvent
 * 
 * // ... ADD MORE INITIALIZATION CODE HERE
 * 
 * return myapp.Run();
 * 
 * }
 * \endcode
 * 
 * At this point, any number and combination of other controls can be created
 * within the form. All of them require a parent form in the constructor, the rest
 * of the arguments varies according to each class (see each individual class documentation).
 * Always the user is responsible for proper deletion of controls. Deleting a form will NOT
 * delete its controls automatically.
 * 
 * Many controls require coordinates to specify its size and location. Always, the coordinates
 * are measured in screen pixels (1 unit = 1 pixel on the screen). The origin (0,0) is always the
 * top-left corner of the parent object client area. In a form with a top caption, this is
 * right under the caption, to the left. Forms, whose parent is the application object,
 * have the origin at the top-left corner of the screen. The X coordinate runs positive to the right,
 * and the Y coordinate runs positive towards the bottom of screen.
 * 
 * Always keep in mind that the screen has 132 pixels wide x 80 pixels high when sizing your controls.
 * Forms can be larger than the screen, and the user can scroll through the controls.
 * 
 * As an example, the code below will display a "Hello GUI world" message by creating a gStaticText control:
 * 
 * \code
 * #include <hpgcc3.h>
 * #include <gui.h>
 *
 * int main()
 * {
 * gApplication myapp;
 * gForm myform(myapp,"Form test",S_TOPCAPTION);
 * gSoftMenu mymenu(myform,S_SINGLESOFTMENU);
 * 
 * mymenu.AddItem("EXIT",&gOKEvent); // USING STANDARD HANDLER gOKEvent
 * 
 * gFont SystemFont8(8);
 * 
 * gStaticText higuitext(myform,10,10,"Hello GUI",&SystemFont8,GBLACK);
 * 
 * // ... ADD MORE INITIALIZATION CODE HERE
 * 
 * return myapp.Run();
 * 
 * }
 * \endcode
 * 
 * This example introduced a couple of new concepts that are common to several controls.
 * 
 *  First, all controls that display text will require a pointer to a gFont object. The gFont object is
 * not a control, it is a data structure that encapsulates the proportional fonts. Any number of
 * gFont objects can be created, but they can be shared between controls so in many cases only one
 * or two will suffice. The constructor of the gFont class takes an integer as the only argument.
 * This integer is the font height in pixels of the system font. The GUI has a proportional version
 * of the calculator's standard System8, System7 and System6 fonts, as well as a tiny font (5 pixels high).
 * The example above creates a gFont object to refer to the System8 font, and then uses a pointer to
 * this object when creating the gStaticText control. See \ref advanced for details on loading and using
 * custom fonts.
 * 
 * The other concept introduced in this example is the colors. The last argument to the gStaticText constructor
 * is the text color. In the example, the constant GBLACK was used. To specify other colors use the constants
 * in the \ref gColors enum. See \ref advanced for more details in colors and graphics in general.
 * 
 * \section customcontrols Customizing controls
 * 
 * This section explains in detail all the constructors used by each class, as well as their styles and options.
 * 
 * \li \ref gForm
 * 
 * Form objects can be created using one of four constructors
 * The class constructors have the following form: 
 * 
 * \code
 * 	gForm::gForm(gControl& Parent,const char *_Caption,gFormStyles _Style);
 *	gForm::gForm(gControl& Parent,const char *_Caption,gFormStyles _Style,int clientwidth,int clientheight);
 *	gForm::gForm(gControl& Parent,const char *_Caption,gFormStyles _Style,int x,int y,int width,int height);
 *	gForm::gForm(gControl& Parent,const char *_Caption,gFormStyles _Style,int x,int y,int width,int height,int clientwidth,int clientheight);
 * \endcode
 * 
 * The first three arguments are common to all constructors. The arguments that follow determine how the form will be created.
 * The meaning of the arguments is simple: x,y,width,height are the position and size of the window in the parent's client area. When these
 * arguments are specified, the form is created as a window at the specified location. When these arguments are not specified,
 * the form takes the entire space in the parent's client area, becoming a full-screen form.
 * 
 * So it's easy to see that constructors can be divided in 2 groups: the first two are full-screen forms, the last 2 are windowed forms. The first
 * and third constructors are equivlent, and the second and fourth are similar, allowing an arbitrary size of client area (larger than visible on
 * the screen). When the second or fourth constructors are used, a scroll bar will appear in either direction as required.
 * 
 * On all constructors, the Parent argument must be the gApplication object. The _Caption argument is used to display a text, but only if the proper
 * style is selected (see below).
 * The _Style argument is a combination of style flags (combined with OR or by addition). The following style flags are supported for Forms:
 * 
 * \ref S_TOPCAPTION = Add a title bar with a caption text shown in 5-pixel tiny font.
 * \ref S_FORCEVSCROLL = Show a vertical scroll bar even if not needed.
 * \ref S_FORCEHSCROLL = Show a horizontal scroll bar even if not needed.
 * \ref S_FORMBORDER = Add a border line around the form (useful for windowed forms).
 * 
 * Notice that there are other flags in the \ref gFormStyles enum. Some are simply not implemented yet, and some are automatically set/cleared, and
 * the user doesn't need to specify those at creation.
 * 
 * \li \ref gSoftMenu
 * 
 * There are two constructors available for gSoftMenu:
 * 
 * \code
 * 	gSoftMenu::gSoftMenu(gControl& Parent,int Style);
 *	gSoftMenu::gSoftMenu(gControl& Parent,int Style,struct gSoftMenuItem *List, int numitems);
 * \endcode
 * 
 * In both constructors, the Parent control must be a gForm object. Only gForms are able to properly interact with soft menus.
 * The Style argument is one of the following flags:
 * 
 * \ref S_SINGLESOFTMENU = Create a standard single-line soft menu
 * \ref S_EXSOFTMENU = Create an extended 3-line soft menu
 * 
 * The single-line soft menu is a standard menu with 6 items. If more than 6 items are added to the menu, the NXT key performs
 * page switching. The extended soft menu generates a 12-item menu in 3 lines, corresponding to keys A thru L. If more than
 * 12 items are added to the menu, the NXT key becomes active. Notice that the last item in the menu will become NEXT on every page. 
 * The second constructor requires a list of items and the number of items in the list. This list should contain all the
 * items in the menu and its handlers. It is completely equivalent to using the first constructor and then calling
 * the member AddItemList. See \ref gSoftMenu::AddItemList for details on how to assemble the list.
 * 
 * \li \ref gStaticText
 * 
 * \section usingcontrols Using controls
 *
 * \page events GUI events overview
 * \page advanced Advanced GUI topics and internals

*/










// PHYSICAL SCREEN SIZE
#define SCREEN_W 160
#define SCREEN_H 80
// LOGICAL (VISIBLE) SCREEN SIZE
#define USERSCREEN_W 131
#define USERSCREEN_H 80

// STYLE DEFINITION CONSTANTS
#define CAPTIONHEIGHT 7
#define SOFTMENUHEIGHT 6
#define SCROLLBARWIDTH 3
#define BORDERWIDTH 1 

#define CURSORBLINKSPEED 40000


class gForm;
class gSoftMenu;
class gApplication;
class gControl;

// CLASS DEFINITIONS

struct gUpdate {
	int clipx,clipy,clipx2,clipy2; // CLIPPING RECTANGLE WITHIN THAT SURFACE
};


struct gEvent {
	unsigned int Type;	// MESSAGE
	unsigned int Arg;	// OPTIONAL ARGUMENT
	unsigned int Data;	// OPTIONAL DATA POINTER/INTEGER/ETC.
	
};

typedef BOOL (*gEvHandler)(gEvent *e,gControl *_This);


struct gEvHandlerList {
	unsigned int Type;
	unsigned int Value;
	unsigned int Mask;
	gEvHandler Action;
	gEvHandlerList *Next;
};




enum gGraphicCommands {
	GCMD_END=0,
	GCMD_SETCLIP,
	GCMD_DRAWICON,
	GCMD_VLINE,
	GCMD_HLINE,
	GCMD_RECT
};

enum gColors {
	GWHITE=0,
	GGRAY1=0x11111111,
	GGRAY2=0x22222222,
	GGRAY3=0x33333333,
	GGRAY4=0x44444444,
	GGRAY5=0x55555555,
	GGRAY6=0x66666666,
	GGRAY7=0x77777777,
	GGRAY8=0x88888888,
	GGRAY9=0x99999999,
	GGRAY10=0xaaaaaaaa,
	GGRAY11=0xbbbbbbbb,
	GGRAY12=0xcccccccc,
	GGRAY13=0xdddddddd,
	GGRAY14=0xeeeeeeee,
	GBLACK=0xffffffff,
};

enum gAppFlags {
	GAPP_EXIT=1,
	GAPP_NEXTFORM,
	GAPP_PREVFORM
};

enum gEvTypes {
	GEVENT_NONE=0,
	GEVENT_KEYB=1,
	GEVENT_IDLE,
	GEVENT_GUI,
	GEVENT_TMR,
	GEVENT_SD,
	GEVENT_SERIAL
};

enum gEvGui {
	GEVGUI_GETFOCUS=1,
	GEVGUI_LOSEFOCUS,
	GEVGUI_FORMCREATE,
	GEVGUI_FORMOPEN,
	GEVGUI_FORMCLOSE,
	GEVGUI_FORMEXIT,
	GEVGUI_NEWFORM,
	GEVGUI_CHANGEFORM,
	GEVGUI_QUERYCLOSE,
	GEVGUI_BEFORECHANGE,
	GEVGUI_AFTERCHANGE,
	GEVGUI_BEFORECOLLAPSE,
	GEVGUI_AFTERCOLLAPSE,
	GEVGUI_BEFOREEXPAND,
	GEVGUI_AFTEREXPAND,
	GEVGUI_BEFORECHECK,
	GEVGUI_AFTERCHECK,
	GEVGUI_ENTER
};

enum gSystemFonts {
	FONT_MINI=-6,
	FONT_TINY=5,
	FONT_SYSTEM6=6,
	FONT_SYSTEM7=7,
	FONT_SYSTEM8=8
};

struct gFontData {
	unsigned int Signature;
	unsigned int BitmapWidth;
	unsigned int BitmapHeight;
	unsigned int BitmapSize;
	unsigned int WidthMap[256];
	unsigned int Bitmap[1];	
};

class gFont {
public:
	gFontData *FontData;
	int MaxWidth;
	int TextWidth(char *Text);
	int CharWidth(char T);
	int TextHeight();
	gFont();
	gFont(int height);
	gFont(gFontData *data);
};



// GENERIC GUI CONTROL
class gControl {

	virtual void vTable();
	
public:
	gglsurface drawsurf;
	int posx,posy;			// POSITION OF THE CONTROL IN PARENT COORDINATES
	int sizex,sizey;		// SIZE OF THE CONTROL (AND THE DRAWING SURFACE)
	int clipx,clipy,clipx2,clipy2;	// CURRENT CLIPPING AREA
	int ncx,ncy,ncwidth,ncheight;	// NON-CLIENT AREA VIEW POSITION
	int viewx,viewy;					// SCROLL POSITION OF CLIENT AREA VIEW
	int vieww,viewh;
	BOOL scrollh,scrollv;
	
	gEvHandler *RedrawBk;	// REDRAW BACKGROUND, CAN BE OVERRIDEN WITHOUT DECLARING NEW CLASS
	unsigned int *CtlData;	// ARBITRARY DATA - CONTROL-DEPENDENT
	gControl *FirstChild,*LastChild;
	gControl *Next,*Prev;
	gControl *Parent,*UpdateFirst;
	gEvHandlerList *Events;
	
	// PURE VIRTUAL MEMBERS
	virtual void Update(gUpdate *);
	virtual BOOL AcceptFocus() { return FALSE; }
	
	// IMPURE VIRTUAL MEMBERS, PASS EVENTS TO HANDLERS & PROPAGATE TO CHILDS
	virtual BOOL DefaultEvent(gEvent *);
	virtual BOOL DoEvent(gEvent *);
	virtual BOOL DoEventExclusive(gEvent *);
	
	
	// MANAGEMENT MEMBERS
	void AddChild(gControl&);
	void RemoveChild(gControl&);
	void AddHandler(unsigned int Type,unsigned int Mask,unsigned int Arg,gEvHandler Function);
	void RemoveHandler(unsigned int Type,unsigned int Mask=0,unsigned int Arg=0);
	gEvHandler GetHandler(unsigned int Type,unsigned int Mask=0,unsigned int Arg=0);
	void InsufMemoryError();
	gApplication *GetApplication();
	
	
	
	// GRAPHIC ROUTINES TO DRAW IN THE CLIENT AREA WITH CLIPPING
	
	// CLIENT AREA MANAGEMENT
	virtual void DrawVScrollBar();			// REDRAW SCROLL BAR
	virtual void DrawHScrollBar();			// REDRAW SCROLL BAR
	virtual void SetViewPos(int x,int y) {};	// SCROLL VIEWPORT TO GIVEN POSITION
	virtual void SetViewX(int x);			// SCROLL ONLY HORIZONTALLY
	virtual void SetViewY(int y);			// SCROLL ONLY VERTICALLY
	virtual void EnsureVisible(int x1,int y1,int x2,int y2) {};
	
	
	
	
	// GRAPHIC MANAGEMENT
	virtual void SetVisible(BOOL visible) {};
	virtual BOOL IsVisible() {return TRUE; }
	virtual BOOL Resize(int width,int height);
	virtual void Move(int x,int y);
	virtual void ParentToClient(int &x,int &y);
	virtual void ClientToParent(int &x,int &y);
	virtual void ClipToView(gUpdate *u);
	virtual void Invalidate(gUpdate *u,gControl *Sender=NULL);
	virtual void Invalidate(gControl *Sender=NULL);
	
	inline static unsigned int SolidColor(int c) { return ggl_mkcolor(c); }

	// CUSTOM DRAW BACKGROUND
	virtual void RedrawBackground(gUpdate *);

	// FLAT COLOR ROUTINES
	virtual void HLine(int y,int x1,int x2,int pattern);
	virtual void VLine(int x,int y1,int y2,int pattern);
	virtual void Line(int x,int y,int x2,int y2,int pattern);
	virtual void Rect(int x1,int y1,int x2,int y2,int pattern);
	virtual void RectP(int x1,int y1,int x2,int y2,int *Pattern);
	virtual void Plot(int x1,int y1,int color);
	virtual int GetPixel(int x1,int y1);
	
	// TEXT OUTPUT ROUTINES
	virtual void DrawText(int x,int y,char *Text,gFont *Font,int color);
	virtual void DrawTextBk(int x,int y,char *Text,gFont *Font,int color,int BkColor);
	
	// BITMAPS AND ICON ROUTINES
	virtual void DrawIcon(int x,int y,unsigned int *IconData,int width,int height);
	virtual void DrawIconPartial(int x,int y,unsigned int *IconData,int iconwidth,int iconx,int icony,int width,int height);

	// CLIPPING ROUTINES
	virtual void SetClipRegion(int x1,int y1,int x2,int y2);
	virtual	void ClearClipRegion();
	
	// CONSTRUCTORS/DESTRUCTORS
	gControl();
	gControl(gControl& _Parent);
	virtual ~gControl();
};


// INTERNAL DATA STRUCTURE FOR OPEN LOOPS
struct gLoopData {
	int BackupFlags;
	int BackupResult;
	gControl *Idleptr;
};


// IMPLEMENT THE HARDWARE LAYER AND MAIN EVENT PUMP


class gApplication: public gControl {
	virtual void vTable();
public:
	unsigned int *screenbuffer;
	int contrastlevel;
	int invx,invy,invx2,invy2;		// INVALID AREA
	volatile unsigned int RunLoopFlags;
	int RunResult;
	gControl *CurrentForm;
	gString Clipboard;
	
	int Run(gForm *Main=NULL);
	int RunInternalLoop();
	gLoopData *StartOpenLoop(gForm *main=NULL);
	BOOL RunOpenLoopOnce(gLoopData *v);
	int EndOpenLoop(gLoopData *v);
	void CloseAllAndExit();
	void Terminate(int result);
	BOOL QueryTerminate();
	void Update(gUpdate *u);
	void Update();
	virtual void Invalidate(gControl *Sender=NULL);
	virtual void Invalidate(gUpdate *u,gControl *Sender=NULL);
	void OpenNewForm(gControl *ptr);
	void CloseForm(gControl *ptr);
	void SetCurrentForm(gControl *ptr);
	gApplication();
	virtual ~gApplication();
	
};


enum gFormStyles {
	
	S_TOPCAPTION = 1,
	S_BOTTOMCAPTION = 2,
	S_SINGLESTATUSLINE = 4,
	S_DOUBLESTATUSLINE = 8,
	S_TOPSTATUS = 16,
	S_FORCEVSCROLL = 32,
	S_FORCEHSCROLL = 64,
	S_FORMBORDER = 128,
	S_SHADE = 256,
	// 512 NOT USED FOR COMPATIBILITY W/OTHER CONTROLS STYLES
	S_SINGLESOFTMENU = 1024,
	S_EXSOFTMENU = 2048,
	S_FULLSCREEN = 4096
	
};

enum gStateFlags {
	STAT_VISIBLE=1,
	STAT_FOCUS=2
};

class gForm : public gControl {
	virtual void vTable();

public:	
	int StateFlags;
	gSoftMenu *SoftMenu;
	char *Caption;
	gFormStyles StyleFlags;
	gControl *Focus;

	virtual void Update(gUpdate *u);
	virtual void Initialize(unsigned int *Data);
	virtual void Destroy(unsigned int *Data);

	virtual void RedrawBackground(gUpdate *u);
	virtual void RedrawNonClient();
	virtual BOOL DoEvent(gEvent *);
	virtual BOOL DefaultEvent(gEvent *);
	
	void AddSoftMenu(gSoftMenu *menu);
	void HideSoftMenu();
	void ShowSoftMenu();
	
	gForm(gControl& Parent,const char *_Caption,gFormStyles _Style);		 // FULL SCREEN FORM
	gForm(gControl& Parent,const char *_Caption,gFormStyles _Style,int clientwidth,int clientheight);	// FULL SCREEN FORM W/ARBITRARY CLIENT AREA SIZE
	gForm(gControl& Parent,const char *_Caption,gFormStyles _Style,int x,int y,int width,int height); // WINDOWED FORM
	gForm(gControl& Parent,const char *_Caption,gFormStyles _Style,int x,int y,int width,int height,int clientwidth,int clientheight); // WINDOWED FORM
	virtual ~gForm();
};

struct gSoftMenuItem {
	int State;
	char Name[8];		// MAX 7 CHARS + NULL TERMINATOR
	gEvHandler Action;
};

class gSoftMenu : public gControl {
	virtual void vTable();
public:
	int Style;
	int Visible;
	int Numitems,ShowItem;
	gSoftMenuItem *Items;
	
	virtual void SetVisible(BOOL _Visible);
	virtual BOOL IsVisible();
	virtual void DrawMenuLabel(gControl *P,int x,int y,int item);
	virtual void Update(gUpdate *u);
	virtual BOOL DefaultEvent(gEvent *);
	virtual void Invalidate(gControl *Sender=NULL);
	
	void ClearItems();
	void AddItem(const char *Name,gEvHandler Function);
	void AddItemList(struct gSoftMenuItem *List, int numitems);
	void ChangeItem(int number,const char *Name,gEvHandler Function);
	
	virtual void NextPage();
	virtual void PrevPage();
	
	
	gSoftMenu(gControl& Parent,int Style);
	gSoftMenu(gControl& Parent,int Style,struct gSoftMenuItem *List, int numitems);
	
	virtual ~gSoftMenu();
};


class gStaticText : public gControl {
	virtual void vTable();

public:
	char *Text;
	gFont Font;
	int Color;

	virtual void Update(gUpdate *u);
	void SetText(const char *text);
	const char *GetText();
	gStaticText(gControl& Parent,int x, int y,const char *Text,gFont *f,int color);
	virtual ~gStaticText();
};

class gStaticTextWrap : public gStaticText 
{
	virtual void vTable();
public:
	int maxwidth;
	
	virtual void Update(gUpdate *);
	virtual void SetText(const char *);
	void RecalcSize();
	static void CalcSize(const char *_text,gFont *font,int maxwidth,int *width,int *height);
	gStaticTextWrap(gControl& _Parent,int x,int y, int width, const char *_Text,gFont *f,int color);
	
	virtual ~gStaticTextWrap();
};




enum gStyles {
	S_UNCHECKED=0,
	S_CHECK = 1,
	S_DOT,
	S_PLUS,
	S_MINUS,
	S_UNKNOWN,
	S_SELECTED=8,
	S_MULTILINE=16,	// CONTROL CAN CONTAIN MORE THAN ONE LINE OF TEXT
	S_HSCROLLBAR=32, // CONTROL WILL DISPLAY A HORIZONTAL SCROLL BAR
	S_VSCROLLBAR=64, // CONTROL WILL DISPLAY A VERTICAL SCROLL BAR
	S_HASBORDER=128,  // CONTROL HAS A BORDER AROUND IT
	S_FOCUSED=512

};


class gCheckBox : public gControl {
	virtual void vTable();

public:
	int TypeFlags;
	int State;
	
	virtual BOOL AcceptFocus() { return TRUE; }
	virtual void Update(gUpdate *u);
	void SetState(int state);
	int GetState();
	virtual void DrawCheck(int state);
	virtual BOOL DefaultEvent(gEvent *);
	
	gCheckBox(gControl& Parent,int x, int y, gStyles Type=S_CHECK);		// STANDARD CHECKBOX
	virtual ~gCheckBox();
};




enum gEditStates {
	ES_SHOWCURSOR =1,
	ES_CURSORBLINK=2,
	ES_RDONLY=4,
	ES_SINGLELINE=8,
	ES_LIMITEDIT=16,
	ES_OVERWRITE=32
};


#define TRIM_BEGIN 	0
#define TRIM_END 	1



class gEditBox : public gControl {
	virtual void vTable();

public:
	gString Text;
	int CursorPos,CursorLine;
	int SelStart,SelEnd;
	gFont Font;
	int Color,BkColor,SelColor;
	int DispLines;
	int TotalLines;
	int ViewLine;
	int EditMin,EditMax;
	unsigned int State;
	int Cursorx,Cursory,Targetx;
	gglsurface cursorbk;
	tmr_t lastblink;
	int InputMode;
	int StyleFlags;

	virtual BOOL AcceptFocus() { return TRUE; }
	virtual void Update(gUpdate *u);
	virtual BOOL DefaultEvent(gEvent *);
	
	// DRAWING FUNCTIONS
	void RedrawFrame();
	void DrawVScrollBar();
	void InvalidateVScrollBar();
	void DrawLine(int LineOffset,int x, int y);
	void ScrollDown1Line(int ytop);
	void ScrollUp1Line(int ybot);
	void ScrollRight(int shift);
	void ScrollLeft(int shift);
	

	void DrawAllLines();
	void ClrEOL(int x,int y);
	void CursorOff();
	void CursorOn();
	void HideCursor();
	void UnhideCursor();
	void StartCursorBlink();
	void StopCursorBlink();
	
	// INPUT RELATED FUNCTIONS
	void CursorUp();
	void CursorDown();
	void CursorRight();
	void CursorLeft();
	void ViewUp();
	void ViewDown();
	void ViewRight();
	void ViewLeft();
	void ViewPageUp();
	void ViewPageDown();
	void ViewHome();
	void ViewEnd();
	void ViewEndOfText();
	void ViewStartOfText();

	void PageUp();
	void PageDown();
	void Home();
	void End();
	void MoveCursor(int Offset);
	void EndOfText();
	void StartOfText();
	void InsertChar(int character);
	void DelChar(int backspace);
	void RemoveText(int StartPos,int EndPos);
	void InsertText(int StartPos,int Length,gString& s);
	void InsertText(int StartPos,int Length,char *string);
	void BeginSelection();
	void EndSelection();
	void Copy();
	void Cut();
	void Paste();
	void BackSpace();
	void Delete();

	void SetEditLimits(int StartPos,int EndPos);
	void ClearEditLimits();
	void GetEditLimits(int *StartPos,int *EndPos);
	BOOL IsReadOnly();
	void SetReadOnly(BOOL rdonly);
	void SetOverwriteMode(BOOL ovrwrite);
	// LOAD/SAVE
	void SetText(const char *text);
	int GetText(char *data,int maxbytes);
	int GetText(char *data,int maxbytes,int StartPos,int EndPos);
	
	void LimitTextSize(int limit,int trimwhere);
	
	int Strlen();
	gEditBox(gControl& Parent,int x, int y,int width,int height,gStyles Style, gFont *f);
	virtual ~gEditBox();
};

// LIST BOXES / TREES

#define ITEM_GROUP 16	// ALLOCATE GROUPS OF 16 ITEMS, NO LESS TO AVOID HEAP TRASHING
#define CHECKHEIGHT 6	// HEIGHT OF CHECKBOX MARK = minimum height of item
#define LEVELWIDTH 8	// DEFINE HOW MUCH TO THE RIGHT ON EACH SUBLEVEL
#define CHECKWIDTH 8
#define CHECKBMPWIDTH 48

enum gListStates {
	LS_HASCHECK =1,
	LS_CHECKED = 2,
	LS_UNKNOWN = 4,
	LS_HIDDEN = 8,
	LS_SELECTED = 16,
	LS_EXPANDABLE = 32,
	LS_EXPANDED = 64
};

struct gListItem {
	int TreeLevel;
	int StateFlags;
	char *Text;
	int Data;
};


class gListBox : public gControl {
	virtual void vTable();
	
public:
	gListItem *Items;
	int NumItems,MaxItems,DispItems;
	int ViewItem,SelectedItem;
	int ItemHeight,MaxWidth;
	int Color,BkColor,SelColor;
	int StyleFlags;
	gFont Font;
	
	void SetItemList(gListItem *List,int NumItems);
	void InsertItem(int Index,char *Text, int TreeLevel,int StateFlags,int Data);
	void RemoveItem(int Index);
	void AddItem(char *Text, int TreeLevel,int StateFlags,int Data);
	gListItem *GetItem(int Index);	
	
	void SetSelection(int Index);
	int NextItem(int Index);
	int NextVisibleItem(int Index);
	int PrevVisibleItem(int Index);
	int PrevItem(int Index);
	int ChildItem(int Index);
	int ParentItem(int Index);
	char *FullPath(int BaseIndex,int Index,const char *Separator);
	
	void Select(int Index);
	void Unselect(int Index);
	void Hide(int Index);
	void Unhide(int Index);
	void Expand(int Index);
	void Collapse(int Index);
	void ExpandAll(int Index);
	void CollapseAll(int Index);
	BOOL HasChildren(int Index);
	BOOL IsExpanded(int Index);
	void SetCheck(int Index,BOOL value);
	BOOL IsChecked(int Index);

	
	
	BOOL ReallocList(int _NumItems);
	void ListChanged();
	void RedrawAll();
	void RedrawSingleItem(int Index);

	BOOL MoveSelectionUp(int nlines);
	BOOL MoveSelectionDown(int nlines);
	void EnsureVisible(int Index);
	virtual int CalcItemWidth(int Index);
	virtual BOOL DrawItem(int Index,int y);
	
	virtual BOOL AcceptFocus() { return TRUE; }
	virtual void Update(gUpdate *u);
	void RedrawFrame();
	void DrawVScrollBar();
	void DrawHScrollBar();
	void InvalidateVScrollBar();
	virtual BOOL DefaultEvent(gEvent *e);

	
	gListBox(gControl& Parent,int x,int y,int w,int h,int Style,gFont *Font);
	~gListBox();
};

// MESSAGE BOXES

enum gMsgBoxStyle {
	S_OKCANCEL=0,
	S_YESNO,
	S_RETRYCANCEL,
	S_OKONLY
};
enum gMsgBoxResult {
	S_OK = 0,
	S_YES = 0,
	S_RETRY = 0,
	S_NO = -1,
	S_CANCEL = -1
};

gMsgBoxResult gDoMessageBox(gApplication *app,const char *Title,const char *Message,gMsgBoxStyle Flags);



// STANDARD EVENT HANDLERS
BOOL gOKEvent(gEvent *e,gControl *This);
BOOL gCancelEvent(gEvent *e,gControl *This);
BOOL gDoNothingEvent(gEvent *e,gControl *This);



#endif /*GUI_H_*/
