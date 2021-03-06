#ifndef SIMPLEIMAGEFILTER_H
#define SIMPLEIMAGEFILTER_H

// simpleimagefilter.h
// 5/17/2012

#include "lib/imagefilter/brightnessimagefilter.h"
#include "lib/imagefilter/contrastimagefilter.h"
#include "lib/imagefilter/gammaimagefilter.h"
#include <QtCore/QObject>

class SimpleImageFilter : public QObject, public ImageFilter
{
  Q_OBJECT
  Q_PROPERTY(int brightness READ brightness WRITE setBrightness)
  Q_PROPERTY(int contrast READ contrast WRITE setContrast)
  Q_PROPERTY(int gamma READ gamma WRITE setGamma)
  Q_DISABLE_COPY(SimpleImageFilter)
  typedef SimpleImageFilter Self;
  typedef QObject Base;

  BrightnessImageFilter *brightnessFilter_;
  ContrastImageFilter *contrastFilter_;
  GammaImageFilter *gammaFilter_;

public:
  explicit SimpleImageFilter(QObject *parent = nullptr)
    : Base(parent)
  {
    brightnessFilter_ = new BrightnessImageFilter(this);
    contrastFilter_ = new ContrastImageFilter(this);
    gammaFilter_ = new GammaImageFilter(this);
  }

public:
  int brightness() const { return brightnessFilter_->value(); }
  int contrast() const { return contrastFilter_->value(); }
  int gamma() const { return gammaFilter_->value(); }

public slots:
  void setBrightness(int value) { brightnessFilter_->setValue(value); }
  void setContrast(int value) { contrastFilter_->setValue(value); }
  void setGamma(int value) { gammaFilter_->setValue(value); }

  void clear()
  {
    brightnessFilter_->clear();
    contrastFilter_->clear();
    gammaFilter_->clear();
  }

public:
  QImage &filterImage(QImage &image) const override
  {
    return (*gammaFilter_)
          ((*contrastFilter_)
          ((*brightnessFilter_)(image)));
  }

  bool needsDisplay() const override
  {
    return *brightnessFilter_ ||
           *contrastFilter_ ||
           *gammaFilter_;
  }
};

#endif // SIMPLEIMAGEFILTER_H
