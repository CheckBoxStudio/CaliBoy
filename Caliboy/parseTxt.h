/* parseTxt.h
 * ½âÎöÎÄ±¾.
 */

#ifndef PARSE_TXT_H
#define PARSE_TXT_H

#include <QString>
#include <QStringList>

bool parseTxt(const QString txtFile,
			  QStringList &openFileList,
			  QStringList &removeFileList);

#endif // !PARSE_TXT_H