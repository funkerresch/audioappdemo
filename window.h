/****************************************************************************
*
* Simple Example for binding libpd, portaudio and QT together
* Based on Qt's Sliders Example
* by Thomas Resch
*
****************************************************************************/

#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include "z_libpd.h"
#include "m_pd.h"
#include "audiooutput.h"
#include "util/z_queued.h"
#include <QDir>

QT_BEGIN_NAMESPACE
class QCheckBox;
class QComboBox;
class QGroupBox;
class QLabel;
class QSpinBox;
class QStackedWidget;
QT_END_NAMESPACE
class SlidersGroup;
class QDial;
class QPushButton;
class QButtonGroup;
class QVBoxLayout;
class QHBoxLayout;

class Window : public QWidget
{
    Q_OBJECT

public:
    Window();


private:
    void createPureDataControls(const QString &title);
    void createPortaudioControls(const QString &title);

    static void bangpd(const char *source);
    static void printpd(const char *s);
    static Window *globalWindow;

    audioProcessor *ap;
    QGroupBox *portaudioControlsGroup;
    QGroupBox *pureDataControlsGroup;
    QDial *volumeDial;
    QComboBox *selectOutputCombo;
    QComboBox *selectInputCombo;
    QPushButton *activateFirstPatcher;
    QPushButton *activateSecondPatcher;
    QPushButton *startStopButton;
    QButtonGroup *activatePatcherGroup;
    QVBoxLayout *innerVerticalLayout;
    QHBoxLayout *innerHorizontalLayout;

public slots:
    void loadPatch(int index);
    void startStopAudio(bool startStop);
    void selectOutputDevice(int index);
    void selectInputDevice(int index);
};

#endif
