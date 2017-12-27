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

#define HAIKU_WEBSEARCH_DIRECTORY "HaikuWebSearch"
#define HAIKU_WEBSEARCH_ENGINES	  "engines"

class _EXPORT SearchView;
class SearchView_sub;
class SearchView_btn;

enum
{
	MSG_SEARCH = 'SEAR'
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
	
	virtual void		MouseDown(BPoint point);
	
private:
	SearchView_sub      *subView;
	std::map<std::string, std::string> search_engines;
	std::string			current_engine;
};

class SearchView_sub : public BView
{
public:
		SearchView_sub(BRect frame);
		
		BTextControl 		*fTextControl;
		SearchView_btn		*fBtnSearch;
};

class SearchView_btn : public BView
{
public:
		SearchView_btn(BRect frame);
		virtual void		MouseDown(BPoint point);
};
#endif

