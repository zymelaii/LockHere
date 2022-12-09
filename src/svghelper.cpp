#include <lockheresdk/svghelper.h>

#include <QFile>
#include <QSvgRenderer>
#include <QPainter>

namespace LockHere {

QPixmap* loadSvgAsPixmap(const QString& path, QSize size, QColor currentColor) {
	const auto sColor = QString(R"("%1"")").arg(currentColor.name());

	QPixmap* pixmap = nullptr;

	QFile file(path);
	file.open(QIODevice::ReadOnly);

	if (!file.isOpen()) {
        return nullptr;
	}

	QSvgRenderer renderer(
        file.readAll().replace(R"("currentColor")", 14, sColor.toLocal8Bit(), sColor.length() - 1));
	file.close();

	auto svgSize = size.isNull() ? renderer.defaultSize() : size;
	pixmap		 = new QPixmap(svgSize);
	pixmap->fill(Qt::transparent);

	QPainter painter;
	painter.begin(pixmap);
	renderer.render(&painter);
	painter.end();

	return pixmap;
}

}	// namespace LockHere
