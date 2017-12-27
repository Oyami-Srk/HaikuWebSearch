/*
 *  Made for Haiku.
 *  Distributed under the terms of the MIT license.
*/
#include "SearchView.h"
#include "MainWindow.h"
#include <Button.h>
#include <LayoutBuilder.h>
#include <Dragger.h>
#include <TextView.h>
#include <Application.h>
#include <String.h>
#include <Locale.h>
#include <InterfaceDefs.h>
#include <Catalog.h>

#include <iostream>


#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "Main view"


MainWindow::MainWindow(void)
	: BWindow(BRect(100,100,500,150), "WebSearch",
				B_TITLED_WINDOW, B_NOT_RESIZABLE | B_NOT_ZOOMABLE)
{
	sview = new SearchView(Bounds());
	AddChild(sview);
	
	BView *bgView = new BView(Bounds(), "bgView", B_FOLLOW_ALL, B_WILL_DRAW);
	bgView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	AddChild(bgView);
}

bool
MainWindow::QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}

void
MainWindow::MessageReceived(BMessage *msg)
{
	switch (msg->what) {
		case MSG_SEARCH:
		{
			//sview->iSearch();
			break;
		}
		default:
			BWindow::MessageReceived(msg);
			break;
	}
}
