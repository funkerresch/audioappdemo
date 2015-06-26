/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>
#include "window.h"
// libpd_queued_receive_pd_messages();

Window *Window::globalWindow;

/*
 * Creating controls for Pure Data
 */

void Window::createPureDataControls(const QString &title)
{
    pureDataControlsGroup = new QGroupBox(title);
    activatePatcherGroup = new QButtonGroup;

    volumeDial = new QDial;
    volumeDial->setFocusPolicy(Qt::StrongFocus);

    activateFirstPatcher = new QPushButton;
    activateFirstPatcher->setCheckable(true);
    activateFirstPatcher->setText(tr("Load First Patcher"));

    activateSecondPatcher = new QPushButton;
    activateSecondPatcher->setCheckable(true);
    activateSecondPatcher->setText(tr("Load Second Patcher"));

    activatePatcherGroup->addButton(activateFirstPatcher,1);
    activatePatcherGroup->addButton(activateSecondPatcher,2);
    activatePatcherGroup->setExclusive(false);

    innerVerticalLayout = new QVBoxLayout;
    innerVerticalLayout->addWidget(activateFirstPatcher);
    innerVerticalLayout->addWidget(activateSecondPatcher);

    innerHorizontalLayout = new QHBoxLayout;
    innerHorizontalLayout->addWidget(volumeDial);
    innerHorizontalLayout->addLayout(innerVerticalLayout);

    pureDataControlsGroup->setLayout(innerHorizontalLayout);

    connect(activatePatcherGroup, SIGNAL(buttonClicked(int)), this, SLOT(loadPatch(int)));
}

/*
 * Creating controls for Portaudio
 */

void Window::createPortaudioControls(const QString &title)
{
    portaudioControlsGroup = new QGroupBox(title);
    selectOutputCombo = new QComboBox;
    selectInputCombo = new QComboBox;
    startStopButton = new QPushButton;
    startStopButton->setText(tr("Start Audio"));
    startStopButton->setCheckable(true);

    connect(startStopButton, SIGNAL(toggled(bool)), this, SLOT(startStopAudio(bool)));

    const PaDeviceInfo* di;

    for(int i = 0;i < Pa_GetDeviceCount();i++)
    {
        if(ap->isOutputDevice(i))
        {
             di = Pa_GetDeviceInfo(i);
             selectOutputCombo->addItem(QString(di->name), i);
        }
    }

    for(int i = 0;i < Pa_GetDeviceCount();i++)
    {
        if(ap->isInputDevice(i))
        {
             di = Pa_GetDeviceInfo(i);
             selectInputCombo->addItem(QString(di->name), i);
        }
    }

    QGridLayout *controlsLayout = new QGridLayout;
    controlsLayout->addWidget(selectOutputCombo, 0, 0, 1, 3);
    controlsLayout->addWidget(selectInputCombo, 1, 0, 1, 3);
    controlsLayout->addWidget(startStopButton, 2, 0, 1, 3);
    portaudioControlsGroup->setLayout(controlsLayout);

    connect(selectOutputCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(selectOutputDevice(int)));
    connect(selectInputCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(selectInputDevice(int)));
}

void Window::selectOutputDevice(int index)
{
    (void) index;
    startStopAudio(false);
    ap->setOutputDevice(selectOutputCombo->currentData().toInt());
}

void Window::selectInputDevice(int index)
{
    (void) index;
    startStopAudio(false);
    ap->setInputDevice(selectInputCombo->currentData().toInt());
}

Window::Window()
{
    ap = new audioProcessor();
    ap->setFrameLength(256); // Default vector size for Pd is 64.
                             // We are calling libpd_process_float with 4 * 64 samples in "audiooutput.c".

    // creating our GUI controls
    createPureDataControls("Pure Data Controls");
    createPortaudioControls(tr("Portaudio Controls"));

    QHBoxLayout *layout = new QHBoxLayout;

    layout->addWidget(portaudioControlsGroup);
    layout->addWidget(pureDataControlsGroup);
    setLayout(layout);

    libpd_banghook = (t_libpd_banghook) Window::bangpd;  // binding the function bangpd to receive bangs from our pd patchers
    libpd_printhook = (t_libpd_printhook) Window::printpd; // binding printpd

    libpd_init();
    libpd_init_audio(0, 2, 44100); // 0 inputs, 2 outputs, we should adjust this (also for portaudio) every time we change the input or output device
                                   // and load a corresponding output patch, in this example it is statically set to 2 outputs and 0 inputs

    QString path = QCoreApplication::applicationDirPath();
    libpd_openfile("puredata/stereoout.pd", path.toLatin1());   // loading an output patcher with 2 outputs
    libpd_openfile("puredata/firstpatcher.pd", path.toLatin1());
    libpd_openfile("puredata/secondpatcher.pd", path.toLatin1());   // preloads the playback patchers, dynamically loading patchers seemed a little unstable on os x
    libpd_bind("playfinishedfirst");    // binding the pd patcher send "playfinishedfirst" to our libpd_banghook, libpd_printhook etc..
    libpd_bind("playfinishedsecond");   // same for the send object "playfinishedsecond"

    setWindowTitle(tr("Libpd, Portaudio and Qt Test Project"));
    Window::globalWindow = this; // binding a static window to our window object in order to access our instance from bangpd and printpd
}

void Window::loadPatch(int index)
{
    qDebug() << index;
    if(index == 1)
        libpd_bang("activatefirstpatcher"); // sending a bang to the Pd receiver object "activatefirstpatcher"
    if(index == 2)
        libpd_bang("activatesecondpatcher");
}

void Window::startStopAudio(bool startStop)
{
   if(startStop)
   {
       qDebug() << "Start";
       libpd_start_message(1);
       libpd_add_float(1.0f);
       libpd_finish_message("pd", "dsp"); //start dsp
       ap->startAudio();
       startStopButton->setText(tr("Stop Audio"));
       startStopButton->setChecked(true);
   }
   else
   {
       qDebug() << "Stop";
       libpd_start_message(1);
       libpd_add_float(0.0f);
       libpd_finish_message("pd", "dsp"); //stop dsp
       ap->stopAudio();
       startStopButton->setText(tr("Start Audio"));
       startStopButton->setChecked(false);
   }
}

void Window::bangpd(const char *source)
{
    if(!strcmp(source, "playfinishedfirst"))
        Window::globalWindow->activateFirstPatcher->setChecked(false);

    if(!strcmp(source, "playfinishedsecond"))
        Window::globalWindow->activateSecondPatcher->setChecked(false);
}

void Window::printpd(const char *s)
{
    qDebug() << s;
}

