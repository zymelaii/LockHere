#ifndef LOCKHERE_UI_UTILS_LINEINPUT_P_H
#define LOCKHERE_UI_UTILS_LINEINPUT_P_H

#include <lockheresdk/ui/lineinput.h>

namespace LockHere {
namespace Ui {
namespace utils {

class QLineEditTrick : public QLineEdit {
    Q_OBJECT

public:
    explicit QLineEditTrick(QWidget* parent = nullptr);

    void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void focusOutEvent(QFocusEvent* event) Q_DECL_OVERRIDE;

signals:
    void loseFocus();
};

}	// namespace utils
}	// namespace Ui
}	// namespace LockHere

#endif	 // LOCKHERE_UI_UTILS_LINEINPUT_P_H
