#ifndef _QTEXT_FONTCOMBOBOX_H
#define _QTEXT_FONTCOMBOBOX_H

// qtext/combobox.h
// 10/11/2011

#include <QtGui/QFontComboBox>

namespace QtExt {

class FontComboBox : public QFontComboBox
{
  Q_OBJECT
  Q_DISABLE_COPY(FontComboBox)
  typedef FontComboBox Self;
  typedef QFontComboBox Base;

  QString styleSheet_;

public:
  explicit FontComboBox(QWidget *parent = nullptr);

public slots:
  bool verifyCurrentText();

  // - Events -
protected:
  void enterEvent(QEvent *event) override;
  void leaveEvent(QEvent *event) override;
};

} // namespace QtExt

#endif // _QTEXT_FONTCOMBOBOX_H
