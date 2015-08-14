#ifndef TRANSLATION_HPP
#define TRANSLATION_HPP

#include <QString>

struct LANG {
	enum {ENG = 0, HUN = 1};
};
QString TXT_APP_TITLE[] =	{ "NetChecker",				"NetChecker" };
QString README_URL[] =		{ ":/res/readme_en.html",	":/res/readme_hu.html" };
QString TXT_NEXT_BTN[] =	{ "Next",					"Tovább" };
QString TXT_DIAG_GROUP[] =	{ "Running diagnostics",	"Diagnosztika futtatása" };
QString TXT_EST_TIME[] =    { "Estimated time",         "Várható befejezés" };
QString TXT_ALMOST_END[] =  { "Please be patient...",   "Kérem várjon türelemmel..." };
QString TXT_FINISHED[] =    { "Finished.",              "Művelet befejeződött." };
QString TXT_LOG_GROUP[] =	{ "Log",					"Napló" };
QString TXT_CLIPB_BTN[] =	{ "Copy to clipboard",		"Másolás vágólapra" };
QString TXT_SAVE_BTN[] =	{ "Save to file...",		"Mentés fájlba..." };
QString TXT_SAVE_DIALOG[] =	{ "Save to...",				"Mentés helye..." };
QString TXT_ROOT_MSG_TITLE_WIN[] ={
	"Diagnostics cannot start",
	"A diagnosztika nem indítható"
};
QString TXT_ROOT_MSG_WIN[] ={
	"Administrator privileges are needed to gather informations.",
	"A program helyes működéséhez rendszergazdai jogosultság szükséges."
};

#endif
