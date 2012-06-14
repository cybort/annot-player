#ifndef BUFFEREDFIFOSTREAM_H
#define BUFFEREDFIFOSTREAM_H

// bufferedfifostream.h
// 2/9/2012

#include "module/stream/fifostream.h"
#include <QtCore/QByteArray>
#include <QtCore/QMutex>
#include <QtCore/QObject>

class BufferedFifoStream : public QObject, public FifoStream
{
  Q_OBJECT
  Q_DISABLE_COPY(BufferedFifoStream)
  typedef BufferedFifoStream Self;
  typedef QObject Base;

  QMutex m_;
  QByteArray data_; // buffer
  qint64 pos_;
  bool finished_;

public:
  explicit BufferedFifoStream(QObject *parent = 0)
    : Base(parent), pos_(0), finished_(false) { }

signals:
  void finished();
  void readyRead();

public:
  bool isFinished() const { return finished_; }

  // - Shared -
  virtual qint64 size() const ///< \reimp
  { return FifoStream::size() ? FifoStream::size() : data_.size(); }

  virtual qint64 availableSize() const ///< \reimp
  { return data_.size(); }

  // - Input -
  virtual qint64 pos() const { return pos_; } ///< \reimp

  virtual bool reset() { m_.lock(); pos_ = 0; m_.unlock(); return true; } ///< \reimp

  virtual qint64 skip(qint64 count); ///< \reimp

  virtual bool seek(qint64 pos); ///< \reimp

  virtual qint64 read(char *data, qint64 maxSize); ///< \reimp
  virtual qint64 tryRead(char *data, qint64 maxSize); ///< \reimp

  virtual QByteArray readAll() ///< \reimp
  { if (!isFinished()) waitForFinished(); return data_; }

  QByteArray &data() { return data_; }
  const QByteArray &data() const { return data_; }

  virtual bool writeFile(const QString &path); ///< \reimp

  // - Output -

  virtual qint64 write(const char *data, qint64 maxSize); ///< \reimp
  virtual void finish(); ///< \reimp

public slots:
  virtual void waitForFinished();
  virtual void waitForReadyRead();
};

#endif // BUFFEREDFIFOSTREAM_H