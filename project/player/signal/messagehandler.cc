// messagehandler.cc
// 10/19/2011

#include "messagehandler.h"
#include "global.h"
#ifdef USE_WIN_QTH
  #include "win/qth/qth.h"
#else
  #error "QTH module is indispensible"
#endif // USE_WIN_QTH
#include "core/data/annotation.h"
#include "core/cmd.h"
#include <QtCore>

//#define DEBUG "MessageHandler"
#include "module/debug/debug.h"

using namespace Core::Data;

//
// Message hashing rule:
//  - if is subtitle, hash count = 1 // TODO!
//  - if len >= 10, hash count = 1
//  - if len in [7,9], hash count = 2
//  - if len in [4,6], hash count = 3
//  - if len in [1,3], hash count = 4
//  - if repeat, hash count = min(repeat count, 4)
//  //- if len > 100, skip
//

namespace { // anonymous
enum { HASH_COUNT = 4};
} // anonymous namespace

// - Constructions -

MessageHandler::MessageHandler(QObject *parent)
  : Base(parent), active_(false), hookId_(0),
    messageCount_(HASH_COUNT), hashes_(HASH_COUNT)
{ }

void
MessageHandler::connectTextHook()
{ connect(QTH, SIGNAL(textReceived(QString,int,qint64)), SLOT(processTextMessage(QString,int,qint64))); }

void
MessageHandler::disconnectTextHook()
{ disconnect(QTH, SIGNAL(textReceived(QString,int,qint64)), this, SLOT(processTextMessage(QString,int,qint64))); }

// - Properties -

const MessageHandler::MessageHash&
MessageHandler::lastMessageHash() const
{ return lastMessageHash_; }

bool
MessageHandler::isActive() const
{ return active_; }

void
MessageHandler::setActive(bool active)
{
  if (active_ != active) {
    if (active)
      connectTextHook();
    else {
      disconnectTextHook();
      clearRecentMessages();
    }
    active_ = active;
  }
}

int
MessageHandler::hookId() const
{ return hookId_; }

void
MessageHandler::setHookId(int hid)
{ hookId_ = hid; }

const ProcessInfo&
MessageHandler::processInfo() const
{ return pi_; }

void
MessageHandler::setProcessInfo(const ProcessInfo &pi)
{ pi_ = pi; }

const QStringList&
MessageHandler::recentMessages() const
{ return messages_; }

int
MessageHandler::recentMessageCapacity() const
{ return messageCount_; }

void
MessageHandler::clearRecentMessages()
{
  messages_.clear();
  hashes_.fill(0);
  lastMessageHash_.clear();
}

void
MessageHandler::processTextMessage(const QString &text, int hookId, qint64 tsMsecs)
{
  Q_UNUSED(tsMsecs);
  DOUT("processTextMessage:enter: hookId =" << hookId << ", text =" << text);
  if (hookId != hookId_) {
    //lastMessageHash_.clear();
    DOUT("processTextMessage:exit: hook mismatch, hookId_ =" << hookId_);
    return;
  }

  if (text.trimmed().isEmpty()) {
    lastMessageHash_.clear();
    DOUT("processTextMessage:exit: skipping empty text");
    return;
  }

  messages_.prepend(text);
  if (messages_.size() >= messageCount_)
    messages_.takeLast();

  // Shift hash similar to above messages_
  qint64 h0 = Annotation::hashTextPos(text);
  for (int i = HASH_COUNT - 1; i > 0; i--)
    hashes_[i] = hashes_[i - 1];
  hashes_[0] = h0;
  DOUT("processTextMessage: h0 =" << h0);

  // See the rules on the top
  Q_ASSERT(hashes_.size() >= 4);
  int hashCount = 0;
  switch (text.size()) {
  case 0: Q_ASSERT(0);
  case 1: case 2: case 3: hashCount = 4; break;
  case 4: case 5: case 6: hashCount = 3; break;
  case 7: case 8: case 9: hashCount = 2; break;

  default:
    if (isSubtitle(text))
      hashCount = 1;
    else if (messages_.size() <= 1)
      hashCount = 1;
    else if (messages_.size() >= 2 && text != messages_[1])
      hashCount = 1;
    else if (messages_.size() >= 3 && text != messages_[2])
      hashCount = 2;
    else if (messages_.size() >= 4 && text != messages_[3])
      hashCount = 3;
    else
      hashCount = 4;
  }
  Q_ASSERT(hashCount >=1 && hashCount <= 4);

  if (hashCount > messages_.size()) {
    lastMessageHash_.clear();
    lastMessageHash_.count = hashCount;
    DOUT("processTextMessage:exit: skipping short message: hashCount =" << hashCount << ", messageSize =" << messages_.size());
    return;
  }

  qint64 h = 0;
  switch (hashCount) {
  case 4:
    h = Annotation::rehashTextPos(messages_[2], hashes_[3]);
    h = Annotation::rehashTextPos(messages_[1], h);
    h = Annotation::rehashTextPos(text, h);
  case 3:
    h = Annotation::rehashTextPos(messages_[1], hashes_[2]);
    h = Annotation::rehashTextPos(text, h);
    break;

  case 2: h = Annotation::rehashTextPos(text, hashes_[1]); break;

  case 1: h = h0; break;

  default: Q_ASSERT(0);
  }

  if (!h) {
    qDebug() << "FIXME: Zero hash encountered! text =" << text;
    lastMessageHash_.hash = 0;
    lastMessageHash_.count = hashCount;
    DOUT("processTextMessage:exit: skip zero hash");
  }

  lastMessageHash_.hash = h;
  lastMessageHash_.count = hashCount;
  Q_ASSERT(lastMessageHash_.isValid());

  DOUT("processTextMessage: h =" << h);
  emit messageReceived(h);

  DOUT("processTextMessage:exit: hashCount =" << hashCount);
}

// - Helpers -

bool
MessageHandler::isSubtitle(const QString &text)
{
  return text.contains(CORE_CMD_SUB " ")
      || text.contains(CORE_CMD_SUBTITLE " ");
}

// EOF