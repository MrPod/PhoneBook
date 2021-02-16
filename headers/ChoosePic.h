#pragma once
#include <QDialog>
#include <QMouseEvent>
#include <QLabel>

class ChoosePic : public QLabel
{
  Q_OBJECT

public:
  ChoosePic( QWidget* parent = Q_NULLPTR);

signals:
  void clicked();

private slots:
  void mousePressEvent( QMouseEvent* event );
};