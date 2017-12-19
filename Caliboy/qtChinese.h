#ifndef QT_CHINESE_H
#define QT_CHINESE_H

#ifndef FromCHN

#include <QString>

#define FromCHN(str) ( QString::fromLocal8Bit(str) )
#define ToCHN(QStr) ( (QStr.toLocal8Bit()).data() )

#endif // !FromCHN

#endif // !QT_CHINESE_H