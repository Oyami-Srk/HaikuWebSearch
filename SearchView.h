/*
 *  Made for Haiku.
 *  Distributed under the terms of the MIT license.
*/
#ifndef SEARCH_VIEW_H_
#define SEARCH_VIEW_H_

#include <stdio.h>
#include <Alert.h>
#include <Dragger.h>
#include <View.h>
#include <Window.h>
#include <TextControl.h>
#include <Button.h>
#include <Bitmap.h>
#include <string>
#include <map>

#include "PrefsWindow.h"

#define HAIKU_WEBSEARCH_DIRECTORY "HaikuWebSearch"
#define HAIKU_WEBSEARCH_ENGINES	  "engines"

class _EXPORT SearchView;
class SearchView_sub;
class SearchView_btn;

class BPopUpMenu;

enum
{
	MSG_PREFS		= 'PREF',
	MSG_SEARCH 		= 'SEAR',
	MSG_POPUPMENU	= 'PUMU',
	MSG_CHANGEENG	= 'CGEG'
};

class SearchView : public BView {
public:
						SearchView(BRect frame);
						SearchView(BMessage *data);
						~SearchView();
	virtual void 		Draw(BRect);
	virtual void 		MessageReceived(BMessage *msg);
	static 				BArchivable *Instantiate(BMessage *archive);
	virtual status_t 	Archive(BMessage *data, bool deep = true) const;
	void				iAboutRequested();
	void 				iSearch();
	void				iChangeEngine(std::string engine);
	void				iLoad();
	
	
private:
	PrefsWindow*		fPrefsWindow;
	SearchView_sub      *subView;
	std::map<std::string, std::string> search_engines;
	std::string			current_engine;
	BPopUpMenu			*fPopupMenu;
};

class SearchView_sub : public BView
{
public:
		SearchView_sub(BRect frame);

		BTextControl 		*fTextControl;
		SearchView_btn		*fBtnSearch;
};

class PrefsButton : public BButton
{
public:
		PrefsButton();
virtual void	MouseDown(BPoint point);
};

class SearchView_btn : public BView
{
public:
		SearchView_btn(BRect frame);
		virtual void		MouseDown(BPoint point);
};
#endif

