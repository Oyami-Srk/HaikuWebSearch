/*
 *  Made for Haiku.
 *  Distributed under the terms of the MIT license.
*/       
#include "SearchView.h"
#include <String.h>
#include <TextView.h>
#include <Button.h>
#include <Message.h>
#include <LayoutBuilder.h>
#include <FindDirectory.h>
#include <Path.h>
#include <Volume.h>
#include <Directory.h>
#include <PopUpMenu.h>
#include <iostream>
#include <cstdlib>
#include <fstream>


#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "Main view"

const float kDraggerSize = 7;


PrefsButton::PrefsButton()
	:
	BButton(BRect(10, 10, 70, 40), "prefs", "Settings", NULL)
{
}


void
PrefsButton::MouseDown(BPoint point)
{
	BPoint cursor;
	uint32 buttons;
	GetMouse(&cursor, &buttons);

	if (buttons & B_PRIMARY_MOUSE_BUTTON) {
		BMessage msg(MSG_PREFS);
		BMessenger messenger(Parent());
		messenger.SendMessage(&msg);
	}
}


SearchView_btn::SearchView_btn(BRect frame)
	:BView(frame, "WebSearch_btn", B_FOLLOW_NONE, B_WILL_DRAW)
{
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
}

void		
SearchView_btn::MouseDown(BPoint point)
{	
	BPoint cursor;
	uint32 buttons;
	
	GetMouse(&cursor, &buttons);
	
	if(buttons & B_PRIMARY_MOUSE_BUTTON)
	{
		BMessage req(MSG_SEARCH);
		BMessenger messenger(Parent());
		messenger.SendMessage(&req);
	}else
	{
		BMessage req(MSG_POPUPMENU);
		BMessenger messenger(Parent());
		req.AddPoint("point", point);
		messenger.SendMessage(&req);
	}
}

SearchView_sub::SearchView_sub(BRect frame)
	:BView(frame, "WebSearch", B_FOLLOW_NONE, B_WILL_DRAW)
{
	AddChild(new PrefsButton());
	frame.left += 80;

	// Initialize fTextControl
	fTextControl = new BTextControl(frame, "search", NULL, "", 
					new BMessage(MSG_SEARCH), B_FOLLOW_NONE);

	fTextControl->MoveTo(frame.left,
		(Bounds().Height() - fTextControl->Bounds().Height()) / 2);
	fTextControl->SetAlignment(B_ALIGN_LEFT, B_ALIGN_LEFT);
	
	// Initialize Button
	fBtnSearch = new SearchView_btn(BRect(BPoint(0,0), BSize(50,30)));

	fTextControl->ResizeBy(-10 - fBtnSearch->Bounds().Width(), 0);
	fBtnSearch->MoveTo(frame.right - fBtnSearch->Bounds().Width() - 5, 
					(Bounds().Height() - fBtnSearch->Bounds().Height())/2);
					
	BTextView *fText = new BTextView(fBtnSearch->Bounds(), "bgView", fBtnSearch->Bounds(), B_FOLLOW_ALL, B_WILL_DRAW);
	BString text;
	text << "Search";
	fText->SetText(text);
	fText->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	fText->MoveTo(frame.right - fBtnSearch->Bounds().right - 5, 
				(Bounds().Height() - fText->Bounds().Height())/2 + 5);
	
	fText->SetAlignment(B_ALIGN_CENTER);
	fText->MakeSelectable(false);
	
	AddChild(fTextControl);
	AddChild(fBtnSearch);
	AddChild(fText);
	SetViewColor(B_TRANSPARENT_COLOR);
}

void
SearchView::iLoad()
{
	// Load Configure
	char buf[128];
	memset(buf, 0, sizeof(char) * 128);
	dev_t volume = dev_for_path("/boot");
	status_t result = find_directory(B_USER_SETTINGS_DIRECTORY, volume, false, buf, 100);
	sprintf(buf, "%s/%s", buf, HAIKU_WEBSEARCH_DIRECTORY);
	result = create_directory(buf, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	sprintf(buf, "%s/%s", buf, HAIKU_WEBSEARCH_ENGINES);
	(void)result;
	std::ifstream engines(buf);
	std::string title;
	std::string link;
	while(!engines.eof())
	{
		engines >> title >> link;
		if(title == "" || link == "")
			break;
		for(size_t i = 0; i < title.size(); i++)
			if(title[i] == '_')
				title[i++] = ' ';
		search_engines.insert(std::pair<std::string, std::string>(title, link));
	}
	if(search_engines.size() == 0 || search_engines.find("Good Search") == search_engines.end())
		search_engines.insert(std::pair<std::string, std::string>("Good Search", 
				"http://www.goodsearch.com/search-web?charityid=949749&keywords=%s"));
	
	iChangeEngine("Good Search");
	
	fPopupMenu = new BPopUpMenu("sel_menu");
	for(std::map<std::string, std::string>::iterator it = search_engines.begin(); it!=search_engines.end(); ++it)
	{
		fPopupMenu->AddItem(new BMenuItem(it->first.c_str(), NULL));
		std::cout << "Title: " << it->first << "; Link: " << it->second << std::endl;
	}
	std::cout << "Current Engine: " << current_engine << " (" << search_engines[current_engine] << ")" << std::endl;
}


SearchView::SearchView(BRect frame) 
	:
	BView(frame, "WebSearch", B_FOLLOW_NONE, B_WILL_DRAW),
	fPrefsWindow(NULL),
	subView(NULL)
{
	subView = new SearchView_sub(frame);
	iLoad();
	frame.left = frame.right - kDraggerSize;
	frame.top = frame.bottom - kDraggerSize;
	BDragger *dragger = new BDragger(frame, this, B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM);
	AddChild(dragger);
	AddChild(subView);
}


SearchView::SearchView(BMessage *archive) 
	:
	BView(archive)
{
	subView = new SearchView_sub(archive->FindRect("bounds"));
	iLoad();
	AddChild(subView);
	SetViewColor(B_TRANSPARENT_COLOR);
}


SearchView::~SearchView()
{}


void
SearchView::Draw(BRect)
{
	SetDrawingMode(B_OP_ALPHA);
}


void
SearchView::MessageReceived(BMessage *msg)
{
	//std::cout << msg->what << " (" << MSG_SEARCH << ") " << std::endl;
	switch (msg->what) {
	    case B_ABOUT_REQUESTED:
	      	iAboutRequested();
    		break; 
		case MSG_PREFS:
			if (fPrefsWindow == NULL)
				fPrefsWindow = new PrefsWindow(current_engine.c_str());
			break;
    	case MSG_SEARCH:
    		iSearch();
    		break;
    	case MSG_POPUPMENU:
    	{
    		BMenuItem *sel;
    		if((sel = fPopupMenu->Go(ConvertToScreen(msg->FindPoint("point")))) != NULL)
    		{
    			iChangeEngine(std::string(sel->Label()));
    		}
    	}
    		break;
		default:
			BView::MessageReceived(msg);
			break;
	}
}


BArchivable *SearchView::Instantiate(BMessage *data)
{
	return new SearchView(data);
}


status_t
SearchView::Archive(BMessage *archive, bool deep) const
{
	BView::Archive(archive, deep);

	archive->AddString("add_on", "application/x-vnd.Haiku-WebSearch");
	archive->AddString("class", "WebSearch");
	
	archive->AddRect("bounds", Bounds());
	
	return B_OK;
}

void
SearchView::iAboutRequested()
{
	BAlert *alert = new BAlert("about",
	"Web Search\n",
		"OK");	
	BTextView *view = alert->TextView();
	BFont font;
	view->SetStylable(true);
	view->GetFont(&font);
	font.SetSize(font.Size() + 7.0f);
	font.SetFace(B_BOLD_FACE);
	view->SetFontAndColor(0, 12, &font);
	alert->SetFlags(alert->Flags() | B_CLOSE_ON_ESCAPE);	
	alert->Go();
}

void
SearchView::iChangeEngine(std::string engine)
{
	current_engine = engine;
}

void
SearchView::iSearch()
{
	char buf[512];
	memset(buf, 0, sizeof(char) * 512);

	if (fPrefsWindow != NULL)
		current_engine = fPrefsWindow->PreferredEngine();

	sprintf(buf,search_engines[current_engine].c_str(), subView->fTextControl->Text());
	char cmd[512+16];
	memset(cmd, 0, sizeof(char) * (512 + 16));
	sprintf(cmd, "open \"%s\"", buf);
	std::cout << cmd << std::endl;
	system(cmd);
}
