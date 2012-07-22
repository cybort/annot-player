#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

// messagehandler.h
// 10/19/2011

#include "processinfo.h"
#include <QtCore/QObject>
#include <QtCore/QVector>

class MessageHandler : public QObject
{
  Q_OBJECT
  Q_DISABLE_COPY(MessageHandler)
  typedef MessageHandler Self;
  typedef QObject Base;

public:
  struct MessageHash {
    qint64 hash;
    int count;

    explicit MessageHash(qint64 h = 0, int c = 0)
      : hash(h), count(c) { }

    void clear() { hash = 0; count = 0; }
    bool isValid() const { return hash && count; }
  };

public:
  explicit MessageHandler(QObject *parent = nullptr);

signals:
  void messageReceivedWithText(const QString &text);
  void messageReceivedWithId(qint64 pos);

  // - Properties -
public:
  bool isActive() const { return active_; }
  ulong hookId() const { return hookId_; }
  const ProcessInfo &processInfo() const { return pi_; }

  ///  Most recent on the top.
  const QVector<QByteArray> &recentMessages() const { return messages_; }
  //int recentMessageCapacity() const { return messageCount_; }

  const MessageHash &lastMessageHash() const { return lastMessageHash_; }

public slots:
  void setActive(bool active);
  void setHookId(ulong hid) { hookId_ = hid; }
  void setProcessInfo(const ProcessInfo &pi) { pi_ = pi; }

  void clearRecentMessages();

  // - Actions
protected slots:
  void processMessage(const QByteArray &data, ulong hookId);

  void connectTextHook();
  void disconnectTextHook();

  // - Implementations -
//protected:
//  static bool isSubtitle(const QString &text);

private:
  bool active_;
  ulong hookId_;
  ProcessInfo pi_;

  QVector<QByteArray> messages_; // recent messages queue
  int messageCount_;     // recent message capacity

  MessageHash lastMessageHash_;
};

#endif // MESSAGEHANDLER_H
