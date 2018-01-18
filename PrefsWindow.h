#ifndef PREFS_WINDOW_H
#define PREFS_WINDOW_H

#include <Window.h>

class PrefsWindow : public BWindow
{
public:
			PrefsWindow(const char* preferredEngine);
	void	MessageReceived(BMessage* msg);

	const char* PreferredEngine() const { return fPreferredEngine; }

private:
	const char* fPreferredEngine;
};

#endif
