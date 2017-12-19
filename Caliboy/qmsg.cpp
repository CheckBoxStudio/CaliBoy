#include "qmsg.h"
#include <fstream>
#include <time.h>

const char debugModePre[] = " |-";
bool bDetailOn = true;
void detialOn()
{
	bDetailOn = true;
}
void detialOff()
{
	bDetailOn = false;
}

#ifdef _On_QtTextBrowser_

QMsgPannel *OutPannel = 0;
void setOutBrowser(QTextBrowser *browser)
{
	OutPannel = new QMsgPannel(browser);
}
void releaseOutBrowser()
{
	if (OutPannel != 0)
		delete OutPannel;
}
void writeMsg(QString &msg)
{
	if (OutPannel != 0) 
		OutPannel->Append(msg);
}
void msgOut(QString &msg)
{
	writeMsg(msg);
}
void _msgOut(QString &msg)
{
	QString debugMsg = QString("%1 %2").arg(debugModePre).arg(msg);
	if (bDetailOn)		
		writeMsg(debugMsg);
}

#endif

void writeMsg(char *msg)
{

#ifdef _On_QtTextBrowser_
	if (OutPannel != 0)
		OutPannel->Append(QString::fromLocal8Bit(msg));
#else
#ifdef _On_Console_
	printf(msg);
	printf("\n");
	fflush(stdout);
#endif
#endif
}
void msgOut(char *msg)
{
	writeMsg(msg);
}
void _msgOut(char *msg)
{
	char debugMsg[512];
		sprintf_s(debugMsg,"%s %s",debugModePre,msg);
	if (bDetailOn)		
		writeMsg(debugMsg);
}