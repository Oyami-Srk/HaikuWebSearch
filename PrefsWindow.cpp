#include "PrefsWindow.h"

#include <Box.h>
#include <Button.h>
#include <GroupLayoutBuilder.h>
#include <LayoutBuilder.h>
#include <RadioButton.h>

#include "SearchView.h"


enum {
	MSG_PREFERRED = 'PRFD',
	MSG_DEFAULT = 'DFLT'
};

static const char* engines[] = {
	"Google", "Bing", "Duckduckgo", "Baidu", "Yandex",
	"Gigablast", "Sogou", "Qwant", "Naver", "Good Search"
};
static const unsigned numEngines = sizeof engines / sizeof engines[0];

static BRadioButton* radioButtons[numEngines];


PrefsWindow::PrefsWindow(const char* preferredEngine)
	:
	BWindow(BRect(0, 0, 0, 0), "Preferences", B_TITLED_WINDOW,
		B_WILL_DRAW | B_AUTO_UPDATE_SIZE_LIMITS)
{
	BMessage msg(MSG_PREFERRED);
	msg.AddInt32("index", 0);

	int32 index;

	for (index = 0; index < numEngines; index++) {
		msg.ReplaceInt32("index", index);
		radioButtons[index] = new BRadioButton(engines[index],
			new BMessage(msg));
	}

	for (index = 0; index < numEngines - 1; index++)
		if (strcmp(engines[index], preferredEngine) == 0)
			break;

	radioButtons[index]->SetValue(B_CONTROL_ON);

	msg = BMessage(MSG_DEFAULT);
	BButton* defaultButton = new BButton("Default", new BMessage(msg));

	BLayoutBuilder::Group<>(this, B_VERTICAL, B_USE_ITEM_SPACING)
		.SetInsets(B_USE_ITEM_SPACING, B_USE_ITEM_SPACING,
			B_USE_ITEM_SPACING, B_USE_ITEM_SPACING)
		.AddGroup(B_HORIZONTAL, B_USE_ITEM_SPACING)
			.AddGroup(B_VERTICAL, B_USE_ITEM_SPACING)
				.Add(radioButtons[0])
				.Add(radioButtons[1])
				.Add(radioButtons[2])
				.Add(radioButtons[3])
				.Add(radioButtons[4])
				.End()
			.AddStrut(10)
			.AddGroup(B_VERTICAL, B_USE_ITEM_SPACING)
				.Add(radioButtons[5])
				.Add(radioButtons[6])
				.Add(radioButtons[7])
				.Add(radioButtons[8])
				.Add(radioButtons[9])
				.End()
			.End()
		.Add(defaultButton)
		.End();

	CenterOnScreen();
	Show();
}


void
PrefsWindow::MessageReceived(BMessage* msg)
{
	int32 index;

	switch (msg->what) {
		case MSG_PREFERRED:
			msg->FindInt32("index", &index);
			fPreferredEngine = engines[index];
			break;
		case MSG_DEFAULT:
			index = numEngines - 1;
			radioButtons[index]->SetValue(B_CONTROL_ON);
			fPreferredEngine = engines[index];
			break;
		default:
			BWindow::MessageReceived(msg);
	}
}
