#pragma once

// picker.h
// 10/30/2011

#include "picker_config.h"
#include <QtGui/QWidget>

class WindowPickerPrivate; ///< \internal

///  This class should be used as a singleton.
class WindowPicker : public QObject
{
  Q_OBJECT
  Q_DISABLE_COPY(WindowPicker)
  typedef WindowPicker Self;
  typedef QObject Base;
  typedef WindowPickerPrivate Private;

  // - Singleton -
public:
  PICKERAPI static Self *globalInstance() { static Self g; return &g; }

signals:
  void windowPicked(WId hwnd);

  // - Service -
public:
  PICKERAPI bool isActive() const;
  PICKERAPI bool isSingleShot() const;

public slots:
  PICKERAPI void start();       ///< Start getting the winId that the user clicked
  PICKERAPI void stop();        ///< Stop hook service
  PICKERAPI void singleShot();  ///< Pick one window and return

  PICKERAPI void setSingleShot(bool t); ///< Property setter

  // - Implementation -
protected:
  explicit WindowPicker(QObject *parent = nullptr);   ///< \internal
  ~WindowPicker();  ///< \internal

public:
  void *hook() const; ///< \internal Return global hook handle
  void pickWindow(WId hwnd); ///< \internal Emit windowPicked signal

private:
  Private *d_;
};

// EOF
