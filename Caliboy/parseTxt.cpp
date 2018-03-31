#include "parseTxt.h"
#include <QFile>
#include <QDir>

QString _dealPath(const QString Path);
QStringList _fromFolder(const QString folder, const QString BasePath = "");
void _removeConstract(QStringList &addList, QStringList &removeList);


enum Mode{
    ModeAdd,
    ModeRemove
};
enum SetOpt{
    SetPath,
    SetFolder,
    SetFolder_AB,
    SetFile,
    SetFile_AB
};

bool parseTxt(
    const QString txtFile,
    QStringList &openFileList,
    QStringList &removeFileList)
{
    QFile fileIn(txtFile);
    if (!fileIn.open(QIODevice::ReadOnly))
        return false;

    bool state = true;

    QString BasePath = "./";
    Mode mode = ModeAdd;
    SetOpt setOpt = SetFile_AB;
    while (!fileIn.atEnd()) {
        QByteArray line = fileIn.readLine();
        for (int i=0; i<line.length(); ++i)
        {
            if (line[i] == '\r' ||
                line[i] == '\n')
            line[i] = '\0';
        }
        QString qLine = line;

        // Skip empty lines and comments
        if (line.at(0) == '\0' || 
            line.at(0) == '#' ||
            line.at(0) == '=' )
            continue;


        if (line[0] == '-') {
            if ( qLine == "-Add")
                mode = ModeAdd;
            else if(qLine == "-Remove")
                mode = ModeRemove;
            else if (qLine == "-Path")
                setOpt = SetPath;
            else if (qLine == "-Folder")
                setOpt = SetFolder_AB;
            else if (qLine == "-folder")
                setOpt = SetFolder;
            else if (qLine == "-File")
                setOpt = SetFile_AB;
            else if (qLine == "-file")
                setOpt = SetFile;
            else 
                state = false;
            continue;
        } else { 
            QStringList files;
            switch (setOpt)
            {
                case SetPath:
                    BasePath = _dealPath(qLine);
                    break;
                case SetFolder:
                    files = _fromFolder(qLine,BasePath);
                    break;
                case SetFolder_AB:
                    files = _fromFolder(qLine);
                    break;
                case SetFile: 
                {
                    QString file = BasePath+"/"+qLine;
                    files<< file.replace('\\','/');
                    break;
                }
                case SetFile_AB:
                    files<<qLine;
                    break;
                default:
                    break;
            }
            if (!files.isEmpty()) {
                switch (mode)
                {
                    case ModeAdd:
                        openFileList<<files;
                        break;
                    case ModeRemove:
                        removeFileList<<files;
                        break;
                    default:
                        break;
                }
            }
        }
    }

    _removeConstract(openFileList,removeFileList);
}

QString _dealPath(const QString Path)
{
    QString retPath;
    if (Path.at(Path.length()-1) == '\\' ||
        Path.at(Path.length()-1) == '/' )
        retPath = Path.left(Path.length()-1);
    else
        retPath = Path;
    return retPath.replace('\\','/');
}
QStringList _fromFolder(const QString folder, const QString BasePath)
{
    QString FolderPath;
    if (BasePath.isEmpty())
        FolderPath = folder;
    else
        FolderPath = BasePath+"/"+folder;
    FolderPath = FolderPath.replace('\\','/');

    QStringList filter;
    filter << "*.bmp" << "*.jpg" <<"*.tif" <<"*.tiff" <<"*.pgm" <<"*.png";
    QDir dir(FolderPath);
    QFileInfoList fileInfoList = dir.entryInfoList(filter);
    QStringList fileList;
    for (int i=0; i<fileInfoList.size(); ++i)
        fileList << fileInfoList.at(i).absoluteFilePath(); 
    return fileList;
}
void _removeConstract(QStringList &addList, QStringList &removeList)
{
    QStringList sameList;
    for (int i=0; i<removeList.size(); ++i) {
        QString k = removeList.at(i);
    if (addList.contains(k))
        sameList<<k;
    }

    for (int i=0; i<sameList.size(); ++i) {
        QString k = sameList.at(i);
        addList.removeAll(k);
        removeList.removeAll(k);
    }
}