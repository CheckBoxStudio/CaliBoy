#ifndef Q_MSG_H
#define Q_MSG_H

#define _On_QtTextBrowser_
//#define _On_Console_

void detialOn();
void detialOff();

void _msgOut(char *msg);
void msgOut(char *msg); 

#ifdef _On_QtTextBrowser_

#include <QTextBrowser>
#include <QScrollBar>
#include <QString>

class QMsgPannel : public QObject
{
	Q_OBJECT
public:
	QMsgPannel(QTextBrowser *browser)
		: m_Browser(0)
	{
		m_Browser = browser;
		connect(this, SIGNAL(AppendText(const QString&)), this, SLOT(SlotAppendText(const QString&)));
	};
	void Append(const QString &text)
	{
		emit AppendText(text);
	};
private:
	QTextBrowser *m_Browser;
private slots:
	void SlotAppendText(const QString &msg)
	{
		if (m_Browser != 0)
		{
			m_Browser->append(msg);
			m_Browser->moveCursor(QTextCursor::End);
			m_Browser->horizontalScrollBar()->setValue(0);
		}
	};
signals:
	void AppendText(const QString &msg);
};

void setOutBrowser(QTextBrowser *browser); 
void releaseOutBrowser();

void msgOut(QString &msg); 
void _msgOut(QString &msg);


#endif

#endif // !Q_MSG_H