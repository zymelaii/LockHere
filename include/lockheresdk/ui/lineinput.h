#ifndef LOCKHERE_UI_UTILS_LINEINPUT_H
#define LOCKHERE_UI_UTILS_LINEINPUT_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QLayout>

namespace LockHere {
namespace Ui {
namespace utils {

class LineInput : public QWidget {
	Q_OBJECT

public:
	using EchoMode = QLineEdit::EchoMode;

	explicit LineInput(QWidget* parent = nullptr);

	bool eventFilter(QObject* obj, QEvent* event) Q_DECL_OVERRIDE;
	void paintEvent(QPaintEvent* e) Q_DECL_OVERRIDE;

	void	 setHint(const QString& hint);
    void	 setInputVisible(bool visible);
    void	 setOptionEnabled(bool enabled);
    void	 setOptionVisible(bool visible);
	void	 setOptionIcon(const QPixmap& icon);
	void	 setOptionIcon(const QString& svgPath, QColor currentColor = Qt::black);
    EchoMode echoMode() const;
	void	 setEchoMode(EchoMode mode);
	QString	 text() const;

protected slots:
    void onLoseFocus();

signals:
	void textChanged(const QString&);
	void clicked();

private:
    QHBoxLayout* hloMain;
    QVBoxLayout* vloEdit;
    QLabel*		 lbHint;	 //!< hint
    QLineEdit*	 leInput;	 //!< input
    QPushButton* btOption;	 //!< option
};

}	// namespace utils
}	// namespace Ui
}	// namespace LockHere

#endif	 // LOCKHERE_UI_UTILS_LINEINPUT_H
