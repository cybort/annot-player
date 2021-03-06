// bilibilicodec.cc
// 2/3/2012
// See:  http://9ch.co/t17836,1-1.html

#include "lib/annotcodec/bilibilicodec.h"
#ifdef WITH_LIB_ANNOTCACHE
# include "lib/annotcache/annotationcachemanager.h"
#endif // WITH_LIB_ANNOTCACHE
#include "lib/annotcloud/annottag.h"
#include "lib/annotcloud/traits.h"
#include "htmlutil/htmltags.h"
#include "qtx/qxnetwork.h"
#ifdef WITH_LIB_COMPRESS
# include "lib/compress/qgzip.h"
#else
# error "require lib compress"
#endif // WITH_LIB_COMPRESS
#include <QtCore/QByteArray>
#include <QtCore/QStringList>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

#define RequestUrlAttribute QNetworkRequest::UserMax

//#define DEBUG "bilibilicodec"
#include "qtx/qxdebug.h"

using namespace AnnotCloud;

enum { MaxRetries = 5 };

// - Construction -

BilibiliCodec::BilibiliCodec(QObject *parent)
  : Base(parent)
{
  nam_ = new QNetworkAccessManager(this);
  connect(nam_, SIGNAL(finished(QNetworkReply*)), SLOT(parseReply(QNetworkReply*)));
}

// - Fetch -

bool
BilibiliCodec::match(const QString &url) const
{ return url.contains("http://comment.bilibili.com/", Qt::CaseInsensitive); }

void
BilibiliCodec::fetch(const QString &url, const QString &originalUrl)
{
  Q_ASSERT(match(url));
  DOUT("enter: url =" << url);
  QNetworkReply *reply = nam_->get(QNetworkRequest(url));
  QxPublicNetworkReply::fromReply(reply)
      ->setAttribute(RequestUrlAttribute, originalUrl);
  DOUT("exit");
}

// - Decode -

void
BilibiliCodec::parseReply(QNetworkReply *reply)
{
  DOUT("enter");
  Q_ASSERT(reply);
  reply->deleteLater();
  QString url = reply->url().toString();
  if (!reply->isFinished() || reply->error() != QNetworkReply::NoError) {
    int retry = ++retries_[url];
    if (retry <= MaxRetries) {
      emit errorMessage(
        tr("network error, retry")
        + QString(" (%1/%2):").arg(QString::number(retry)).arg(QString::number(MaxRetries))
        + url
      );
      QString originalUrl = reply->attribute(RequestUrlAttribute).toString();
      fetch(url, originalUrl);
    } else
      emit errorMessage(tr("network error, failed to resolve media URL") + ": " + url);
    DOUT("exit: network error:" << reply->errorString());
    return;
  }
  retries_.remove(url);
  QByteArray data = reply->readAll();
  AnnotationList l;
  if (!data.isEmpty()) {
#ifdef WITH_LIB_COMPRESS
    QByteArray unzipped = ::gHttpUncompress(data);
    if (!unzipped.isEmpty())
      data = unzipped;
#endif // WITH_LIB_COMPRESS
    l = parseDocument(data);
  }
  if (l.isEmpty())
    emit errorMessage(tr("failed to resolve annotations from URL") + ": " + reply->url().toString());
  else {
    QString url = reply->url().toString(),
            originalUrl = reply->attribute(QNetworkRequest::UserMax).toString();
#ifdef WITH_LIB_ANNOTCACHE
    AnnotationCacheManager::globalInstance()->saveData(data, originalUrl);
#endif // WITH_LIB_ANNOTCACHE
    emit fetched(l, url, originalUrl);
  }
  DOUT("exit: annots.size =" << l.size());
}

// - Decode -

AnnotationList
BilibiliCodec::parseDocument(const QByteArray &data)
{
  DOUT("enter: data.size =" << data.size());
  AnnotationList ret;

  if (data.isEmpty()) {
    DOUT("exit: empty data");
    return ret;
  }
  QDomDocument doc;
  doc.setContent(skipXmlLeadingComment(
    QChar(data[0]).isSpace() ? data.trimmed() : data
  ));
  if (doc.isNull()) {
    DOUT("exit: invalid document root");
    return ret;
  }
  QDomElement root = doc.firstChildElement("i");
  if (root.isNull()) {
    DOUT("exit: invalid root element");
    return ret;
  }

  QDomElement e = root.firstChildElement("d");
  while (!e.isNull()) {
    QString text = e.text().trimmed();
    QString attr = e.attribute("p");
    if (!text.isEmpty() && !attr.isEmpty()) {
      Annotation a = parseComment(attr, text);
      if (a.hasText())
        ret.append(a);
    }
    e = e.nextSiblingElement("d");
  }
  DOUT("exit: size =" << ret.size());
  return ret;
 }

QString
BilibiliCodec::parseText(const QString &text)
{
  return text.isEmpty() ? text : ::htmlEscape(text)
    .replace("/n", "\n ")
    .trimmed();
}

// Example: <d p="0,1,25,16777215,1312863760,0,eff85771,42759017">Annotation</d>
Annotation
BilibiliCodec::parseAttribute(const QString &attr)
{
  enum { AttrPos = 0, AttrFloatStyle, AttrFontSize, AttrFontColor, AttrCreateTime, AttrSubType, AttrUserId, AttrRowId, AttrCount };

  if (attr.isEmpty())
    return Annotation();
  QStringList l = attr.split(',');
  if (l.size() != AttrCount)
    return Annotation();

  double pos = l[AttrPos].toDouble();
  int floatStyle = l[AttrFloatStyle].toInt();
  int fontSize = l[AttrFontSize].toInt();
  int fontColor = l[AttrFontColor].toInt();
  qint64 createTime = l[AttrCreateTime].toLongLong();
  int subType = l[AttrSubType].toInt();
  QString userAlias = l[AttrUserId];
  qint64 rowId = l[AttrRowId].toLongLong();

  Annotation ret;
  QString t;
  // 0
  ret.setPos(pos * 1000);

  // 1
  switch (floatStyle) {
  case 5: case 6: case 7:
    t += CORE_CMD_VIEW_TOP;
    break;
  case 4:
    t += CORE_CMD_VIEW_BOTTOM;
    break;
  }

  // 2
  //fontSize
#ifdef Q_OS_WIN
  fontSize *= 0.9;
#endif // Q_OS_WIN
  if (fontSize > 0)
    t += CORE_CMD_SIZE  "[" + QString::number(fontSize) + "]";

  // 3
  //fontColor
  if (fontColor > 0)
    t += CORE_CMD_COLOR "[" "#" + QString().sprintf("%06X", fontColor) + "]";

  // 4
  ret.setCreateTime(createTime);

  // 5
  if (subType == 1)
    t += CORE_CMD_SUB;

  // 6
  bool ok;
  ret.setUserAlias(userAlias);
  qint64 uid = userAlias.toLongLong(&ok, 16);
  if (uid <= 0)
    uid = rowId;
  ret.setUserId(-uid);

  // 7
  ret.setId(-rowId);

  // Others
  //if (t.isEmpty())
  //  t = CORE_CMD_VERBATIM;
  ret.setText(t);

  ret.setLanguage(Traits::SimplifiedChinese);

  return ret;
}

// EOF
