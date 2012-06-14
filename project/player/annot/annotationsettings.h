#ifndef ANNOTATIONSETTINGS_H
#define ANNOTATIONSETTINGS_H
#include <QDebug>

// annotationsettings.h
// 5/24/2012

#include "project/common/acsettings.h"
#include <QtGui/QColor>
#include <QtGui/QFont>
#include <QtCore/QObject>

#ifdef Q_WS_MAC
#  define ANNOTATION_JAPANESE_FONT_FAMILY "Helvetica"
#else
#  define ANNOTATION_JAPANESE_FONT_FAMILY "MS Gothic"
#endif // Q_WS_MAC

#define ANNOTATION_CHINESE_FONT_FAMILY  "YouYuan"

#define ANNOTATION_FONT_FAMILY  "Helvetica"

#ifdef Q_WS_WIN
#  define ANNOTATION_FONT_SIZE  18
#else
#  define ANNOTATION_FONT_SIZE  20
#endif // Q_WS_WIN

#define ANNOTATION_OUTLINE_COLOR        QColor(50,100,100)
#define ANNOTATION_OUTLINE_COLOR_HOVER  Qt::red
#define ANNOTATION_OUTLINE_COLOR_SELF   Qt::darkYellow
#define ANNOTATION_OUTLINE_COLOR_SUB    Qt::magenta

#define ANNOTATION_EFFECT_OPACITY 0.9

class AnnotationSettings : public QObject
{
  Q_OBJECT
  Q_DISABLE_COPY(AnnotationSettings)
  typedef AnnotationSettings Self;
  typedef QObject Base;

  qreal scale_;
  qreal rotation_;
  int offset_;
  QFont font_,
        japaneseFont_,
        chineseFont_;

  bool avatarVisible_;
  bool metaVisible_;
  bool motionless_;

  QColor outlineColor_,
         subtitleColor_;

public:
  static Self *globalSettings() { static Self g; return &g; }
protected:
  explicit AnnotationSettings(QObject *parent = 0)
    : Base(parent), scale_(1), rotation_(0), offset_(0),
      avatarVisible_(false), motionless_(true)
  {
    resetOutlineColor();
    resetFont();
    resetJapaneseFont();
    resetChineseFont();
  }

signals:
  void scaleChanged(qreal value);
  void rotationChanged(qreal value);
  void offsetChanged(int value);
  void avatarVisibleChanged(bool value);
  void metaVisibleChanged(bool value);
  void preferMotionlessChanged(bool value);
  void outlineColorChanged(QColor color);
  void subtitleColorChanged(QColor color);
  //void fontChanged(const QFont &value);
public:
  qreal scale() const { return scale_; }
  qreal rotation() const { return rotation_; }
  int offset() const { return offset_; }
  bool isAvatarVisible() const { return avatarVisible_; }
  bool isMetaVisible() const { return metaVisible_; }

  QFont &font() { return font_; }
  QFont font() const { return font_; }
  QString fontFamily() const { return font_.family(); }

  QFont &japaneseFont() { return japaneseFont_; }
  QFont japaneseFont() const { return japaneseFont_; }
  QString japaneseFontFamily() const { return japaneseFont_.family(); }

  QFont &chineseFont() { return chineseFont_; }
  QFont chineseFont() const { return chineseFont_; }
  QString chineseFontFamily() const { return chineseFont_.family(); }

  const QColor &outlineColor() const { return outlineColor_; }
  const QColor &subtitleColor() const { return subtitleColor_; }

  bool preferMotionless() const { return motionless_; }

public slots:

  void setScale(qreal value) { if (!qFuzzyCompare(scale_, value)) emit scaleChanged(scale_ = value); }
  void resetScale() { setScale(0); }

  void setRotation(qreal value) { if (!qFuzzyCompare(rotation_, value)) emit rotationChanged(rotation_ = value); }
  void resetRotation() { setRotation(0); }

  void setOffset(int value) { if (offset_ != value) emit offsetChanged(offset_ = value); }
  void resetOffset() { setOffset(0); }

  void setOutlineColor(const QColor &c)
  {
    if (c.isValid()) {
      if (outlineColor_ != c)
        emit outlineColorChanged(outlineColor_ = c);
    } else if (outlineColor_ != ANNOTATION_OUTLINE_COLOR)
      emit outlineColorChanged(outlineColor_ = ANNOTATION_OUTLINE_COLOR);
  }

  void resetOutlineColor() { setOutlineColor(QColor()); }

  void setSubtitleColor(const QColor &c)
  {
    if (c.isValid()) {
      if (subtitleColor_ != c)
        emit subtitleColorChanged(subtitleColor_ = c);
    } else if (subtitleColor_ != ANNOTATION_OUTLINE_COLOR_SUB)
      emit subtitleColorChanged(subtitleColor_ = ANNOTATION_OUTLINE_COLOR_SUB);
  }

  void resetSubtitleColor() { setSubtitleColor(QColor()); }

  void setFont(const QFont &value) { font_ = value; }
  void setJapaneseFont(const QFont &value) { japaneseFont_ = value; }
  void setChineseFont(const QFont &value) { chineseFont_ = value; }

  void setAvatarVisible(bool value)
  { if (avatarVisible_ != value) emit avatarVisibleChanged(avatarVisible_ = value); }

  void setMetaVisible(bool value)
  { if (metaVisible_ != value) emit metaVisibleChanged(metaVisible_ = value); }

  void setPreferMotionless(bool value)
  { if (motionless_ != value) emit preferMotionlessChanged(motionless_ = value);}

  //void resetFontFamily() { resetFont(); }

  void setFontFamily(const QString &family)
  {
    QString f = family.isEmpty() ? defaultFont().family() : family;
    if (f.compare(f, font_.family(), Qt::CaseInsensitive))
      setFont(makeFont(f));
  }

  void setJapaneseFontFamily(const QString &family)
  {
    QString f = family.isEmpty() ? defaultJapaneseFont().family() : family;
    if (f.compare(f, font_.family(), Qt::CaseInsensitive))
      setJapaneseFont(makeFont(f));
  }

  void setChineseFontFamily(const QString &family)
  {
    QString f = family.isEmpty() ? defaultChineseFont().family() : family;
    if (f.compare(f, font_.family(), Qt::CaseInsensitive))
      setChineseFont(makeFont(f));
  }

  void resetFont()
  { setFont(defaultFont()); }

  void resetJapaneseFont()
  { setFont(defaultJapaneseFont()); }

  void resetChineseFont()
  { setFont(defaultChineseFont()); }

public:
  static const QFont &defaultFont()
  {
    static QFont ret = makeFont(ANNOTATION_FONT_FAMILY);
    return ret;
  }

  static const QFont &defaultJapaneseFont()
  {
    static QFont ret = makeFont(ANNOTATION_JAPANESE_FONT_FAMILY);
    return ret;
  }

  static const QFont &defaultChineseFont()
  {
    static QFont ret = makeFont(ANNOTATION_CHINESE_FONT_FAMILY);
    return ret;
  }

protected:
  static QFont makeFont(const QString &family)
  {
    QFont ret(family, ANNOTATION_FONT_SIZE);
    ret.setWeight(QFont::DemiBold);
    ret.setStyleStrategy((QFont::StyleStrategy)(
      QFont::PreferAntialias | QFont::PreferQuality
    ));
    return ret;
  }
};

#endif // ANNOTATIONSETTINGS_H