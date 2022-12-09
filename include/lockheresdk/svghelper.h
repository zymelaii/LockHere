#ifndef SVGHELPER_H
#define SVGHELPER_H

#include <QPixmap>

namespace LockHere {

QPixmap* loadSvgAsPixmap(const QString& path, QSize size = QSize(), QColor currentColor = Qt::black);

}

#endif	 // SVGHELPER
