#include <QLabel>
#include <QGridLayout>
#include <QPixmap>

#include "ChoosePic.h"

ChoosePic::ChoosePic(  QWidget* parent ) : QLabel( parent )
{
}


void ChoosePic::mousePressEvent( QMouseEvent* event )
{
  if (event->type() == QEvent::MouseButtonPress)
    emit clicked();
}