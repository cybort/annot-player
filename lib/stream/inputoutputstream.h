#ifndef INPUTOUTPUTSTREAM_H
#define INPUTOUTPUTSTREAM_H

// inputoutputstream.h
// 3/18/2012

#include "lib/stream/inputstream.h"
#include "lib/stream/outputstream.h"

class InputOutputStream : public InputStream, public OutputStream
{ };

class InputOutputStreamList : public QList<InputOutputStream *>
{
  typedef InputOutputStreamList Self;
  typedef QList<InputOutputStream *> Base;
public:
  InputOutputStreamList() { }
  InputOutputStreamList(const Self &that) : Base(that) { }

  void reset() const
  { foreach (value_type v, *this) v->reset(); }
};

#endif // INPUTOUTPUTSTREAM_H
