#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "SearchView.h"
#include <Window.h>
#include <ListView.h>
#include <TextControl.h>

class MainWindow : public BWindow
{
public:
	MainWindow(void);
	virtual bool QuitRequested();
	virtual void 		MessageReceived(BMessage *msg);
private:
	SearchView *sview;
};

#endif
