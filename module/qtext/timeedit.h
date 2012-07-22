#ifndef _QTEXT_TIMEEDIT_H
#define _QTEXT_TIMEEDIT_H

// qtext/timeedit.h
// 5/30/2012

#include <QtGui/QTimeEdit>

namespace QtExt {

class TimeEdit : public QTimeEdit
{
  Q_OBJECT
  Q_DISABLE_COPY(TimeEdit)
  typedef TimeEdit Self;
  typedef QTimeEdit Base;

public:
  explicit TimeEdit(QWidget *parent = nullptr)
    : Base(parent) { }

  explicit TimeEdit(const QTime &time, QWidget *parent = nullptr)
    : Base(time, parent) { }

  QLineEdit *lineEdit() const { return Base::lineEdit(); }

signals:
  void returnPressed();

  // - Events -
protected:
  void keyPressEvent(QKeyEvent *event) override;
  void enterEvent(QEvent *event) override;
  void leaveEvent(QEvent *event) override;
};

} // namespace QtExt

#endif // _QTEXT_TIMEEDIT_H
